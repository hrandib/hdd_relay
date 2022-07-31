/*
 * Copyright (c) 2022 Dmytro Shestakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "text_display.h"
#include "hd44780.h"
#include "i2c.h"
#include "i2c_lcd_backpack.h"
#include <cstring>

namespace Mcudrv {
namespace Wk {

typedef Twis::SoftTwi<Twis::Standard, Pd2, Pd3> Twi;
typedef LcdBackpack<Twi, 0x27> LcdPort;
typedef Hd44780<LcdPort::Databus, LcdPort::Rs, LcdPort::E, 2, Mcudrv::LCD_KS0066, 0> Lcd;

uint8_t TextDisplay::charbuf[CHARBUF_LENGTH];
bool TextDisplay::isUpdate;

void TextDisplay::Init()
{
    memset(charbuf, ' ', CHARBUF_LENGTH);
    Twi::Init();
    Lcd::Init();
    Lcd::Puts("  HOME SERVER");
}

bool TextDisplay::Process()
{
    bool result = true;
    switch(cmd) {
        case C_READBYTES:
        case C_SETMASK:
        case C_CLEARMASK:
            pdata.buf[0] = ERR_NI;
            pdata.n = 1;
            break;
        case C_WRITEBYTES: {
            uint8_t offset = pdata.buf[0];
            const uint8_t* data = (const uint8_t*)&pdata.buf[1];
            uint8_t size = pdata.n - 1;
            if(size + offset <= CHARBUF_LENGTH) {
                memcpy(charbuf + offset, data, size);
                isUpdate = true;
                pdata.buf[0] = ERR_NO;
            }
            else {
                pdata.buf[0] = ERR_PA;
            }
            pdata.n = 1;
        } break;
        default:
            result = false;
    }
    return result;
}

void TextDisplay::UpdateContent()
{
    if(isUpdate) {
        Putbuf();
    }
}

void TextDisplay::Putbuf()
{
    const uint8_t* data = charbuf;
    for(uint8_t i = 0; i < 2; ++i) {
        Lcd::SetPosition(0, i);
        uint8_t len = CHARBUF_LENGTH / 2;
        while(len--) {
            Lcd::Putch(*data++);
        }
    }
}

} // Wk
} // Mcudrv
