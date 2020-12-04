#include "Util.h"

std::string CUtil::curDir()
{
	char path[MAX_PATH] = {};
	GetModuleFileNameA(0, path, MAX_PATH);
	std::string tmp_path = path;

	int pos = tmp_path.find_last_of("\\");
	if (pos != tmp_path.npos)
	{
		return std::move(tmp_path.substr(0, pos));
	}

	return std::string();
}
