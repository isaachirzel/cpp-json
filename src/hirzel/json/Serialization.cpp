#include "hirzel/json/Serialization.hpp"
#include <string>

namespace hirzel::json
{
	std::string serialize(const Value& value)
	{
		switch (value.type())
		{
			case ValueType::Null:
				return serializeNull(value);

			case ValueType::Boolean:
				return serializeBoolean(value);

			case ValueType::Number:
				return serializeNumber(value);

			case ValueType::String:
				return serializeString(value);

			case ValueType::Array:
				return serializeArray(value);

			case ValueType::Object:
				return serializeObject(value);

			default:
				break;
		}

		abort();
	}

	std::string serializeObject(const Value& value)
	{
		assert(value.isObject());

		const auto& object = value.object();

		if (object.empty())
		{
			return "{}";
		}

		auto text = std::string();

		text += '{';

		auto isFirst = true;

		for (const auto& pair : object)
		{
			if (isFirst)
			{
				isFirst = false;
			}
			else
			{
				text += ',';
			}

			text += '\"';
			text += pair.first;
			text += "\":";

			auto itemText = serialize(pair.second);

			text += itemText;
		}

		text += '}';

		return text;
	}

	std::string serializeArray(const Value& value)
	{
		assert(value.isArray());

		const auto& array = value.array();

		if (array.empty())
		{
			return "[]";
		}

		auto text = std::string();

		text += '[';

		auto isFirst = true;

		for (const auto& item : array)
		{
			if (isFirst)
			{
				isFirst = false;
			}
			else
			{
				text += ',';
			}

			auto itemText = serialize(item);

			text += itemText;
		}

		text += ']';

		return text;
	}

	std::string serializeString(const Value& value)
	{
		// TODO: Escaping text
		auto text = std::string();
		auto valueText = value.string();

		text.reserve(valueText.length() + 2);

		text += '\"';
		text += valueText;
		text += '\"';

		return text;
	}

	std::string serializeNumber(const Value& value)
	{
		assert(value.isNumber());

		auto number = value.number();
		auto text = std::to_string(number);

		return text;
	}

	std::string serializeBoolean(const Value& value)
	{
		assert(value.isBoolean());

		const auto* text = value.boolean()
			? "true"
			: "false";

		return text;
	}

	std::string serializeNull(const Value& value)
	{
		assert(value.isNull());

		return "null";
	}
}
