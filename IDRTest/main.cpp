#include <iostream>
#include <thread>
#include <tchar.h>
#include "../IDRead/idr_interface.h"

#ifdef _DEBUG
#pragma comment(lib, "../Debug/IDRead.lib")
#else
#pragma comment(lib, "../Release/IDRead.lib")
#endif
void test_call_back(const id_info& info)
{
	std::wcout.imbue(std::locale("chs"));
	std::wcout << "-------------Ë¢¿¨------------->" << std::endl;
	std::wcout << _T("Name:") << info.sName.c_str() << std::endl;
	std::wcout << _T("IDNum:") << info.sIdNum.c_str() << std::endl;
	std::wcout << _T("Sex:") << info.sSex.c_str() << std::endl;
	std::wcout << _T("Folk:") << info.sFolk.c_str() << std::endl;
	std::wcout << _T("Birth:") << info.sBirth.c_str() << std::endl;
	std::wcout << _T("ValidBegin:") << info.sValidBegin.c_str() << std::endl;
	std::wcout << _T("ValidEnd:") << info.sValidEnd.c_str() << std::endl;
	std::wcout << _T("Address:") << info.sAddress.c_str() << std::endl;
	std::wcout << _T("NewAddress:") << info.sNewAddress.c_str() << std::endl;
	std::wcout << _T("PassCode:") << info.sPassCode.c_str() << std::endl;
	std::wcout << _T("Issues:") << info.sIssues.c_str() << std::endl;
	std::wcout << _T("headIcon:") << info.sHeadIconPath.c_str() << std::endl;
	std::wcout << "<-------------Ë¢¿¨-------------" << std::endl << std::endl;
}

int main(char argc, char* argv[])
{
	auto idr = idr_interface::instance();
	idr->set_swip_call_back(test_call_back);
	idr->init();
	idr->start();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}

	return 0;
}