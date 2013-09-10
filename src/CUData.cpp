/*
 * CUData.cpp
 *
 *  Created on: 17.05.2012
 *
 * Origin: iiChantra
 *
 */

#include "CUData.h"
#include "CUDataUser.h"
#include "LuaRoutines.h"

void CUData::onGC()
{
	if(pUser){
		pUser->setUData(NULL);
		pUser = NULL;
	}
}

void CUData::clearUser()
{
	pUser = NULL;
	LuaRoutines::registry_remove(regRef);
	LuaRoutines::registry_remove(storage);
}



