#include "hirzel/json/Deserialization.hpp"
#include "hirzel/json/Token.hpp"

#include <stdexcept>
#include <utility>
#include <cassert>
#include <cstdlib>

namespace hirzel::json
{
	std::optional<Value> deserialize(const char* json)
	{
		try
		{
			auto token = Token::initialFor(json);
			auto out = deserializeValue(token);

			if (token.type() != TokenType::EndOfFile)
				throw std::runtime_error("Unexpected token: " + token.text());

			return out;
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error("Failed to deserialize JSON: " + std::string(e.what()));
		}
	}

	std::optional<Value> deserialize(const std::string& json)
	{
		return deserialize(json.c_str());
	}

	std::optional<Value> deserializeValue(Token& token)
	{
		switch (token.type())
		{
			case TokenType::LeftBrace:
				return deserializeObject(token);

			case TokenType::LeftBracket:
				return deserializeArray(token);

			case TokenType::String:
				return deserializeString(token);

			case TokenType::Number:
				return deserializeNumber(token);

			case TokenType::True:
			case TokenType::False:
				return deserializeBoolean(token);

			case TokenType::Null:
				return deserializeNull(token);

			case TokenType::EndOfFile:
				throw std::runtime_error("Unexpected end of file.");

			default:
				throw std::runtime_error("Unexpected token: '" + token.text() + "'.");
		}
	}

	std::optional<Value> deserializeObject(Token& token)
	{
		if (token.type() != TokenType::LeftBrace)
			return {};

		token.seekNext();

		auto object = Object();

		if (token.type() != TokenType::RightBrace)
		{
			while (true)
			{
				if (token.type() != TokenType::String)
					throw std::runtime_error("Expected label, got '" + token.text() + "'.");

				auto label = std::string(token.src() + token.pos() + 1, token.length() - 2);

				token.seekNext();

				if (token.type() != TokenType::Colon)
					throw std::runtime_error("Expected ':' before '" + token.text() + "'.");

				token.seekNext();

				auto value = deserializeValue(token);

				if (!value)
					return {};

				object.emplace(std::move(label), *value);

				if (token.type() == TokenType::Comma)
				{
					token.seekNext();
					continue;
				}

				break;
			}

			if (token.type() != TokenType::RightBrace)
				throw std::runtime_error("Expected '}' before '" + token.text() + "'.");
		}

		token.seekNext();

		return object;
	}

	std::optional<Value> deserializeArray(Token& token)
	{
		if (token.type() != TokenType::LeftBracket)
			return {};

		token.seekNext();

		auto arr = Array();

		if (token.type() != TokenType::RightBracket)
		{
			while (true)
			{
				auto value = deserializeValue(token);

				if (!value)
					return {};

				arr.emplace_back(*value);

				if (token.type() == TokenType::Comma)
				{
					token.seekNext();
					continue;
				}

				break;
			}

			if (token.type() != TokenType::RightBracket)
				return {};
		}

		token.seekNext();

		return arr;
	}

	std::optional<Value> deserializeString(Token& token)
	{
		if (token.type() != TokenType::String)
			return {};

		auto text = std::string(token.src() + token.pos() + 1, token.length() - 2);
		auto json = Value(std::move(text));

		token.seekNext();

		return json;
	}

	std::optional<Value> deserializeNumber(Token& token)
	{
		if (token.type() != TokenType::Number)
			return {};

		auto text = token.text();
		auto value = atof(text.c_str());
		auto json = Value(value);

		token.seekNext();

		return json;
	}

	std::optional<Value> deserializeBoolean(Token& token)
	{
		if (token.type() == TokenType::True)
			return true;

		if (token.type() == TokenType::False)
			return false;

		return {};
	}

	std::optional<Value> deserializeNull(Token& token)
	{
		assert(token.type() == TokenType::Null);

		return {};
	}
}
