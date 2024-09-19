#ifndef HIRZEL_JSON_DESERIALIZER_HPP
#define HIRZEL_JSON_DESERIALIZER_HPP

#include "hirzel/json/Token.hpp"
#include "hirzel/json/Value.hpp"
#include <optional>

namespace hirzel::json
{
	std::optional<Value> deserialize(const char *json);
	std::optional<Value> deserialize(const std::string& json);
	std::optional<Value> deserializeValue(Token& token);
	std::optional<Value> deserializeObject(Token& token);
	std::optional<Value> deserializeArray(Token& token);
	std::optional<Value> deserializeString(Token& token);
	std::optional<Value> deserializeNumber(Token& token);
	std::optional<Value> deserializeBoolean(Token& token);
	std::optional<Value> deserializeNull(Token& token);
}

#endif
