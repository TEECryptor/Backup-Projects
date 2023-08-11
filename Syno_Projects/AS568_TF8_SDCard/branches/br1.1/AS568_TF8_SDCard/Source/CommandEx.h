/***************************************************************************
 *	File name:CommandEx.h
 *	Introduce:Extends command definition
 *	Author:Syno common
 *	Date:2015/07/06
 *  Version:v1.0
 **************************************************************************/
#ifndef _COMMAND_EX_H_
#define _COMMAND_EX_H_ 

/*
 *  ����Ӧ���Ƕ���
 */
#define RT_OK               0x00    //����ִ�гɹ����޺�����Ӧ��
#define RT_OK_WITH_PACKAGE  0xFE    //����ִ�гɹ�����������Ӧ���������Ӧ�������
#define RT_FAI              0x01    //ʧ��
#define RT_COMMAND_ERROR    0x02    //�������
#define RT_PARAM_ERROR      0x03    //��������
#define RT_TIME_OUT         0x04    //��ʱ
#define RT_ECC_ERROR        0x05    //У�����
#define RT_PACKAGE_ERROR    0x08    //���շ�����
#define RT_WAIT			    0x60    //
/*
 *  ״̬��Ӧ�붨��
 */
#define SW_OK               0x9000  //ִ�гɹ�
#define SW_DATALENERR       0x6700  //���ȴ���
#define SW_DATAERR          0x6B00  //�����ֶ����ݴ���
#define SW_FAIL             0x6900  //ִ��ʧ��
#define SW_ADDRESSERR       0x6A80  //Address��Χ����
#define SW_P1P2ERROR        0x6A86  //P1P2����
#define SW_NOFINISH         0x6060  //��������ִ�У���ȴ�
#define SW_ADDRNOTCLEAN     0x6581  //��ַ�ռ�δ����

/*
 *	Error code definition
 */
#define SAR_EX_OK                                0x9000    //�ɹ�
#define SAR_EX_FAIL                              0x6901
#define SAR_EX_UNKNOWNERR                        0x6902
#define SAR_EX_NOTSUPPORTYETERR                  0x6903
#define SAR_EX_FILEERR                           0x6904
#define SAR_EX_INVALIDHANDLEERR                  0x6905
#define SAR_EX_INVALIDPARAMERR                   0x6906
#define SAR_EX_READFILEERR                       0x6907
#define SAR_EX_WRITEFILEERR                      0x6908
#define SAR_EX_NAMELENERR                        0x6909
#define SAR_EX_KEYUSAGEERR                       0x690A
#define SAR_EX_MODULUSLENERR                     0x690B
#define SAR_EX_NOTINITIALIZEERR                  0x690C
#define SAR_EX_OBJERR                            0x690D
#define SAR_EX_MEMORYERR                         0x690E
#define SAR_EX_TIMEOUTERR                        0x690F
#define SAR_EX_INDATALENERR                      0x6910
#define SAR_EX_INDATAERR                         0x6911
#define SAR_EX_GENRANDERR                        0x6912
#define SAR_EX_HASHOBJERR                        0x6913
#define SAR_EX_HASHERR                           0x6914
#define SAR_EX_GENRSAKEYERR                      0x6915
#define SAR_EX_RSAMODULUSLENERR                  0x6916
#define SAR_EX_CSPIMPRTPUBKEYERR                 0x6917
#define SAR_EX_RSAENCERR                         0x6918
#define SAR_EX_RSADECERR                         0x6919
#define SAR_EX_HASHNOTEQUALERR                   0x691A
#define SAR_EX_KEYNOTFOUNTERR                    0x691B
#define SAR_EX_CERTNOTFOUNTERR                   0x691C
#define SAR_EX_NOTEXPORTERR                      0x691D
#define SAR_EX_DECRYPTPADERR                     0x691E
#define SAR_EX_MACLENERR                         0x691F
#define SAR_EX_BUFFER_TOO_SMALL                  0x6920
#define SAR_EX_KEYINFOTYPEERR                    0x6921
#define SAR_EX_NOT_EVENTERR                      0x6922
#define SAR_EX_DEVICE_REMOVED                    0x6923
#define SAR_EX_PIN_INCORRECT                     0x6924
#define SAR_EX_PIN_LOCKED                        0x6925
#define SAR_EX_PIN_INVALID                       0x6926
#define SAR_EX_PIN_LEN_RANGE                     0x6927
#define SAR_EX_USER_ALREADY_LOGGED_IN            0x6928
#define SAR_EX_USER_PIN_NOT_INITIALIZED          0x6929
#define SAR_EX_USER_TYPE_INVALID                 0x692A
#define SAR_EX_APPLICATION_NAME_INVALID          0x692B
#define SAR_EX_APPLICATION_EXISTS                0x692C
#define SAR_EX_USER_NOT_LOGGED_IN                0x692D
#define SAR_EX_APPLICATION_NOT_EXISTS            0x692E
#define SAR_EX_FILE_ALREADY_EXIST                0x692F
#define SAR_EX_NO_ROOM                           0x6930
#define SAR_EX_FILE_NOT_EXIST                    0x6931
#define SAR_EX_REACH_MAX_CONTAINER_COUNT         0x6932
//#define SAR_EX_VERFIYRANDOM_FAIL         	  0x6933
//#define SAR_EX_VERFIYID_FAIL         		  0x6934
//#define SAR_EX_CONTAINER_NOT_EXISTS			  0x6935

/*
 *  �����豸��������
 */
#define	CMD_GET_DEVINFO				0xA4	//��ȡ�豸��Ϣ
#define CMD_SET_DEVPARAM            0xF2    //�����豸����
#define CMD_SET_DEVSLEEP            0xFF    //�����豸����
#define CMD_SET_BURNMODE            0x1A    //����COS��дģʽ
#define CMD_GET_DEVSTATUS           0xE0    //��ȡ�豸״̬
#define CMD_CLEAN_FLASH             0xE1    //���Flash����
#define CMD_WRITE_FLASH             0xE3    //дFlash����
#define CMD_SET_UPDATEPARAM         0xE2    //������������
#define	CMD_DEVINIT					0xE4	//�豸��ʼ��
#define CMD_FORMATCD                0x0A    //��ISO�ļ�д�� 
#define CMD_RAND_XN                 0x0F    //�������������
#define CMD_GENPRIME                0x06    //�������� 

/*
 *  �������
 */
void CMD_GetDevInfo(void);
void CMD_SetDevParam(void);
void CMD_SetDevSleep(void);
void CMD_SetBurnMode(void);
void CMD_GetDevStatus(void);
void CMD_CleanFlash(void);
void CMD_WriteFlash(void);
void CMD_SetUpdateParam(void);
void CMD_DevInit(void);
void CMD_FormatCD(void);
void CMD_RandXN(void);
void CMD_GenPrime(void);

/*
 *  ������ں���
 */
void DoAS5xxCMDEx(void);
#endif	//_COMMAND_EX_H_
