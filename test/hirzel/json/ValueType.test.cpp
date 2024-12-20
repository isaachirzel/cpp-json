#include "hirzel/json/ValueType.hpp"

#include <cassert>
#include <cstring>
#include <sstream>

using namespace hirzel::json;

bool checkStream(ValueType valueType, const char *text)
{
	auto out = std::ostringstream();

	out << valueType;

	return out.str() == text;
}


int main()
{
	assert(!strcmp(valueTypeName(ValueType::Null), "null"));
	assert(!strcmp(valueTypeName(ValueType::Number), "number"));
	assert(!strcmp(valueTypeName(ValueType::Boolean), "boolean"));
	assert(!strcmp(valueTypeName(ValueType::String), "string"));
	assert(!strcmp(valueTypeName(ValueType::Array), "array"));
	assert(!strcmp(valueTypeName(ValueType::Object), "object"));
	assert(!strcmp(valueTypeName((ValueType)-1), "invalid value"));

	assert(checkStream(ValueType::Null, "null"));
	assert(checkStream(ValueType::Number, "number"));
	assert(checkStream(ValueType::Boolean, "boolean"));
	assert(checkStream(ValueType::String, "string"));
	assert(checkStream(ValueType::Array, "array"));
	assert(checkStream(ValueType::Object, "object"));
	assert(checkStream((ValueType)-1, "invalid value"));

	return 0;
}
