#ifndef HIRZEL_JSON_TOKEN_HPP
#define HIRZEL_JSON_TOKEN_HPP

#include "hirzel/json/TokenType.hpp"

#include <string>

namespace hirzel::json
{
	class Token
	{
		const char* _src;
		size_t _pos;
		size_t _length;
		TokenType _type;

	private:

		Token(const char* src, size_t pos, size_t length, TokenType type);

		static Token parse(const char *src, size_t pos);
		static Token parseString(const char *src, size_t pos);
		static Token parseNumber(const char *src, size_t pos);
		static Token parseTrue(const char *src, size_t pos);
		static Token parseFalse(const char *src, size_t pos);
		static Token parseNull(const char *src, size_t pos);

	public:

		Token(Token&&) = default;
		Token(const Token&) = default;

		static Token initialFor(const char* src);

		void seekNext();
		std::string text() const;

		const auto* src() const { return _src; }
		const auto& pos() const { return _pos; }
		const auto& length() const { return _length; }
		const auto& type() const { return _type; }
	};
}

#endif
