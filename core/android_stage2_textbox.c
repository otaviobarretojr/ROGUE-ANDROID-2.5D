#include "global.h"
#include "menu.h"
#include "string_util.h"
#include "text.h"
#include "window.h"

static const struct WindowTemplate sAndroidStage2TextWindows[] =
{
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 14,
        .width = 28,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE
};

void InitStandardTextBoxWindows(void)
{
    InitWindows(sAndroidStage2TextWindows);
}

void FreeAllOverworldWindowBuffers(void)
{
    FreeAllWindowBuffers();
}

void LoadMessageBoxAndBorderGfx(void)
{
    /* Android 2.5D renderer will provide the visual frame. */
}

void InitTextBoxGfxAndPrinters(void)
{
    DeactivateAllTextPrinters();
    LoadMessageBoxAndBorderGfx();
}

u16 RunTextPrintersAndIsPrinter0Active(void)
{
    RunTextPrinters();
    return IsTextPrinterActive(0);
}

u16 AddTextPrinterParameterized2(
    u8 windowId,
    u8 fontId,
    const u8 *str,
    u8 speed,
    void (*callback)(struct TextPrinterTemplate *, u16),
    u8 fgColor,
    u8 bgColor,
    u8 shadowColor)
{
    struct TextPrinterTemplate printer = {0};
    printer.currentChar = str;
    printer.windowId = windowId;
    printer.fontId = fontId;
    printer.x = 0;
    printer.y = 1;
    printer.currentX = 0;
    printer.currentY = 1;
    printer.letterSpacing = 0;
    printer.lineSpacing = 0;
    printer.fgColor = fgColor;
    printer.bgColor = bgColor;
    printer.shadowColor = shadowColor;
    return AddTextPrinter(&printer, speed, callback);
}

void AddTextPrinterForMessage(bool8 allowSkippingDelayWithButtonPress)
{
    gTextFlags.canABSpeedUpPrint = allowSkippingDelayWithButtonPress;
    AddTextPrinterParameterized2(
        0,
        FONT_NORMAL,
        gStringVar4,
        1,
        NULL,
        TEXT_COLOR_DARK_GRAY,
        TEXT_COLOR_WHITE,
        TEXT_COLOR_LIGHT_GRAY);
}

static void DrawSimpleFrame(u8 windowId, bool8 copyToVram)
{
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    PutWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, COPYWIN_FULL);
}

void DrawDialogueFrame(u8 windowId, bool8 copyToVram)
{
    DrawSimpleFrame(windowId, copyToVram);
}

void DrawStdWindowFrame(u8 windowId, bool8 copyToVram)
{
    DrawSimpleFrame(windowId, copyToVram);
}

void LoadMessageBoxAndFrameGfx(u8 windowId, bool8 copyToVram)
{
    DrawSimpleFrame(windowId, copyToVram);
}

void ClearDialogWindowAndFrame(u8 windowId, bool8 copyToVram)
{
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ClearWindowTilemap(windowId);
    if (copyToVram)
        CopyWindowToVram(windowId, COPYWIN_FULL);
}
