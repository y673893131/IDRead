#include "idr_interface.h"
#include "ShensiAdapter.h"

idr_interface* g_instance = nullptr;
CShensiAdapter* g_shensi = nullptr;
idr_interface* idr_interface::instance()
{
	if (!g_instance)
	{
		g_instance = new idr_interface();
		g_shensi = new CShensiAdapter();
	}

	return g_instance;
}

bool idr_interface::init(const std::string& sPath)
{
	if (g_shensi)
	{
		return g_shensi->init(sPath) == 0;
	}

	return false;
}

void idr_interface::uninit()
{
	if (g_shensi)
	{
		g_shensi->unInit();
	}
}

bool idr_interface::start()
{
	if (g_shensi)
	{
		return g_shensi->start() == 0;
	}

	return false;
}

bool idr_interface::stop()
{
	if (g_shensi)
	{
		return g_shensi->stop() == 0;
	}

	return false;
}

bool idr_interface::set_swip_call_back(id_info_call_back cb)
{
	if (g_shensi)
	{
		g_shensi->setCallBack(cb);
	}

	return true;
}

bool idr_interface::set_time_rate(int ms)
{
	if (g_shensi)
	{
		g_shensi->setTimeRate(ms);
	}

	return true;
}

idr_interface::idr_interface()
{
}

idr_interface::idr_interface(const idr_interface&)
{
}

idr_interface::~idr_interface()
{
}