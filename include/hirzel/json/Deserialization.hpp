#ifndef HIRZEL_JSON_DESERIALIZATION_HPP
#define HIRZEL_JSON_DESERIALIZATION_HPP

#include "hirzel/json/Token.hpp"
#include "hirzel/json/Value.hpp"

#include <optional>

namespace hirzel::json
{
	std::optional<Value> deserialize(const char *json);
	std::optional<Value> deserialize(const std::string& json);
}

#endif
