#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <queue>

#include "TypeDef.h"

#pragma pack(1)

struct ST_JPGINFO_HEADER
{
	//unsigned char 			szFiileName[FILE_NAME_LEN]; //256

	unsigned char			nFrom;
	unsigned char			nTo;
	unsigned char			nRsvd[3]; //5

	unsigned char 			nIsEvent;
	unsigned char 			nIsTrsf;
	unsigned char 			nIsSave; //3 

	unsigned int			tCreateTime; // 4
};

struct ST_JPGINFO_PACKET
{
	struct ST_JPGINFO_HEADER		stHeader;
	char							pBody[614400];
	unsigned int					nLen;
};

#pragma pack(0)

class RPPacket
{
public:
	RPPacket(void);
	~RPPacket(void);

	bool Init(void);
	void Free(void);

};