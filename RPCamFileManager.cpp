
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

bool RPCamFileManager::PushData(JpgFileInfo& stJpgData)
{
	mqJpgData.push(stJpgData);
	return true;
}

bool RPCamFileManager::PopFrontData(JpgFileInfo& stJpgData)
{
	if(IsEmpty()) return false;

	 stJpgData = mqJpgData.front();
	 mqJpgData.pop();
	 return true;
}



























