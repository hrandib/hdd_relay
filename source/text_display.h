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

#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "wake_base.h"

namespace Mcudrv {
namespace Wk {

class TextDisplay : public NullModule, WakeData
{
private:
    enum
    {
        C_READBYTES = 44,
        C_SETMASK,
        C_CLEARMASK,
        C_WRITEBYTES,
    };
    enum
    {
        CHARBUF_LENGTH = 32
    };

    static uint8_t charbuf[32];
    static bool isUpdate;
public:
    enum
    {
        deviceMask = DevGenericIO,
    };

    static void Init();
    static bool Process();
    static void UpdateContent();
};

} // Wk
} // Mcudrv

#endif // TEXT_DISPLAY_H
