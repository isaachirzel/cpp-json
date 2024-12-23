#include "hirzel/json/Deserialization.hpp"
#include "hirzel/json/Error.hpp"
#include "hirzel/json/Token.hpp"

#include <utility>
#include <cstdlib>

namespace hirzel::json
{
	std::optional<Value> deserializeValue(Token& token);
	std::optional<Value> deserializeObject(Token& token);
	std::optional<Value> deserializeArray(Token& token);
	std::optional<Value> deserializeString(Token& token);
	std::optional<Value> deserializeNumber(Token& token);
	std::optional<Value> deserializeBoolean(Token& token);
	std::optional<Value> deserializeNull(Token& token);

	static bool incrementToken(Token& token)
	{
		auto nextToken = token.parseNext();

		if (!nextToken)
			return false;

		token = *nextToken;

		return true;
	}

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

		pushError(message);
	}

	std::optional<Value> deserialize(const char* json)
	{
		auto token = Token::parse(json);

		if (!token)
			return {};

		auto out = deserializeValue(*token);

		if (!out)
			return {};

		if (token->type() != TokenType::EndOfFile)
		{
			expectedError(*token, "end of file");
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

		if (!incrementToken(token))
			return {};

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

				auto label = std::string(token.src() + token.index() + 1, token.length() - 2);

				if (!incrementToken(token))
					return {};

				if (token.type() != TokenType::Colon)
				{
					expectedError(token, "':'");
					return {};
				}

				if (!incrementToken(token))
					return {};

				auto value = deserializeValue(token);

				if (!value)
					return {};

				object.emplace(std::move(label), *value);

				if (token.type() == TokenType::Comma)
				{
					if (!incrementToken(token))
						return {};

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

		if (!incrementToken(token))
			return {};

		return object;
	}

	std::optional<Value> deserializeArray(Token& token)
	{
		if (token.type() != TokenType::LeftBracket)
		{
			expectedError(token, "'['");
			return {};
		}

		if (!incrementToken(token))
			return {};

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
					if (!incrementToken(token))
						return {};

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

		if (!incrementToken(token))
			return {};

		return arr;
	}

	std::optional<Value> deserializeString(Token& token)
	{
		if (token.type() != TokenType::String)
		{
			expectedError(token, "string");
			return {};
		}

		auto text = std::string(token.src() + token.index() + 1, token.length() - 2);
		auto json = Value(std::move(text));

		if (!incrementToken(token))
			return {};

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

		if (!incrementToken(token))
			return {};

		return json;
	}

	std::optional<Value> deserializeBoolean(Token& token)
	{
		bool value;

		switch (token.type())
		{
			case TokenType::True:
				value = true;
				break;

			case TokenType::False:
				value = false;
				break;

			default:
				expectedError(token, "boolean");
				return {};
		}

		incrementToken(token);

		return value;
	}

	std::optional<Value> deserializeNull(Token& token)
	{
		if (token.type() != TokenType::Null)
		{
			expectedError(token, "null");
			return {};
		}

		incrementToken(token);

		return Value();
	}
}
