#pragma once
#include <windows.h>

typedef enum
{
	shensi_cmd_init = 0x41,						//初始化端口
	shensi_cmd_uninit,							//关闭端口
	shensi_cmd_verify_id,						//验证卡
	shensi_cmd_read_base,						//读基本信息
	shensi_cmd_read_new_address,				//读最新住址信息
	shensi_cmd_read_only_txt,					//仅读文字信息
	shensi_cmd_read_base_not_decode_img,		//读基本信息但不进行图像解码
}ShensiCmd;

typedef enum
{
	Code_Parse_Error = -11,							//解码失败
	Code_Port_Error,								//端口操作失败
	Code_Load_Translate_Function_Error,				//加载通讯函数错误
	Code_File_Save_Error,							//文件存储失败
	Code_Device_Error,								//设备不正确（非法设备）
	Code_Connect_Device_Error,						//设备连接错误
	Code_Software_Verify_Error,						//软件未授权
	Code_Wlt_File_Format_Error,						//Wlt文件格式错误
	Code_Wlt_File_Open_Error,						//Wlt文件打开错误
	Code_Wlt_File_Suffix_Error,						//Wlt文件后缀错误
	Code_Phote_Decode_Error,						//相片解码错误
	Code_Recv_Data_Timeout = 2,						//接收数据超时

	Code_IDCard_31,									//卡认证机具失败
	Code_IDCard_32,									//机具认证卡失败
	Code_IDCard_33,									//信息验证错误
	Code_IDCard_34,									//尚未找卡，不能进行对卡的操作
	Code_IDCard_40,									//无法识别的卡类型
	Code_IDCard_41,									//读卡操作失败
	Code_IDCard_50,									//写卡操作失败
	Code_IDCard_61,									//用户登录失败
	Code_IDCard_80,									//找卡不成功
	Code_IDCard_81,									//选卡不成功


	Code_Success = 62171,							//读没有指纹数据的身份证时，返回62171为成功
	Code_Finger_Success = 62172,							//当读有指纹数据的身份证时，返回62172为成功
}ShensiCode;

//ID2CovToGBK
typedef enum
{
	str_to_id = 0,
	id_to_str,
};

// RdCard.dll 导出函数

#ifdef __cplusplus
#define EC __stdcall
#else
#define EC
#endif

// ShensiCmd对应命令操作
typedef int(EC *UCommand1)(BYTE *pCmd, int * parg0, int * parg1, int * parg2);

//读取姓名
typedef int(EC *GetName)(char *buf);
//读取性别编码
typedef int(EC *GetSex)(char *buf);
//读取民族编码
typedef int(EC *GetFolk)(char *buf);
//读取性别
typedef int(EC *GetSexGB)(char *buf);
//读取民族
typedef int(EC *GetFolkGB)(char *buf);
//读取出生
typedef int(EC *GetBirth)(char *buf);
//读取住址
typedef int(EC *GetAddr)(char *buf);
//读取公民身份号码
typedef int(EC *GetIDNum)(char *buf);
//读取签发机关
typedef int(EC *GetDep)(char *buf);
//读取有效期起
typedef int(EC *GetBegin)(char *buf);
//读取有效期止
typedef int(EC *GetEnd)(char *buf);
//读取最新地址
typedef int(EC *GetNewAddr)(char *buf);
//读取头像图片路径
typedef int(EC *GetBmpPath)(char *buf);
//unicode -> GBK
typedef int(EC *UnicodeToGBK)(wchar_t *pwc, char *pc, int nsize);
//
typedef int(EC *ID2CovToGBK)(char * sfileSrc, char *sfileDest, int nDict);