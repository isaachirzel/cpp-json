#include "hirzel/json/Error.hpp"

namespace hirzel::json
{
	static std::function<void(const char* message)> _callback;

	bool hasErrorCallback()
	{
		return !!_callback;
	}

	void onError(std::function<void(const char*)>&& callback)
	{
		_callback = std::move(callback);
	}

	void pushError(const std::string& message)
	{
		_callback(message.c_str());
	}

	void pushError(const char *message)
	{
		_callback(message);
	}
}
