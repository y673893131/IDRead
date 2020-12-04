#pragma once
#include "shensi.h"
#include "idr_define.h"
#include <thread>

class CShensiAdapter
{
public:
	CShensiAdapter();
	~CShensiAdapter();

	int init(const std::string&);
	int unInit();
	void setCallBack(id_info_call_back cb);
	void setTimeRate(int ms);
	int start();
	int stop();
private:
	int loadDll(const std::string&);
	void threadFunc();
	int exec(BYTE);
	int exec_query(BYTE, const char*);
	std::wstring removeEndSpace(wchar_t*, int, int = -1);
	std::wstring A2U(const std::string &str);
	std::wstring getSex(wchar_t*);
	std::wstring getFolk(wchar_t*);
	std::wstring getStr(FILE*);
private:
	UCommand1 m_UCommand1;
	GetName m_GetName;
	GetSex m_GetSex;
	GetFolk m_GetFolk;
	GetSexGB m_GetSexGB;
	GetFolkGB m_GetFolkGB;
	GetBirth m_GetBirth;
	GetAddr m_GetAddr;
	GetIDNum m_GetIDNum;
	GetDep m_GetDep;
	GetBegin m_GetBegin;
	GetEnd m_GetEnd;
	GetNewAddr m_GetNewAddr;
	GetBmpPath m_GetBmpPath;
	UnicodeToGBK m_UnicodeToGBK;
	ID2CovToGBK m_ID2CovToGBK;

	HMODULE m_hModule;
	int m_timeRate;
	id_info_call_back m_cb;
	std::thread* m_thread;
	std::string m_path;
};

