#include "RPCamMain.h"
#include "V4L2Device.h"

#include "RPCamSockManager.h"

V4L2Device 				theV4L2Dev;
RPCamSockManager 		theRPCamSockMgr;

void* ThreadFunc(void* pArg)
{
	void* pRet = NULL;
	RPCamMain* pMain = (RPCamMain*)pArg;
	pRet = pMain->ThreadRun(pMain->mpArg);
	pMain->mbRun = false;
	return pRet;
}

RPCamMain::RPCamMain(void)
{

}

RPCamMain::~RPCamMain(void)
{

}

bool RPCamMain::InitMain(int nArg, char** szArg)
{
	return true;
}

void RPCamMain::FreeMain(void)
{

}

bool RPCamMain::StartMain(unsigned int nCnt)
{
	int nRst;
	size_t thr_stack_size;

	if(nCnt>0)
		mnCnt = nCnt;
	else
		mnCnt = 1;
	
	pthread_attr_getstacksize(&mThreadAttr, &thr_stack_size);   // 스택사이즈 확인

	fprintf(stdout, "StakSize : %dMB(%dKB) \n", (int)(thr_stack_size/1024)/1024, (int)thr_stack_size/1024);
	//pthread_attr_setstacksize(&mThreadAttr, thr_stack_size);
	nRst = pthread_create(&mMainThread, NULL, ThreadFunc, this);
	if(nRst!=0)
	{
		fprintf(stderr, "Creation Main Thread Failure..");
		return false;
	}
	mbRun = true;
	return true;
}

void RPCamMain::StopMain(void)
{
	mbRun = false;
}

void* RPCamMain::ThreadRun(void *pArg)
{	
	if(theRPCamSockMgr.Init())
	{
		// Start Socket Manager Thread
		if(!theRPCamSockMgr.StartRPCamSockManager())
		{
			fprintf(stderr, "RPCam Manger Start Failure.. \n");
			theRPCamSockMgr.Free();
			return NULL;
		}
	}
	else 
	{
		fprintf(stderr, "RPCam Manger Init Failure.. \n");
		return NULL;
	}

	// Device Open
	if(theV4L2Dev.OpenDevice(mnCnt))
	{
		if(theV4L2Dev.InitDevice())
		{
			theV4L2Dev.StartCapturing();
		}
		else
		{
			mbRun = false;
			return NULL;
		} 
	}
	else
	{
		mbRun = false; 
		return NULL;
	}	

	while(true)
	{
		theV4L2Dev.MainProcessing();
		if(!mbRun) break;
		sleep(1);
	}

	theRPCamSockMgr.Free();
	theRPCamSockMgr.StopRPCamSockManager();

	theV4L2Dev.FreeDevice();
	theV4L2Dev.CloseDevice();
	mbRun = false;

	return NULL;
}

