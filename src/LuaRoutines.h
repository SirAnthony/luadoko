/*
 * routines.h
 *
 *  Created on: 05.09.2013
 *
 */

#ifndef LUA_ROUTINES_H_
#define LUA_ROUTINES_H_

typedef int LuaRegRef;
struct luaState;

namespace LuaRoutines
{
	void routines_open( lua_State* L );
	lua_State* routines_state( );

	LuaRegRef registry_add( );
	void registry_remove( LuaRegRef& r );
	void registry_get( lua_State* L, LuaRegRef r );

	void reg_proc( lua_State* L, LuaRegRef* procref, int n );
	void reg_object( lua_State* L, LuaRegRef* procref, int n );
	void reserve_object( LuaRegRef procref );
	void release_object( LuaRegRef* procref );

	int exec_chunk( unsigned args = 0 );
	int exec_chunk_from_reg( LuaRegRef r, unsigned args = 0 );
}

#endif /* LUA_ROUTINES_H_ */
