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

		template<class ...ValueArg>
		reference _construct(iterator vPosition, ValueArg&&... vValue)
		{
			m_DataAllocator.construct(vPosition, std::forward<ValueArg>(vValue)...);
			reference voResult = *vPosition;
			return voResult;
		}

		void _fill_initialize(size_type vSize,const reference vValue)
		{
			try
			{
				m_Start = _allocate(vSize);
				m_Finish = m_Start + vSize;
				m_EndOfStorage = m_Finish;
				std::uninitialized_fill_n(m_Start, vSize, vValue);
			}
			catch (...) { _tidy(); }
		}

		template<class IterType>
		void _fill_initialize(IterType vStart, IterType vFinish, std::forward_iterator_tag)
		{
			size_type InitSize = static_cast<size_type>(std::distance(vStart, vFinish));
			try
			{
				m_Start = _allocate(InitSize);
				m_Finish = m_Start + InitSize;
				m_EndOfStorage = m_Finish;
				std::uninitialized_copy(vStart, vFinish, m_Start);
			}
			catch (...) { _tidy(); }
		}

		template<class IterType>
		void _fill_initialize(IterType vStart, IterType vFinish, std::input_iterator_tag)
		{
			try
			{
				for (IterType it = vStart; it != vStart; it++)
					emplace_back(*it);
			}
			catch (...) { _tidy(); }
		}

		void _tidy()
		{
			if (m_Start && m_Start != m_Finish)
				_destroy(m_Start, m_Finish);
			_deallocate();
		}

	public:
		iterator begin() const noexcept { return m_Start; }
		iterator end() const noexcept { return m_Finish; }
		size_type size() const noexcept { return static_cast<size_type>(m_Finish - m_Start); }
		size_type capacity() const noexcept { return static_cast<size_type>(m_EndOfStorage - m_Start); }
		bool empty()const noexcept { return m_Start == m_Finish; }
		reference operator[](size_type vIndex) { return *(m_Start + vIndex); }

		Vector() :m_Start(nullptr), m_Finish(nullptr), m_EndOfStorage(nullptr) {}
		explicit Vector(size_type vSize, const reference vValue) { _fill_initialize(vSize, vValue); }
		explicit Vector(size_type vSize) { _fill_initialize(vSize, value_type()); }
		template<class IterType>
		explicit Vector(IterType vBegin, IterType vFinish) { _fill_initialize(vBegin, vFinish, std::iterator_traits<IterType>::iterator_category); }
		Vector(const Vector& vVector) { _fill_initialize(vVector.begin(), vVector.end(), std::random_access_iterator_tag); }
		Vector(std::initializer_list<value_type> vList) { _fill_initialize(vList.begin(), vList.end(), std::random_access_iterator_tag); }

		~Vector() noexcept{ _tidy(); }

	private:
		bool __has_unused_capacity() { return !(m_Finish == m_EndOfStorage); }

		size_type __calculate_growth(size_type vOldSize)
		{
			return vOldSize != 0 ? vOldSize * 2 + 1 : 1;
		}

		template<class ...ValueArg>
		iterator __emplace_reallocate(iterator vPosition,ValueArg&&... vArg)
		{
			const size_type OldSize = size();
			const size_type NewSize = __calculate_growth(OldSize);
			try
			{
				iterator NewStart = _allocate(NewSize);
				iterator NewFinish = NewStart;
				if (vPosition == m_Finish)
				{
					NewFinish = std::uninitialized_copy(m_Start, m_Finish, NewStart);
					_construct(NewFinish++, std::forward(vArg)...);
				}
				else
				{
					NewFinish = std::uninitialized_copy(m_Start, vPosition, NewStart);
					_construct(NewFinish++, std::forward(vArg)...);
					NewFinish = std::uninitialized_copy(vPosition, m_Finish, NewFinish);
				}
				_destroy(m_Start, m_Finish);
				_deallocate();
				m_Start = NewStart;
				m_Finish = NewFinish;
				m_EndOfStorage = NewStart + NewSize;
			}
			catch (...) { _tidy(); }
		}

	public:
		reference front() { return *m_Start; }
		reference back() { return *m_Finish; }

		void push_back(const reference vValue) { emplace_back(vValue); }
		void push_back(value_type&& vValue) { emplace_back(std::move(vValue);) }

		template<class ...ValueArg>
		reference emplace_back(ValueArg&&... vArg)
		{
			reference voResult = *emplace(m_Finish++, std::forward(vArg)...);
			return voResult;
		}

		template<class ...ValueArg>
		iterator emplace(iterator vioPosition, ValueArg&&... vArg)
		{
			if (__has_unused_capacity())
			{
				if (vioPosition == m_Finish)
					_construct(vioPosition, std::forward(vArg)...);
				else
				{
					value_type NewObj(std::forward(vArg)...);
					_construct(m_Finish, std::forward(*(m_Finish - 1)));
					m_Finish++;
					std::copy_backward(vioPosition, m_Finish - 2, m_Finish - 1);
					*vioPosition = std::move(NewObj);
				}
				return vioPosition;
			}
			return __emplace_reallocate(m_Finish, std::forward(vArg)...);
		}

		void pop_back() { _destory(--m_Finish); }

		iterator erase(iterator vPosition);
		iterator erase(iterator vStart, iterator vFinish);

		iterator insert(iterator position, const reference vValue);
		iterator insert(iterator position, size_type vSize, const reference vValue);
		iterator insert(iterator position, iterator vBegin, iterator vFinish);
		iterator insert(iterator position, std::initializer_list<value_type>vList);


		void clear() { erase(m_Start, m_Finish); }


	};
}
