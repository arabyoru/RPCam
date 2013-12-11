#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h> 

#include "TypeDef.h"

class RPCamMain
{
public:
	RPCamMain(void);
	virtual ~RPCamMain(void);

	bool InitMain(int nArg, char** szArg);
	void FreeMain(void);

	bool StartMain(unsigned int nCnt);
	void StopMain(void);

	void* ThreadRun(void *pArg);
	bool IsRunning(void) { return mbRun; };

public: 
	void* 				mpArg;
	bool 				mbRun;
protected:
	unsigned int mnCnt;
	pthread_t 			mMainThread;
	pthread_attr_t 		mThreadAttr;
};
