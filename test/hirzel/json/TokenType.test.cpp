#include "hirzel/json/TokenType.hpp"

#include <cassert>
#include <cstring>
#include <sstream>

using namespace hirzel::json;

bool checkStream(TokenType tokenType, const char *text)
{
	auto out = std::ostringstream();

	out << tokenType;

	return out.str() == text;
}

int main()
{
	assert(!strcmp(tokenTypeName(TokenType::LeftBrace), "'{'"));
	assert(!strcmp(tokenTypeName(TokenType::RightBrace), "'}'"));
	assert(!strcmp(tokenTypeName(TokenType::LeftBracket), "'['"));
	assert(!strcmp(tokenTypeName(TokenType::RightBracket), "']'"));
	assert(!strcmp(tokenTypeName(TokenType::Comma), "','"));
	assert(!strcmp(tokenTypeName(TokenType::Colon), "':'"));
	assert(!strcmp(tokenTypeName(TokenType::String), "string"));
	assert(!strcmp(tokenTypeName(TokenType::Number), "number"));
	assert(!strcmp(tokenTypeName(TokenType::True), "true"));
	assert(!strcmp(tokenTypeName(TokenType::False), "false"));
	assert(!strcmp(tokenTypeName(TokenType::Null), "null"));
	assert(!strcmp(tokenTypeName(TokenType::EndOfFile), "end of file"));
	assert(!strcmp(tokenTypeName((TokenType)-1), "invalid token"));

	assert(checkStream(TokenType::LeftBrace, "'{'"));
	assert(checkStream(TokenType::RightBrace, "'}'"));
	assert(checkStream(TokenType::LeftBracket, "'['"));
	assert(checkStream(TokenType::RightBracket, "']'"));
	assert(checkStream(TokenType::Comma, "','"));
	assert(checkStream(TokenType::Colon, "':'"));
	assert(checkStream(TokenType::String, "string"));
	assert(checkStream(TokenType::Number, "number"));
	assert(checkStream(TokenType::True, "true"));
	assert(checkStream(TokenType::False, "false"));
	assert(checkStream(TokenType::Null, "null"));
	assert(checkStream(TokenType::EndOfFile, "end of file"));
	assert(checkStream((TokenType)-1, "invalid token"));
	
	return 0;
}
