#ifndef HIRZEL_JSON_SERIALIZER_HPP
#define HIRZEL_JSON_SERIALIZER_HPP

#include "hirzel/json/Value.hpp"

namespace hirzel::json
{
	std::string serialize(const Value& value);
	std::string serializeObject(const Value& value);
	std::string serializeArray(const Value& value);
	std::string serializeString(const Value& value);
	std::string serializeNumber(const Value& value);
	std::string serializeBoolean(const Value& value);
	std::string serializeNull(const Value& value);
}

#endif
