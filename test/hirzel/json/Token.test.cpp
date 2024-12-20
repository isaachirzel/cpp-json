#include "hirzel/json/Token.hpp"
#include "hirzel/json/TokenType.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

// const char *testSrc1 = R"%(
// 	{}
// )%";

using namespace hirzel::json;

bool confirmToken(const std::optional<Token>& token, TokenType tokenType, const char* text, const char *src, size_t index, size_t length)
{
	if (!token)
	{
		std::cerr << "token result is not valid\n";
		return false;
	}

	auto success = true;

	if (token->type() != tokenType)
	{
		std::cerr << "Incorrect token type\n";
		success = false;
	}

	if (token->text() != text)
	{
		std::cerr << "Incorrect token text\n";
		success = false;
	}

	if (token->src() != src)
	{
		std::cerr << "Incorrect src\n";
		success = false;
	}

	if (token->index() != index)
	{
		std::cerr << "Incorrect index";
		success = false;
	}

	if (token->length() != length)
	{
		std::cerr << "Incorrect length\n";
		success = false;
	}

	return success;
}

bool confirmEndOfFile(const Token& token)
{
	auto next = token.parseNext();
	
	return confirmToken(next, TokenType::EndOfFile, "", token.src(), strlen(token.src()), 0);
}

bool confirmStandaloneToken(const char *src, TokenType tokenType)
{
	auto token = Token::parse(src);

	if (!confirmToken(token, tokenType, src, src, 0, strlen(src)))
		return false;

	if (!confirmEndOfFile(*token))
		return false;

	return true;
}

bool confirmText(const char* src, const std::vector<TokenType>& types)
{
	auto token = Token::parse(src);
	size_t i = 0;
	auto success = true;

	for (const auto& type : types)
	{
		if (!token)
		{
			std::cerr << "token " << i << " failed to parse\n";
			success = false;
		}

		if (token->type() != type)
		{
			std::cerr << "Token " << i << " is not correct type\n";
			success = false;
		}

		token = token->parseNext();
		i += 1;
	}

	if (token->type() != TokenType::EndOfFile)
	{
		std::cerr << "Expected end of file, got " << token->type() << "\n";
		success = false;
	}

	return success;
}

void testString()
{
	assert(confirmStandaloneToken("\"hello\"", TokenType::String));
}

void testNumber()
{
	assert(confirmStandaloneToken("123", TokenType::Number));
	assert(confirmStandaloneToken("123.456", TokenType::Number));
}

void testTrue()
{
	assert(confirmStandaloneToken("true", TokenType::True));
}

void testFalse()
{
	assert(confirmStandaloneToken("false", TokenType::False));
}

void testNull()
{
	assert(confirmStandaloneToken("null", TokenType::Null));
}

void testEmpty()
{
	assert(confirmStandaloneToken("", TokenType::EndOfFile));
}

void testColon()
{
	assert(confirmStandaloneToken(":", TokenType::Colon));
}

void testComma()
{
	assert(confirmStandaloneToken(",", TokenType::Comma));
}

void testLeftBracket()
{
	assert(confirmStandaloneToken("[", TokenType::LeftBracket));
}

void testRightBracket()
{
	assert(confirmStandaloneToken("]", TokenType::RightBracket));
}

void testLeftBrace()
{
	assert(confirmStandaloneToken("{", TokenType::LeftBrace));
}

void testRightBrace()
{
	assert(confirmStandaloneToken("}", TokenType::RightBrace));
}

void testText()
{
	assert(confirmText(R"(
		[ 123, "hello", false, true, null ]
	)",
	{
		TokenType::LeftBracket,
		TokenType::Number,
		TokenType::Comma,
		TokenType::String,
		TokenType::Comma,
		TokenType::False,
		TokenType::Comma,
		TokenType::True,
		TokenType::Comma,
		TokenType::Null,
		TokenType::RightBracket
	}));

	assert(confirmText(R"(
		{
			"a": 123.456,
			"b": null,
			"c": false,
			"d": true,
			"e": "text"
		}
	)",
	{
		TokenType::LeftBrace,
		TokenType::String,
		TokenType::Colon,
		TokenType::Number,
		TokenType::Comma,
		TokenType::String,
		TokenType::Colon,
		TokenType::Null,
		TokenType::Comma,
		TokenType::String,
		TokenType::Colon,
		TokenType::False,
		TokenType::Comma,
		TokenType::String,
		TokenType::Colon,
		TokenType::True,
		TokenType::Comma,
		TokenType::String,
		TokenType::Colon,
		TokenType::String,
		TokenType::RightBrace
	}));
}

int main()
{
	testString();
	testNumber();
	testTrue();
	testFalse();
	testNull();
	testEmpty();
	testColon();
	testComma();
	testLeftBracket();
	testRightBracket();
	testLeftBrace();
	testRightBrace();
	testText();

	return 0;
}
