
#include <errno.h>

#include "RPCamSockManager.h"
#include "RPCamFileManager.h"

extern RPCamFileManager theFileMgr;

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

	memset(&mRPSockAddr, 0x00, sizeof(mRPSockAddr));
	memset(&mClntSockAddr, 0x00, sizeof(mClntSockAddr));

	return true;
}

void RPCamSockManager::Free(void)
{
	close(mRPSock);
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
	int option = 1; 
	char test[] = "Hello\n";

	// Open server socket
	mRPSock = socket(PF_INET, SOCK_STREAM, 0);
	if(mRPSock == -1)
	{
		fprintf(stderr, "Socket Init Failure\n" );
		perror("Sock : ");
		return NULL;
	}

	// SO_REUSEADDR = true
	if(setsockopt(mRPSock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option))<0)
	{
		perror("Sock Opt : ");
		return NULL; 
	}

	mRPSockAddr.sin_family = AF_INET;
	mRPSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//mRPSockAddr.sin_addr.s_addr = inet_addr ("100.100.105.99");
	mRPSockAddr.sin_port = htons(DFT_SERVER_PORT);

	if(bind(mRPSock, (struct sockaddr *)&mRPSockAddr, sizeof(mRPSockAddr)) == -1)
	{
		fprintf(stderr, "Socket Bind Failure(%d)\n", errno );
		perror("Bind Error : ");
		close(mRPSock);
		return NULL;	
	}
	JpgFileInfo stJpgInfo;
	while(true)
	{
		// Listen Client .. 
		if(listen(mRPSock, 5) == -1)
		{
			perror("Listen : ");
			fprintf(stderr, "Socket Listen Failure\n" );
		}
		mClntAddrSize = sizeof(mClntSockAddr);

		mClntSock= accept(mRPSock, (struct sockaddr *)&mClntSockAddr, &mClntAddrSize);

		// Write Client
		theFileMgr.PopFrontData(stJpgInfo);
		write(mClntSock, &stJpgInfo, sizeof(stJpgInfo)+1);
		free(stJpgInfo.pCamImagePtr);
		DBGTRC;
		sleep(1);
	}

	close(mClntSock);

	return pRet;
}