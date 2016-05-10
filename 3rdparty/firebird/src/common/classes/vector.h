/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		vector.h
 *	DESCRIPTION:	static array of simple elements
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "../common/gdsassert.h"
#include <string.h>

namespace Firebird {

// Very fast static array of simple types
template <typename T, FB_SIZE_T Capacity>
class Vector
{
public:
	Vector() : count(0) {}

	T& operator[](FB_SIZE_T index) throw()
	{
  		fb_assert(index < count);
  		return data[index];
	}

	const T& operator[](FB_SIZE_T index) const throw()
	{
  		fb_assert(index < count);
  		return data[index];
	}

	T* begin() throw() { return data; }
	T* end() throw() { return data + count; }
	const T* begin() const throw() { return data; }
	const T* end() const throw() { return data + count; }
	bool hasData() const throw() { return (count != 0); }
	FB_SIZE_T getCount() const throw() { return count; }
	FB_SIZE_T getCapacity() const throw() { return Capacity; }
	void clear() throw() { count = 0; }

	void insert(FB_SIZE_T index, const T& item)
	{
	  fb_assert(index <= count);
	  fb_assert(count < Capacity);
	  memmove(data + index + 1, data + index, sizeof(T) * (count++ - index));
	  data[index] = item;
	}

	FB_SIZE_T add(const T& item)
	{
		fb_assert(count < Capacity);
		data[count] = item;
  		return ++count;
	}

	T* remove(FB_SIZE_T index) throw()
	{
  		fb_assert(index < count);
  		memmove(data + index, data + index + 1, sizeof(T) * (--count - index));
		return &data[index];
	}

	void shrink(FB_SIZE_T newCount) throw()
	{
		fb_assert(newCount <= count);
		count = newCount;
	}

	void join(const Vector<T, Capacity>& L) throw()
	{
		fb_assert(count + L.count <= Capacity);
		memcpy(data + count, L.data, sizeof(T) * L.count);
		count += L.count;
	}

	// prepare vector to be used as a buffer of capacity items
	T* getBuffer(FB_SIZE_T capacityL) throw()
	{
		fb_assert(capacityL <= Capacity);
		count = capacityL;
		return data;
	}

	void push(const T& item)
	{
		add(item);
	}

	T pop()
	{
		fb_assert(count > 0);
		count--;
		return data[count];
	}

	// This method only assigns "pos" if the element is found.
	// Maybe we should modify it to iterate directy with "pos".
	bool find(const T& item, FB_SIZE_T& pos) const
	{
		for (FB_SIZE_T i = 0; i < count; i++)
		{
			if (data[i] == item)
			{
				pos = i;
				return true;
			}
		}
		return false;
	}

protected:
	FB_SIZE_T count;
	T data[Capacity];
};

// Template for default value comparsion
template <typename T>
class DefaultComparator
{
public:
	static bool greaterThan(const T& i1, const T& i2)
	{
	    return i1 > i2;
	}
};

// Template to convert value to index directly
template <typename T>
class DefaultKeyValue
{
public:
	static const T& generate(const void* /*sender*/, const T& item) { return item; }
	static const T& generate(const T& item) { return item; }
};

// Fast sorted array of simple objects
// It is used for B+ tree nodes lower, but can still be used by itself
template <typename Value, FB_SIZE_T Capacity, typename Key = Value,
	typename KeyOfValue = DefaultKeyValue<Value>,
	typename Cmp = DefaultComparator<Key> >
class SortedVector : public Vector<Value, Capacity>
{
public:
	SortedVector() : Vector<Value, Capacity>() {}
	bool find(const Key& item, FB_SIZE_T& pos) const
	{
		FB_SIZE_T highBound = this->count, lowBound = 0;
		while (highBound > lowBound)
		{
			const FB_SIZE_T temp = (highBound + lowBound) >> 1;
			if (Cmp::greaterThan(item, KeyOfValue::generate(this, this->data[temp])))
				lowBound = temp + 1;
			else
				highBound = temp;
		}
		pos = lowBound;
		return highBound != this->count &&
			!Cmp::greaterThan(KeyOfValue::generate(this, this->data[lowBound]), item);
	}
	FB_SIZE_T add(const Value& item)
	{
	    FB_SIZE_T pos;
  	    find(KeyOfValue::generate(this, item), pos);
		this->insert(pos, item);
		return pos;
	}
};

} // namespace Firebird

#endif
