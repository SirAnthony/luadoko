/*
 * CUDataMacroExt.h
 *
 *  Created on: 20.05.2012
 *
 *
 * This file was created for decomposition. It included once in
 * CUData.cpp and does not need to be precompiled.
 *
 */

#include "defines.h"

#ifdef _MSC_VER
    #define typeof decltype
#endif

//////////////////////////////////////////////////////////////////////////
// Macros for declaration of execution method
#define EXEC_METHOD_DECL(FIELD)                                         \
    template <typename T>                                               \
    int exec_##FIELD( lua_State* L ) {                                  \
        CUData* ud = check_userdata<T>( L, 1 );                         \
        if( !ud->getUser( ) )                                           \
            luaL_error( L, "Object destroyed" );                        \
        else                                                            \
            lua_remove( L, 1 ); /* Remove userdata from stack  */       \
        return pushToLua(L, static_cast<T*>(ud->getUser())->FIELD(L));  \
    }

#define EXEC_NORET_METHOD_DECL(FIELD)                                   \
    template <typename T>                                               \
    int exec_##FIELD( lua_State* L ) {                                  \
        CUData* ud = check_userdata<T>( L, 1 );                         \
        if( !ud->getUser( ) )                                           \
            luaL_error( L, "Object destroyed" );                        \
        else                                                            \
            lua_remove( L, 1 ); /* Remove userdata from stack  */       \
        return static_cast<T*>(ud->getUser())->FIELD(L);                \
    }


// Macros for getting the pointer to getter method
#define EXEC_METHOD(ID, FIELD)                          \
    &exec_##FIELD<TL::TypeAt<ClassesList, ID>::Result>


// Macros, that puts new method entry in metatable methods array
#define EXEC_METHOD_ENTRY(ID, FIELD)    \
    { #FIELD, EXEC_METHOD(ID, FIELD) },
//////////////////////////////////////////////////////////////////////////



// Macros for declaration of execution method with specified NAME
#define EXEC_NAMED_METHOD_DECL(NAME, FIELD)                             \
    template <typename T>                                               \
    int exec_##NAME( lua_State* L ){                                    \
        CUData* ud = check_userdata<T>( L, 1 );                         \
        if( !ud->getUser( ) )                                           \
            luaL_error( L, "Object destroyed" );                        \
        else                                                            \
            lua_remove( L, 1 ); /* Remove userdata from stack  */       \
        return pushToLua(L, static_cast<T*>(ud->getUser())->FIELD(L));  \
    }


// Macros for getting the pointer to getter method with specified NAME
#define EXEC_NAMED_METHOD(ID, NAME)                         \
    &exec_##NAME<TL::TypeAt<ClassesList, ID>::Result>


// Macros, that puts new method entry in metatable methods array
#define EXEC_NAMED_METHOD_ENTRY(ID, NAME)    \
    { #NAME, EXEC_NAMED_METHOD(ID, NAME) },
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// Macros for declaration of getter function method
#define GETTER_FUNC_METHOD_DECL(FIELD)                                          \
    template <typename T>                                                   \
    int getter_##FIELD(lua_State* L) {                                      \
        CUData* ud = check_userdata<T>(L, 1);                               \
        if( !ud->getUser() )                                                \
            luaL_error( L, "Object destroyed" );                            \
        return pushToLua(L, static_cast<T*>(ud->getUser())->FUNC_GET##FIELD());  \
    }

//////////////////////////////////////////////////////////////////////////


// Macros for declaration of getset function method
#define GETSET_FUNC_METHOD_DECL(FIELD)                                                          \
    template <typename T>                                                                       \
    int getset_##FIELD( lua_State* L ){                                                         \
        CUData* ud = check_userdata<T>(L, 1);                                                   \
        if (!ud->getUser())                                                                     \
            luaL_error(L, "Object destroyed");                                                  \
        T* obj = static_cast<T*>(ud->getUser());                                                \
        bool typecheck = CHECK_INPUT_PARAM(L, 2, obj->FUNC_GET##FIELD());                       \
        luaL_argcheck(L, typecheck || lua_isnone(L, 2), 2, "typecheck failed");                 \
        if (typecheck)                                                                          \
            obj->FUNC_SET##FIELD( getFromLua< typeof(obj->FUNC_GET##FIELD()) >( L, 2 ) );            \
        return pushToLua(L, obj->FUNC_GET##FIELD() );                                           \
    }

//////////////////////////////////////////////////////////////////////////


// Macros for declaration of getset method fro flag with specified NAME
// FLAG is flagname: in IsDead, the FLAG will be Dead
#define GETSET_FLAG_METHOD_DECL(FLAG)                                                            \
    template <typename T>                                                                        \
    int getset_##FLAG(lua_State* L)                                                              \
    {                                                                                            \
        CUData* ud = check_userdata<T>(L, 1);                                                    \
        T* obj = static_cast<T*>(ud->getUser());                                                 \
        if (!obj)                                                                                \
            luaL_error(L, "Object destroyed");                                                   \
        bool typecheck = lua_isboolean(L, 2) != 0;                                               \
        luaL_argcheck(L, typecheck || lua_isnone(L, 2), 2, "typecheck lua_isboolean failed");    \
        if (typecheck)                                                                           \
            getFromLua<bool>(L, 2) ? obj->FLAG_SET##FLAG() : obj->FLAG_REMOVE##FLAG();           \
        return pushToLua(L, obj->FLAG_CHECK##FLAG());                                            \
    }

//////////////////////////////////////////////////////////////////////////
