#include "Game.h"
#include "CRT.h"

Uint32 DefaultScanlineData[] =
{
	0xFFFFEEFF, 0xFFE5FFF6,	// 2x2 pixel bitmap
	0xFF92609B, 0xFF609268,	// for 2X zoom

	0xFF65346E, 0xFF34653C,	// 2x3 pixel bitmap
	0xFFFFEEFF, 0xFFE5FFF6,	// for 3X zoom
	0xFFBE95BE, 0xFF8CBE9D,

	0xFF92609B, 0xFF609268,	// 2x4 pixel bitmap
	0xFFFFEEFF, 0xFFE5FFF6,	// for 4X zoom
	0xFFFFD6FF, 0xFFCDFFDE,
	0xFF65346E, 0xFF34653C,

	0xFFBF8DC8, 0xFF8DBF95,	// 2x5 pixel bitmap
	0xFFF1D2F1, 0xFFC9F1DA,	// for 5X zoom
	0xFFFFEBFF, 0xFFE2FFF3,
	0xFFE0B7E0, 0xFFAEE0BF,
	0xFF92609B, 0xFF609268,

	0xFF92609B, 0xFF609268,	// 2x6 pixel bitmap
	0xFFFFD6FF, 0xFFCDFFDE,	// for 6X zoom and further
	0xFFFFEEFF, 0xFFE5FFF6,
	0xFFFFD6FF, 0xFFCDFFDE,
	0xFF92609B, 0xFF609268,
	0xFF65346E, 0xFF34653C
};

bool ScanlinesEnabled = true;
float ScanlineBloom = 0.0f;
float ScanlineBrightness = 1.15f;

void CRT::EnableCRTEffect()
{

}

void CRT::DisableCRTEffect()
{

}

void CRT::Initialize()
{

}