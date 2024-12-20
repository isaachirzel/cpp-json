#include "hirzel/json/Token.hpp"

#include <cassert>
#include <cstring>
#include <iostream>

// const char *testSrc1 = R"%(
// 	{}
// )%";

using namespace hirzel::json;

void confirmEndOfFile(const Token& token)
{
	auto next = token.parseNext();
	
	assert(next);
	assert(next->type() == TokenType::EndOfFile);
	assert(next->text() == "");
	assert(next->src() == token.src());
	assert(next->length() == 0);
	assert(next->index() == strlen(token.src()));
}

void testString1()
{
	const auto* src = "\"hello\"";
	auto result = Token::parse(src);

	assert(result);

	auto token = *result;

	assert(token.text() == src);
	assert(token.src() == src);
	assert(token.index() == 0);
	assert(token.length() == strlen(src));
	assert(token.type() == TokenType::String);

	confirmEndOfFile(token);
}

void testNumber1()
{
	const auto* src = "123";
	auto result = Token::parse(src);

	assert(result);

	auto token = *result;

	assert(token.text() == src);
	assert(token.src() == src);
	assert(token.index() == 0);
	assert(token.length() == strlen(src));
	assert(token.type() == TokenType::Number);
	
	confirmEndOfFile(token);
}

void testTrue1()
{
	const auto* src = "true";

	auto token = Token::parse(src);

	assert(token);
	assert(token->src() == src);
	assert(token->index() == 0);
	assert(token->length() == 4);
	assert(token->type() == TokenType::True);
}

void testFalse1()
{
	const auto* src = "false";

	auto token = Token::parse(src);

	assert(token);
	assert(token->src() == src);
	assert(token->index() == 0);
	assert(token->length() == 5);
	assert(token->type() == TokenType::False);
}

void testNull1()
{
	const auto* src = "null";

	auto token = Token::parse(src);

	assert(token);
	assert(token->src() == src);
	assert(token->index() == 0);
	assert(token->length() == 4);
	assert(token->type() == TokenType::Null);
}


void testEmpty1()
{
	const auto* src = "";
	auto token = Token::parse(src);
	
	assert(token);
	assert(token->type() == TokenType::EndOfFile);
	assert(token->text() == "");
	assert(token->src() == src);
	assert(token->length() == 0);
	assert(token->index() == strlen(src));

	confirmEndOfFile(*token);
}

int main()
{
	testString1();
	testNumber1();
	testTrue1();
	testFalse1();
	testNull1();
	testEmpty1();

	return 0;
}
