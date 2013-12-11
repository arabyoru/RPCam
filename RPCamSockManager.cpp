

#include "RPCamSockManager.h"


void* SockThreadFunc(void *pArg)
{
	void* pRet = NULL;
	RPCamSockManager *pSockManger = (RPCamSockManager *)pArg;
	pRet = pSockManger->ThreadRun(pSockManger->mpArg);
	pSockManger->mbRun = false;
	return pRet;
}


RPCamSockManager::RPCamSockManager(void)
{
	mbRun = false;
}

RPCamSockManager::~RPCamSockManager(void)
{

}

bool RPCamSockManager::Init(void)
{
	return true;
}

void RPCamSockManager::Free(void)
{

}

bool RPCamSockManager::StartRPCamSockManager(void)
{
	int nRst = -1;

	nRst = pthread_create(&mSockThread, NULL, SockThreadFunc, this);
	if(nRst!=0)
	{
		fprintf(stderr, "Creation SockThread Failure..");
		return false;
	}

	mbRun = true;
	return true;
}

void RPCamSockManager::StopRPCamSockManager(void)
{
	mbRun = false;
}

void* RPCamSockManager::ThreadRun(void *pArg)
{
	void *pRet = NULL;

	while(true)
	{
		DBGTRC;
		sleep(1);
		if(!mbRun){DBGTRC; break;}
	}

	return pRet;
}