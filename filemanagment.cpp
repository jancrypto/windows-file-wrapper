#include "wrapper.h"

File::File(std::string path)
{
	this->path = path;

	this->path = std::replace(this->path, "\\", "/");

	std::vector<std::string> vec = std::split(this->path, "/");
	
	this->name = vec.at(vec.size() - 1);
}

std::string File::GetPath() {
	return this->path;
}

std::string File::GetName() {
	return this->name;
}

bool filemanagment::create_directory(File f)
{
	return CreateDirectoryA(f.GetPath().c_str(), NULL);
}

HANDLE filemanagment::open_file(File f, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes) {
	return CreateFileA(f.GetPath().c_str(), dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
}

HANDLE filemanagment::create_file(File f) {
	return open_file(f, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, CREATE_NEW, FILE_ATTRIBUTE_NORMAL);
}

bool filemanagment::write_file(File f, std::string valuesToWrite)
{
	if (!exists(f)) {
		LogA("File " + f.GetPath() + " doesn't exist");
		return false;
	}

	HANDLE h = open_file(f, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);
	DWORD dwBytes;

	char str[1024];
	strcpy(str, valuesToWrite.c_str());

	bool result = WriteFile(h, str, strlen(str), &dwBytes, NULL);
	CloseHandle(h);

	return result;
}

bool filemanagment::delte_file(File f)
{
	if (!exists(f)) {
		LogA("File " + f.GetPath() + " doesn't exist");
		return false;
	}
	return DeleteFileA(f.GetPath().c_str());
}

bool filemanagment::move_file(File f, File newFile)
{
	if (!exists(f)) {
		LogA("File " + f.GetPath() + " doesn't exist");
		return false;
	}


	return MoveFileA(f.GetPath().c_str(), newFile.GetPath().c_str());
}

std::vector<std::string> filemanagment::read_file(File f)
{
	if (!exists(f)) {
		LogA("File " + f.GetPath() + " doesn't exist");
		return {};
	}

	HANDLE h = open_file(f, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);

	DWORD dwSize = GetFileSize(h, NULL);
	DWORD dwRead;

	std::vector<char> vec(dwSize);

	if (!ReadFile(h, static_cast<LPVOID>(&vec[0]), dwSize, &dwRead, NULL))
		LogA("Failed to read file: " + f.GetPath());

	std::string str;
	for (char& i : vec) {
		str += i;
	}
	CloseHandle(h);
	return std::split(str, "\n");
}

std::string filemanagment::evaluate_checksum(File f)
{
	if (!exists(f)) {
		LogA("File " + f.GetPath() + " doesn't exist");
		return "0";
	}

	HANDLE h = open_file(f, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);

	_FILETIME time = { 0 };
	GetFileTime(h, NULL, NULL, &time);

	CloseHandle(h);

	std::string lastModified = std::to_string(time.dwLowDateTime + 2 ^ 32 * time.dwHighDateTime);

	return md5(lastModified + f.GetPath());
}

bool filemanagment::exists(File f) {
	HANDLE h = open_file(f, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);

	bool result = h != INVALID_HANDLE_VALUE;

	CloseHandle(h);
	return result;
}

std::string filemanagment::get_working_directory()
{
	char buffer[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}
