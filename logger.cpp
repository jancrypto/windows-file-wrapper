#include "wrapper.h"


std::vector<std::string> std::split(std::string str, std::string token) {
    std::vector<std::string> result;
    while (str.size()) {
        int index = str.find(token);
        if (index != std::string::npos) {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)result.push_back(str);
        }
        else {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}



std::string std::replace(std::string original, std::string toReplace, std::string replaceWith)
{
    size_t start_pos = 0;
    while ((start_pos = original.find(toReplace, start_pos)) != std::string::npos) {
        original.replace(start_pos, toReplace.length(), replaceWith);
        start_pos += replaceWith.length(); // Handles case where 'to' is a substring of 'from'
    }
    return original;
}



void logger::log(std::string value, const char* file, int line) {
	std::string str = file;

    std::vector<std::string> vec = std::split(str, "\\");


	std::cout << "[" << vec.at(vec.size() - 1) << ":" << line << "] " << value << std::endl;
}