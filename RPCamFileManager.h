#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <queue>

#include "TypeDef.h"
#include "RPPacket.h"
#include "RPCircularQueue.h"


typedef std::queue<ST_JPGINFO_PACKET> 		QJpgData;
typedef class RPCircularQueue<ST_JPGINFO_PACKET, MAX_CQUEUE_SIZE>				RPCQueue;

class RPCamFileManager
{
public:
	RPCamFileManager(void);
	virtual ~RPCamFileManager(void);

	bool Init(void);
	void Free(void);

	bool SetSaved(int nIdx, const char *szFileName, bool bEnable=true);
	bool SetTrsf(int nIdx, const char *szFileName, bool bEnable=true);
	bool SetEvent(int nIdx, const char *szEvent, bool bEnable=true);

	bool PushData(ST_JPGINFO_PACKET& stJpgData);
	bool PopFrontData(ST_JPGINFO_PACKET& stJpgData);
	int GetQueueSize(void) { return mcqJpgData.GetSize(); }
	bool IsEmpty(void) { return mcqJpgData.IsEmpty()?true:false; }

private:
	//QJpgData mqJpgData;
	RPCQueue mcqJpgData;

};
