#include "ACharacterLoadHelper.h"
#include "..\utils\addr.h"

int ACharacterLoadHelper::GetNumCharacters()
{
    return *(int*)((int64)this + 0x390);
}

int64 ACharacterLoadHelper::GetInfoOffset()
{
    return *(int64*)((int64)this + 0x388);
}

ACharacterLoadInfo* ACharacterLoadHelper::GetLoadInfo(int num)
{
    int max = GetNumCharacters();

    if (num > max)
        return nullptr;

    ACharacterLoadInfo* info = (ACharacterLoadInfo*)(GetInfoOffset() + (sizeof(ACharacterLoadInfo) * num));
    return info;
}
