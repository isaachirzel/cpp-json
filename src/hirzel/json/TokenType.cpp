#include "hirzel/json/TokenType.hpp"

namespace hirzel::json
{
	const char* tokenTypeName(TokenType tokenType)
	{
		switch (tokenType)
		{
			case TokenType::LeftBrace:
				return "'{'";

			case TokenType::RightBrace:
				return "'}'";

			case TokenType::LeftBracket:
				return "'['";

			case TokenType::RightBracket:
				return "']'";

			case TokenType::Comma:
				return "','";

			case TokenType::Colon:
				return "':'";

			case TokenType::String:
				return "string";

			case TokenType::Number:
				return "number";

			case TokenType::True:
				return "true";

			case TokenType::False:
				return "false";

			case TokenType::Null:
				return "null";

			case TokenType::EndOfFile:
				return "end of file";
			
			default:
				break;
		}

		return "invalid token";
	}

	std::ostream& operator<<(std::ostream& out, TokenType tokenType)
	{
		const auto* text = tokenTypeName(tokenType);

		out << text;

		return out;
	}
}