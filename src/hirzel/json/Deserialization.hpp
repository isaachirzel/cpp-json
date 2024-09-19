#ifndef HIRZEL_JSON_DESERIALIZER_HPP
#define HIRZEL_JSON_DESERIALIZER_HPP

#include "hirzel/json/Token.hpp"
#include "hirzel/json/Value.hpp"

namespace hirzel::json
{
	Value deserialize(const char *json);
	Value deserialize(const std::string& json);
	Value deserializeValue(Token& token);
	Value deserializeObject(Token& token);
	Value deserializeArray(Token& token);
	Value deserializeString(Token& token);
	Value deserializeNumber(Token& token);
	Value deserializeBoolean(Token& token);
	Value deserializeNull(Token& token);
}

#endif
