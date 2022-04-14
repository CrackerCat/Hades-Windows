#pragma once
#include <string>

// OS�̶�����
typedef struct _SystemAttributesNode
{
	std::string currentUser;	//��ǰ�û�	
	std::string cpuinfo;		//cpu��Ϣ
	std::string verkerlinfo;	//�汾
	std::string mainboard;		//����
	std::string monitor;		//�Կ�
	std::string netcard;		//����
	std::string battery;		//���
	std::string camera;			//����ͷ
	std::string bluetooth;		//����
	std::string voice;			//��Ƶ
}SystemAttributesNode, * PSystemAttributesNode;


// OS�䶯����
typedef struct _SystemDynamicNode
{
	std::string cpu_temperature;
	std::string monitor_temperature;
	std::string mainboard_temperature;
	std::string disk_temperature;
	std::string cpu_utilization;
	std::string sys_memory;
	std::string disk_io;
	std::string GPU;
}SystemDynamicNode, * PSystemDynamicNode;


namespace SYSTEMPUBLIC {
	extern SystemAttributesNode sysattriinfo;
	extern SystemDynamicNode sysdynamicinfo;
}

// ��ʼ��ϵͳ����
extern void SysAttributesInit();


// ϵͳ���ݶ�̬�ص�
extern void SysDynCpuTempera();
extern void SysDynMonTempera();
extern void SysDynManBoardTempera();
extern void SysDynDiskTempera();
extern void SysDynCpuUtilizaTempera();
extern void SysDynSysMemTempera();
extern void SysDynDiskIoTempera();
extern void SysDynGpuTempera();