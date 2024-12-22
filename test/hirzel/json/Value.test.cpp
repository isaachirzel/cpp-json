#include "hirzel/json/Value.hpp"
#include "hirzel/json/ValueType.hpp"
#include <unordered_map>
#include <vector>

using namespace hirzel::json;

bool confirmValue(const Value& value, ValueType valueType, int64_t intValue, double decimalValue, bool booleanValue, const char* stringValue)
{
	auto success = true;

	if (value.type() != valueType)
	{
		std::cerr << "value type: expected " << valueType << ", got " << value.type() << "\n";
	}

	if (value.asInteger() != intValue)
	{
		std::cerr << "integer conversion: expected " << intValue << ", got " << value.asInteger() << "\n";
		success = false;
	}

	if (value.asDecimal() != decimalValue)
	{
		std::cerr << "decimal conversion: expected " << decimalValue << ", got " << value.asDecimal() << "\n";
		success = false;
	}

	if (value.asBoolean() != booleanValue)
	{
		std::cerr << "bool conversion: expected " << (booleanValue ? "true" : "false") << ", got " << (value.asBoolean() ? "true" : "false") << "\n";
		success = false;
	}

	if (value.asString() != stringValue)
	{
		std::cerr << "string conversion expected " << stringValue << ", got " << value.asString() << "\n";
		success = false;
	}

	return success;
}

void testNull()
{
	assert(confirmValue(Value(), ValueType::Null, 0, 0.0, false, "null"));
}

void testNumber()
{
	assert(confirmValue(Value(1), ValueType::Number, 1, 1.0, true, "1.000000"));
	assert(confirmValue(Value(123.456), ValueType::Number, 123, 123.456, true, "123.456000"));
}

void testBoolean()
{
	assert(confirmValue(Value(true), ValueType::Boolean, 1, 1.0, true, "true"));
	assert(confirmValue(Value(false), ValueType::Boolean, 0, 0.0, false, "false"));
}

void testString()
{
	assert(confirmValue(Value(""), ValueType::String, 0, 0.0, false, ""));
	assert(confirmValue(Value("abcd"), ValueType::String, 0, 0.0, true, "abcd"));
	assert(confirmValue(Value("123"), ValueType::String, 123, 123.0, true, "123"));
	assert(confirmValue(Value("123.456"), ValueType::String, 123, 123.456, true, "123.456"));
}

void testArray()
{
	assert(confirmValue(Value::from(std::vector<int>{ 1, 2, 3 }), ValueType::Array, 0, 0.0, true, "[1.000000,2.000000,3.000000]"));
	assert(confirmValue(Value::from(std::vector<bool>{ true, false, true }), ValueType::Array, 0, 0.0, true, "[true,false,true]"));
}

void testObject()
{
	assert(confirmValue(Value::from(std::unordered_map<std::string, std::string> {
		{ "first", "abc" },
		{ "second", "123" }
	}), ValueType::Object, 0, 0, true, "{\"first\":\"abc\",\"second\":\"123\"}"));
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
