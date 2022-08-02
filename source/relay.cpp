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

static const char* const ON_MESSAGE = "   RELAY ON";
static const char* const OFF_MESSAGE = "   RELAY OFF";

uint8_t Relay::nv_state;
Relay::StatusCb Relay::ShowStatus;

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

void Relay::Init()
{
    RelayPinGroup::Write(STATE_OFF);
    RelayPinGroup::SetConfig<GpioBase::Out_OpenDrain>();
    RelayPinGroup::Write(nv_state);
}

void Relay::On()
{
    RelayPinGroup::Write(STATE_ON);
    if(ShowStatus) {
        ShowStatus(ON_MESSAGE);
    }
}

void Relay::Off()
{
    RelayPinGroup::Write(STATE_OFF);
    if(ShowStatus) {
        ShowStatus(OFF_MESSAGE);
    }
}

bool Relay::Process()
{
    bool result = true;
    switch(cmd) {
        case C_GetState:
            if(!pdata.n) {
                bool currentState = RelayPinGroup::ReadODR() == 0;
                pdata.buf[0] = ERR_NO;
                pdata.buf[1] = currentState;
                pdata.buf[2] = !currentState;
                pdata.n = 3;
            }
            else {
                pdata.buf[0] = ERR_PA;
                pdata.n = 1;
            }
            break;
        case C_SetState:
        case C_SetChannel:
            FormResponse(RelayPinGroup::Clear);
            break;
        case C_ClearState:
        case C_ClearChannel:
            FormResponse(RelayPinGroup::Set);
            break;
        case C_WriteState:
            FormResponse(RelayPinGroup::Write);
            break;
        case C_ToggleChannel:
            pdata.buf[0] = ERR_NI;
            pdata.n = 1;
            break;
        default:
            result = false;
    }
    if(result && ShowStatus) {
        ShowStatus(RelayPinGroup::ReadODR() == 0 ? ON_MESSAGE : OFF_MESSAGE);
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
