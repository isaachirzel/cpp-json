#include "hirzel/json/Serialization.hpp"

#include <cassert>

using namespace hirzel::json;

bool confirmSerialization(const Value& value, const char* text)
{
	auto success = true;

	if (serialize(value) != text)
	{
		success = false;
	}

	return success;
}

void testNull()
{
	assert(confirmSerialization(Value(), "null"));
}

void testNumber()
{
	assert(confirmSerialization(Value(123), "123.000000"));
	assert(confirmSerialization(Value(123.456), "123.456000"));
}

void testBoolean()
{
	assert(confirmSerialization(Value(true), "true"));
	assert(confirmSerialization(Value(false), "false"));
}

void testString()
{
	assert(confirmSerialization(Value("abc"), "\"abc\""));
}

void testArray()
{
	assert(confirmSerialization(Value::from(std::vector<int> { 1, 2, 3 }), "[1.000000,2.000000,3.000000]"));
}

void testObject()
{
	assert(confirmSerialization(Value::from(std::unordered_map<std::string, bool> {
		{ "yes", true },
		{ "no", false }
	}), "{\"yes\":true,\"no\":false}"));
}

int main()
{
	testNull();
	testNumber();
	testBoolean();
	testString();
	testArray();
	testObject();

	return 0;
}
