#include "UObject.h"
#include "..\utils\Patterns.h"
#include "..\utils\addr.h"

FName UObject::GetName()
{
    return *(FName*)((int64)this + 24);
}

FString* UObject::GetFullName(FString* pString, int64 unk)
{
    static void* pat = hook::txn::get_pattern("48 89 5C 24 18 55 56 41 56 48 83 EC 20 45 33 F6 48 8B F1");
    return 	((FString *(__thiscall*)(UObject*, FString*, int64))pat)(this, pString, unk);
}

UClass* UObject::GetClass()
{
	return *(UClass**)((int64)this + 16);
}

wchar_t* UObject::GetStringName()
{
	static FString clsName;
	GetName().ToString(&clsName);

	return clsName.GetStr();
}

TArray<UObject*>* UObject::GetObjects()
{
	if (*(int64*)(_addr(0x143693550 + 12)))
		return (TArray<UObject*>*)_addr(0x143693550 + 12);
	else
		return nullptr;
}
