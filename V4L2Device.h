#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <linux/videodev2.h>

#include "TypeDef.h"

enum io_method 
{
	IO_METHOD_READ,
	IO_METHOD_MMAP,
	IO_METHOD_USERPTR,
};

struct FrameBuffer {
	void * pStartPtr;
	size_t nLength;
};

class V4L2Device
{
public:
	V4L2Device(void);
	virtual ~V4L2Device(void);

	bool OpenDevice(unsigned int nFrameCnf=1, char *szDevName=NULL, enum io_method eIoMethod=IO_METHOD_READ);
	bool InitDevice(void);
	void CloseDevice(void);
	void FreeDevice(void);

	bool StartCapturing(void);
	void StopCapturing(void);
	bool ReadFrame(void);

	void MainProcessing(void);

private:
	void InitRead(unsigned int nBufferSize);
	int xioctl(int nFd, int nRequest, void *pArg);
	void ProcessImage(const void *pFrame, size_t nLength);
	void MakeFileName(const char *szPrfx, unsigned int nIdx, char* szFileName);
	
private:
	enum io_method meIoMethod;
	char 		mszDevName[DEV_NAME_LEN];
	int 		mFdDevice;
	int 		mFdJpgFile;

	FrameBuffer *mpFrameBuff;
	unsigned int mnBuffNum;
	unsigned int mnFrameCnt;
};