#pragma once

extern bool       g_bMBTimerIrqActive;
extern UINT32	g_uTimer1IrqCount;	// DEBUG

void	MB_Initialize();
void	MB_Reinitialize();
void	MB_Destroy();
void    MB_Reset();
void    MB_Mute();
void    MB_Demute();
void    MB_StartOfCpuExecute();
void    MB_EndOfVideoFrame();
void    MB_CheckIRQ();
void    MB_UpdateCycles(ULONG uExecutedCycles);
eSOUNDCARDTYPE MB_GetSoundcardType();
void    MB_SetSoundcardType(eSOUNDCARDTYPE NewSoundcardType);
double  MB_GetFramePeriod();
bool    MB_IsActive();
DWORD   MB_GetVolume();
void    MB_SetVolume(DWORD dwVolume, DWORD dwVolumeMax);
DWORD   MB_GetSnapshot(SS_CARD_MOCKINGBOARD* pSS, DWORD dwSlot);
DWORD   MB_SetSnapshot(SS_CARD_MOCKINGBOARD* pSS, DWORD dwSlot);

extern short 	* pDSMockBuf;	// Mockingboard data buffer (in size of g_dwDSMockBufferSize samples?)
extern UINT	nDSMockWCur;	// write cursor
extern UINT	nDSMockRCur;	// read cursor
//extern DWORD	g_dwDSMockBufferSize;	// size of low-level buffer

