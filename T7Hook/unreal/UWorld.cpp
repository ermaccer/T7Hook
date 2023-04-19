#include "UWorld.h"
#include "..\utils\addr.h"

UWorld* GetWorld()
{
    return *(UWorld**)(_addr(0x14379C7A0));
}
