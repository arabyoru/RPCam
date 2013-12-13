#include <errno.h>
#include <fcntl.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/ioctl.h>

#include "V4L2Device.h"
#include "RPCamSockManager.h"
#include "RPCamFileManager.h"

RPCamFileManager theFileMgr;

V4L2Device::V4L2Device(void)
{
	CLEARx0(mszDevName);
	mFdDevice = -1;
	meIoMethod = IO_METHOD_READ;
	mnBuffNum = 0;
	mnFrameCnt = 1;
}

V4L2Device::~V4L2Device(void)
{

}

int V4L2Device::xioctl(int nFd, int nRequest, void *pArg)
{
	int nRst;

	do {
		nRst = ioctl(nFd, nRequest, pArg);
	} while (-1 == nRst && EINTR == errno);

	return nRst;
}

bool V4L2Device::OpenDevice( unsigned int nFrameCnt, char *szDevName, enum io_method eIoMethod)
{
	struct stat stFileSt;

	if(!szDevName)
		strncpy(mszDevName, DFT_DEV_NAME, sizeof(mszDevName));
	else
		strncpy(mszDevName, szDevName, sizeof(mszDevName));	

	if(stat(mszDevName, &stFileSt)<0)
	{
		fprintf(stderr, "File Exist %s | ERRNO:%d(%s) \n", mszDevName, errno, strerror(errno));
		return false;
	}

	mnFrameCnt = nFrameCnt;

	// Check Character Deivce Driver
	if(!S_ISCHR(stFileSt.st_mode))
	{
		fprintf(stderr, "%s is Not Device Driver\n", mszDevName);
		return false;
	}

	mFdDevice = open(mszDevName, O_RDWR | O_NONBLOCK, 0);
	if(mFdDevice<0)
	{
		fprintf(stderr, "Cannot Open %s | ERRNO:%d(%s) \n", mszDevName, errno, strerror(errno));
		return false;	
	}
	return true;
}

