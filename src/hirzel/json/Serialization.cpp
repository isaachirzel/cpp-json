#include "hirzel/json/Serialization.hpp"
#include "hirzel/json/Error.hpp"

#include <string>

namespace hirzel::json
{
	std::string serializeNull(const Value& value);
	std::string serializeNumber(const Value& value);
	std::string serializeBoolean(const Value& value);
	std::string serializeString(const Value& value);
	std::string serializeArray(const Value& value);
	std::string serializeObject(const Value& value);

	std::string serialize(const Value& value)
	{
		switch (value.type())
		{
			case ValueType::Null:
				return serializeNull(value);

			case ValueType::Number:
				return serializeNumber(value);

			case ValueType::Boolean:
				return serializeBoolean(value);

			case ValueType::String:
				return serializeString(value);

			case ValueType::Array:
				return serializeArray(value);

			case ValueType::Object:
				return serializeObject(value);
			
			default:
				break;
		}

		return "";
	}

	std::string serializeObject(const Value& value)
	{
		assert(value.isObject());

		const auto& object = value.object();

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
		auto text = std::string();

		text += '[';

		for (size_t i = 0; i < array.size(); ++i)
		{
			if (i > 0)
			{
				text += ',';
			}

			const auto& item = array[i];
			auto itemText = serialize(item);

			text += itemText;
		}

		text += ']';

		return text;
	}

	std::string serializeString(const Value& value)
	{
		assert(value.isString());
		
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
