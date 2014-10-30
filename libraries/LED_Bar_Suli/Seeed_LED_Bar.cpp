/*
  Seeed_LED_Bar.cpp
  This is a Suly compatable Library
  
  2014 Copyright (c) Seeed Technology Inc.  All right reserved.
  
  Loovee
  2013-3-26

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Seeed_LED_Bar.h"


IO_T __pinClk;
IO_T __pinDta;

int16 __led_state = 0;

#define DELAY_TIME      10

void led_bar_init(PIN_T pinClk, PIN_T pinDta)
{
    suli_pin_init(&__pinClk, pinClk);
    suli_pin_dir(&__pinClk, HAL_PIN_OUTPUT);
    
    suli_pin_init(&__pinDta, pinDta);
    suli_pin_dir(&__pinDta, HAL_PIN_OUTPUT);
}

void latchData()
{
    suli_pin_write(&__pinDta, LOW);
    suli_delay_us(10);

    for(int16 i=0; i<4; i++)
    {
        suli_pin_write(&__pinDta, HIGH);
        suli_delay_us(DELAY_TIME);
        suli_pin_write(&__pinDta, LOW);
        suli_delay_us(DELAY_TIME);
    }
}

void send16bitData(uint16 data)
{
    int16 clk_st = 0;
    
    for(int16 i=0; i<16; i++)
    {
        uint16 state = data&0x8000 ? HIGH : LOW;
        
        suli_pin_write(&__pinDta, state);

        suli_pin_write(&__pinClk, clk_st);
        
        suli_delay_us(DELAY_TIME);
        
        clk_st = 1-clk_st;
        
        data <<= 1;
    }
}

// set led single bit, a bit contrl a led
// such as, index_bits = 0x05, then led 0  and led 3 will on, the others will off
void led_bar_index_bit(uint16 index_bits)
{

    send16bitData(CMDMODE);

    for (int16 i=0;i<12;i++)
    {
        uint16 state = (index_bits&0x0001) ? ON : SHUT;
        send16bitData(state);

        index_bits = index_bits>>1;
    }

    latchData();
}

// set level, there level 0-10
// level 0 means all leds off while level 5 means 5led on and the other will off
void led_bar_set_level(int16 level)
{

    if(level>10)return;

    send16bitData(CMDMODE);

    for(int16 i=0; i<12; i++)
    {
        uint16 state1 = (i<level) ? ON : SHUT;
        send16bitData(state1);
    }

    latchData();
}

// control a single led
// num - which led
// st - 1: on   0: off
void led_bar_single_led(int16 num, int16 st)
{
    if(num>10)return;
    __led_state = st ? (__led_state | (0x01<<num)) : (__led_state & ~(0x01<<num));
    led_bar_index_bit(__led_state);

}

