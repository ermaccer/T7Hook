#include "ACharacterManager.h"
#include "..\utils\addr.h"
#include "..\unreal\UWorld.h"

ACharacterLoadHelper* ACharacterManager::GetLoadHelper()
{
    return *(ACharacterLoadHelper**)((int64)this + 0x398);
}

ACharacterManager* GetCharacterManager()
{
    UWorld* world = GetWorld();
    if (!world)
       return nullptr;

    ULevel* persistentLevel = world->PersistentLevel;

    if (!persistentLevel)
        return nullptr;

    for (int i = 0; i < persistentLevel->ActorsForGC.Count; i++)
    {
        AActor* a = persistentLevel->ActorsForGC.Get(i);
        if (a)
        {
            if (a->GetClass())
            {
                if (wcscmp(a->GetClass()->GetStringName(), CHARACATERMANAGER_CLASSNAME) == 0)
                {
                    return (ACharacterManager*)a;
                    break;
                }
            }
        }
    }

    return nullptr;
}
