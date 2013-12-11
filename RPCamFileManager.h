#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <queue>

#include "TypeDef.h"

struct JpgFileInfo
{
	char szFileName[FILE_NAME_LEN];
	//char szFilePath[FILE_PATH_LEN];

	time_t tCreateTime;

	bool bIsEvent;
	bool bIsTrsf;
	bool bIsSave;
	bool bRsvd;

	void *pCamImagePtr;
	size_t nCamImageSize;
};

typedef std::queue<JpgFileInfo> 		QJpgData;

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

	bool PushData(JpgFileInfo& stJpgData);
	bool PopFrontData(JpgFileInfo& stJpgData);
	int GetQueueSize(void) { return mqJpgData.size(); }
	bool IsEmpty(void) { return mqJpgData.empty()?true:false; }


private:
	QJpgData mqJpgData;
};
