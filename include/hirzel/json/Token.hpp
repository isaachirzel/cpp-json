#ifndef HIRZEL_JSON_TOKEN_HPP
#define HIRZEL_JSON_TOKEN_HPP

#include "hirzel/json/TokenType.hpp"

#include <optional>
#include <string>

namespace hirzel::json
{
	class Token
	{
		const char* _src;
		size_t _index;
		size_t _length;
		TokenType _type;

	private:

		Token(const char* src, size_t index, size_t length, TokenType type);

		static std::optional<Token> parse(const char *src, size_t index);
		static std::optional<Token> parseString(const char *src, size_t index);
		static std::optional<Token> parseNumber(const char *src, size_t index);
		static std::optional<Token> parseTrue(const char *src, size_t index);
		static std::optional<Token> parseFalse(const char *src, size_t index);
		static std::optional<Token> parseNull(const char *src, size_t index);

	public:

		Token(Token&&) = default;
		Token(const Token&) = default;
		Token& operator=(Token&&) = default;
		Token& operator=(const Token&) = default;

		static std::optional<Token> parse(const char* src);
		std::optional<Token> parseNext() const;

		std::string text() const;

		const auto* src() const { return _src; }
		const auto& index() const { return _index; }
		const auto& length() const { return _length; }
		const auto& type() const { return _type; }
	};
}

#endif
