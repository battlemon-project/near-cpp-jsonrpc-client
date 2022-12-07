
#include "Helper.h"

void Helper::allocateMemory(const TYPE_STRING& copy, TYPE_CHAR*& target)
{
	if (target == nullptr && copy != _T(""))
	{
		target = new TYPE_CHAR[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), target);
		target[copy.size()] = '\0';
	}
}

void Helper::free(TYPE_CHAR** data)
{
	if (data != nullptr)
	{
		delete[] *data;
		*data = nullptr;
	}
}