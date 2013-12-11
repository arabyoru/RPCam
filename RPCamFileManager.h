#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "TypeDef.h"

struct JpgFileInfo
{
	char szFileName[FILE_NAME_LEN];
	char szFilePath[FILE_PATH_LEN];

	time_t tCreateTime;

	bool bIsEvent;
	bool bIsTrsf;
	bool bIsSave;
};


class RPCamFileManager
{
public:
	RPCamFileManager(void);
	virtual ~RPCamFileManager(void);

	bool Init(void);
	void Free(void);

	bool SetSaved(const char *szFileName);
	bool SetTrsf(const char *szFileName);
	bool SetEvent(const char *szEvent);


private:
	struct JpgFileInfo mJpgInfo;
};