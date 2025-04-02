/*
 * DB.h
 *
 * Created: 16.12.2021 10:02:39
 *  Author: amaslov
 */ 


#ifndef DB_H_
#define DB_H_
#include <stdint.h>
#include <string.h>
#include <limits.h>

typedef enum _LubricantType
{
	LUBRICANT_OFF = 0,
	BY_COUNTER,
	BY_TIME,
} LubricantType;

#define DATA_BASE_NOT_VALID 0
#define TRIAL_COUNT_MAX 50

typedef struct _DataBaseData
{
	uint32_t defaultValueOnStartAddress = 0x05;
	uint32_t programVersion = DATA_BASE_NOT_VALID;
	uint32_t lubricantTime = DATA_BASE_NOT_VALID;        //s
	uint32_t lubricantCount = DATA_BASE_NOT_VALID;
	uint32_t lubricantOnTime = DATA_BASE_NOT_VALID;      //s
	uint32_t lubricantOffTime = DATA_BASE_NOT_VALID;     //minutes
	uint32_t lubricantOffCount = DATA_BASE_NOT_VALID;
	uint32_t lubricantType = DATA_BASE_NOT_VALID;
	uint32_t trialProtection1 = DATA_BASE_NOT_VALID;
	uint32_t trialCount = DATA_BASE_NOT_VALID;
	uint32_t trialProtection2 = DATA_BASE_NOT_VALID;
	uint32_t trialProtection3 = DATA_BASE_NOT_VALID;
	uint32_t hasStopControl = DATA_BASE_NOT_VALID;
	uint32_t adjustmentModeTime = DATA_BASE_NOT_VALID;
	uint32_t browingOffWorkTime = DATA_BASE_NOT_VALID;
	uint32_t browingOffEnabled = DATA_BASE_NOT_VALID;
	uint32_t protectiveShieldEnabled = DATA_BASE_NOT_VALID;
} DataBaseData;

class DataBase
{
private:
	DataBaseData data;

public:
	void setProgrammVersion(uint32_t value);
	uint32_t getProgrammVersion();

	void setProtectiveShieldEnabled(uint32_t value);
	uint32_t getProtectiveShieldEnabled();

	void setLubricantTime(uint32_t value);
	uint32_t getLubricantTime();

	void setLubricantCount(uint32_t value);
	uint32_t getLubricantCount();

	void setLubricantOnTime(uint32_t value);
	uint32_t getLubricantOnTime();

	void setLubricantOffTime(uint32_t value);
	uint32_t getLubricantOffTime();

	void setLubricantOffCount(uint32_t value);
	uint32_t getLubricantOffCount();

	void setLubricantType(uint32_t value);
	uint32_t getLubricantType();

	void setStopControl(uint32_t value);
	uint32_t getStopControl();

	void setAdjustmentModeTime(uint32_t value);
	uint32_t getAdjustmentModeTime();

	void setTrialCount(uint32_t value);
	uint32_t getTrialCount();

	void setTrialProtection1(uint32_t value);
	uint32_t getTrialProtection1();

	void setTrialProtection2(uint32_t value);
	uint32_t getTrialProtection2();

	void setTrialProtection3(uint32_t value);
	uint32_t getTrialProtection3();

	void setBrowingOffWorkTime(uint32_t value);
	uint32_t getBrowingOffWorkTime();

	void setBrowingOffEnabled(uint32_t value);
	uint32_t getBrowingOffEnabled();

	void updateData();
	void saveData();
	void setDefaults();
	void setDefaults(DataBaseData *data);
	void setAll(DataBaseData data);
	void saveAllToStorage(DataBaseData data);
	DataBaseData getAllFromDataBase();
	DataBaseData getAllFromStorage();
};



#endif /* DB_H_ */
