#include "hirzel/json/Serialization.hpp"
#include "hirzel/json/Error.hpp"

#include <string>

namespace hirzel::json
{
	static void serializationError(const Value& value, ValueType expectedType)
	{
		if (!hasErrorCallback())
			return;

		auto message = std::string();

		message += "Unable to serialize ";
		message += valueTypeName(value.type());
		message += " '' as ";
		message += valueTypeName(expectedType);

		pushError(message);
	}

	std::string serialize(const Value& value)
	{
		switch (value.type())
		{
			case ValueType::Null:
				return *serializeNull(value);

			case ValueType::Number:
				return *serializeNumber(value);

			case ValueType::Boolean:
				return *serializeBoolean(value);

			case ValueType::String:
				return *serializeString(value);

			case ValueType::Array:
				return *serializeArray(value);

			case ValueType::Object:
				return *serializeObject(value);
			
			default:
				break;
		}

		return "";
	}

	std::optional<std::string> serializeObject(const Value& value)
	{
		if (!value.isObject())
		{
			serializationError(value, ValueType::Object);
			return {};
		}

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

	std::optional<std::string> serializeArray(const Value& value)
	{
		if (!value.isArray())
		{
			serializationError(value, ValueType::Array);
			return {};
		}

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

	std::optional<std::string> serializeString(const Value& value)
	{
		if (!value.isString())
		{
			serializationError(value, ValueType::String);
			return {};
		}

		// TODO: Escaping text
		auto text = std::string();
		auto valueText = value.string();

		text.reserve(valueText.length() + 2);

		text += '\"';
		text += valueText;
		text += '\"';

		return text;
	}

	std::optional<std::string> serializeNumber(const Value& value)
	{
		if (!value.isNumber())
		{
			serializationError(value, ValueType::Number);
			return {};
		}

		auto number = value.number();
		auto text = std::to_string(number);

		return text;
	}

	std::optional<std::string> serializeBoolean(const Value& value)
	{
		if (!value.isBoolean())
		{
			serializationError(value, ValueType::Boolean);
			return {};
		}

		const auto* text = value.boolean()
			? "true"
			: "false";

		return text;
	}

	std::optional<std::string> serializeNull(const Value& value)
	{
		if (!value.isNull())
		{
			serializationError(value, ValueType::Null);
			return {};
		}

		return "null";
	}
}
