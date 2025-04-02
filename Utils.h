/*
 * Utils.h
 *
 *  Created on: 11 мар. 2022 г.
 *      Author: amaslov
 */

#ifndef INC_UTILS_H_
#define INC_UTILS_H_
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


#define htonl(A) ((((uint32_t)(A) & 0xff000000) >> 24) | (((uint32_t)(A) & 0x00ff0000) >> 8) | (((uint32_t)(A) & 0x0000ff00) << 8) | (((uint32_t)(A) & 0x000000ff) << 24))
#define htons(A) ((((uint16_t)(A) & 0xFF00) >> 8) | (((uint16_t)(A) & 0x00FF) << 8))

char* floatToChar (char *buf, size_t size, int32_t floatValue, const int devider);
size_t strUtf8len(const char* text, const int size);
uint32_t powx(uint8_t x, uint8_t y);

struct tm diffTime(struct tm &t1, struct tm &t2);
uint32_t diffTimeInSec(struct tm &t1, struct tm &t2);
uint32_t timeInSec(struct tm &t);
struct tm timeInUTC(uint32_t sec);

void udelay(uint16_t us);
uint8_t crc8(uint8_t ndata, uint8_t crc);

void nops(uint32_t n);

#endif /* INC_UTILS_H_ */
