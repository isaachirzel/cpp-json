#ifndef HIRZEL_JSON_SERIALIZATION_HPP
#define HIRZEL_JSON_SERIALIZATION_HPP

#include "hirzel/json/Value.hpp"

namespace hirzel::json
{
	std::string serialize(const Value& value);
}

#endif
