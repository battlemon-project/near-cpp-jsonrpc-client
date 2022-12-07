#pragma once

#include <string>
#include "include/Client.h"
#include "EdKeys.h"

#define ED25519 ((EdKeys*)keyPair)

#define HOOK_ERROR TYPE_CHAR*& error, void(*errorH)(const TYPE_STRING& copy, TYPE_CHAR*& error)
#define THROW_HOOK error, Helper::allocateMemory
#define SET_ERROR(str) Helper::allocateMemory(str, error)



namespace Helper
{
	void allocateMemory(const TYPE_STRING& copy, TYPE_CHAR*& target);
	void free(TYPE_CHAR** data);
}