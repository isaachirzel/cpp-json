#include <functional>
#include <string>

namespace hirzel::json
{
	bool hasErrorCallback();
	void onError(std::function<void(const char*)>&& callback);
	void error(const std::string& message);
	void error(const char* message);
}
