#pragma once
#include "idr_define.h"
#ifdef IDREAD_DLL
#define IDREAD_INTERFACE __declspec(dllexport)
#else
#define IDREAD_INTERFACE __declspec(dllimport)
#endif
class IDREAD_INTERFACE idr_interface final
{
public:
	static idr_interface* instance();
	bool init(const std::string& = std::string());
	void uninit();
	bool start();
	bool stop();
	bool set_swip_call_back(id_info_call_back cb);
	bool set_time_rate(int ms);
private:
	idr_interface();
	idr_interface(const idr_interface&);
	~idr_interface();
};

