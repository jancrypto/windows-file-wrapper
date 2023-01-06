#pragma once

class File {

public:
	File(std::string path);

	// Getter

	std::string GetPath();
	std::string GetName();
private:
	std::string path;
	std::string name;
};

namespace filemanagment {

	bool create_directory(File f);

	HANDLE open_file(File f, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes);

	HANDLE create_file(File f);

	bool write_file(File f, std::string valuesToWrite);

	bool delte_file(File f);

	bool move_file(File f, File newFile);

	std::vector<std::string> read_file(File f);

	std::string evaluate_checksum(File f);

	bool exists(File f);
	
	std::string get_working_directory();
}
