#pragma once
#ifndef _MIA_LIST_
#define _MIA_LIST_

#include <cstddef>
#include <memory>
namespace MIA
{
	template<class Value>
	class ListNode
	{
	public:
		using node_pointer = ListNode<Value>*;
		using node_type = Value;
		node_pointer m_NodePrev;
		node_pointer m_NodeNext;
		node_type m_NodeValue;
	};

	template<class MyList>
	class ListIterator
	{
		using iterator_category = std::bidirectional_iterator_tag;


	};

	template<class AllocType>
	class ListAlloc
	{

	};

	template<class Value, class Alloc = std::allocator<Value>>
	class list
	{

	};
}

#endif //_MIA_LIST_