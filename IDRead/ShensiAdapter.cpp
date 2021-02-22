#include "ShensiAdapter.h"
#include "Util.h"
#include <thread>
#include <filesystem>
#include <tchar.h>

#ifdef _WIN32
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif

CShensiAdapter::CShensiAdapter()
	:m_hModule(nullptr)
	, m_cb(nullptr)
	, m_thread(nullptr)
	, m_timeRate(300)
{
}


CShensiAdapter::~CShensiAdapter()
{
}

int CShensiAdapter::init(const std::string& sPath)
{
	auto ret = loadDll(sPath);
	if (ret)
		return ret;

	ret = exec(shensi_cmd_init);
	if (ret == Code_Success)
		return ret;
	return 0;
}

int CShensiAdapter::unInit()
{
	if (m_hModule)
	{
		::FreeLibrary(m_hModule);
		m_hModule = nullptr;
	}

	return 0;
}

void CShensiAdapter::setCallBack(id_info_call_back cb)
{
	m_cb = cb;
}

void CShensiAdapter::setTimeRate(int ms)
{
	m_timeRate = ms;
}

int CShensiAdapter::start()
{
	if (!m_thread)
	{
		m_thread = new std::thread(&CShensiAdapter::threadFunc, this);
		m_thread->detach();
		return 0;
	}

	return -1;
}

int CShensiAdapter::stop()
{
	int ret = 0;
	if (m_thread)
	{
		ret = exec(shensi_cmd_uninit);
	}

	return ret;
}

#define LOAD_FUNCTION(x,y) m_##x=(x)GetProcAddress(y,""#x); if(!m_##x) return GetLastError();

int CShensiAdapter::loadDll(const std::string& sPath)
{
	m_path = CUtil::curDir();
	auto path = m_path + "\\RdCard.dll";
	if (!sPath.empty())
		path = sPath;

	m_path.append("\\temp");

#ifdef _WIN32
	if (!PathFileExistsA(m_path.c_str()))
	{
		CreateDirectoryA(m_path.c_str(), FALSE);
	}
#else
	if (!std::tr2::sys::exists(m_path))
	{
		std::tr2::sys::create_directories(path);
	}
#endif
	m_path.append("\\");
	
	m_hModule = LoadLibraryA(path.c_str());
	if (!m_hModule)
		return GetLastError();
	//LOAD_FUNCTION(UCommand1, m_hModule);
	m_UCommand1 = (UCommand1)GetProcAddress(m_hModule, "UCommand1");

	LOAD_FUNCTION(GetName, m_hModule);
	LOAD_FUNCTION(GetSex, m_hModule);
	LOAD_FUNCTION(GetFolk, m_hModule);
	LOAD_FUNCTION(GetSexGB, m_hModule);
	LOAD_FUNCTION(GetFolkGB, m_hModule);
	LOAD_FUNCTION(GetBirth, m_hModule);
	LOAD_FUNCTION(GetAddr, m_hModule);
	LOAD_FUNCTION(GetIDNum, m_hModule);
	LOAD_FUNCTION(GetDep, m_hModule);
	LOAD_FUNCTION(GetBegin, m_hModule);
	LOAD_FUNCTION(GetEnd, m_hModule);
	LOAD_FUNCTION(GetNewAddr, m_hModule);
	LOAD_FUNCTION(GetBmpPath, m_hModule);
	LOAD_FUNCTION(UnicodeToGBK, m_hModule);
	LOAD_FUNCTION(ID2CovToGBK, m_hModule);

	return 0;
}

struct pass_format
{
	wchar_t sPassCode[9];
	wchar_t sIssues[5];
	wchar_t sOther[4];
	wchar_t sExtend[17];
};

struct wz_txt_format
{
	wchar_t sName[15];		//姓名
	wchar_t sSex[1];		//性别
	wchar_t sFolk[2];		//名族
	wchar_t sBirth[8];		//出生日期
	wchar_t sAddress[35];	//地址
	wchar_t sIDNum[18];		//证件号码
	wchar_t sDep[15];		//发证机关
	wchar_t sValidBegin[8];	//有效期始（YYYYMMDD）
	wchar_t sValidEnd[8];	//有效期止(YYYYMMDD或者长期）
	union
	{
		pass_format pass;			//居住证格式
		wchar_t sNewAddress[35];	//新地址
	};
};

