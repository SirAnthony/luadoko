/*
 * TypeList.h
 *
 *  Created on: 19.05.2012
 *
 * Origin: iiChantra
 *
 */

#ifndef TYPELIST_H_
#define TYPELIST_H_


struct NullType {};

template <typename T, typename U>
struct Typelist
{
	typedef T Head;
	typedef U Tail;
};


#include "generated/TypeListDef.h"


namespace TL
{
	// Length
	template <typename TList> struct Length;

	template <>
	struct Length<NullType>
	{
		enum { value = 0 };
	};

	template <typename T, typename U>
	struct Length< Typelist<T, U> >
	{
		enum { value = 1 + Length<U>::value };
	};

	// TypeAt
	template <typename TList, unsigned int index> struct TypeAt;

	template <typename Head, typename Tail>
	struct TypeAt<Typelist<Head, Tail>, 0>
	{
		typedef Head Result;
	};

	template <typename Head, typename Tail, unsigned int i>
	struct TypeAt<Typelist<Head, Tail>, i>
	{
		typedef typename TypeAt<Tail, i - 1>::Result Result;
	};

	// IndexOf
	template <typename TList, typename T> struct IndexOf;

	//template <typename T>
	//struct IndexOf<NullType, T>
	//{
	//  enum { value = -1 };
	//};

	template <typename T, typename Tail>
	struct IndexOf<Typelist<T, Tail>, T>
	{
		enum { value = 0 };
	};

	template <typename Head, typename Tail, typename T>
	struct IndexOf<Typelist<Head, Tail>, T>
	{
	private:
		enum { temp = IndexOf<Tail, T>::value };
	public:
		enum { value = (temp == -1 ? -1 : 1 + temp) };
	};
}


#endif /* TYPELIST_H_ */
