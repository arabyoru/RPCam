
#include "RPCamFileManager.h"

RPCamFileManager::RPCamFileManager(void)
{

}

RPCamFileManager::~RPCamFileManager(void)
{

}

bool RPCamFileManager::Init(void)
{
	return true;
}
void RPCamFileManager::Free(void)
{
	
}

bool RPCamFileManager::PushData(ST_JPGINFO_PACKET& stJpgData)
{
	mcqJpgData.EnQueue(stJpgData);
	return true;
}

bool RPCamFileManager::PopFrontData(ST_JPGINFO_PACKET& stJpgData)
{
	if(IsEmpty()) return false;
	 stJpgData = mcqJpgData.DeQueue();
	 return true;
}