void CShensiAdapter::threadFunc()
{
	wz_txt_format wz;
	auto uPath = A2U(m_path);
	do 
	{
		auto ret = exec(shensi_cmd_verify_id);
		if (ret == Code_Success || ret == Code_Finger_Success)
		{
			if (m_cb)
			{
				ret = exec_query(shensi_cmd_read_base, m_path.c_str());
				if (ret == Code_Success || ret == Code_Finger_Success)
				{
					auto path = m_path + "wz.txt";
					
					FILE* file = nullptr;
					auto code = fopen_s(&file, path.c_str(), "rb");
					fread((char*)&wz, 1, sizeof(wz), file);
					if (file) fclose(file);

					id_info info;
					info.sHeadIconPath = uPath + _T("zp.bmp");
					info.sName = removeEndSpace(wz.sName, sizeof(wz.sName));
					info.sBirth = removeEndSpace(wz.sBirth, sizeof(wz.sBirth), 8);
					info.sAddress = removeEndSpace(wz.sAddress, sizeof(wz.sAddress));
					info.sIdNum = removeEndSpace(wz.sIDNum, sizeof(wz.sIDNum), 18);
					info.sDep = removeEndSpace(wz.sDep, sizeof(wz.sDep));
					info.sValidBegin = removeEndSpace(wz.sValidBegin, sizeof(wz.sValidBegin), 8);
					info.sValidEnd = removeEndSpace(wz.sValidEnd, sizeof(wz.sValidEnd), 8);
					auto isPass = (wz.sFolk[0] == 0x20 && wz.sFolk[1] == 0x20);
					if (isPass)
					{
						info.sIssues = removeEndSpace(wz.pass.sIssues, sizeof(wz.pass.sIssues));
						info.sPassCode = removeEndSpace(wz.pass.sPassCode, sizeof(wz.pass.sPassCode));
					}
					else
					{
						info.sNewAddress = removeEndSpace(wz.sNewAddress, sizeof(wz.sNewAddress));
					}
					
					path = m_path + "wx.txt";
					file = nullptr;
					code = fopen_s(&file, path.c_str(), "rb");
					char sTemp[1024] = {};
					fgets(sTemp, 1024, file);
					info.sSex = getStr(file);
					if (!isPass)
					{
						info.sFolk = getStr(file);
					}

					if (file) fclose(file);

					m_cb(info);
				}
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(m_timeRate));
	} while (true);
}

int CShensiAdapter::exec(BYTE cmd)
{
	if (!m_UCommand1)
		return 0;
	int arg0 = 0, arg1 = 8811, arg2 = 9986;
	return m_UCommand1(&cmd, &arg0, &arg1, &arg2);
}

int CShensiAdapter::exec_query(BYTE cmd, const char* path)
{
	if (!m_UCommand1)
		return 0;
	int arg0 = 0, arg1 = 8811;
	return m_UCommand1(&cmd, &arg0, &arg1, (int*)path);
}

std::wstring CShensiAdapter::removeEndSpace(wchar_t* buff, int size, int realSize)
{
	if (realSize > 0)
	{
		return std::wstring(buff, realSize);
	}

	std::wstring sText;
	int n = 0;
	for (; n < size; ++n)
	{
		if (buff[n] == 0x20)
			break;
		sText.insert(sText.end(), buff[n]);
	}

	return sText;
}

std::wstring CShensiAdapter::A2U(const std::string &str)
{
	std::wstring wstr;
	int nWideCount = MultiByteToWideChar(CP_ACP, NULL, str.c_str(), -1, NULL, 0);
	if (nWideCount < 0 || nWideCount > 100000)
	{
		return wstr;
	}
	wchar_t* pWchar = new wchar_t[nWideCount + 1];
	if (pWchar == NULL)
	{
		return wstr;
	}
	pWchar[nWideCount] = 0;
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pWchar, nWideCount*sizeof(wchar_t));
	wstr = pWchar;
	delete[]pWchar;
	return std::move(wstr);
}

std::wstring CShensiAdapter::getSex(wchar_t* buff)
{
	static wchar_t* sSex[] = {_T(""), _T("男"),_T("女"), _T("")};
	int n = buff[0] - '0';
	if (n > 0 && n < 4)
		return  sSex[n];
	return _T("");
}

std::wstring CShensiAdapter::getFolk(wchar_t* buff)
{
	static wchar_t* sFolk[] = { _T(""), _T("汉族"), _T("蒙古族"), _T("回族"), _T("藏族"), _T("维吾尔族"), _T("苗族"), _T("彝族"), _T("壮族"), _T("布依族"),
		_T("朝鲜族"), _T("满族"), _T("侗族"), _T("瑶族"), _T("白族"), _T("土家族"), _T("哈尼族"), _T("哈萨克族"), _T("傣族"), _T("黎族"),
		_T("傈傈族"), _T("佤族"), _T("畲族")/*she*/, _T("高山族"), _T("拉祜族"), _T("水族"), _T("东乡族"), _T("纳西族"), _T("景颇族"), _T("柯尔克牧族") ,
		_T("土族"), _T("达翰尔族"), _T("仫佬族"), _T("羌族"), _T("布朗族"), _T("撒拉族"), _T("毛南族"), _T("仡佬族"), _T("锡伯族"), _T("阿昌族") ,
		_T("普米族"), _T("塔吉克族"), _T("怒族"), _T("乌孜别克族"), _T("俄罗斯族"), _T("鄂温克族"), _T("崩龙族"), _T("保安族"), _T("裕固族"), _T("京族") ,
		_T("塔塔尔族"), _T("鄂伦春族"), _T("赫哲族"), _T("门巴族"), _T("珞巴族"), _T("基诺族"), _T(""), _T(""), _T(""), _T("") ,
		_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("") ,
		_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""),
		_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""),
		_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("其他"), _T("外国血统"), _T("") };
	auto sIndex = removeEndSpace(buff, 4, 2);
	int nIndex = _wtoi(sIndex.c_str());
	nIndex = 97;
	if (nIndex > 0 && nIndex < 100)
		return sFolk[nIndex];
	return _T("");
}

std::wstring CShensiAdapter::getStr(FILE* file)
{
	char sTemp[1024] = {};
	fgets(sTemp, 1024, file);
	int len = strlen(sTemp);
	if (len >= 2)
	{
		sTemp[len - 1] = 0x00;
		sTemp[len - 2] = 0x00;
	}

	return A2U(sTemp);
}
