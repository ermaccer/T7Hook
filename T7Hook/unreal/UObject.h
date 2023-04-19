#pragma once
#include "FName.h"
#include "..\utils\addr.h"

class UClass;


class UObject {
public:
	FName GetName();
	FString* GetFullName(FString* pString, int64 unk);
	UClass* GetClass();

	wchar_t* GetStringName();

	static TArray<UObject*>* GetObjects();
};


class UClass : public UObject {
public:
};