/*
 * CUDataUser.cpp
 *
 *  Created on: 17.05.2012
 *
 * Origin: iiChantra
 *
 */

#include "CUDataUser.h"
#include "CUData.h"
#include "LuaRoutines.h"

CUDataUser::~CUDataUser()
{
	if( pUdata )
		pUdata->clearUser();
}


int CUDataUser::pushUData( lua_State* L )
{
	if( !pUdata ){
		pUdata = this->createUData();
	}

	if( !pUdata ){
		luaL_error( L, "CUDataUser::pushUData: userdata creation failed" );
	}else{
		LuaRoutines::registry_get( L, pUdata->getRegRef() );
		return 1;
	}
	return 0;
}

int CUDataUser::pushUStorage( lua_State* L )
{
	if( !pUdata )
		return 0;
	if( pUdata->getStorage() < 0 ){
		lua_newtable( L );
		pUdata->setStorage( LuaRoutines::registry_add() );
	}
	LuaRoutines::registry_get( L, pUdata->getStorage() );
	return 1;
}


CUDataUser* getUserData( lua_State* L, int idx )
{
	luaL_argcheck( L, lua_isuserdata( L, idx ), idx, "Userdata expected" );

	CUData* ud = reinterpret_cast<CUData*>( lua_touserdata( L, idx ) );
	if( !ud )
		return NULL;
	return ud->getUser();
}


