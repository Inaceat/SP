#include "stdafx.h"


char serviceName[] = "TTTServerLocator";

SERVICE_STATUS_HANDLE statusHandle;
SERVICE_STATUS status;

bool shouldWork;
std::thread worker;


void Worker()
{
	while (shouldWork)
	{
		Sleep(1000);
	}
}


void ControlsHandler(DWORD dwControl)
{
	switch (dwControl)
	{
		case SERVICE_CONTROL_SHUTDOWN:
		case SERVICE_ACCEPT_STOP:
		{
			shouldWork = false;
			worker.join();

			status.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(statusHandle, &status);

		}break;

		default:
			break;
	}
}


void TestMain(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors)
{
	statusHandle = RegisterServiceCtrlHandlerA(serviceName, ControlsHandler);

	shouldWork = true;
	worker = std::thread(Worker);

	
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_RUNNING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	status.dwWin32ExitCode = NO_ERROR;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;

	SetServiceStatus(statusHandle, &status);
}


int main()
{
	SERVICE_TABLE_ENTRYA services[2];
	services[1].lpServiceName = nullptr;
	services[1].lpServiceProc = nullptr;

	services[0].lpServiceName = serviceName;
	services[0].lpServiceProc = TestMain;


	StartServiceCtrlDispatcherA(services);
	
    return 0;
}
