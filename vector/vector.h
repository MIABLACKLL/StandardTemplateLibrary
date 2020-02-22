#pragma once
#include <cstddef>
#include <memory>

namespace MIA {
	template <class Value, class Alloc = std::allocator<Value>>
	class Vector
	{
	public:
		using ValueType = Value;
		using Pointer = ValueType * ;
		using Iterator = ValueType * ;
		using Reference = ValueType & ;
		using SizeType = size_t;
		using DifferenceType = ptrdiff_t;

	protected:
		Alloc m_DataAllocator;
		Iterator m_Start;
		Iterator m_Finish;
		Iterator m_EndOfStorage;
		
	};
}
