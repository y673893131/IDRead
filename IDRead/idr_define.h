#ifndef _IDR_DEFINE_H_
#define _IDR_DEFINE_H_

#include <string>
#include <functional>
struct id_info
{
	std::wstring sName;
	std::wstring sIdNum;
	std::wstring sSex;
	std::wstring sFolk;
	std::wstring sBirth;
	std::wstring sDep;
	std::wstring sValidBegin;
	std::wstring sValidEnd;
	std::wstring sAddress;
	std::wstring sNewAddress;
	std::wstring sPassCode;
	std::wstring sIssues;
	std::wstring sHeadIconPath;
};

using id_info_call_back = std::function<void(const id_info&)>;

#endif // !_IDR_DEFINE_H_
