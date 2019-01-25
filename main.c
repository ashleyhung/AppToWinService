#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <stdbool.h>
#include <time.h>

#include "main.h"

char onExit = 0;
static char connection=-1;

static clock_t EventLastTime[4];
static clock_t LedLastTime;

static int comport;
static char ledstate;

void ProgramMain()
{
	while(onExit == 0)
	{
		printf("This is a windows service example\n");
		Sleep(10000);
	}
}
//service setting
/********************************************/
#ifdef DAEMON
	#define   SERVICE_NAME   "TEST"
    #define   SVC_ERROR      1

    void ServiceMain    (int argc, char *argv[]);
    void SvcCtrlHandler (DWORD dwCtrl);
    void ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
    void SvcReportEvent (LPTSTR szFunction);

    HANDLE                  ghSvcStopEvent = NULL;
    SERVICE_STATUS          gSvcStatus; 
    SERVICE_STATUS_HANDLE   gSvcStatusHandle; 
    SERVICE_TABLE_ENTRY     ServiceTable[2] =
    {
      {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
      {NULL, NULL}
    };

    void ServiceMain (int argc, char** argv)
    {
      int  rtn = -1;
      int wait_interval = 1000;

      /* Register the handler function for the service */
      gSvcStatusHandle = RegisterServiceCtrlHandler (SERVICE_NAME, (LPHANDLER_FUNCTION) SvcCtrlHandler);

      if( !gSvcStatusHandle )
      { 
        SvcReportEvent(TEXT("RegisterServiceCtrlHandler"));
        return;
      }

      /* These SERVICE_STATUS members remain as set here */
      gSvcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
      gSvcStatus.dwServiceSpecificExitCode = 0;

      /* Report initial status to the SCM */
      ReportSvcStatus( SERVICE_START_PENDING, NO_ERROR, 3000 );

      /* Perform service-specific initialization and work */
      ghSvcStopEvent = CreateEvent( NULL,    /* default security attributes */
                                    true,    /* manual reset event */
                                    false,   /* not signaled */
                                    NULL);   /* no name */

      if ( ghSvcStopEvent == NULL)
      {
        ReportSvcStatus( SERVICE_STOPPED, NO_ERROR, 0 );
        return;
      }

      /* Report running status when initialization is complete. */
      ReportSvcStatus( SERVICE_RUNNING, NO_ERROR, 0 );

      if(gSvcStatus.dwCurrentState == SERVICE_RUNNING)
      {
//        remove(LOG_FILE_NAME);
      }

      /* function here */
      ProgramMain();

      WaitForSingleObject(ghSvcStopEvent, INFINITE);
      ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
    }

    void SvcCtrlHandler (DWORD dwCtrl)
    {
      /* Handle the requested control code */

      switch(dwCtrl) 
      {  
        case SERVICE_CONTROL_STOP:
          onExit = 1;
          ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
          SetEvent(ghSvcStopEvent); /* Signal the service to stop */
          ReportSvcStatus(gSvcStatus.dwCurrentState, NO_ERROR, 0);
          break;
        case SERVICE_CONTROL_INTERROGATE:
          break;
        case SERVICE_CONTROL_SHUTDOWN:
          break;
        default:
          break;
      }
    }

    void ReportSvcStatus (DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
    {
      static DWORD dwCheckPoint = 1;

      /* Fill in the SERVICE_STATUS structure */
      gSvcStatus.dwCurrentState  = dwCurrentState;
      gSvcStatus.dwWin32ExitCode = dwWin32ExitCode;
      gSvcStatus.dwWaitHint      = dwWaitHint;

      if(dwCurrentState == SERVICE_START_PENDING)
        gSvcStatus.dwControlsAccepted = 0;
      else
        gSvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

      if((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
        gSvcStatus.dwCheckPoint = 0;
      else
        gSvcStatus.dwCheckPoint = dwCheckPoint++;

      /* Report the status of the service to the SCM. */
      SetServiceStatus(gSvcStatusHandle, &gSvcStatus);


    }   

    void SvcReportEvent (LPTSTR szFunction) 
    { 
      HANDLE    hEventSource;
      LPCTSTR   lpszStrings[2];
      TCHAR     Buffer[80];

      hEventSource = RegisterEventSource(NULL, SERVICE_NAME);

      if(NULL != hEventSource)
      {
        sprintf(Buffer, "%s failed with %d", szFunction, (int) GetLastError());

        lpszStrings[0] = SERVICE_NAME;
        lpszStrings[1] = Buffer;

        ReportEvent( hEventSource,        /* event log handle */
                     EVENTLOG_ERROR_TYPE, /* event type */
                     0,                   /* event category */
                     SVC_ERROR,           /* event identifier */
                     NULL,                /* no security identifier */
                     2,                   /* size of lpszStrings array */
                     0,                   /* no binary data */
                     lpszStrings,         /* array of strings */
                     NULL);               /* no binary data */

        DeregisterEventSource(hEventSource);
      }
    }

     void SysServiceStartup()
    {
      StartServiceCtrlDispatcher(ServiceTable);
    }
    BOOL WINAPI gotExitCmd(DWORD dwType)
    {
        switch(dwType) {
        case CTRL_C_EVENT:
        case CTRL_CLOSE_EVENT:
        case CTRL_SHUTDOWN_EVENT:
          onExit = 1;
          break;
        }
        return TRUE;
    }
#else

BOOL WINAPI gotExitCmd(DWORD dwType)
{
    switch(dwType) {
		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			onExit = 1;
			break;
    }
    return TRUE;
}

#endif
/********************************************/

int main(int argc, char** argv)
{
#ifdef DAEMON
	SysServiceStartup();
#else
	if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)gotExitCmd,TRUE))
	{
		printf("Fail to listen Ctrl-C signal!\n");
	}
	ProgramMain();
#endif
	return 0;
}
