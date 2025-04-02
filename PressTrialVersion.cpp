/*
 * PressTrialVersion.cpp
 *
 *  Created on: 31 авг. 2022 г.
 *      Author: amaslov
 */
#include "Press.h"
#include "Globals.h"
#include "Utils.h"


void Press::checkTrialLimtis()
{
	bool trialExpired = false;
	uint8_t error = 0;
	uint32_t trialCount = dataBase->getTrialCount();
	uint32_t trialProtection1db = dataBase->getTrialProtection1();
	uint32_t trialProtection2db = dataBase->getTrialProtection2();
	uint32_t trialProtection3db = dataBase->getTrialProtection3();

	uint32_t trialProtection1;
	uint32_t trialProtection2;
	uint32_t trialProtection3;

	if (trialCount >= TRIAL_COUNT_MAX)
	{
		trialExpired = true;
		error = 33;
	}
	else
	{
		error = 34;

		trialProtection1 = crc8(trialCount, 0x14);
		trialProtection2 = crc8(trialProtection1, 0x14);
		trialProtection3 = crc8(trialProtection2, 0x14);

		if (trialProtection1 != trialProtection1db)
			trialExpired = true;
		else if (trialProtection2 != trialProtection2db)
			trialExpired = true;
		else if (trialProtection3 != trialProtection3db)
			trialExpired = true;
	}

	while (trialExpired)
	{
		DisplayData d;
		watchdog->blockOutputCircuits();

		d.error = error;
		d.digit7 = trialCount / 10;
		d.digit6 = trialCount % 10;
		d.digit5 = 7;
		d.digit4 = 7;
		d.digit3 = 7;

		hmi.display->updateData(d);
		hmi.display->updateState();
	}

	trialCount++;
	trialProtection1 = crc8(trialCount, 0x14);
	trialProtection2 = crc8(trialProtection1, 0x14);
	trialProtection3 = crc8(trialProtection2, 0x14);

	dataBase->setTrialCount(trialCount);
	dataBase->setTrialProtection1(trialProtection1);
	dataBase->setTrialProtection2(trialProtection2);
	dataBase->setTrialProtection3(trialProtection3);

	dataBase->saveData();
}



