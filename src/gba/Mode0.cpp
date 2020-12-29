#include "GBA.h"
#include "GBAGfx.h"
#include "Globals.h"

void mode0RenderLine()
{
    uint16_t* palette = (uint16_t*)paletteRAM;

    if (DISPCNT & 0x80) {
        for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
                for (int x = 0; x < 240; x++) {
                    lineMix[r][l][x] = 0x7fff;
                }
            }
        }
        return;
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            if (layerEnable & 0x0100) {
                gfxDrawTextScreen(BG0CNT, BG0HOFS, BG0VOFS, line0[r][l], r, l);
            }
        }
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            if (layerEnable & 0x0200) {
                gfxDrawTextScreen(BG1CNT, BG1HOFS, BG1VOFS, line1[r][l], r, l);
            }
        }
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            if (layerEnable & 0x0400) {
                gfxDrawTextScreen(BG2CNT, BG2HOFS, BG2VOFS, line2[r][l], r, l);
            }
        }
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            if (layerEnable & 0x0800) {
                gfxDrawTextScreen(BG3CNT, BG3HOFS, BG3VOFS, line3[r][l], r, l);
            }
        }
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            gfxDrawSprites(lineOBJ[r][l], r, l);
        }
    }

    uint32_t backdrop;
    if (customBackdropColor == -1) {
        backdrop = (READ16LE(&palette[0]) | 0x30000000);
    } else {
        backdrop = ((customBackdropColor & 0x7FFF) | 0x30000000);
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            for (int x = 0; x < 240; x++) {
                uint32_t color = backdrop;
                uint8_t top = 0x20;

                if (line0[r][l][x] < color) {
                    color = line0[r][l][x];
                    top = 0x01;
                }

                if ((uint8_t)(line1[r][l][x] >> 24) < (uint8_t)(color >> 24)) {
                    color = line1[r][l][x];
                    top = 0x02;
                }

                if ((uint8_t)(line2[r][l][x] >> 24) < (uint8_t)(color >> 24)) {
                    color = line2[r][l][x];
                    top = 0x04;
                }

                if ((uint8_t)(line3[r][l][x] >> 24) < (uint8_t)(color >> 24)) {
                    color = line3[r][l][x];
                    top = 0x08;
                }

                if ((uint8_t)(lineOBJ[r][l][x] >> 24) < (uint8_t)(color >> 24)) {
                    color = lineOBJ[r][l][x];
                    top = 0x10;
                }

                if ((top & 0x10) && (color & 0x00010000)) {
                    // semi-transparent OBJ
                    uint32_t back = backdrop;
                    uint8_t top2 = 0x20;

                    if ((uint8_t)(line0[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                        back = line0[r][l][x];
                        top2 = 0x01;
                    }

                    if ((uint8_t)(line1[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                        back = line1[r][l][x];
                        top2 = 0x02;
                    }

                    if ((uint8_t)(line2[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                        back = line2[r][l][x];
                        top2 = 0x04;
                    }

                    if ((uint8_t)(line3[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                        back = line3[r][l][x];
                        top2 = 0x08;
                    }

                    if (top2 & (BLDMOD >> 8))
                        color = gfxAlphaBlend(color, back,
                            coeff[COLEV & 0x1F],
                            coeff[(COLEV >> 8) & 0x1F]);
                    else {
                        switch ((BLDMOD >> 6) & 3) {
                        case 2:
                            if (BLDMOD & top)
                                color = gfxIncreaseBrightness(color, coeff[COLY & 0x1F]);
                            break;
                        case 3:
                            if (BLDMOD & top)
                                color = gfxDecreaseBrightness(color, coeff[COLY & 0x1F]);
                            break;
                        }
                    }
                }

                lineMix[r][l][x] = color;
            }
        }
    }
}

void mode0RenderLineNoWindow()
{
    uint16_t* palette = (uint16_t*)paletteRAM;

    if (DISPCNT & 0x80) {
        for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
                for (int x = 0; x < 240; x++) {
                    lineMix[r][l][x] = 0x7fff;
                }
            }
        }
        return;
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            if (layerEnable & 0x0100) {
                gfxDrawTextScreen(BG0CNT, BG0HOFS, BG0VOFS, line0[r][l], r, l);
            }

            if (layerEnable & 0x0200) {
                gfxDrawTextScreen(BG1CNT, BG1HOFS, BG1VOFS, line1[r][l], r, l);
            }

            if (layerEnable & 0x0400) {
                gfxDrawTextScreen(BG2CNT, BG2HOFS, BG2VOFS, line2[r][l], r, l);
            }

            if (layerEnable & 0x0800) {
                gfxDrawTextScreen(BG3CNT, BG3HOFS, BG3VOFS, line3[r][l], r, l);
            }
        }
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            gfxDrawSprites(lineOBJ[r][l], r, l);
        }
    }

    uint32_t backdrop;
    if (customBackdropColor == -1) {
        backdrop = (READ16LE(&palette[0]) | 0x30000000);
    } else {
        backdrop = ((customBackdropColor & 0x7FFF) | 0x30000000);
    }

    int effect = (BLDMOD >> 6) & 3;
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            for (int x = 0; x < 240; x++) {
                uint32_t color = backdrop;
                uint8_t top = 0x20;

                if (line0[r][l][x] < color) {
                    color = line0[r][l][x];
                    top = 0x01;
                }

                if (line1[r][l][x] < (color & 0xFF000000)) {
                    color = line1[r][l][x];
                    top = 0x02;
                }

                if (line2[r][l][x] < (color & 0xFF000000)) {
                    color = line2[r][l][x];
                    top = 0x04;
                }

                if (line3[r][l][x] < (color & 0xFF000000)) {
                    color = line3[r][l][x];
                    top = 0x08;
                }

                if (lineOBJ[r][l][x] < (color & 0xFF000000)) {
                    color = lineOBJ[r][l][x];
                    top = 0x10;
                }

                if (!(color & 0x00010000)) {
                    switch (effect) {
                    case 0:
                        break;
                    case 1: {
                        if (top & BLDMOD) {
                            uint32_t back = backdrop;
                            uint8_t top2 = 0x20;
                            if (line0[r][l][x] < back) {
                                if (top != 0x01) {
                                    back = line0[r][l][x];
                                    top2 = 0x01;
                                }
                            }

                            if (line1[r][l][x] < (back & 0xFF000000)) {
                                if (top != 0x02) {
                                    back = line1[r][l][x];
                                    top2 = 0x02;
                                }
                            }

                            if (line2[r][l][x] < (back & 0xFF000000)) {
                                if (top != 0x04) {
                                    back = line2[r][l][x];
                                    top2 = 0x04;
                                }
                            }

                            if (line3[r][l][x] < (back & 0xFF000000)) {
                                if (top != 0x08) {
                                    back = line3[r][l][x];
                                    top2 = 0x08;
                                }
                            }

                            if (lineOBJ[r][l][x] < (back & 0xFF000000)) {
                                if (top != 0x10) {
                                    back = lineOBJ[r][l][x];
                                    top2 = 0x10;
                                }
                            }

                            if (top2 & (BLDMOD >> 8))
                                color = gfxAlphaBlend(color, back,
                                    coeff[COLEV & 0x1F],
                                    coeff[(COLEV >> 8) & 0x1F]);
                        }
                    } break;
                    case 2:
                        if (BLDMOD & top)
                            color = gfxIncreaseBrightness(color, coeff[COLY & 0x1F]);
                        break;
                    case 3:
                        if (BLDMOD & top)
                            color = gfxDecreaseBrightness(color, coeff[COLY & 0x1F]);
                        break;
                    }
                } else {
                    // semi-transparent OBJ
                    uint32_t back = backdrop;
                    uint8_t top2 = 0x20;

                    if (line0[r][l][x] < back) {
                        back = line0[r][l][x];
                        top2 = 0x01;
                    }

                    if (line1[r][l][x] < (back & 0xFF000000)) {
                        back = line1[r][l][x];
                        top2 = 0x02;
                    }

                    if (line2[r][l][x] < (back & 0xFF000000)) {
                        back = line2[r][l][x];
                        top2 = 0x04;
                    }

                    if (line3[r][l][x] < (back & 0xFF000000)) {
                        back = line3[r][l][x];
                        top2 = 0x08;
                    }

                    if (top2 & (BLDMOD >> 8))
                        color = gfxAlphaBlend(color, back,
                            coeff[COLEV & 0x1F],
                            coeff[(COLEV >> 8) & 0x1F]);
                    else {
                        switch ((BLDMOD >> 6) & 3) {
                        case 2:
                            if (BLDMOD & top)
                                color = gfxIncreaseBrightness(color, coeff[COLY & 0x1F]);
                            break;
                        case 3:
                            if (BLDMOD & top)
                                color = gfxDecreaseBrightness(color, coeff[COLY & 0x1F]);
                            break;
                        }
                    }
                }

                lineMix[r][l][x] = color;
            }
        }
    }
}

