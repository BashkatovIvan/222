/*
 * Utils.cpp
 *
 * Created: 13.12.2021 9:56:02
 *  Author: amaslov
 */ 
#include "Utils.h"

void nops(uint32_t n)
{
	for (uint32_t k = 0; k < n; k++) {
		asm("NOP");
	}
}


void udelay(uint16_t us)
{
	for (uint8_t k = 0; k < us; k++) {
		for (uint8_t i = 0; i < 16; ++i)
			asm("NOP");
	}
}

uint8_t crc8(uint8_t ndata, uint8_t crc)
{
	uint8_t tmp = crc;
	ndata ^= tmp;
	tmp = 0;
	if ((ndata & (1)) != 0) tmp ^= 0x5e;
	if ((ndata & (2)) != 0) tmp ^= 0xbc;
	if ((ndata & (4)) != 0) tmp ^= 0x61;
	if ((ndata & (8)) != 0) tmp ^= 0xc2;
	if ((ndata & (16)) != 0) tmp ^= 0x9d;
	if ((ndata & (32)) != 0) tmp ^= 0x23;
	if ((ndata & (64)) != 0) tmp ^= 0x46;
	if ((ndata & (128)) != 0) tmp ^= 0x8c;
	return tmp;
}


struct tm diffTime(struct tm &t1, struct tm &t2)
{
	struct tm ret = {};
	uint32_t remainder;
	uint32_t devider;

	uint32_t res = diffTimeInSec(t1, t2);

	devider = 24 * 3600;
	ret.tm_mday = res / devider;
	remainder = res % devider;

	devider = 3600;
	ret.tm_hour = remainder / devider;
	remainder = remainder % devider;

	devider = 60;
	ret.tm_min = remainder / devider;
	remainder = remainder % devider;

	devider = 1;
	ret.tm_sec = remainder / devider;
	remainder = remainder % devider;

	return ret;
}

uint32_t diffTimeInSec(struct tm &t1, struct tm &t2)
{
	int64_t _t1 = t1.tm_sec + t1.tm_min * 60 + t1.tm_hour * 3600 + t1.tm_mday * 24 * 3600;
	int64_t _t2 = t2.tm_sec + t2.tm_min * 60 + t2.tm_hour * 3600 + t2.tm_mday * 24 * 3600;

	int64_t diff = _t1 - _t2;

	return diff < 0 ? 0 : diff;
}

uint32_t timeInSec(struct tm &t)
{
	uint32_t _t = t.tm_sec + t.tm_min * 60 + t.tm_hour * 3600 + t.tm_mday * 24 * 3600;
	return _t;
}

struct tm timeInUTC(uint32_t sec)
{
	struct tm ret = {};
	uint32_t remainder;
	uint32_t devider;

	devider = 24 * 3600;
	ret.tm_mday = sec / devider;
	remainder = sec % devider;

	devider = 3600;
	ret.tm_hour = remainder / devider;
	remainder = remainder % devider;

	devider = 60;
	ret.tm_min = remainder / devider;
	remainder = remainder % devider;

	devider = 1;
	ret.tm_sec = remainder / devider;
	remainder = remainder % devider;

	return ret;
}

char* floatToChar (char *buf, size_t size, int32_t floatValue, const int devider)
{
	char tmpValue[size + 1] = {};
	memset(buf, 0x00, size);

	bool sign = floatValue < 0 ? true : false;
	floatValue = floatValue < 0 ? floatValue * -1 : floatValue;

	if (sign) {
		strncat(buf, "-", size - strlen(buf));
	}

	if (devider > 0)
	{
		snprintf(tmpValue, size, "%ld", floatValue / powx(10, devider));
		strncat(buf, tmpValue, size - strlen(buf));

		if (floatValue % powx(10, devider))
		{
			strncat(buf, ",", size - strlen(buf));

			char format[20] {};
			sprintf(format, "%%0%dd", devider);
			snprintf(tmpValue, size, format, floatValue % powx(10, devider));
			strncat(buf, tmpValue, size - strlen(buf));

			for (size_t i = strlen(buf) - 1; i > 0; i ++)
			{
				if (buf[i] == '0') {
					buf[i] = 0;
				} else {
					break;
				}
			}
		}
	} else {
		snprintf(tmpValue, size, "%ld", floatValue);
		strncat(buf, tmpValue, size - strlen(buf));
	}

	return buf;
}

size_t strUtf8len(const char* text, const int size)
{
	size_t len = 0;

	for (int i = 0; i < size; i++)
	{
		if (!(text[i] == 0xd0 || text[i] == 0xd1 || text[i] == 0xd2 || text[i] == 0xd3 || text[i] == '\0')){
			len++;
		}
	}
	return len;
}

uint32_t powx(uint8_t x, uint8_t y)
{
	uint32_t res = x;

	if (y == 0)
	return 1;

	for (int i = 0; i < ((int)y - 1); i++) {
		res = res * x;
	}

	return res;
}
