#include "global.h"
#include "link.h"
#include "m4a.h"
#include "rtc.h"
#include "siirtc.h"
#include "sound.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void PlatformLog(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void IntrMain(void) {}
void Timer3Intr(void) {}
void LinkVSync(void) {}
void RfuVSync(void) {}

u16 SetFlashTimerIntr(u8 timerNum, void (**intrFunc)(void))
{
    (void)timerNum;
    if (intrFunc != NULL)
        *intrFunc = NULL;
    return 0;
}

void SiiRtcUnprotect(void) {}
void SiiRtcProtect(void) {}
u8 SiiRtcProbe(void) { return 0; }
bool8 SiiRtcReset(void) { return TRUE; }
bool8 SiiRtcGetStatus(struct SiiRtcInfo *rtc)
{
    if (rtc != NULL)
        rtc->status = SIIRTCINFO_24HOUR;
    return TRUE;
}
bool8 SiiRtcSetStatus(struct SiiRtcInfo *rtc)
{
    (void)rtc;
    return TRUE;
}
bool8 SiiRtcGetDateTime(struct SiiRtcInfo *rtc)
{
    if (rtc != NULL)
    {
        memset(rtc, 0, sizeof(*rtc));
        rtc->status = SIIRTCINFO_24HOUR;
    }
    return TRUE;
}
bool8 SiiRtcSetDateTime(struct SiiRtcInfo *rtc)
{
    (void)rtc;
    return TRUE;
}
bool8 SiiRtcGetTime(struct SiiRtcInfo *rtc)
{
    return SiiRtcGetDateTime(rtc);
}
bool8 SiiRtcSetTime(struct SiiRtcInfo *rtc)
{
    (void)rtc;
    return TRUE;
}
bool8 SiiRtcSetAlarm(struct SiiRtcInfo *rtc)
{
    (void)rtc;
    return TRUE;
}

void RtcInit(void)
{
}

struct SoundInfo gSoundInfo;
struct MusicPlayerInfo gMPlayInfo_BGM;
struct MusicPlayerInfo gMPlayInfo_SE1;
struct MusicPlayerInfo gMPlayInfo_SE2;
struct MusicPlayerInfo gMPlayInfo_SE3;
bool8 gSoundInit = FALSE;

void m4aSoundInit(void)
{
    gSoundInit = TRUE;
}
void m4aSoundMain(void) {}
void m4aSoundVSync(void) {}
void m4aSoundVSyncOn(void) {}
void m4aSoundVSyncOff(void) {}

static u16 sCurrentMapMusic;

void InitMapMusic(void) { sCurrentMapMusic = 0; }
void MapMusicMain(void) {}
void ResetMapMusic(void) { sCurrentMapMusic = 0; }
u16 GetCurrentMapMusic(void) { return sCurrentMapMusic; }
void PlayNewMapMusic(u16 songNum) { sCurrentMapMusic = songNum; }
void StopMapMusic(void) { sCurrentMapMusic = 0; }
void FadeOutMapMusic(u8 speed)
{
    (void)speed;
    sCurrentMapMusic = 0;
}
void FadeOutAndPlayNewMapMusic(u16 songNum, u8 speed)
{
    (void)speed;
    sCurrentMapMusic = songNum;
}
void FadeOutAndFadeInNewMapMusic(u16 songNum, u8 fadeOutSpeed, u8 fadeInSpeed)
{
    (void)fadeOutSpeed;
    (void)fadeInSpeed;
    sCurrentMapMusic = songNum;
}
bool8 IsNotWaitingForBGMStop(void) { return TRUE; }
bool8 WaitFanfare(bool8 stop)
{
    (void)stop;
    return TRUE;
}
void PlaySE(u16 songNum) { (void)songNum; }
void PlayCry_NormalNoDucking(u16 species, s8 pan, s8 volume, u8 priority)
{
    (void)species;
    (void)pan;
    (void)volume;
    (void)priority;
}
