#ifndef HIRZEL_JSON_VALUE_TYPE_HPP
#define HIRZEL_JSON_VALUE_TYPE_HPP

#include <ostream>

namespace hirzel::json
{
	enum class ValueType: unsigned char
	{
		Null,
		Number,
		Boolean,
		String,
		Array,
		Object
	};
	
	const char *valueTypeName(ValueType valueType);
	
	std::ostream& operator<<(std::ostream& out, ValueType valueType);
}

#endif
