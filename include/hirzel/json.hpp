#ifndef HIRZEL_JSON_HPP
#define HIRZEL_JSON_HPP

#include "hirzel/json/Value.hpp"
#include <functional>

namespace hirzel::json
{
	Value deserialize(const char* json);
	Value deserialize(const std::string& json);
	std::string serialize(const Value& value);
	void onError(std::function<void()>&& callback);
}

#endif
