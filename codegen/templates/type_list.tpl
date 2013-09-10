/*
 * TypeListDef.h
 *
 * THIS FILE WAS CREATED AUTOMATICALLY!
 *
 * This file is part of TypeList.h and included once there.
 * It was separated for automatic generation.
 *
 */

#define TYPELIST_1(T1)                                      Typelist<T1, NullType>
<% types = [] %>
% for number in range(2, count + 1):
<%
  types.append('T{0}'.format(number))
  types_string = ', '.join(types)
%>
#define TYPELIST_${number}(T1, ${types_string}) Typelist<T1, TYPELIST_${number - 1}(${types_string})>
% endfor