bool V4L2Device::InitDevice(void)
{
	struct v4l2_capability stCap;
	struct v4l2_cropcap stCroCap;
	struct v4l2_crop stCrop;
	struct v4l2_format stFormat;
	int nMin;

	// 현재 디바이스가 무엇을 하는지 알아오기 위한 ioctl. 
	if(-1 == xioctl(mFdDevice, VIDIOC_QUERYCAP, &stCap))
	{
		if(EINVAL == errno)
		{
			fprintf(stderr, "%s No V4L2 Device \n", mszDevName);
			goto ERR_RETURN;
		}
		else
		{
			fprintf(stderr, "%s Failure Query Capability \n", mszDevName);
			goto ERR_RETURN;
		}
	}

	// Check Video Capture Device 
	if(!(stCap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
	{
		fprintf(stderr, "%s is No Video Capture Device \n", mszDevName);
		goto ERR_RETURN;
	}

	// Check V4L2 Method 
	switch(meIoMethod)
	{
	case IO_METHOD_READ:
	{
		if(!(stCap.capabilities & V4L2_CAP_READWRITE))
		{
			fprintf(stderr, "%s is No Video Capture Device \n", mszDevName);
			goto ERR_RETURN;
		}
		break;
	}
	case IO_METHOD_MMAP:
	case IO_METHOD_USERPTR:
	{
		if(!(stCap.capabilities & V4L2_CAP_STREAMING))
		{
			fprintf(stderr, "%s Does Not Support Device \n", mszDevName);
			goto ERR_RETURN;
		}
		break;
	}
	default:
	{
		fprintf(stderr, "%s Invalid IO Method (%d) \n", mszDevName, meIoMethod);
		goto ERR_RETURN;
		break;
	}
	}

	// 
	CLEARx0(stCroCap);
	stCroCap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if(0 ==  xioctl(mFdDevice, VIDIOC_CROPCAP, &stCroCap))
	{
		stCrop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		stCrop.c = stCroCap.defrect;

		if(-1 == xioctl(mFdDevice, VIDIOC_S_CROP, &stCrop))
		{
			switch(errno)
			{
				case EINVAL: 
					fprintf(stderr, "%s Cropping is Not Support Device \n", mszDevName); 
					goto ERR_RETURN; 
					break;
				default: 		break; // Error Ignored
			}
		}
	}
	else
	{
		// Error ignored 
	}

	CLEARx0(stFormat);
	stFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
#if 0
	if(mWidth!=0)
		stFormat.fmt.pix.width = mWidth;
	else
		stFormat.fmt.pix.width = DFT_WIDTH_SIZE;
	if(mHeight!=0)
		stFormat.fmt.pix.height = mHeight;
	else
		stFormat.fmt.pix.height = DFT_HEIGHT_SIZE;

	stFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	stFormat.fmt.pix.field = V4L2_FIELD_INTERLACED;
#endif
	if(-1 == xioctl(mFdDevice, VIDIOC_G_FMT, &stFormat))
	{
		fprintf(stderr, "%s Invalid Video Format|W(%d)h(%d) errno(%d)\n", 
			mszDevName, stFormat.fmt.pix.width, stFormat.fmt.pix.height, errno);
		goto ERR_RETURN;
	}

	nMin = stFormat.fmt.pix.width*2;
	if(stFormat.fmt.pix.bytesperline < nMin)
		stFormat.fmt.pix.bytesperline = nMin;
	
	nMin = stFormat.fmt.pix.bytesperline * stFormat.fmt.pix.height;
	if(stFormat.fmt.pix.sizeimage < nMin)
		stFormat.fmt.pix.sizeimage = nMin;

	// fprintf(stdout, "WIDTH:%d HEIGHT(%d)|IMAGE SIZE(%d)", 
	// 	stFormat.fmt.pix.width, stFormat.fmt.pix.height, stFormat.fmt.pix.sizeimage);

	switch(meIoMethod)
	{
		case IO_METHOD_READ: InitRead(stFormat.fmt.pix.sizeimage); break;
		case IO_METHOD_MMAP: /* TOOD */ break;
		case IO_METHOD_USERPTR: /* TOOD */ break;
	}

	if(!theFileMgr.Init())
	{
		fprintf(stderr, "File Manager Init Failure\n");
		goto ERR_RETURN;	
	} 

	return true;

ERR_RETURN:
	return false;
}

void V4L2Device::CloseDevice(void)
{
	if(close(mFdDevice) == -1)
	{
		fprintf(stderr, "Deice File Close Filaure \n");
		return ;
	}
	mFdDevice = -1;
	theFileMgr.Free();
}

void V4L2Device::FreeDevice(void)
{
	//unsigned int i=0;
	switch(meIoMethod)
	{
		case IO_METHOD_READ:
		{
			free(mpFrameBuff[0].pStartPtr); break;
		}
		case IO_METHOD_MMAP: /* TODO */ break;
		case IO_METHOD_USERPTR: /* TODO */ break;
	}
	free(mpFrameBuff);
	mpFrameBuff = NULL;
}

bool V4L2Device::StartCapturing(void)
{
	//enum v4l2_buf_type eBufType;

	switch(meIoMethod)
	{
		case IO_METHOD_READ: /* Nothing */ break;
		case IO_METHOD_MMAP: /* TODO */ break;
		case IO_METHOD_USERPTR: /* TODO */ break;
	}
	return true;
}

void V4L2Device::StopCapturing(void)
{
	//enum v4l2_buf_type eBufType;

	switch(meIoMethod)
	{
		case IO_METHOD_READ: /* Nothing */ break;
		case IO_METHOD_MMAP: /* TODO */ break;
		case IO_METHOD_USERPTR: /* TODO */ break;
	}
}

void V4L2Device::InitRead(unsigned int nBufferSize)
{
	mpFrameBuff =(FrameBuffer *)calloc(1, sizeof(*mpFrameBuff));
	if(!mpFrameBuff)
	{
		fprintf(stderr, "Out of Memory \n");
		return;
	}

	mpFrameBuff[0].nLength = nBufferSize;
	mpFrameBuff[0].pStartPtr = malloc(nBufferSize);

	if(!mpFrameBuff[0].pStartPtr)
	{
		free(mpFrameBuff);
		fprintf(stderr, "Out of Memory -  \n");
		return;
	}
	return;
}

bool V4L2Device::ReadFrame(void)
{
//	struct v4l2_buffer buf;

	switch(meIoMethod)
	{
		case IO_METHOD_READ:
		{
			if(-1 == read(mFdDevice, mpFrameBuff[0].pStartPtr, mpFrameBuff[0].nLength))
			{
				switch(errno)
				{
					case EAGAIN:
					{
						fprintf(stderr, "Read Failure (%d) \n",errno );
						goto ERR_EAGAIN; break;
					}
					case EIO: break;
					default:
					{
						fprintf(stderr, "Read Failure (%d) \n",errno );
						goto ERR_EAGAIN; break;
					}
				}
			}
			ProcessImage(mpFrameBuff[0].pStartPtr, mpFrameBuff[0].nLength);
			break;
		}
		case IO_METHOD_MMAP: /* TODO */ break;
		case IO_METHOD_USERPTR: /* TODO */ break;
	}

	return true;
ERR_EAGAIN:
	return false;


}

void V4L2Device::ProcessImage(const void *pFrame, size_t nLength)
{
	ST_JPGINFO_PACKET stJpgInfo;

	memset(&stJpgInfo, 0x00, sizeof(stJpgInfo));
	if(pFrame)
	{

		if(RPCamSockManager::GetClientCnt()>0)
		{
			DBGTRC;
			stJpgInfo.stHeader.nFrom = 10;
			stJpgInfo.stHeader.nTo = 20;
			stJpgInfo.stHeader.tCreateTime = time(0);
			//stJpgInfo.pBody = new char[nLength + 1];

			memset(stJpgInfo.pBody , 0x00, nLength);
			memcpy(stJpgInfo.pBody, pFrame, nLength);
			//write(mFdJpgFile, pFrame, nLength);
			stJpgInfo.nLen = nLength;
			theFileMgr.PushData(stJpgInfo);	
			printf("EnQueue Size : %d \n", theFileMgr.GetQueueSize());
		}	

		
		//write(mFdJpgFile, pFrame, nLength);
		//
		// memset(&stJpgInfo, 0x00, sizeof(stJpgInfo));
		// if(RPCamSockManager::GetClientCnt()>0)
		// {
		// 	DBGINT(RPCamSockManager::GetClientCnt());
		// 	//stJpgInfo.pCamImagePtr = malloc(nLength);
		// 	if(stJpgInfo.pCamImagePtr)
		// 	{
		// 		DBGINT(nLength);
		// 		memcpy(stJpgInfo.pCamImagePtr, pFrame, nLength);
		// 		strncpy(stJpgInfo.szFileName, mszCurFileName, sizeof(stJpgInfo.szFileName));
		// 		stJpgInfo.nCamImageSize = nLength;
		// 		stJpgInfo.bIsEvent = false;
		// 		stJpgInfo.bIsTrsf = false;
		// 		stJpgInfo.bIsSave = false;
		// 		stJpgInfo.tCreateTime = time(0);

		// 		theFileMgr.PushData(stJpgInfo);
		// 	}
		// }		
	}
}

void V4L2Device::MainProcessing(void)
{
	unsigned int nCnt = mnFrameCnt;
	char szFileName[FILE_NAME_LEN];

	while(nCnt-- > 0)
	{
		CLEARx0(szFileName);
		MakeFileName("KDG", (mnFrameCnt-nCnt), szFileName);
		 //mFdJpgFile = open(szFileName, O_WRONLY | O_CREAT, 0644);
		// if(mFdJpgFile<0)
		// {
		// 	fprintf(stderr, "File Open Failure\n");
		// 	continue;
		// }
			
		while(true)
		{
			fd_set stFd;
			struct timeval tv;
			int nRst;

			FD_ZERO(&stFd);
			FD_SET(mFdDevice, &stFd);

			tv.tv_sec = 2;
			tv.tv_usec = 2;

			nRst = select(mFdDevice + 1, &stFd, NULL, NULL, &tv);
			if(nRst == -1)
			{
				if(EINTR == errno) continue;
				fprintf(stderr, "Select Failure (%d)\n", errno);
				return;
			}

			if(nRst == 0)
			{
				fprintf(stderr, "Selec TimeOut \n");
				return;
			}
			if(ReadFrame()) break;
		}
		//close(mFdJpgFile);
		fprintf(stderr, " %d ",  mnFrameCnt-nCnt);
	}
	fprintf(stderr, "\n");
}

void V4L2Device::MakeFileName(const char *szPrfx, unsigned int nIdx, char* szFileName)
{
	time_t nCurTime = time(0);
	struct tm stTm;

	localtime_r(&nCurTime, &stTm);
	snprintf(szFileName, FILE_NAME_LEN, "%s_%d%02d%02d_%02d%02d%02d_%d.jpg", 
		szPrfx, stTm.tm_year+1900, stTm.tm_mon+1, stTm.tm_mday, stTm.tm_hour, stTm.tm_min, stTm.tm_sec, nIdx);

	strncpy(mszCurFileName, szFileName, sizeof(mszCurFileName));

}




































