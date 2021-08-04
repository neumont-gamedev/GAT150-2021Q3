#pragma once
#include <string>

namespace nc
{
	bool FileExists(const std::string& filename);
	bool FileSize(const std::string& filename, size_t& size);

	void SetFilePath(const std::string& pathname);
	std::string GetFilePath();
}