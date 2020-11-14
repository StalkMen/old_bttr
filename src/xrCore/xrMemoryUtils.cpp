#include "stdafx.h"
#include "xrMemoryUtils.h"

xrScopeMemPtr::xrScopeMemPtr(void* InMem)
	: memPtr(InMem)
{}

xrScopeMemPtr::xrScopeMemPtr()
	: memPtr(nullptr)
{}

xrScopeMemPtr::xrScopeMemPtr(const size_t memSize)
{
	memPtr = Memory.mem_alloc(memSize);
}

xrScopeMemPtr::~xrScopeMemPtr()
{
	delete memPtr;
}

xrScopeMemPtr::operator void* () const
{
	return memPtr;
}

void xrScopeMemPtr::reset(void* newMem)
{
	delete memPtr; memPtr = nullptr;
	memPtr = newMem;
}

xrScopeMemPtr& xrScopeMemPtr::operator=(void* InMem)
{
	memPtr = InMem;
	return *this;
}