#pragma once
#include "FString.h"

enum EFindName
{
	FNAME_Find,
	FNAME_Add,
	FNAME_Replace,
};



class FName {
public:
	int Index;
	int Number;

	FString* ToString(FString* str);

	FName(const wchar_t* Name, EFindName FindType);
	FName(int idx);
	FName();
};