#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h> 

#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>



#include "TypeDef.h"

class RPCamSockManager
{
public:
	RPCamSockManager(void);
	virtual ~RPCamSockManager(void);

	bool Init(void);
	void Free(void);


	bool StartRPCamSockManager(void);
	void StopRPCamSockManager(void);

	bool IsRunning(void) { return mbRun; };

	void* ThreadRun(void *pArg);

	inline static int GetClientCnt(void) { return mClientCnt; };

public:
	void* 				mpArg;
	bool 				mbRun;
protected:

	int mRPSock;
	int mClntSock;
	int mArrClntSocks[MAX_CLIENT_SIZE];
	int mnClntNum;

	socklen_t mClntAddrSize;

	struct sockaddr_in mRPSockAddr;
	struct sockaddr_in mClntSockAddr;
	
	pthread_t 			mSockThread;
	pthread_attr_t 		mSockThreadAttr;

	pthread_t 			mClntThread;
	pthread_attr_t 		mClntThreadAttr;	

	static int mClientCnt;

};