void mode0RenderLineAll()
{
    uint16_t* palette = (uint16_t*)paletteRAM;

    if (DISPCNT & 0x80) {
        for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
                for (int x = 0; x < 240; x++) {
                    lineMix[r][l][x] = 0x7fff;
                }
            }
        }
        return;
    }

    bool inWindow0 = false;
    bool inWindow1 = false;

    if (layerEnable & 0x2000) {
        uint8_t v0 = WIN0V >> 8;
        uint8_t v1 = WIN0V & 255;
        inWindow0 = ((v0 == v1) && (v0 >= 0xe8));
        if (v1 >= v0)
            inWindow0 |= (VCOUNT >= v0 && VCOUNT < v1);
        else
            inWindow0 |= (VCOUNT >= v0 || VCOUNT < v1);
    }
    if (layerEnable & 0x4000) {
        uint8_t v0 = WIN1V >> 8;
        uint8_t v1 = WIN1V & 255;
        inWindow1 = ((v0 == v1) && (v0 >= 0xe8));
        if (v1 >= v0)
            inWindow1 |= (VCOUNT >= v0 && VCOUNT < v1);
        else
            inWindow1 |= (VCOUNT >= v0 || VCOUNT < v1);
    }

    if ((layerEnable & 0x0100)) {
        for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
                gfxDrawTextScreen(BG0CNT, BG0HOFS, BG0VOFS, line0[r][l], r, l);
            }
        }
    }

    if ((layerEnable & 0x0200)) {
        for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
                gfxDrawTextScreen(BG1CNT, BG1HOFS, BG1VOFS, line1[r][l], r, l);
            }
        }
    }

    if ((layerEnable & 0x0400)) {
        for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
                gfxDrawTextScreen(BG2CNT, BG2HOFS, BG2VOFS, line2[r][l], r, l);
            }
        }
    }

    if ((layerEnable & 0x0800)) {
        for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
                gfxDrawTextScreen(BG3CNT, BG3HOFS, BG3VOFS, line3[r][l], r, l);
            }
        }
    }

    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            gfxDrawSprites(lineOBJ[r][l], r, l);
        }
    }
    for (int r = 0; r < enhance_multiplier; r++) { 
        for (int l = 0; l < enhance_multiplier; l++) { 
            gfxDrawOBJWin(lineOBJWin[r][l], r, l);
        }
    }
    

    uint32_t backdrop;
    if (customBackdropColor == -1) {
        backdrop = (READ16LE(&palette[0]) | 0x30000000);
    } else {
        backdrop = ((customBackdropColor & 0x7FFF) | 0x30000000);
    }

    uint8_t inWin0Mask = WININ & 0xFF;
    uint8_t inWin1Mask = WININ >> 8;
    uint8_t outMask = WINOUT & 0xFF;
    for (int r = 0; r < enhance_multiplier; r++) { 
            for (int l = 0; l < enhance_multiplier; l++) { 
            for (int x = 0; x < 240; x++) {
                uint32_t color = backdrop;
                uint8_t top = 0x20;
                uint8_t mask = outMask;

                if (!(lineOBJWin[r][l][x] & 0x80000000)) {
                    mask = WINOUT >> 8;
                }

                if (inWindow1) {
                    if (gfxInWin1[x])
                        mask = inWin1Mask;
                }

                if (inWindow0) {
                    if (gfxInWin0[x]) {
                        mask = inWin0Mask;
                    }
                }

                if ((mask & 1) && (line0[r][l][x] < color)) {
                    color = line0[r][l][x];
                    top = 0x01;
                }

                if ((mask & 2) && ((uint8_t)(line1[r][l][x] >> 24) < (uint8_t)(color >> 24))) {
                    color = line1[r][l][x];
                    top = 0x02;
                }

                if ((mask & 4) && ((uint8_t)(line2[r][l][x] >> 24) < (uint8_t)(color >> 24))) {
                    color = line2[r][l][x];
                    top = 0x04;
                }

                if ((mask & 8) && ((uint8_t)(line3[r][l][x] >> 24) < (uint8_t)(color >> 24))) {
                    color = line3[r][l][x];
                    top = 0x08;
                }

                if ((mask & 16) && ((uint8_t)(lineOBJ[r][l][x] >> 24) < (uint8_t)(color >> 24))) {
                    color = lineOBJ[r][l][x];
                    top = 0x10;
                }

                if (color & 0x00010000) {
                    // semi-transparent OBJ
                    uint32_t back = backdrop;
                    uint8_t top2 = 0x20;

                    if ((mask & 1) && ((uint8_t)(line0[r][l][x] >> 24) < (uint8_t)(back >> 24))) {
                        back = line0[r][l][x];
                        top2 = 0x01;
                    }

                    if ((mask & 2) && ((uint8_t)(line1[r][l][x] >> 24) < (uint8_t)(back >> 24))) {
                        back = line1[r][l][x];
                        top2 = 0x02;
                    }

                    if ((mask & 4) && ((uint8_t)(line2[r][l][x] >> 24) < (uint8_t)(back >> 24))) {
                        back = line2[r][l][x];
                        top2 = 0x04;
                    }

                    if ((mask & 8) && ((uint8_t)(line3[r][l][x] >> 24) < (uint8_t)(back >> 24))) {
                        back = line3[r][l][x];
                        top2 = 0x08;
                    }

                    if (top2 & (BLDMOD >> 8))
                        color = gfxAlphaBlend(color, back,
                            coeff[COLEV & 0x1F],
                            coeff[(COLEV >> 8) & 0x1F]);
                    else {
                        switch ((BLDMOD >> 6) & 3) {
                        case 2:
                            if (BLDMOD & top)
                                color = gfxIncreaseBrightness(color, coeff[COLY & 0x1F]);
                            break;
                        case 3:
                            if (BLDMOD & top)
                                color = gfxDecreaseBrightness(color, coeff[COLY & 0x1F]);
                            break;
                        }
                    }
                } else if (mask & 32) {
                    // special FX on in the window
                    switch ((BLDMOD >> 6) & 3) {
                    case 0:
                        break;
                    case 1: {
                        if (top & BLDMOD) {
                            uint32_t back = backdrop;
                            uint8_t top2 = 0x20;
                            if ((mask & 1) && (uint8_t)(line0[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                                if (top != 0x01) {
                                    back = line0[r][l][x];
                                    top2 = 0x01;
                                }
                            }

                            if ((mask & 2) && (uint8_t)(line1[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                                if (top != 0x02) {
                                    back = line1[r][l][x];
                                    top2 = 0x02;
                                }
                            }

                            if ((mask & 4) && (uint8_t)(line2[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                                if (top != 0x04) {
                                    back = line2[r][l][x];
                                    top2 = 0x04;
                                }
                            }

                            if ((mask & 8) && (uint8_t)(line3[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                                if (top != 0x08) {
                                    back = line3[r][l][x];
                                    top2 = 0x08;
                                }
                            }

                            if ((mask & 16) && (uint8_t)(lineOBJ[r][l][x] >> 24) < (uint8_t)(back >> 24)) {
                                if (top != 0x10) {
                                    back = lineOBJ[r][l][x];
                                    top2 = 0x10;
                                }
                            }

                            if (top2 & (BLDMOD >> 8))
                                color = gfxAlphaBlend(color, back,
                                    coeff[COLEV & 0x1F],
                                    coeff[(COLEV >> 8) & 0x1F]);
                        }
                    } break;
                    case 2:
                        if (BLDMOD & top)
                            color = gfxIncreaseBrightness(color, coeff[COLY & 0x1F]);
                        break;
                    case 3:
                        if (BLDMOD & top)
                            color = gfxDecreaseBrightness(color, coeff[COLY & 0x1F]);
                        break;
                    }
                }

                lineMix[r][l][x] = color;
            }
        }
    }
}
