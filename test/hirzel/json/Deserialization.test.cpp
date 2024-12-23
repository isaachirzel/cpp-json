#include "hirzel/json/Deserialization.hpp"
#include "hirzel/json/ValueType.hpp"

#include <cassert>

using namespace hirzel::json;

bool confirmDeserialization(const char* json, ValueType valueType)
{
	auto value = deserialize(json);

	if (!value)
	{
		std::cerr << "failed to deserialize value\n";
		return false;
	}

	if (value->type() != valueType)
	{
		std::cerr << "deserialzation: expected type " << valueType << ", got " << value->type() << "\n";
		return false;
	}

	return true;
}



void testNull()
{
	assert(confirmDeserialization("null", ValueType::Null));
}

void testNumber()
{
	assert(confirmDeserialization("123", ValueType::Number));
	assert(deserialize("123")->number() == 123);
	assert(confirmDeserialization("123.456", ValueType::Number));
	assert(deserialize("123.456")->number() == 123.456);
}

void testBoolean()
{
	assert(confirmDeserialization("true", ValueType::Boolean));
	assert(confirmDeserialization("false", ValueType::Boolean));
	assert(deserialize("true")->boolean() == true);
	assert(deserialize("false")->boolean() == false);
}

void testString()
{
	assert(confirmDeserialization("\"\"", ValueType::String));
	assert(deserialize("\"\"")->string() == "");
	assert(confirmDeserialization("\"abc\"", ValueType::String));
	assert(deserialize("\"abc\"")->string() == "abc");
}

bool confirmArray(const char *json, const std::vector<ValueType>& elementTypes)
{
	auto arr = deserialize(json);

	if (!arr)
	{
		std::cerr << "failed to deserialize array\n";
		return false;
	}

	if (arr->type() != ValueType::Array)
	{
		std::cerr << "value is not an array\n";
		return false;
	}

	if (arr->length() != elementTypes.size())
	{
		std::cerr << "array length: expected " << elementTypes.size() << ", got " << arr->length() << "\n";
		return false;
	}

	size_t i = 0;

	for (const auto& elementType : elementTypes)
	{
		auto element = arr->at(i);

		if (!element)
		{
			std::cerr << "array does not have element " << i << "\n";
			return false;
		}

		if (element->type() != elementType)
		{
			std::cerr << "element " << i << " type: expected " << elementType << ", got " << element->type() << "\n";
			return false;
		}

		i += 1;
	}

	return true;
}

void testArray()
{
	assert(confirmArray("[]", {}));
	assert(confirmArray("[1, true, null, \"abc\", [], {}]", {
		ValueType::Number,
		ValueType::Boolean,
		ValueType::Null,
		ValueType::String,
		ValueType::Array,
		ValueType::Object,
	}));
}

bool confirmObject(const char *json, const std::unordered_map<std::string, ValueType>& elementTypes)
{
	auto obj = deserialize(json);

	if (!obj)
	{
		std::cerr << "failed to deserialize object\n";
		return false;
	}

	if (obj->type() != ValueType::Object)
	{
		std::cerr << "expected object, got " << obj->type() << "\n";
		return false;
	}

	for (const auto& item : elementTypes)
	{
		auto element = obj->at(item.first);

		if (!element)
		{
			std::cerr << "element " << item.first << " does not exist\n";
			return false;
		}

		if (element->type() != item.second)
		{
			std::cerr << "element " << item.first << ": expected " << item.second << ", got " << element->type() << "\n";
			return false;
		}
	}

	return true;
}

void testObject()
{
	assert(confirmObject("{}", {}));
	assert(confirmObject(R"(
		{
			"a": 1,
			"b": true,
			"c": null,
			"d": "abc",
			"e": [],
			"f": {}
		}
	)",
	{
		{ "a", ValueType::Number },
		{ "b", ValueType::Boolean },
		{ "c", ValueType::Null },
		{ "d", ValueType::String },
		{ "e", ValueType::Array },
		{ "f", ValueType::Object }
	}));
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