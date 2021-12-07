#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#define INITGUID
#include <evntrace.h>
#include <evntcons.h>

#include "uetw.h"
#include "sync.h"

#include <map>

using namespace std;


// Session - Guid - tracconfig
typedef struct _TracGuidNode
{
    DWORD                         event_tracid;
    EVENT_TRACE_PROPERTIES*       bufconfig;
}TracGuidNode, *PTracGuidNode;

static AutoCriticalSection g_ms;
static map<TRACEHANDLE, TracGuidNode> g_tracMap;

////////////////////////////////////
// CallBack
void WINAPI ProcessEvent(PEVENT_RECORD pEvent);
void WINAPI ThreadEvent(PEVENT_RECORD pEvent);
void WINAPI ImageEvent(PEVENT_RECORD pEvent);
void WINAPI TcpIpEvent(PEVENT_RECORD pEvent);
void WINAPI RegisterEvent(PEVENT_RECORD pEvent);
void WINAPI FileEvent(PEVENT_RECORD pEvent);
void WINAPI SystemInfoEvent(PEVENT_RECORD pEvent);

///////////////////////////////////
// ���������߳�
DWORD WINAPI consumer_thread(LPVOID param)
{
    EVENT_TRACE_LOGFILE trace;
    memset(&trace, 0, sizeof(trace));
    trace.LoggerName = const_cast<wchar_t*>(KERNEL_LOGGER_NAME);
    trace.LogFileName = NULL;
    /// ָ�������¼���ģʽ��������ʵʱ����
    trace.ProcessTraceMode = PROCESS_TRACE_MODE_REAL_TIME | PROCESS_TRACE_MODE_EVENT_RECORD;
    trace.Context = NULL;
    /// ָ���¼����������ú���ʵ�ֽ�Ϊ����
    trace.EventRecordCallback = ProcessEvent;

    /// ��session
    TRACEHANDLE handle = OpenTrace(&trace);
    if (handle == (TRACEHANDLE)INVALID_HANDLE_VALUE)
    {
        printf("handle is invalid value.\n");
        return -1;
    }

    /// ��ʼ�����¼����ú���Ϊ���������������ﲻ�ϵ����¼������������¼���ֱ��session�ر�
    ProcessTrace(&handle, 1, 0, 0);

    CloseTrace(handle);
    return 0;
}

UEtw::UEtw()
{
}

UEtw::~UEtw()
{
}

///////////////////////////////////
// ע��ETW�¼�
bool UEtw::uf_RegisterTrace(const int dwEnableFlags)
{
    ULONG status = 0;
    TRACEHANDLE hSession = NULL;
    uint32_t event_buffer = sizeof(EVENT_TRACE_PROPERTIES) + sizeof(KERNEL_LOGGER_NAME);
    if (event_buffer <= 0)
        return false;

    EVENT_TRACE_PROPERTIES* m_traceconfig = reinterpret_cast<EVENT_TRACE_PROPERTIES*>(new char[event_buffer]);
    if (!m_traceconfig)
        return false;
    RtlZeroMemory(m_traceconfig, event_buffer);

    m_traceconfig->Wnode.BufferSize = event_buffer;
    m_traceconfig->Wnode.Flags = WNODE_FLAG_TRACED_GUID;
    m_traceconfig->Wnode.ClientContext = 1;
    // ʹ�� NT Kernel Logger + SystemTraceControlGuid
    // See Msdn: https://docs.microsoft.com/en-us/windows/win32/etw/nt-kernel-logger-constants
    m_traceconfig->Wnode.Guid = SystemTraceControlGuid;
    // �ļ�����
    m_traceconfig->EnableFlags = dwEnableFlags;
    m_traceconfig->LogFileMode = EVENT_TRACE_REAL_TIME_MODE;
    m_traceconfig->LoggerNameOffset = sizeof(EVENT_TRACE_PROPERTIES);

    /// NT Kernel Logger
    status = StartTrace(&hSession, KERNEL_LOGGER_NAME, m_traceconfig);
    if (ERROR_SUCCESS != status)
    {
        /// �Ѿ����� Stop
        if (ERROR_ALREADY_EXISTS == status)
        {
            status = ControlTrace(NULL, KERNEL_LOGGER_NAME, m_traceconfig, EVENT_TRACE_CONTROL_STOP);
            if (SUCCEEDED(status))
            {
                status = StartTrace(&hSession, KERNEL_LOGGER_NAME, m_traceconfig);
                if (ERROR_SUCCESS != status)
                {
                    if (m_traceconfig)
                        delete[] m_traceconfig;
                    return false;
                }      
            }
        }
        else
        {
            if (m_traceconfig)
                delete[] m_traceconfig;
            return false;
        }
    }

    TracGuidNode tracinfo = { 0, };
    tracinfo.bufconfig = m_traceconfig;
    tracinfo.event_tracid = dwEnableFlags;
    g_ms.Lock();
    g_tracMap[hSession] = tracinfo;
    g_ms.Unlock();
    return true;
}

/// <summary>
/// ����ĳ�¼�״̬
/// </summary>
/// <param name="hSession"></param>
/// <param name="m_traceconfig"></param>
/// <param name="ioct"></param>
/// <returns></returns>
unsigned long UEtw::uf_setmonitor(
    unsigned __int64 hSession,
    PVOID64 m_traceconfig,
    const int ioct
)
{
    ULONG nRet = 0;

    if (hSession && m_traceconfig)
        nRet = ControlTrace(hSession, KERNEL_LOGGER_NAME, (PEVENT_TRACE_PROPERTIES)m_traceconfig, ioct);

    if (m_traceconfig)
    {
        delete[] m_traceconfig;
        m_traceconfig = NULL;
    }

    return nRet;
}

/// <summary>
/// �ر�ȫ���¼�
/// </summary>
/// <returns></returns>
bool UEtw::uf_close()
{
    map<TRACEHANDLE, TracGuidNode>::iterator  iter;

    for (iter = g_tracMap.begin(); iter != g_tracMap.end();)
    {
        
        if (iter->first && iter->second.bufconfig)
            ControlTrace(iter->first, KERNEL_LOGGER_NAME, iter->second.bufconfig, EVENT_TRACE_CONTROL_STOP);

        if (iter->second.bufconfig)
        {
            delete[] iter->second.bufconfig;
            iter->second.bufconfig = NULL;
        }

        g_ms.Lock();
        g_tracMap.erase(iter++);
        g_ms.Unlock();

    }

}


///////////////////////////////////////////////////
// ProcessEvent_callback
void WINAPI ProcessEvent(PEVENT_RECORD pEventRecord)
{
    //auto session = (TraceSession*)pEventRecord->UserContext;
    auto const& hdr = pEventRecord->EventHeader;

#pragma warning(push)
#pragma warning(disable: 4127) // constant conditional expressions

#pragma warning(pop)

}

void WINAPI ThreadEvent(PEVENT_RECORD pEvent)
{
}

void WINAPI ImageEvent(PEVENT_RECORD pEvent)
{
}

void WINAPI TcpIpEvent(PEVENT_RECORD pEvent)
{
}

void WINAPI RegisterEvent(PEVENT_RECORD pEvent) 
{
}

void WINAPI FileEvent(PEVENT_RECORD pEvent)
{
}

void WINAPI SystemInfoEvent(PEVENT_RECORD pEvent)
{
}