#include "art.h"

void RenderBrailleArt(PlatformContext *Platform, const CHAR16 *Art[], UINTN LinesCount,
                      UINTN startX, UINTN startY, UINTN dotScale, UINT32 Color) {
    UINTN dotGap = 0;

    UINTN charWidth = (2 * dotScale) + (2 * dotGap);
    UINTN charHeight = (4 * dotScale) + (4 * dotGap);

    for (UINTN line = 0; line < LinesCount; line++) {
        const CHAR16 *rowStr = Art[line];
        UINTN currentX = startX;
        UINTN currentY = startY + (line * charHeight);

        for (UINTN i = 0; rowStr[i] != L'\0'; i++) {
            CHAR16 c = rowStr[i];

            if (c == L' ') {
                currentX += charWidth;
                continue;
            }

            if (c >= 0x2800 && c <= 0x28FF) {
                UINT16 pattern = c - 0x2800;

                if (pattern & (1 << 0))
                    DrawRectangle(Platform, currentX, currentY, dotScale, dotScale, Color);
                if (pattern & (1 << 1))
                    DrawRectangle(Platform, currentX, currentY + (1 * (dotScale + dotGap)),
                                  dotScale, dotScale, Color);
                if (pattern & (1 << 2))
                    DrawRectangle(Platform, currentX, currentY + (2 * (dotScale + dotGap)),
                                  dotScale, dotScale, Color);
                if (pattern & (1 << 6))
                    DrawRectangle(Platform, currentX, currentY + (3 * (dotScale + dotGap)),
                                  dotScale, dotScale, Color);

                UINTN col1X = currentX + dotScale + dotGap;
                if (pattern & (1 << 3))
                    DrawRectangle(Platform, col1X, currentY, dotScale, dotScale, Color);
                if (pattern & (1 << 4))
                    DrawRectangle(Platform, col1X, currentY + (1 * (dotScale + dotGap)), dotScale,
                                  dotScale, Color);
                if (pattern & (1 << 5))
                    DrawRectangle(Platform, col1X, currentY + (2 * (dotScale + dotGap)), dotScale,
                                  dotScale, Color);
                if (pattern & (1 << 7))
                    DrawRectangle(Platform, col1X, currentY + (3 * (dotScale + dotGap)), dotScale,
                                  dotScale, Color);
            }
            currentX += charWidth;
        }
    }
}

void RenderColoredWindows(PlatformContext *Platform, const CHAR16 *Art[], UINTN LinesCount,
                          UINTN startX, UINTN startY, UINTN dotScale) {
    UINTN dotGap = 0;
    UINTN charWidth = (2 * dotScale) + (2 * dotGap);
    UINTN charHeight = (4 * dotScale) + (4 * dotGap);

    UINTN rightTileStart[11] = {19, 13, 13, 14, 13, 13, 13, 12, 12, 12, 10};

    for (UINTN line = 0; line < LinesCount; line++) {
        const CHAR16 *rowStr = Art[line];
        UINTN currentX = startX;
        UINTN currentY = startY + (line * charHeight);

        for (UINTN i = 0; rowStr[i] != L'\0'; i++) {
            CHAR16 c = rowStr[i];

            if (c == L' ') {
                currentX += charWidth;
                continue;
            }

            if (c >= 0x2800 && c <= 0x28FF) {
                UINT16 pattern = c - 0x2800;

                for (UINTN dot = 0; dot < 8; dot++) {
                    if (pattern & (1 << dot)) {

                        UINTN dotCol = (dot == 3 || dot == 4 || dot == 5 || dot == 7) ? 1 : 0;
                        UINTN dotRow = 0;
                        if (dot == 1 || dot == 4)
                            dotRow = 1;
                        else if (dot == 2 || dot == 5)
                            dotRow = 2;
                        else if (dot == 6 || dot == 7)
                            dotRow = 3;

                        UINTN absY = (line * 4) + dotRow;

                        BOOLEAN isLeft = (i < rightTileStart[line]);

                        UINT32 dotColor = 0x00FFFFFF;

                        if (isLeft) {
                            if (absY <= 18)
                                dotColor = 0x00FF3333;
                            else
                                dotColor = 0x003388FF;
                        } else {
                            if (absY <= 21)
                                dotColor = 0x0033FF33;
                            else
                                dotColor = 0x00FFDD33;
                        }

                        UINTN drawX = currentX + (dotCol * (dotScale + dotGap));
                        UINTN drawY = currentY + (dotRow * (dotScale + dotGap));

                        DrawRectangle(Platform, drawX, drawY, dotScale, dotScale, dotColor);
                    }
                }
            }
            currentX += charWidth;
        }
    }
}
