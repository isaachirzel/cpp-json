#include "hirzel/json/Token.hpp"
#include "hirzel/json/TokenType.hpp"
#include "hirzel/json/Error.hpp"

#include <cstring>
#include <string>
#include <cctype>

namespace hirzel::json
{
	static void parseError(const char *subject, const size_t index, const char *message)
	{
		if (!hasErrorCallback())
			return;

		auto error = std::string();

		error += "Unable to parse ";
		error += subject;
		error += " at index ";
		error += std::to_string(index);
		error += ": ";
		error += message;

		pushError(error);
	}

	Token::Token(const char* src, size_t index, size_t length, TokenType type):
		_src(src),
		_index(index),
		_length(length),
		_type(type)
	{}

	static size_t getEndOfLineCommentIndex(const char* src, size_t index)
	{
		size_t i;

		for (i = index; src[i]; ++i)
		{
			if (src[i] == '\n')
			{
				i += 1;
				break;
			}
		}

		return i;
	}

	static size_t getEndOfBlockCommentIndex(const char* src, size_t index)
	{
		size_t i;

		for (i = index; src[i]; ++i)
		{
			if (src[i] == '*' && src[i + 1] == '/')
			{
				i += 2;
				break;
			}
		}

		return i;
	}

	static size_t getNextTokenIndex(const char* src, size_t index)
	{
		size_t i;

		for (i = index; src[i]; ++i)
		{
			auto c = src[i];

			if (c <= ' ')
				continue;

			if (c == '/')
			{
				switch (src[i + 1])
				{
					case '/':
						i = getEndOfLineCommentIndex(src, i + 2) - 1;
						continue;

					case '*':
						i = getEndOfBlockCommentIndex(src, i + 2) - 1;
						continue;

					default:
						break;
				}
			}

			break;
		}

		return i;
	}

	std::optional<Token> Token::parse(const char* src, const size_t index)
	{
		auto c = src[index];

		switch (c)
		{
			case '\0':
				return Token(src, index, 0, TokenType::EndOfFile);

			case '{':
				return Token(src, index, 1, TokenType::LeftBrace);

			case '}':
				return Token(src, index, 1, TokenType::RightBrace);

			case '[':
				return Token(src, index, 1, TokenType::LeftBracket);

			case ']':
				return Token(src, index, 1, TokenType::RightBracket);

			case ',':
				return Token(src, index, 1, TokenType::Comma);

			case ':':
				return Token(src, index, 1, TokenType::Colon);

			case '\"':
				return Token::parseString(src, index);

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
				return Token::parseNumber(src, index);

			case 't':
				return Token::parseTrue(src, index);

			case 'f':
				return Token::parseFalse(src, index);

			case 'n':
				return Token::parseNull(src, index);

			default:
				break;
		}

		parseError("token", index, "Invalid character.");
		return {};
	}

	std::optional<Token> Token::parseString(const char* src, const size_t startIndex)
	{
		if (src[startIndex] != '\"')
		{
			parseError("string", startIndex, "String must begin with '\"'.");
			return {};
		}

		size_t i = startIndex + 1;

		while (true)
		{
			if (src[i] == '\"')
				break;

			if (src[i] == '\0')
			{
				parseError("string", startIndex, "String is unterminated.");
				return {};
			}

			i += 1;
		}

		i += 1;

		return Token(src, startIndex, i - startIndex, TokenType::String);
	}

	static size_t numberLength(const char* const src)
	{
		const char *iter = src;

		while (isdigit(*iter))
			iter += 1;

		return iter - src;
	}

	std::optional<Token> Token::parseNumber(const char* src, const size_t start)
	{
		auto i = start;

		if (src[i] == '-')
		{
			i += 1;

			if (!isdigit(src[i]))
			{
				parseError("number", start, "A number must follow '-'.");
				return {};
			}
		}

		i += numberLength(&src[i]);

		if (src[i] == '.')
		{
			i += 1;

			auto fractionLength = numberLength(&src[i]);

			if (fractionLength == 0)
			{
				parseError("number", start, "A number must follow the decimal point.");
			}

			i += fractionLength;
		}

		switch (src[i])
		{
		case '.':
			parseError("number", start, "Invalid number format.");
			return {};

		case 'e':
		case 'E':
		{
			i += 1;

			auto exponentLength = numberLength(&src[i]);

			if (exponentLength == 0)
			{
				parseError("number", start, "Exponent is missing.");
				return {};
			}

			i += exponentLength;

			if (src[i] == '.')
			{
				parseError("number", start, "Exponents must be integers.");
				return {};
			}
			break;
		}

		default:
			break;
		}

		auto length = i - start;
		auto token =  Token(src, start, length, TokenType::Number);

		return token;
	}

	static bool parseKeyword(const char* src, const size_t startIndex, const char* keyword, const size_t keywordLength)
	{
		auto endIndex = startIndex;

		while (isalpha(src[endIndex]))
			endIndex += 1;

		const auto length = endIndex - startIndex;

		if (length != keywordLength || strncmp(keyword, &src[startIndex], keywordLength))
		{
			parseError(keyword, startIndex, "Invalid keyword.");
			return false;
		}
		
		return true;
	}

	std::optional<Token> Token::parseTrue(const char* src, const size_t startIndex)
	{
		const size_t length = 4;

		if (!parseKeyword(src, startIndex, "true", length))
			return {};

		return Token(src, startIndex, length, TokenType::True);
	}

	std::optional<Token> Token::parseFalse(const char* src, const size_t startIndex)
	{
		const size_t length = 5;

		if (!parseKeyword(src, startIndex, "false", length))
			return {};

		return Token(src, startIndex, length, TokenType::False);
	}

	std::optional<Token> Token::parseNull(const char* src, const size_t startIndex)
	{
		const size_t length = 4;

		if (!parseKeyword(src, startIndex, "null", length))
			return {};

		return Token(src, startIndex, length, TokenType::Null);
	}

	std::optional<Token> Token::parse(const char* src)
	{
		auto index = getNextTokenIndex(src, 0);
		auto token = parse(src, index);

		return token;
	}

	std::optional<Token> Token::parseNext() const
	{
		auto index = getNextTokenIndex(_src, _index + _length);
		auto token = parse(_src, index);

		return token;
	}

	std::string Token::text() const
	{
		return std::string(&_src[_index], _length);
	}
}
