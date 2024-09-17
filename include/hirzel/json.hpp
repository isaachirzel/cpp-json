#ifndef HIRZEL_JSON_HPP
#define HIRZEL_JSON_HPP

#include "hirzel/json/Value.hpp"

namespace hirzel::json
{
	Value deserialize(const char* json);
	Value deserialize(const std::string& json);
	void serialize(std::ostream& out, const Value& json, bool minimized = false);
	std::string serialize(const Value& json, bool minimized = false);
}

#endif
