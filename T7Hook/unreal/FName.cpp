#include "FName.h"
#include "..\utils\addr.h"
#include "..\utils\Patterns.h"


FString* FName::ToString(FString* str)
{
    static void* pat = hook::txn::get_pattern("40 53 48 83 EC 20 33 C0 48 8B DA 48 89 02 48 89 42 08 E8");
    return 	((FString*(__thiscall*)(FName*, FString*))pat)(this, str);
}

FName::FName(const wchar_t* Name, EFindName FindType)
{
    static void* pat = hook::txn::get_pattern("40 53 48 83 EC 30 48 8B D9 48 85 D2 74 ? 45 8B C8 C7 44 24 28 FF FF FF FF 45 33 C0 C6 44 24 20 01 E8 3A 45 00 00");
    ((void(__thiscall*)(FName*, const wchar_t*, EFindName))pat)(this, Name, FindType);
}

FName::FName(int idx)
{
    Index = idx;
    Number = 0;
}

FName::FName()
{
    Index = 0;
    Number = 0;
}
