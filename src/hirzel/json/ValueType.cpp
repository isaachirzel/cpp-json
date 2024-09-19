#include "hirzel/json/ValueType.hpp"

namespace hirzel::json
{
	const char *valueTypeName(ValueType valueType)
	{
		switch (valueType)
		{
			case ValueType::Null:
				return "null";

			case ValueType::Number:
				return "number";

			case ValueType::Boolean:
				return "boolean";

			case ValueType::String:
				return "string";

			case ValueType::Array:
				return "array";

			case ValueType::Object:
				return "object";

			default:
				break;
		}

		return "invalid-type";
	}

	std::ostream& operator<<(std::ostream& out, ValueType valueType)
	{
		const auto* text = valueTypeName(valueType);

		out << text;

		return out;
	}
}
