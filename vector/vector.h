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

		void _deallocate(iterator vStart, size_type vSize)
		{
			if (vStart)
				m_DataAllocator.deallocate(vStart, vSize);
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

		void _fill_initialize(size_type vSize,const value_type& vValue)
		{
			try
			{
				m_Start = _allocate(vSize);
				m_Finish = m_Start + vSize;
				m_EndOfStorage = m_Finish;
				std::uninitialized_fill_n(m_Start, vSize, vValue);
			}
			catch (...) { _tidy(); throw;}
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
			catch (...) { _tidy(); throw;}
		}

		template<class IterType>
		void _fill_initialize(IterType vStart, IterType vFinish, std::input_iterator_tag)
		{
			try
			{
				for (; vFinish != vStart; vStart++)
					emplace_back(*vStart);
			}
			catch (...) { _tidy(); throw;}
		}

		void _tidy()
		{
			if (m_Start && m_Start != m_Finish)
				_destroy(m_Start, m_Finish);
			_deallocate(m_Start, capacity());
		}

	public:
		iterator begin() const noexcept { return m_Start; }
		iterator end() const noexcept { return m_Finish; }
		size_type size() const noexcept { return static_cast<size_type>(m_Finish - m_Start); }
		size_type capacity() const noexcept { return static_cast<size_type>(m_EndOfStorage - m_Start); }
		bool empty()const noexcept { return m_Start == m_Finish; }
		reference operator[](size_type vIndex) { return *(m_Start + vIndex); }

		Vector() :m_Start(nullptr), m_Finish(nullptr), m_EndOfStorage(nullptr) {}
		explicit Vector(const size_type vSize, const value_type& vValue) { _fill_initialize(vSize, vValue); }
		explicit Vector(const size_type vSize) { _fill_initialize(vSize, value_type()); }
		template<class IterType,class = std::enable_if_t<std::_Is_iterator<IterType>>>
		explicit Vector(IterType vBegin, IterType vFinish) { _fill_initialize(vBegin, vFinish, std::iterator_traits<IterType>::iterator_category{}); }
		Vector(const Vector& vVector) { _fill_initialize(vVector.begin(), vVector.end(), std::random_access_iterator_tag{}); }
		Vector(std::initializer_list<value_type> vList) { _fill_initialize(vList.begin(), vList.end(), std::random_access_iterator_tag{}); }

		~Vector() noexcept{ _tidy(); }

	private:
		bool __has_unused_capacity() { return !(m_Finish == m_EndOfStorage); }
		size_type __unused_capacity() { return static_cast<size_type>(m_EndOfStorage - m_Finish); }
		bool __check_address(iterator vPosition) { return vPosition >= m_Start && vPosition < m_Finish; }
		bool __check_address(iterator vStart, iterator vFinish) { return vStart <= vFinish && vStart >= m_Start && vFinish <= m_Finish; }

		size_type __calculate_growth(size_type vNewSize)
		{
			return vNewSize > size() * 2 ? size() * 2 : vNewSize + size();
		}

		template<class ...ValueArg>
		iterator __emplace_reallocate(const iterator vPosition,ValueArg&&... vArg)
		{
			const size_type OldSize = size();
			const size_type NewSize = __calculate_growth(OldSize + 1);
			const size_type PositionOff = static_cast<size_type>(vPosition - m_Start);
			iterator NewStart = _allocate(NewSize);
			iterator NewFinish = NewStart;
			try
			{
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
				_deallocate(m_Start,capacity());
				m_Start = NewStart;
				m_Finish = NewFinish;
				m_EndOfStorage = NewStart + NewSize;
			}
			catch (...) 
			{ 
				_destroy(NewStart, NewFinish);
				_deallocate(NewStart, NewSize);
				throw;
			}
			return m_Start + PositionOff;
		}

		template<class IterType>
		void __insert_range(const_iterator vPosition, IterType vStart, IterType vFinish, std::input_iterator_tag)
		{
			if (vStart == vFinish)
				return;
			const size_type PositionOff = static_cast<size_type>(vPosition - m_Start);
			const size_type OldSize = size();

			for (; vFinish != vStart; vStart++)
				emplace_back(*vStart);

			std::reverse(m_Start + PositionOff, m_Start + OldSize);
			std::reverse(m_Start + OldSize, m_Finish);
			std::reverse(m_Start + PositionOff, m_Finish);
		}

		template<class IterType>
		void __insert_range(const_iterator vPosition, IterType vStart, IterType vFinish, std::forward_iterator_tag)
		{
			if (vStart == vFinish)
				return;
			size_type InsertSize = static_cast<size_type>(std::distance(vStart, vFinish));
			const size_type PositionOff = static_cast<size_type>(vPosition - m_Start);
			if (InsertSize == 1) { emplace(vPosition, *vStart); }
			else
			{
				if (InsertSize <= __unused_capacity())
				{
					const size_type PositionAfterElement = static_cast<size_type>(m_Finish - vPosition);
					if (vPosition == m_Finish)
					{
						m_Finish = std::uninitialized_copy(vStart, vFinish,vPosition)
					}
					else if (PositionAfterElement > vSize)
					{
						std::uninitialized_copy(m_Finish - InsertSize, m_Finish, m_Finish);
						std::copy_backward(vPosition, m_Finish - InsertSize, m_Finish);
						std::copy(vStart, vFinish, vPosition);
						m_Finish += InsertSize;
					}
					else
					{
						std::uninitialized_copy(vPosition, m_Finish, m_Finish + (InsertSize - PositionAfterElement));
						_destroy(vPosition, m_Finish);
						std::uninitialized_copy(vStart, vFinish, vPosition);
						m_Finish += InsertSize - PositionAfterElement;
					}
				}
				else
				{
					const size_type OldSize = size();
					const size_type NewSize = __calculate_growth(OldSize + InsertSize);
					iterator NewStart = _allocate(NewSize);
					iterator NewFinish = NewStart;
					try
					{
						NewFinish = std::uninitialized_copy(m_Start, vPosition, NewStart);
						NewFinish = std::uninitialized_copy(vStart, vFinish, NewFinish);
						NewFinish = std::uninitialized_copy(vPosition, m_Finish, NewFinish);
						_destroy(m_Start, m_Finish);
						_deallocate(m_Start, capacity());
						m_Start = NewStart;
						m_Finish = NewFinish;
						m_EndOfStorage = NewStart + NewSize;
					}
					catch (...) 
					{ 
						_destroy(NewStart, NewFinish);
						_deallocate(NewStart, NewSize);
						throw;
					}
				}
			}
			return m_Start + PositionOff;
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
		iterator emplace(const_iterator vioPosition, ValueArg&&... vArg)
		{
			_ASSERT(__check_address(vioPosition));
			const size_type PositionOff = static_cast<size_type>(vPosition - m_Start);
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
			}
			else
				__emplace_reallocate(vioPosition, std::forward(vArg)...);
			return m_Start + PositionOff;
		}

		void pop_back() { _destory(--m_Finish); }

		iterator erase(const_iterator vPosition)
		{
			_ASSERT(__check_address(vPosition));
			if (vPosition + 1 != m_Finish)
				std::copy(vPosition + 1, m_Finish, vPosition);
			_destroy(m_Finish--);
			return vPosition;
		}

		iterator erase(const_iterator vStart, const_iterator vFinish)
		{
			_ASSERT(__check_address(vStart, vFinish));
			if ( vFinish != vStart)
			{
				if (vFinish != m_Finish)
					std::copy(vFinish, m_Finish, vStart);
				iterator NewFinish = m_Finish - (vFinish - vStart);
				_destroy(NewFinish, m_Finish);		
			}
			return vStart;
		}

		void resize(size_type vNewSize) { resize(vNewSize, value_type()); }

		void resize(size_type vNewSize, const reference vValue)
		{
			if (vNewSize < size())
				erase(m_Start + vNewSize, m_Finish);
			else if (vNewSize > size())
				insert(m_Finish, vNewSize - size(), vValue);
		}

		iterator insert(iterator vPosition, const reference vValue) 
		{ 
			_ASSERT(__check_address(vPosition));
			return emplace(vPosition, vValue); 
		}

		iterator insert(iterator vPosition, value_type&& vValue) 
		{ 
			_ASSERT(__check_address(vPosition));
			return emplace(vPosition, std::move(vValue)); 
		}

		iterator insert(iterator vPosition, size_type vSize, const reference vValue)
		{
			_ASSERT(__check_address(vPosition) && vSize);
			const size_type PositionOff = static_cast<size_type>(vPosition - m_Start);
			if (vSize == 1) { emplace(vPosition, vValue); }
			else
			{
				if (vSize <= __unused_capacity())
				{
					const size_type PositionAfterElement = static_cast<size_type>(m_Finish - vPosition);
					if (vPosition == m_Finish)
					{
						m_Finish = std::uninitialized_fill_n(m_Finish, vSize, vValue);
					}
					else if (PositionAfterElement > vSize)
					{
						std::uninitialized_copy(m_Finish - vSize, m_Finish, m_Finish);
						std::copy_backward(vPosition, m_Finish - vSize, m_Finish);
						std::fill(vPosition, vPosition + vSize, vValue);
						m_Finish += vSize;
					}
					else
					{
						std::uninitialized_fill_n(m_Finish, vSize - PositionAfterElement, vValue);
						std::uninitialized_copy(vPosition, m_Finish, m_Finish + (vSize - PositionAfterElement));
						std::fill(vPosition, m_Finish, vValue);
						m_Finish += vSize - PositionAfterElement;
					}
				}
				else
				{
					const size_type OldSize = size();
					const size_type NewSize = __calculate_growth(OldSize + vSize);
					iterator NewStart = _allocate(NewSize);
					iterator NewFinish = NewStart;
					try
					{
						NewFinish = std::uninitialized_copy(m_Start, vPosition, NewStart);
						NewFinish = std::uninitialized_fill_n(NewFinish, vSize, vValue);
						NewFinish = std::uninitialized_copy(vPosition, m_Finish, NewFinish);
					}
					catch (...) 
					{ 
						
					}
					_destroy(m_Start, m_Finish);
					_deallocate(m_Start, capacity());
					m_Start = NewStart;
					m_Finish = NewFinish;
					m_EndOfStorage = NewStart + NewSize;
				}
			}
			return m_Start + PositionOff;
		}

		template<class IterType>
		iterator insert(iterator vPosition, IterType vBegin, IterType vFinish)
		{
			_ASSERT(__check_address(vPosition) && vBegin <= vFinish);
			const size_type PositionOff = static_cast<size_type>(vPosition - m_Start);
			__insert_range(vPosition, vBegin, vFinish, std::iterator_traits<IterType>::iterator_category{});
			return m_Start + PositionOff;
		}

		iterator insert(iterator vPosition, std::initializer_list<value_type>vList)
		{
			_ASSERT(__check_address(vPosition));
			const size_type PositionOff = static_cast<size_type>(vPosition - m_Start);
			__insert_range(vPosition, vBegin, vFinish, std::random_access_iterator_tag{});
			return m_Start + PositionOff;
		}

		void clear() { erase(m_Start, m_Finish); }
	};
}
