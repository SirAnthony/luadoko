/*
 * routines.cpp
 *
 *  Created on: 05.09.2013
 *
 */
#include "LuaRoutines.h"
#include <string>
#include <map>
#include <assert.h>

namespace
{

	typedef std::map< LuaRegRef, int > RegRefCountsMap;
	static RegRefCountsMap regRefCounts;
	lua_State* Lst;

}

// Adds local lua state.
void LuaRoutines::routines_open( lua_State* L )
{
	Lst = L;
}


lua_State* routines_state( )
{
	return Lst;
}


//////////////////////////////////////////////////////////////////////////
// Adds to registry stack top value and returns reference to it
// Value removed from the stack top
LuaRegRef LuaRoutines::registry_add( )
{
	// Stack: ... data
	return luaL_ref( Lst, LUA_REGISTRYINDEX);	// Stack:
}

// Removes referenced value from registry
void LuaRoutines::registry_remove( LuaRegRef& r )
{
	lua_unref( Lst, r );
	r = LUA_NOREF;
}

// Puts registry value to stack top
void LuaRoutines::registry_get( lua_State* L, LuaRegRef r )
{
	lua_rawgeti( Lst, LUA_REGISTRYINDEX, r );
	lua_xmove( Lst, L, 1 );
}

// Помещает в procref ссылку реестра луа на функцию, находящуюся в стеке под номером n
void LuaRoutines::reg_proc( lua_State* L, LuaRegRef* procref, int n )
{
	luaL_argcheck( L, lua_isfunction(L, n) || lua_isnil(L, n), n,
			"Function or nil expected" );
	reg_object( L, procref, n );
}

// Помещает в procref ссылку реестра луа на объект, находящийся в стеке под номером n
void LuaRoutines::reg_object( lua_State* L, LuaRegRef* procref, int n )
{
	release_object( procref );

	if( !lua_isnil(L, n)){
		// TODO: сделать перемещение функции на самый верх стека и забирать ее оттуда
		// Сейчас же просто забирается одно верхнее значение со стека, это может быть и не функция
		lua_xmove( L, Lst, 1 );
		*procref = registry_add( );
		reserve_object( *procref );
	}
}

// Увеличивает счетчик количества использований ссылки на реестр
void LuaRoutines::reserve_object( LuaRegRef procref )
{
	if( procref == LUA_NOREF)
		return;
	RegRefCountsMap::iterator it = regRefCounts.find( procref );
	if( it == regRefCounts.end( ) ){
		regRefCounts[procref] = 1;
	}else{
		assert( it->second >= 0 );
		++(it->second);
	}
}

// Уменьшает счетчик количества использований ссылки на реестр.
// Если счетчик достигает 0, объект из реестра удаляется.
void LuaRoutines::release_object( LuaRegRef* procref )
{
	if( *procref == LUA_NOREF)
		return;

	RegRefCountsMap::iterator it = regRefCounts.find( *procref );
	if( it == regRefCounts.end( ) )
		return;

	assert( it->second > 0 );
	if( it->second <= 0 ){
		luaL_error( Lst,
				"LuaScript::ReleaseObject: procref %d references count is %d.\n",
				*procref, it->second );
		it->second = 1;
	}

	--(it->second);
	if( it->second == 0 ){
		registry_remove( *procref );
		*procref = LUA_NOREF;
		regRefCounts.erase( it );
	}
}

int LuaRoutines::exec_chunk( unsigned args )
{
	int level = lua_gettop( Lst ) - args - 1; //Верх стека - аргументы - одна функция
	if( lua_pcall( Lst, args, LUA_MULTRET, 0 ) ){
		const char* err = lua_tostring( Lst, -1 );
		luaL_error( Lst, "In ExecChunk(): %s.\n", err );
		return -1;
	}
	return lua_gettop( Lst ) - level; //Новый верх стека - старый = количество результатов
}

int LuaRoutines::exec_chunk_from_reg( LuaRegRef r, unsigned args )
{
	registry_get( Lst, r );

	int i = lua_gettop( Lst ) - args;
	if( i <= 0 )
		i = 1;

	lua_insert( Lst, i );

	return exec_chunk( args );
}
