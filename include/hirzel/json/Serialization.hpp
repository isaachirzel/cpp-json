#ifndef HIRZEL_JSON_SERIALIZATION_HPP
#define HIRZEL_JSON_SERIALIZATION_HPP

#include "hirzel/json/Value.hpp"

#include <optional>

namespace hirzel::json
{
	std::string serialize(const Value& value);
	std::optional<std::string> serializeObject(const Value& value);
	std::optional<std::string> serializeArray(const Value& value);
	std::optional<std::string> serializeString(const Value& value);
	std::optional<std::string> serializeNumber(const Value& value);
	std::optional<std::string> serializeBoolean(const Value& value);
	std::optional<std::string> serializeNull(const Value& value);
}

#endif
