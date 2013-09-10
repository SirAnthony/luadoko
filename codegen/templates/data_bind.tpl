/*
 * CUDataBind.h
 * <%! from time import strftime as time %>
 *  Created on: ${"%d of %B" | time}
 *
 * THIS FILE WAS GENERATED AUTOMATICALLY!
 */

//////////////////////////////////////////////////////////////////////////
//
// How to add new class?
// 1. Put name of class in typelist ClassesList (the order of classes is important, they enumerated from 0).
//    Class must implement createUData virtual function from CUDataUser class
// 2. Declare the function for userdata creation (use UDATA_CREATOR_FUNC_DECL macro)
// 3. Add RegisterTypeMetatable call for new class in RegisterAllTypeMetatables function.
//
//////////////////////////////////////////////////////////////////////////
//
// 4-a. How to add new getter for field?
//    Add GETTER_METHOD_DECL(field_name)
//    Add GETTER_METHOD_ENTRY(number, field_name) in ud_meta array, where
//      number is number of class in ClassesList typelist;
//      field_name is name of public field in this class
// 4-b. How to add new getter/setter for field?
//    Add GETSET_METHOD_DECL(field_name)
//    Add GETSET_METHOD_ENTRY(number, field_name) in ud_meta array, where
//      number is number of class in ClassesList typelist;
//      field_name is name of public field in this class
//
// There are more declaration macros like GETSET_METHOD_DECL in CUData.cpp.
//
// If there is field with equal names in different classes, only one declaration must be.
// METAMETHODS_MAX_COUNT value can be increased, if needed.
//
//////////////////////////////////////////////////////////////////////////
//
// Structure of this file:
// This file consists of several include blocks. This helps to separate implementation details
// from declarations and to simplify the maintance of the code.
// This file is included by the udata.cpp only.

<% classes_keys = [x[0] for x in classes] %>

#if INCLUDE_BLOCK == 1

// Includes of declarations of all types in typelist
% for name, data in classes:
/* Includes for ${name} */
% for header in data['includes']:
#include "${header}"
% endfor

% endfor

// Typelist with all types, that can be passed to lua as userdata
typedef TYPELIST_${len(classes)}( ${', '.join(classes_keys)} ) ClassesList;

#elif INCLUDE_BLOCK == 2

// 2. For each type, it's needed to explicitly declare functions for userdata creation.
% for name in classes_keys:
UDATA_CREATOR_FUNC_DECL(${name});
% endfor

#elif INCLUDE_BLOCK == 3

// 3. Registration of metatables
// This functions will be called on lua initialization in SCRIPT::InitScript
void RegisterAllTypeMetatables(lua_State* L)
{
    // Please, don't change this line
    std::fill(udata_refs, udata_refs + TL::Length<ClassesList>::value, LUA_NOREF);

    // Make RegisterTypeMetatable calls for each type
    % for name in classes_keys:
    RegisterTypeMetatable<${name}>(L);
    % endfor
}


#elif INCLUDE_BLOCK == 4

// 4. Declaration of methods in metatables

% for classname, data in classes:
<% capname = classname.upper() %>
// Declarations for ${classname}
#define DECL_${capname}_METH        \\

% for method in data['methods']:
    ${method.decl()}                \\

% endfor

DECL_${capname}_METH

// Methatable record for ${classname}
#define ${capname}_METH_ENTRY(ID)     \\

% if 'inherit' in data:
    ${'{0}_METH_ENTRY(ID)'.format(data['inherit'].upper())}  \\

% endif
% for method in data['methods']:
    ${method.entry()}                 \\

% endfor

## classname, data in classes
% endfor


// Meta methods
const size_t METAMETHODS_MAX_COUNT = 70;
static const struct luaL_reg ud_meta[TL::Length<ClassesList>::value][METAMETHODS_MAX_COUNT] = {
% for i, cname in enumerate(map(lambda x: x.upper(), classes_keys)):
    {
        STD_METHODS(${i}),
        ${cname}_METH_ENTRY(${i})
        END
    },
% endfor
};



#endif
