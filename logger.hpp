#pragma once

#define LogA logger::log

namespace logger {

	void log(std::string, const char* file = __builtin_FILE(), int line = __builtin_LINE());
}

namespace std {
	std::string replace(std::string original, std::string toReplace, std::string replaceWith);
	std::vector<std::string> split(std::string str, std::string token);
}