#include <functional>
#include <string>

namespace hirzel::json
{
	bool hasErrorCallback();
	void onError(std::function<void(const char*)>&& callback);
	void pushError(const std::string& message);
	void pushError(const char* message);
}
