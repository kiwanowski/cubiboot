/* 
 * Copyright (c) 2019-2021, Extrems <extrems@extremscorner.org>
 * 
 * This file is part of Swiss.
 * 
 * Swiss is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * Swiss is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * with Swiss.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "tinyprintf/tinyprintf.h"
#include "usbgecko.h"

extern volatile u32 EXI[3][5];

static void exi_select(void)
{
	EXI[EXI_CHANNEL_1][0] = (EXI[EXI_CHANNEL_1][0] & 0x405) | ((1 << EXI_DEVICE_0) << 7) | (EXI_SPEED_32MHZ << 4);
}

static void exi_deselect(void)
{
	EXI[EXI_CHANNEL_1][0] &= 0x405;
}

static uint32_t exi_imm_read_write(uint32_t data, uint32_t len)
{
	EXI[EXI_CHANNEL_1][4] = data;
	EXI[EXI_CHANNEL_1][3] = ((len - 1) << 4) | (EXI_READ_WRITE << 2) | 0b01;
	while (EXI[EXI_CHANNEL_1][3] & 0b01);
	return EXI[EXI_CHANNEL_1][4] >> ((4 - len) * 8);
}

static bool usb_probe(void)
{
	uint16_t val;

	exi_select();
	val = exi_imm_read_write(0x9 << 28, 2);
	exi_deselect();

	return val == 0x470;
}

static bool usb_transmit_byte(const uint8_t *data)
{
	uint16_t val;

	exi_select();
	val = exi_imm_read_write(0xB << 28 | *data << 20, 2);
	exi_deselect();

	return !(val & 0x400);
}

static bool usb_transmit_check(void)
{
	uint8_t val;

	exi_select();
	val = exi_imm_read_write(0xC << 28, 1);
	exi_deselect();

	return !(val & 0x4);
}

static int usb_transmit(const void *data, int size, int minsize)
{
	int i = 0, j = 0, check = 1;

	while (i < size) {
		if ((check && usb_transmit_check()) ||
			(check = usb_transmit_byte(data + i))) {
			j = i % 128;
			if (i < minsize)
				continue;
			else break;
		}

		i++;
		check = i % 128 == j;
	}

	return i;
}

s32 WriteUARTN(const void *buf, u32 len)
{
	if (usb_probe()) {
		usb_transmit(buf, len, len);
		return 0;
	}

	return -1;
}

void usb_OSReport(const char *fmt, ...) {
	va_list args;
    static char buf[256];

    va_start(args, fmt);
    int length = vsprintf((char *)buf, (char *)fmt, args);

	WriteUARTN(buf, length);

    va_end(args);
}
