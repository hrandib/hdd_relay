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

#include "hd44780.h"
#include "i2c.h"
#include "i2c_lcd_backpack.h"
#include "relay.h"

using namespace Mcudrv;

typedef Wk::ModuleList<Wk::Relay> ModuleList;
typedef Wk::Wake<ModuleList, 57600UL, Nullpin> Wake;

template void Wake::OpTime::UpdIRQ();
template void Wake::TxISR();
template void Wake::RxISR();

typedef Twis::SoftTwi<Twis::Standard, Pd2, Pd3> Twi;
typedef LcdBackpack<Twi, 0x27> LcdPort;
typedef Hd44780<LcdPort::Databus, LcdPort::Rs, LcdPort::E, 2, Mcudrv::LCD_KS0066, 0> Lcd;

int main()
{
    // SysClock::Select(SysClock::HSE);
    GpioA::WriteConfig<0xFF, GpioBase::In_Pullup>();
    GpioB::WriteConfig<0xFF, GpioBase::In_Pullup>();
    GpioC::WriteConfig<0xFF, GpioBase::In_Pullup>();
    GpioD::WriteConfig<0xFF, GpioBase::In_Pullup>();
    Twi::Init();
    Lcd::Init();
    //    Wake::Init();
    //    enableInterrupts();
    while(true) {
        LcdPort::SetBacklight(true);
        LcdPuts<Lcd>("Hello World!\n");
        delay_ms(2000);

        LcdPort::SetBacklight(false);
        LcdPuts<Lcd>("Another message\n");
        delay_ms(2000);

        LcdPort::SetBacklight(true);
        LcdPuts<Lcd>("Yuhooo0oo\n");
        delay_ms(2000);

        LcdPort::SetBacklight(false);
        LcdPuts<Lcd>("Test-test-test\n");
        delay_ms(2000);

        //        Wake::Process();
    }
}
