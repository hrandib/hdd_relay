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

#include "relay.h"

namespace Mcudrv {
namespace Wk {

uint8_t Relay::nv_state;

void Relay::FormResponse(void (*cb)(uint16_t))
{
    if(pdata.n == 1) {
        cb(pdata.buf[0] ? STATE_ON : STATE_OFF);
        pdata.buf[0] = ERR_NO;
        pdata.buf[1] = RelayPinGroup::ReadODR() == 0;
        pdata.n = 2;
    }
    else {
        pdata.buf[0] = ERR_PA;
        pdata.n = 1;
    }
}

bool Relay::Process()
{
    bool result = true;
    switch(cmd) {
        case C_GetState:
            if(!pdata.n) {
                pdata.buf[0] = ERR_NO;
                pdata.buf[1] = RelayPinGroup::ReadODR() > 0;
                pdata.buf[2] = RelayPinGroup::ReadODR() == 0;
                pdata.n = 3;
            }
            else {
                pdata.buf[0] = ERR_PA;
                pdata.n = 1;
            }
            break;
        case C_SetState:
        case C_SetChannel:
            FormResponse(RelayPinGroup::Set);
            break;
        case C_ClearState:
        case C_ClearChannel:
            FormResponse(RelayPinGroup::Clear);
            break;
        case C_WriteState:
            FormResponse(RelayPinGroup::Write);
            break;
        case C_ToggleChannel:
            FormResponse(RelayPinGroup::Toggle);
            break;
        default:
            result = false;
    }
    return result;
}

void Relay::SaveState()
{
    uint8_t currentState = RelayPinGroup::ReadODR();
    if(nv_state == currentState) {
        return;
    }
    nv_state = currentState;
}

} // Wk
} // Mcudrv
