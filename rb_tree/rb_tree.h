#pragma once
#include <cstddef>
#include <memory>

using rbTreeColorType = bool;
const rbTreeColorType rbTreeColorRed = false;
const rbTreeColorType rbTreeColorBlack = true;

class CrbTreeNodeBase
{
public:
	using ColorType = rbTreeColorType;
	using BasePtr = CrbTreeNodeBase*;

	ColorType m_Color;
	BasePtr m_Parent;
	BasePtr m_Left;
	BasePtr m_Right;

	static BasePtr minimum(BasePtr x)
	{
		while (x->m_Left != nullptr) x = x->m_Left;
		return x;
	}

	static BasePtr maximum(BasePtr x)
	{
		while (x->m_Right != nullptr) x = x->m_Right;
		return x;
	}
};

template<class ValueType>
class CrbTreeNode : public CrbTreeNodeBase
{
public:
	using LinkType = CrbTreeNode<ValueType>*;
	ValueType m_ValueField;
	CrbTreeNode(ValueType vValue) :m_ValueField(vValue) {};
};

class CrbTreeBaseIterator
{
	using BasePtr = CrbTreeNodeBase::BasePtr;
	using DifferenceType = ptrdiff_t;

	BasePtr m_Node;

	void increment()
	{
		if (m_Node->m_Right)
		{
			m_Node = m_Node->m_Right;
			while(m_Node->m_Left)
				m_Node = m_Node->m_Left;
		}
		else
		{
			BasePtr NodeParent = m_Node->m_Parent;
			while (NodeParent->m_Right == m_Node)
			{
				m_Node = NodeParent;
				NodeParent = NodeParent->m_Parent;
			}
			if (m_Node->m_Right != NodeParent)//当根节点右边无节点时的情况，此时m_Node为最终结果(end()header)
				m_Node = NodeParent;
		}
	}

	void decrement()
	{
		if (m_Node->m_Color == rbTreeColorRed && m_Node->m_Parent->m_Parent == m_Node)//end()header节点
			m_Node = m_Node->m_Right;
		else if (m_Node->m_Left)
		{
			m_Node = m_Node->m_Left;
			while (m_Node->m_Right)
				m_Node = m_Node->m_Right;
		}
		else
		{
			BasePtr NodeParent = m_Node->m_Parent;
			while (NodeParent->m_Left == m_Node)
			{
				m_Node = NodeParent;
				NodeParent = NodeParent->m_Parent;
			}
			m_Node = NodeParent;
		}
	}
};

template<class Value, class Ref, class Ptr>
class CrbTreeIterator:public CrbTreeBaseIterator
{

};

template<class Key, class Value, class KeyOfValue, class Compare = std::less<Key>, class Alloc = std::allocator<CrbTreeNode<Value>>>
class CrbTree
{
protected:
	using VoidPointer = void*;
	using BasePointer = CrbTreeNodeBase*;
	using rbTreeNode = CrbTreeNode<Value>;
	using ColorType = rbTreeColorType;
	Alloc m_rbTreeNodeAllocator;

public:
	using KeyType = Key;
	using ValueType = Value;
	using Pointer = ValueType*;
	using ConstPointer = const ValueType*;
	using Reference = ValueType&;
	using ConstReference = const ValueType&;
	using LinkType = rbTreeNode*;
	using SizeType = size_t;
	using DifferenceType = ptrdiff_t;

protected:
	SizeType AllocateNum = 1;
	SizeType DeallocateNum = 1;

	LinkType _getNode() { return m_rbTreeNodeAllocator.allocate(AllocateNum);}
	void _putNode(LinkType vNode) { m_rbTreeNodeAllocator.deallocate(vNode, DeallocateNum); }

	LinkType _createNode(ConstReference vValue)
	{
		LinkType voTemp = _getNode();
		try 
		{
			m_rbTreeNodeAllocator.construct(voTemp, vValue);
		}
		catch (...)
		{
			_putNode(voTemp);
		}
		return voTemp;
	}

	LinkType _cloneNode(LinkType vNode)
	{
		LinkType voTemp = _createNode(vNode->m_ValueField);
		voTemp->m_Color = vNode->m_Color;
		voTemp->m_Left = nullptr;
		voTemp->m_Right = nullptr;
		return voTemp;
	}

	void _destroyNode(LinkType vNode)
	{
		m_rbTreeNodeAllocator.destroy(vNode);
		_putNode(vNode);
	}

protected:

};