/*
 * MCU.cpp
 *
 *  Created on: 2 авг. 2022 г.
 *      Author: amaslov
 */

#include "MCU.h"
#include "gpio.h"
#include "spi.h"
#include "main.h"
#include "dma.h"
#include "iwdg.h"
#include "rtc.h"

std::list<InterruptHandler*> MCU::tim2Callbacks;
std::list<InterruptHandler*> MCU::tim21Callbacks;

MCU::MCU()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();

	MX_TIM2_Init();
	HAL_TIM_RegisterCallback(&htim2, HAL_TIM_PERIOD_ELAPSED_CB_ID, tim2Callback);

	MX_TIM21_Init();
	HAL_TIM_RegisterCallback(&htim21, HAL_TIM_PERIOD_ELAPSED_CB_ID, tim21Callback);

	MX_DMA_Init();
	MX_RTC_Init();
}

void MCU::clearRTCDate() {
	RTC_TimeTypeDef sTime = {};
	RTC_DateTypeDef sDate = {};
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

struct tm MCU::getRTCTime()
{
	struct tm ret = {};
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	if (HAL_OK == HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) &&  HAL_OK == HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN)) {
		ret.tm_sec = sTime.Seconds;
		ret.tm_min = sTime.Minutes;
		ret.tm_hour = sTime.Hours;
		ret.tm_mday = sDate.Date;
		ret.tm_mon = sDate.Month;
		ret.tm_year = sDate.Year;
	}
	return ret;
};

void MCU::tim21Start() {
	HAL_TIM_Base_Start_IT(&htim21);
}

void MCU::tim21Stop() {
	HAL_TIM_Base_Stop_IT(&htim21);
}

void MCU::tim21AddCallback(InterruptHandler* handler)
{
	tim21Callbacks.push_back(handler);
}

void MCU::tim21Callback(TIM_HandleTypeDef *htim)
{
    for (InterruptHandler* handler : tim21Callbacks) {
    	handler->handle();
    }
}

bool MCU::isIWDGRestart()
{
	if ( __HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	{
		__HAL_RCC_CLEAR_RESET_FLAGS();
		return true;
	}

	return false;
}

void MCU::IWDGRun()
{
	MX_IWDG_Init();
}

void MCU::IWDGRefresh()
{
	HAL_IWDG_Refresh(&hiwdg);
}

void MCU::tim2Start() {
	HAL_TIM_Base_Start_IT(&htim2);
}

void MCU::tim2Stop() {
	HAL_TIM_Base_Stop_IT(&htim2);
}

void MCU::tim2AddCallback(InterruptHandler* handler)
{
	tim2Callbacks.push_back(handler);
}

void MCU::tim2Callback(TIM_HandleTypeDef *htim)
{
    for (InterruptHandler* handler : tim2Callbacks)
    	handler->handle();
}
