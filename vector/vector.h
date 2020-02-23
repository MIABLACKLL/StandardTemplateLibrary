#pragma once
#include <cstddef>
#include <memory>

namespace MIA {
	template <class Value>
	class VectorIterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = Value;
		using pointer = Value * ;
		using iterator = Value * ;
		using const_iterator = const Value *;
		using reference = Value & ;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
	};

	template <class Value, class Alloc = std::allocator<Value>>
	class Vector
	{
	public:
		using value_type = Value;
		using base_iterator = VectorIterator<value_type>;
		using iterator = typename base_iterator::iterator;
		using const_iterator = typename base_iterator::const_iterator;
		using pointer = typename base_iterator::pointer;
		using reference = typename base_iterator::reference;
		using size_type = typename base_iterator::size_type;
		using difference_type = typename base_iterator::difference_type;

	protected:
		Alloc m_DataAllocator;
		iterator m_Start;
		iterator m_Finish;
		iterator m_EndOfStorage;
		
		void _insert_aux(iterator position, const reference vValue);
		void _insert_aux(iterator position, size_type vSize,const reference vValue);
		void _insert_aux(iterator position, iterator vBegin, iterator vFinish);

		void _deallocate()
		{
			if (m_Start)
				m_DataAllocator.deallocate(m_Start, m_EndOfStorage - m_Start);
		}

		iterator _allocate(size_type vSize)
		{
			return m_DataAllocator.allocate(vSize);
		}

		void _destroy(iterator vStart,iterator vFinish)
		{
			std::_Destroy_range(vStart, vFinish);
		}

		void _destroy(iterator vPosition)
		{
			m_DataAllocator.destroy(vPosition);
		}

		void _construct(iterator vPosition, const reference vValue)
		{
			m_DataAllocator.construct(vPosition, vValue);
		}

		void _fillInitialize(size_type vSize,const reference vValue)
		{
			try
			{
				m_Start = _allocate(vSize);
				m_Finish = m_Start + vSize;
				m_EndOfStorage = m_Finish;
				std::uninitialized_fill_n(m_Start, vSize, vValue);
			}
			catch (...)
			{
				if (m_Start)
					_destroy(m_Start, m_Finish);
				_deallocate();
			}
		}

		template<class IterType>
		void _fillInitialize(IterType vStart, IterType vFinish, std::forward_iterator_tag)
		{
			size_type InitSize = static_cast<size_type>(std::distance(vStart, vFinish));
			try
			{
				m_Start = _allocate(InitSize);
				m_Finish = m_Start + InitSize;
				m_EndOfStorage = m_Finish;
				std::uninitialized_copy(vStart, vFinish, m_Start);
			}
			catch (...)
			{
				if (m_Start)
					_destroy(m_Start, m_Finish);
				_deallocate();
			}
		}

		template<class IterType>
		void _fillInitialize(IterType vStart, IterType vFinish, std::input_iterator_tag)
		{
			try
			{
				for (IterType it = vStart; it != vStart; it++)
					emplace_back(*it);

			}
			catch (...)
			{
				if (m_Start)
					_destroy(m_Start, m_Finish);
				_deallocate();
			}

		}

	public:
		iterator begin() { return m_Start; }
		iterator end() { return m_Finish; }
		size_type size() const { return static_cast<size_type>(m_Finish - m_Start); }
		size_type capacity() const{ return static_cast<size_type>(m_EndOfStorage - m_Start); }
		bool empty()const { return m_Start == m_Finish; }
		reference operator[](size_type vIndex) { return *(m_Start + vIndex); }

		Vector() :m_Start(nullptr), m_Finish(nullptr), m_EndOfStorage(nullptr) {}
		explicit Vector(size_type vSize, const reference vValue) { _fillInitialize(vSize, vValue); }
		explicit Vector(size_type vSize) { _fillInitialize(vSize, value_type()); }

		template<class IterType>
		explicit Vector(IterType vBegin, IterType vFinish) { _fillInitialize(vBegin, vFinish, std::iterator_traits<IterType>::iterator_category); }

		Vector(const Vector& vVector) { _fillInitialize(vVector.begin(), vVector.end(), vVector.capacity()); }

		~Vector()
		{
			while (m_Finish != m_Start)
				m_DataAllocator.destory(m_Finish--);
			deallocate();
		
		}

	private:

	public:
		reference front() { return *m_Start; }
		reference back() { return *m_Finish; }

		void push_back(const reference vValue){}
		void push_back(value_type&& vValue){}

		template<class ...ValueArg>
		reference emplace_back()
		{
			if (m_Finish != m_EndOfStorage)
				_construct(m_Finish++, vValue);
			else
				_insert_aux(m_Finish,vValue);
		}

		void pop_back() { _destory(--m_Finish); }

		iterator erase(iterator vPosition);
		iterator erase(iterator vStart, iterator vFinish);

		void insert(iterator position, const reference vValue);
		void insert(iterator position, size_type vSize, const reference vValue);
		void insert(iterator position, iterator vBegin, iterator vFinish);

		void clear() { erase(m_Start, m_Finish); }


	};
}
