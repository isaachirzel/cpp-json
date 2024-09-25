#include "hirzel/json/Deserialization.hpp"
#include "hirzel/json/Error.hpp"
#include "hirzel/json/Token.hpp"
#include "hirzel/json/ValueType.hpp"

#include <utility>
#include <cassert>
#include <cstdlib>

namespace hirzel::json
{
	static void expectedError(const Token& token, const char *expected)
	{
		if (!hasErrorCallback())
			return;

		auto message = std::string();

		message += "Unable to deserialize value: Expected ";
		message += expected;
		message += ", but got '";
		message += token.text();
		message += "'.";

		error(message);
	}

	std::optional<Value> deserialize(const char* json)
	{
		auto token = Token::initialFor(json);
		auto out = deserializeValue(token);

		if (!out)
			return {};

		if (token.type() != TokenType::EndOfFile)
		{
			expectedError(token, "end of file");
			return {};
		}

		return out;
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

			default:
				break;
		}

		expectedError(token, "object, array, string, number, boolean, or null");

		return {};
	}

	std::optional<Value> deserializeObject(Token& token)
	{
		if (token.type() != TokenType::LeftBrace)
		{
			expectedError(token, "'{'");
			return {};
		}

		token.seekNext();

		auto object = Object();

		if (token.type() != TokenType::RightBrace)
		{
			while (true)
			{
				if (token.type() != TokenType::String)
				{
					expectedError(token, "label");
					return {};
				}

				auto label = std::string(token.src() + token.pos() + 1, token.length() - 2);

				token.seekNext();

				if (token.type() != TokenType::Colon)
				{
					expectedError(token, "':'");
					return {};
				}

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
			{
				expectedError(token, "'}'");
				return {};
			}
		}

		token.seekNext();

		return object;
	}

	std::optional<Value> deserializeArray(Token& token)
	{
		if (token.type() != TokenType::LeftBracket)
		{
			expectedError(token, "'['");
			return {};
		}

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
			{
				expectedError(token, "']'");
				return {};
			}
		}

		token.seekNext();

		return arr;
	}

	std::optional<Value> deserializeString(Token& token)
	{
		if (token.type() != TokenType::String)
		{
			expectedError(token, "string");
			return {};
		}

		auto text = std::string(token.src() + token.pos() + 1, token.length() - 2);
		auto json = Value(std::move(text));

		token.seekNext();

		return json;
	}

	std::optional<Value> deserializeNumber(Token& token)
	{
		if (token.type() != TokenType::Number)
		{
			expectedError(token, "number");
			return {};
		}

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

		expectedError(token, "boolean");
		return {};
	}

	std::optional<Value> deserializeNull(Token& token)
	{
		if (token.type() != TokenType::Null)
		{
			expectedError(token, "null");
			return {};
		}

		return Value();
	}
}
