#pragma once
#include <windows.h>

typedef enum
{
	shensi_cmd_init = 0x41,						//��ʼ���˿�
	shensi_cmd_uninit,							//�رն˿�
	shensi_cmd_verify_id,						//��֤��
	shensi_cmd_read_base,						//��������Ϣ
	shensi_cmd_read_new_address,				//������סַ��Ϣ
	shensi_cmd_read_only_txt,					//����������Ϣ
	shensi_cmd_read_base_not_decode_img,		//��������Ϣ��������ͼ�����
}ShensiCmd;

typedef enum
{
	Code_Parse_Error = -11,							//����ʧ��
	Code_Port_Error,								//�˿ڲ���ʧ��
	Code_Load_Translate_Function_Error,				//����ͨѶ��������
	Code_File_Save_Error,							//�ļ��洢ʧ��
	Code_Device_Error,								//�豸����ȷ���Ƿ��豸��
	Code_Connect_Device_Error,						//�豸���Ӵ���
	Code_Software_Verify_Error,						//���δ��Ȩ
	Code_Wlt_File_Format_Error,						//Wlt�ļ���ʽ����
	Code_Wlt_File_Open_Error,						//Wlt�ļ��򿪴���
	Code_Wlt_File_Suffix_Error,						//Wlt�ļ���׺����
	Code_Phote_Decode_Error,						//��Ƭ�������
	Code_Recv_Data_Timeout = 2,						//�������ݳ�ʱ

	Code_IDCard_31,									//����֤����ʧ��
	Code_IDCard_32,									//������֤��ʧ��
	Code_IDCard_33,									//��Ϣ��֤����
	Code_IDCard_34,									//��δ�ҿ������ܽ��жԿ��Ĳ���
	Code_IDCard_40,									//�޷�ʶ��Ŀ�����
	Code_IDCard_41,									//��������ʧ��
	Code_IDCard_50,									//д������ʧ��
	Code_IDCard_61,									//�û���¼ʧ��
	Code_IDCard_80,									//�ҿ����ɹ�
	Code_IDCard_81,									//ѡ�����ɹ�


	Code_Success = 62171,							//��û��ָ�����ݵ����֤ʱ������62171Ϊ�ɹ�
	Code_Finger_Success = 62172,							//������ָ�����ݵ����֤ʱ������62172Ϊ�ɹ�
}ShensiCode;

//ID2CovToGBK
typedef enum
{
	str_to_id = 0,
	id_to_str,
};

// RdCard.dll ��������

#ifdef __cplusplus
#define EC __stdcall
#else
#define EC
#endif

// ShensiCmd��Ӧ�������
typedef int(EC *UCommand1)(BYTE *pCmd, int * parg0, int * parg1, int * parg2);

//��ȡ����
typedef int(EC *GetName)(char *buf);
//��ȡ�Ա����
typedef int(EC *GetSex)(char *buf);
//��ȡ�������
typedef int(EC *GetFolk)(char *buf);
//��ȡ�Ա�
typedef int(EC *GetSexGB)(char *buf);
//��ȡ����
typedef int(EC *GetFolkGB)(char *buf);
//��ȡ����
typedef int(EC *GetBirth)(char *buf);
//��ȡסַ
typedef int(EC *GetAddr)(char *buf);
//��ȡ������ݺ���
typedef int(EC *GetIDNum)(char *buf);
//��ȡǩ������
typedef int(EC *GetDep)(char *buf);
//��ȡ��Ч����
typedef int(EC *GetBegin)(char *buf);
//��ȡ��Ч��ֹ
typedef int(EC *GetEnd)(char *buf);
//��ȡ���µ�ַ
typedef int(EC *GetNewAddr)(char *buf);
//��ȡͷ��ͼƬ·��
typedef int(EC *GetBmpPath)(char *buf);
//unicode -> GBK
typedef int(EC *UnicodeToGBK)(wchar_t *pwc, char *pc, int nsize);
//
typedef int(EC *ID2CovToGBK)(char * sfileSrc, char *sfileDest, int nDict);