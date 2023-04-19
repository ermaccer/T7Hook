#include "CharacterInfo.h"
#include "..\plugin\Menu.h"

CharacterInfo* GetCharacterInfo(int id)
{
	static void* pat = hook::txn::get_pattern("83 F9 2F 77 0F 48 63 C1");
    return	((CharacterInfo*(__cdecl*)(int))pat)(id);
}

void SetPlayer1Character(int64 ptr, int ID)
{
	int id = *(int*)(ptr + 8);

	if (id == 1 && TheMenu->m_bPlayer1Modifier)
		ID = TheMenu->m_nPlayer1Character;

	if (id == 0 && TheMenu->m_bPlayer2Modifier)
		ID = TheMenu->m_nPlayer2Character;

	*(int*)(ptr + 16) = ID;
	*(int*)(ptr + 49316) = ID;
}

void SetPlayer2Character(int64 ptr, int ID)
{
	int id = *(int*)(ptr + 8);

	if (id == 1 && TheMenu->m_bPlayer1Modifier)
		ID = TheMenu->m_nPlayer1Character;

	if (id == 0 && TheMenu->m_bPlayer2Modifier)
		ID = TheMenu->m_nPlayer2Character;

	*(int*)(ptr + 20) = ID;
}

void CharacterInfo::SetID(int ID)
{
	int64 a1 = (int64)this;
	int64 v2 = *(int64*)(a1 + 5472);
	*(int*)(a1 + 220) = ID;
	if (v2)
		*(int*)(v2 + 220) = ID;
	int64 v3 = *(int64*)(a1 + 5464);
	if (v3)
		*(int*)(v3 + 220) = ID;
}

void CharacterInfo::SetIDHooked(int ID)
{
	if (this == GetCharacterInfo(0) && TheMenu->m_bPlayer1Modifier)
		ID = TheMenu->m_nPlayer1Character;

	if (this == GetCharacterInfo(1) && TheMenu->m_bPlayer2Modifier)
		ID = TheMenu->m_nPlayer2Character;

	SetID(ID);
}
