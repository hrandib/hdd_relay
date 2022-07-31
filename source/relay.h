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

#ifndef RELAY_H
#define RELAY_H

#include "pinlist.h"
#include "wake_base.h"

namespace Mcudrv {
namespace Wk {

class Relay : public NullModule, WakeData
{
private:
    // Used as a single pin to increase load capacity
    typedef PinSequence<Pc3, 5, Pb4, 2> RelayPinGroup;

    enum
    {
        C_GetState = 24,
        C_SetState,
        C_ClearState,
        C_WriteState,
        C_SetChannel,
        C_ClearChannel,
        C_ToggleChannel
    };
    enum
    {
        STATE_OFF = 0xFF,
        STATE_ON = 0
    };

#pragma data_alignment = 4
#pragma location = ".eeprom.noinit"
    static uint8_t nv_state;

    static void FormResponse(void (*cb)(uint16_t));
public:
    enum
    {
        deviceMask = DevSwitch,
    };

    FORCEINLINE
    static void Init()
    {
        RelayPinGroup::Write(STATE_OFF);
        RelayPinGroup::SetConfig<GpioBase::Out_OpenDrain>();
        RelayPinGroup::Write(nv_state);
    }

    FORCEINLINE
    static void On()
    {
        RelayPinGroup::Write(STATE_ON);
    }

    FORCEINLINE
    static void Off()
    {
        RelayPinGroup::Write(STATE_OFF);
    }

    FORCEINLINE
    static void ToggleOnOff()
    {
        RelayPinGroup::Toggle(STATE_ON);
    }

    static void SaveState();
    static bool Process();
};

} // Wk
} // Mcudrv

#endif // RELAY_H
