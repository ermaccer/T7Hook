#pragma once
#include "..\utils\addr.h"
#include "..\utils\core.h"
#include "..\utils\Patterns.h"
#include "..\unreal\FVector.h"
#include "..\unreal\FRotator.h"

enum ECharacterID {
	CHARACTER_Paul = 0,
	CHARACTER_Law = 1,
	CHARACTER_King = 2,
	CHARACTER_Yoshimitsu = 3,
	CHARACTER_Hwoarang = 4,
	CHARACTER_Dragunov = 16,
	CHARACTER_Nina = 28,
	CHARACTER_MasterRaven = 29,

};


enum ESideType {
	SIDE_LEFT,
	SIDE_RIGHT
};

class CharacterInfo {
public:
	char pad[8];
	uint8 Visible;
	uint8 flag2;
	uint8 flag3;
	uint8 flag4;
	char _pad[208];
	int		CharacterID;
	FVector Position;
	FRotatorCompressed Rotation;

	void SetID(int ID);
	void SetIDHooked(int ID);
};
VALIDATE_OFFSET(CharacterInfo, Position, 0xE0);

CharacterInfo* GetCharacterInfo(int id);


void SetPlayer1Character(int64 ptr, int ID);
void SetPlayer2Character(int64 ptr, int ID);