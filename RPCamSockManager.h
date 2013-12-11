#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h> 

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

public:
	void* 				mpArg;
	bool 				mbRun;
protected:

	pthread_t 			mSockThread;
	pthread_attr_t 		mSockThreadAttr;

};