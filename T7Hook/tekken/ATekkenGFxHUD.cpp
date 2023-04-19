#include "ATekkenGFxHUD.h"
#include "..\utils\addr.h"
#include "..\unreal\UWorld.h"

ATekkenGFxHUD* GetTekkenHUD()
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
                if (wcscmp(a->GetClass()->GetStringName(), TEKKENGFXHUD_CLASSNAME) == 0)
                {
                    return (ATekkenGFxHUD*)a;
                    break;
                }
            }
        }
    }

    return nullptr;
}
