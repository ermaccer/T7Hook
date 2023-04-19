#include "GameSpeed.h"
#include "..\plugin\Menu.h"
void SetGameSpeed(int value)
{
	if (TheMenu->m_fSlowMotionSpeed < 0.1f)
		TheMenu->m_fSlowMotionSpeed = 0.1f;

	if (TheMenu->m_bSlowMotion)
	{
		value = (int)(TheMenu->m_fSlowMotionSpeed * 100.0f);
		if (value <= 0) value = 0;
	}

	*(int*)_addr(0x1434DF51C) = value;
}
