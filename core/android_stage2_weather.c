#include "global.h"
#include "constants/weather.h"
#include "coord_event_weather.h"
#include "field_weather.h"

struct Weather gWeather = {0};
struct Weather *const gWeatherPtr = &gWeather;

static u8 sSavedWeather = WEATHER_NONE;

void StartWeather(void)
{
    gWeather.currWeather = WEATHER_NONE;
    gWeather.nextWeather = WEATHER_NONE;
    gWeather.weatherChangeComplete = TRUE;
}

void DoCurrentWeather(void)
{
    gWeather.currWeather = WEATHER_NONE;
    gWeather.nextWeather = WEATHER_NONE;
    gWeather.weatherChangeComplete = TRUE;
}

void ResumePausedWeather(void) {}

void DoCoordEventWeather(u8 weather)
{
    (void)weather;
}

void SetSavedWeatherFromCurrMapHeader(void)
{
    sSavedWeather = WEATHER_NONE;
}

u8 GetSavedWeather(void)
{
    return sSavedWeather;
}

void SetSavedWeather(u32 weather)
{
    sSavedWeather = (u8)weather;
}

void SetWeather(u32 weather)
{
    (void)weather;
    sSavedWeather = WEATHER_NONE;
}

u8 GetCurrentWeather(void)
{
    return WEATHER_NONE;
}

void SetNextWeather(u8 weather)
{
    (void)weather;
    gWeather.nextWeather = WEATHER_NONE;
}

void SetCurrentAndNextWeather(u8 weather)
{
    (void)weather;
    StartWeather();
}

void SetCurrentAndNextWeatherNoDelay(u8 weather)
{
    (void)weather;
    StartWeather();
}

u8 IsWeatherChangeComplete(void)
{
    return TRUE;
}

bool8 IsWeatherNotFadingIn(void)
{
    return TRUE;
}

void ApplyWeatherColorMapToPal(u8 paletteIndex)
{
    (void)paletteIndex;
}

void UpdateSpritePaletteWithWeather(u8 spritePaletteIndex)
{
    (void)spritePaletteIndex;
}

void PreservePaletteInWeather(u8 preservedPalIndex)
{
    (void)preservedPalIndex;
}

void ResetPreservedPalettesInWeather(void) {}


void PlayRainStoppingSoundEffect(void)
{
}
