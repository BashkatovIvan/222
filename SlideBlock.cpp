/*
 * SlideBlock.cpp
 *
 *  Created on: 18 авг. 2022 г.
 *      Author: amaslov
 */

#include "SlideBlock.h"
#include "Globals.h"

#define SLIDE_BLOCK_INTERRUPT_PRESCALER 1
#define SLIDE_BLOCK_FIX_TIME (10 * SLIDE_BLOCK_INTERRUPT_PRESCALER)

SlideBlock::SlideBlock()
{
	clutch = new Clutch();
	samplesLimit = dataBase->getStopControl() == 0 ? 4 : 6;
}

void SlideBlock::handle()
{
	((InterruptHandler *)clutch)->handle();

	interruptPrescaler = interruptPrescaler >= (SLIDE_BLOCK_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (SLIDE_BLOCK_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

MainSensorPosition SlideBlock::mainSensorGetPosition()
{
	return mainSensorPosition;
}

HandSensorPosition SlideBlock::handSensorGetPosition()
{
	return handSensorPosition;
}

void SlideBlock::clearErrors()
{
	/*
	 * При возникновении ошибки в работе пресса, ползун будет принудительно остановлен.
	 * При этом состояние SQ1, SQ2, SQ3 отслеживаться не будет
	 * Поэтому нужно инициализировать все переменные заново, даже если ошибка не связана с работой ползуна.
	 */

	clutch->clearErrors();

	stateTime = 0;
	samples = 0;
	mainSensorPosition = MainSensorPosition::MAIN_SENSOR_UNKNOWN_POSITION;
	handSensorPosition = HandSensorPosition::HAND_SENSOR_UNKNOWN_POSITION;

	error = SystemError::NO_ERROR;
	state = SlideBlockStates::SLIDE_BLOCK_STOPPED;
}

void SlideBlock::rotateSamples(SQState *buff, int8_t size, int8_t offset)
{
	if (offset >= size || offset == 0)
		return;

	int8_t start = offset;
	int8_t stop = offset - 1;

	int8_t i = 0, j = start;

	SQState tmp[size];

	for (;;)
	{
		tmp[i++] = buff[j];

		j = j < (size - 1) ? j + 1 : 0;

		if (j == stop)
			break;
	}

	memcpy(buff, tmp, sizeof (tmp));
}

SlideBlockStates SlideBlock::postInterruptHandler(Sensors *s)
{
	bool SQ3ChangedState = false;

	if (!interruptFlag)
		return state;

	interruptFlag = false;

	bool enabled = clutch->getState() == ClutchStates::CLUTCH_ENABLED;

	/*В режимах "Ручной проворот" и "Наладка" ошибки позуна, связанные с его положением не нужно фиксировать, поэтому достаточно только одного отсчета и текущего положения ползуна*/
	if (!checkErrors)
		samples = 0;

	if (s->SQ3 == sensors.SQ3)
	{
		uint8_t prevSQ3 = SQ3;
		SQ3ChangedState = prevSQ3 != (uint8_t)s->SQ3;
		SQ3 = s->SQ3 ? 1 : 0;

		//Обновить состояние положения датчика сопровождения рук
		handSensorPosition = SQ3 == 0 ? HandSensorPosition::HAND_SENSOR_ON : HandSensorPosition::HAND_SENSOR_OFF;
	}

	if (s->SQ1 == sensors.SQ1 && s->SQ2 == sensors.SQ2)
	{
		stateTime = stateTime < INT_MAX ? stateTime + 1 : INT_MAX;

		SQState curState = {.SQ1 = (uint8_t)s->SQ1, .SQ2 = (uint8_t)s->SQ2};

		/*Обновить положение ползуна. Если контроль тормозного пути отключен, значит выреза на диске нет и состояния MainSensorPosition::MAIN_SENSOR_STOP_CONTROL быть не может*/
		if (dataBase->getStopControl())
		{
			mainSensorPosition = (curState.SQ1 == 0 && curState.SQ2 == 1) ? MainSensorPosition::MAIN_SENSOR_STOP_CONTROL :
			                     (curState.SQ1 == 0 && curState.SQ2 == 0) ? MainSensorPosition::MAIN_SENSOR_ON :
	                             (curState.SQ1 == 1 && curState.SQ2 == 1) ? MainSensorPosition::MAIN_SENSOR_OFF :
	                            		                                    MainSensorPosition::MAIN_SENSOR_UNKNOWN_POSITION;
		}
		else
		{
			mainSensorPosition = (curState.SQ1 == 0 && curState.SQ2 == 0) ? MainSensorPosition::MAIN_SENSOR_ON :
	                             (curState.SQ1 == 1 && curState.SQ2 == 1) ? MainSensorPosition::MAIN_SENSOR_OFF :
	                            		                                    MainSensorPosition::MAIN_SENSOR_UNKNOWN_POSITION;
		}

		if (stateTime >= SLIDE_BLOCK_FIX_TIME)
		{
			//Первый семпл
			if (samples == 0)
			{
				stateSamples[samplesLimit - 1] = curState;
				samples++;
			}
			//Если состояние изменилось, это состояние сохраняется в буфер сэмплов
			else if (memcmp(&stateSamples[samplesLimit - 1], &curState, sizeof(curState)))
			{
				rotateSamples(stateSamples, samplesLimit, 1);
				stateSamples[samplesLimit - 1] = curState;
				samples = samples < samplesLimit ? samples + 1 : samplesLimit;
			}
		}

		if (checkErrors)
		{
			if (samples > 1)
			{
				if (stateSamples[samplesLimit - 2].SQ2 != stateSamples[samplesLimit - 1].SQ2 && stateSamples[samplesLimit - 1].SQ2 == 0 && SQ3 != 1)
				{
					error = N02_SQ3_MUSTBE_1_IF_SQ2_SWITCH_TO_0;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}
				if (!memcmp(&stateSamples[samplesLimit - 2], &CHECK_ERR_N25, sizeof(SQState) * 2))
				{
					error = N25_HAS_SQ1_0_AND_HAS_SQ2_0_LATER;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}
				if (!memcmp(&stateSamples[samplesLimit - 2], &CHECK_ERR_N27, sizeof(SQState) * 2))
				{
					error = N27_HAS_SQ2_0_AND_HAS_SQ1_0_LATER;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}
			}

			if (samples >= samplesLimit)
			{
				bool notN22State = true;
				bool notN23State = true;
				bool hasSQ1 = false;
				bool hasSQ2 = false;

				for (uint8_t i = 0; i < samplesLimit; i++)
				{
					if (notN22State)
					{
						if (!memcmp(&stateSamples[i], &CHECK_ERR_N22, sizeof(SQState)))
						{
							notN22State = false;
						}
					}

					if (notN23State)
					{
						if (!memcmp(&stateSamples[i], &CHECK_ERR_N23, sizeof(SQState)))
						{
							notN23State = false;
						}
					}

					if (!hasSQ1)
					{
						hasSQ1 = stateSamples[i].SQ1 == 0? true : false;
					}

					if (!hasSQ2)
					{
						hasSQ2 = stateSamples[i].SQ2 == 0 ? true : false;
					}
				}

				if (hasSQ1 && !hasSQ2)
				{
					error = N24_HAS_SQ1_0_AND_NOT_SQ2_0;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}

				if (hasSQ2 && !hasSQ1)
				{
					error = N26_HAS_SQ2_0_AND_NOT_SQ1_0;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}

				if (notN22State)
				{
					error = N22_NOT_SQ1_1_AND_NOT_SQ2_1;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}

				if (notN23State)
				{
					error = N23_NOT_SQ1_0_AND_NOT_SQ2_0;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}
			}
		}
	}
	else
	{
		stateTime = 0;
	}

	if (checkErrors)
	{
		SQ2Timeout = enabled ? (s->SQ2 == 1 ? (SQ2Timeout < INT_MAX ? SQ2Timeout + 1 : INT_MAX) : 0) : 0;
		if (SQ2Timeout >= 10000)
		{
			error = N29_CLUTCH_ENABLED_AND_SQ2_10S_TIMEOUT;
			return SlideBlockStates::SLIDE_BLOCK_ERROR;
		}

		if (SQ3ChangedState)
		{
			if (SQ3 == 1)
			{
				if (stateSamples[samplesLimit - 1].SQ1 == 0)
				{
					error = N30_SQ1_MUST_BE_1_IF_SQ3_SWITCH_0;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}
			}
			else if (SQ3 == 0)
			{
				if (stateSamples[samplesLimit - 1].SQ1 == 0 && stateSamples[samplesLimit - 1].SQ2 == 0)
				{
					error = N03_SQ3_SWITCH_0_BEFORE_SQ1_SQ2;
					return SlideBlockStates::SLIDE_BLOCK_ERROR;
				}
			}
		}

	}

	return state = enabled ? SlideBlockStates::SLIDE_BLOCK_STARTED : SlideBlockStates::SLIDE_BLOCK_STOPPED;
}

void SlideBlock::run()
{
	if (state == SlideBlockStates::SLIDE_BLOCK_ERROR)
		return;

	if (state == SlideBlockStates::SLIDE_BLOCK_STARTED)
		return;

	clutch->switchOn();
}

void SlideBlock::stop()
{
	clutch->switchOff();
}

SystemError SlideBlock::getError()
{
	return error;
}

SlideBlockStates SlideBlock::getState()
{
	return state;
}

SlideBlockStates SlideBlock::updateState(Sensors *s)
{
	for (;;)
	{
		if (state == SlideBlockStates::SLIDE_BLOCK_ERROR)
			break;

		if (ClutchStates::CLUTCH_ERROR == clutch->updateState(s))
		{
			error = clutch->getError();
			state = SlideBlockStates::SLIDE_BLOCK_ERROR;
			break;
		}

		if (SlideBlockStates::SLIDE_BLOCK_ERROR == (state = postInterruptHandler(s)))
		{
			stop();
			break;
		}

		break;
	}

	sensors = *s;

	return state;
}

