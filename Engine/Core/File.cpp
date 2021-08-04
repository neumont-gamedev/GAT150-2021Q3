#include "File.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace nc
{
	bool FileExists(const std::string& filename)
	{
		return fs::exists(filename.c_str()) && fs::is_regular_file(filename.c_str());
	}

	bool FileSize(const std::string& filename, size_t& size)
	{
		if (FileExists(filename))
		{
			size = static_cast<size_t>(fs::file_size(filename));
			return true;
		}

		return false;
	}

	void SetFilePath(const std::string& pathname)
	{
		fs::current_path(pathname);
	}

	std::string GetFilePath()
	{
		return fs::current_path().string();
	}
}