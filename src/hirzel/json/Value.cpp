#include "hirzel/json.hpp"
#include "hirzel/json/Serialization.hpp"
#include "hirzel/json/ValueType.hpp"
#include "hirzel/json/Value.hpp"

#include <cstdlib>
#include <stdexcept>
#include <string>

namespace hirzel::json
{
	Value::Value() :
		_type(ValueType::Null),
		_number(0)
	{}

	Value::Value(ValueType type) :
		_type(type),
		_number(0)
	{
		switch (type)
		{
			case ValueType::Number:
				_number = 0.0;
				break;

			case ValueType::Boolean:
				_boolean = false;
				break;

			case ValueType::String:
				_string = new std::string();
				break;

			case ValueType::Array:
				_array = new Array();
				break;

			case ValueType::Object:
				_object = new Object();
				break;

			default:
				break;
		}
	}

	Value::Value(short i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(int i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(long i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(long long i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(unsigned short i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(unsigned int i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(unsigned long i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(unsigned long long i) :
		_type(ValueType::Number),
		_number(i)
	{}

	Value::Value(float d) :
		_type(ValueType::Number),
		_number(d)
	{}

	Value::Value(double d) :
		_type(ValueType::Number),
		_number(d)
	{}

	Value::Value(bool b) :
		_type(ValueType::Boolean),
		_boolean(b)
	{}

	Value::Value(std::string&& s) :
		_type(ValueType::String),
		_string(new std::string(std::move(s)))
	{}

	Value::Value(const std::string& s) :
		_type(ValueType::String),
		_string(new std::string(s))
	{}

	Value::Value(char* s) :
		_type(ValueType::String),
		_string(new std::string(s))
	{}

	Value::Value(const char* s) :
		_type(ValueType::String),
		_string(new std::string(s))
	{}

	Value::Value(Array&& array) :
		_type(ValueType::Array),
		_array(new Array(std::move(array)))
	{}

	Value::Value(const Array& array) :
		_type(ValueType::Array),
		_array(new Array(array))
	{}

	Value::Value(Object&& object) :
		_type(ValueType::Object),
		_object(new Object(std::move(object)))
	{}

	Value::Value(const Object& object) :
		_type(ValueType::Object),
		_object(new Object(object))
	{}

	Value::Value(Value&& other) noexcept :
		_type(other._type),
		_number(0)
	{
		switch (_type)
		{
		case ValueType::Null:
			break;

		case ValueType::Number:
			_number = other._number;
			break;

		case ValueType::Boolean:
			_boolean = other._boolean;
			break;

		case ValueType::String:
			_string = other._string;
			other._string = nullptr;
			break;

		case ValueType::Array:
			_array = other._array;
			other._array = nullptr;
			break;

		case ValueType::Object:
			_object = other._object;
			other._object = nullptr;
			break;
		}

		other._type = ValueType::Null;
	}

	Value::Value(const Value& other) :
		_type(other._type),
		_number(0)
	{
		switch (_type)
		{
		case ValueType::Null:
			_number = 0;
			break;

		case ValueType::Number:
			_number = other._number;
			break;

		case ValueType::Boolean:
			_boolean = other._boolean;
			break;

		case ValueType::String:
			_string = new std::string(*other._string);
			break;

		case ValueType::Array:
			_array = new Array(*other._array);
			break;

		case ValueType::Object:
			_object = new Object(*other._object);
			break;
		}
	}

	Value::~Value()
	{
		switch (_type)
		{
		case ValueType::String:
			delete _string;
			break;

		case ValueType::Array:
			delete _array;
			break;

		case ValueType::Object:
			delete _object;
			break;

		default:
			break;
		}
	}

	Value& Value::operator=(Value&& other)
	{
		new (this) auto(std::move(other));

		return *this;
	}

	Value& Value::operator=(const Value& other)
	{
		new (this) auto(other);

		return *this;
	}

	Value *Value::at(const std::string& key)
	{
		if (_type != ValueType::Object)
			return nullptr;

		auto iter = _object->find(key);
		auto *ptr = iter != _object->end()
			? &iter->second
			: nullptr;

		return ptr;
	}

	const Value *Value::at(const std::string& key) const
	{
		if (_type != ValueType::Object)
			return nullptr;

		auto iter = _object->find(key);
		auto *ptr = iter != _object->end()
			? &iter->second
			: nullptr;

		return ptr;
	}

	Value *Value::at(size_t i)
	{
		if (_type != ValueType::Array || i >= _array->size())
			return nullptr;

		return &(*_array)[i];
	}

	const Value *Value::at(size_t i) const
	{
		if (_type != ValueType::Array || i >= _array->size())
			return nullptr;

		return &(*_array)[i];
	}

	Value& Value::operator[](size_t i)
	{
		if (_type != ValueType::Array)
			throw std::runtime_error("Value is not an array.");

		if (i >= _array->size())
			throw std::runtime_error("Index " + std::to_string(i) + " is out of bounds.");

		return (*_array)[i];
	}

	const Value& Value::operator[](size_t i) const
	{
		if (_type != ValueType::Array)
			throw std::runtime_error("Value is not an array.");

		if (i >= _array->size())
			throw std::runtime_error("Index " + std::to_string(i) + " is out of bounds.");

		return (*_array)[i];
	}

	Value& Value::operator[](const std::string& key)
	{
		if (_type != ValueType::Object)
			throw std::runtime_error("Value is not an object.");

		auto iter = _object->find(key);

		if (iter == _object->end())
			throw std::runtime_error("No member with key '" + key + "' exists.");

		return iter->second;
	}

	const Value& Value::operator[](const std::string& key) const
	{
		if (_type != ValueType::Object)
			throw std::runtime_error("Value is not an object.");

		auto iter = _object->find(key);

		if (iter == _object->end())
			throw std::runtime_error("No member with key '" + key + "' exists.");

		return iter->second;
	}

	int64_t Value::asInteger() const
	{
		switch (_type)
		{
		case ValueType::Number:
			return (int64_t)_number;

		case ValueType::Boolean:
			return (int64_t)_boolean;

		case ValueType::String:
			try
			{
				return std::stoll(*_string);
			}
			catch (const std::exception&)
			{
				break;
			}

		default:
			break;
		}

		return 0;
	}

	double Value::asDecimal() const
	{
		switch (_type)
		{
		case ValueType::Number:
			return _number;

		case ValueType::Boolean:
			return (double)_boolean;
			
		case ValueType::String:
			try
			{
				return std::stod(*_string);
			}
			catch (const std::exception&)
			{
				break;
			}

		default:
			break;
		}

		return 0.0;
	}

	bool Value::asBoolean() const
	{
		switch (_type)
		{
		case ValueType::Number:
			return (bool)_number;

		case ValueType::Boolean:
			return _boolean;

		case ValueType::String:
			return !_string->empty();

		default:
			return false;
		}
	}

	std::string Value::asString() const
	{
		if (_type == ValueType::String)
			return *_string;

		return serialize(*this);
	}

	bool Value::isEmpty() const
	{
		switch (_type)
		{
		case ValueType::String:
			return _string->empty();

		case ValueType::Array:
			return _array->empty();

		case ValueType::Object:
			return _object->empty();

		case ValueType::Null:
			return true;

		default:
			return false;
		}
	}

	size_t Value::length() const
	{
		switch (_type)
		{
		case ValueType::String:
			return _string->length();

		case ValueType::Array:
			return _array->size();

		case ValueType::Object:
			return _object->size();

		default:
			return 0;
		}
	}

	bool Value::operator==(const Value& other) const
	{

		if (_type != other.type())
			return false;

		switch (_type)
		{
		case ValueType::Null:
			return true;

		case ValueType::Number:
			return _number == other.number();

		case ValueType::Boolean:
			return _boolean == other.boolean();

		case ValueType::String:
			return *_string == other.string();

		case ValueType::Array:
		{
			const auto& arr = *_array;
			const auto& oarr = other.array();

			if (arr.size() != oarr.size())
				return false;

			for (size_t i = 0; i < arr.size(); ++i)
			{
				if (arr[i] != oarr[i])
					return false;
			}

			return true;
		}

		case ValueType::Object:
		{
			const auto& aTable = *_object;
			const auto& bTable = other.object();

			if (aTable.size() != bTable.size())
				return false;

			for (auto& aPair : aTable)
			{
				auto iter = bTable.find(aPair.first);

				if (iter == bTable.end())
					return false;

				auto& bPair = *iter;

				if (aPair.second != bPair.second)
					return false;
			}

			return true;
		}
		}

		return false;
	}

	std::ostream& operator<<(std::ostream& out, const Value& json)
	{
		auto text = json.asString();

		out << text;

		return out;
	}
}
