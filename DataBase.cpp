/*
 * DB.cpp
 *
 * Created: 16.12.2021 10:02:25
 *  Author: amaslov
 */ 
#include "DataBase.h"
#include "main.h"
#include "Utils.h"
#include <stdio.h>

#define ADDRESS_DATA_STORAGE	0x08080000

void DataBase::setAdjustmentModeTime(uint32_t value) {
	data.adjustmentModeTime = value;
}

uint32_t DataBase::getAdjustmentModeTime() {
	return data.adjustmentModeTime;
}

void DataBase::setProtectiveShieldEnabled(uint32_t value){
	data.protectiveShieldEnabled = value;
}
uint32_t DataBase::getProtectiveShieldEnabled(){
	return data.protectiveShieldEnabled;
}

void DataBase::setLubricantTime(uint32_t value){
	data.lubricantTime = value;
}
uint32_t DataBase::getLubricantTime(){
	return data.lubricantTime;
}

void DataBase::setLubricantCount(uint32_t value){
	data.lubricantCount = value;
}
uint32_t DataBase::getLubricantCount(){
	return data.lubricantCount;
}

void DataBase::setLubricantOnTime(uint32_t value){
	data.lubricantOnTime = value;
}
uint32_t DataBase::getLubricantOnTime(){
	return data.lubricantOnTime;
}

void DataBase::setLubricantOffTime(uint32_t value){
	data.lubricantOffTime = value;
}
uint32_t DataBase::getLubricantOffTime(){
	return data.lubricantOffTime;
}

void DataBase::setLubricantOffCount(uint32_t value){
	data.lubricantOffCount = value;
}
uint32_t DataBase::getLubricantOffCount(){
	return data.lubricantOffCount;
}

void DataBase::setLubricantType(uint32_t value){
	data.lubricantType = value;
}
uint32_t DataBase::getLubricantType(){
	return data.lubricantType;
}

void DataBase::setStopControl(uint32_t value){
	data.hasStopControl = value;
}
uint32_t DataBase::getStopControl() {
	return data.hasStopControl;
}
void DataBase::setTrialCount(uint32_t value) {
	data.trialCount = value;
}
uint32_t DataBase::getTrialCount(){
	return data.trialCount;
}
void DataBase::setTrialProtection1(uint32_t value){
	data.trialProtection1 = value;
}
uint32_t DataBase::getTrialProtection1(){
	return data.trialProtection1;
}
void DataBase::setTrialProtection2(uint32_t value){
	data.trialProtection2 = value;
}
uint32_t DataBase::getTrialProtection2(){
	return data.trialProtection2;
}
void DataBase::setTrialProtection3(uint32_t value){
	data.trialProtection3 = value;
}
uint32_t DataBase::getTrialProtection3(){
	return data.trialProtection3;
}
void DataBase::setProgrammVersion(uint32_t value) {
	data.programVersion = value;
}
uint32_t DataBase::getProgrammVersion(){
	return data.programVersion;
}
void DataBase::setBrowingOffWorkTime(uint32_t value){
	data.browingOffWorkTime = value;
}
uint32_t DataBase::getBrowingOffWorkTime(){
	return data.browingOffWorkTime;
}

void DataBase::setBrowingOffEnabled(uint32_t value){
	data.browingOffEnabled = value;
}
uint32_t DataBase::getBrowingOffEnabled(){
	return data.browingOffEnabled;
}

void DataBase::setAll(DataBaseData data) {
	this->data = data;
}

DataBaseData DataBase::getAllFromDataBase() {
	return data;
}

void DataBase::saveAllToStorage(DataBaseData data)
{
	int cnt = sizeof(DataBaseData) / 4;
	uint32_t *ptr = (uint32_t*)&data;

	HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Erase(0);
	HAL_FLASHEx_DATAEEPROM_Lock();

	HAL_FLASHEx_DATAEEPROM_Unlock();
	for (int i = 0; i < cnt; i++) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, ADDRESS_DATA_STORAGE + i * 4, *(ptr + i));
	}
	HAL_FLASHEx_DATAEEPROM_Lock();
}

DataBaseData DataBase::getAllFromStorage()
{
	DataBaseData data;

	if (*((__IO uint32_t *)ADDRESS_DATA_STORAGE) == (uint32_t)DATA_BASE_NOT_VALID)
	{
		setDefaults(&data);
	}
	else
	{
		int cnt = sizeof(DataBaseData) / 4;
		uint32_t *ptr = (uint32_t*)&data;

		for (int i = 0; i < cnt; i++) {
			*(ptr + i) = *((__IO uint32_t *)(ADDRESS_DATA_STORAGE + i * 4));
		}
	}

	return data;
}

void DataBase::updateData()
{
	if (*((__IO uint32_t *)ADDRESS_DATA_STORAGE) == (uint32_t)DATA_BASE_NOT_VALID)
	{
		setDefaults();
		saveData();
	}
	else
	{
		int cnt = sizeof(DataBaseData) / 4;
		uint32_t *ptr = (uint32_t*)&data;

		for (int i = 0; i < cnt; i++) {
			*(ptr + i) = *((__IO uint32_t *)(ADDRESS_DATA_STORAGE + i * 4));
		}
	}
}

void DataBase::saveData()
{
	saveAllToStorage(data);
}

void DataBase::setDefaults()
{
	setDefaults(&data);
}

void DataBase::setDefaults(DataBaseData *data)
{
	data->defaultValueOnStartAddress = 5;

	data->lubricantTime = 10;//s
	data->lubricantCount = 2;

	data->lubricantOnTime = 5;//s
	data->lubricantOffTime = 1;//minutes
	data->lubricantOffCount = 10;
	data->lubricantType = LubricantType::BY_TIME;

	data->hasStopControl = 1; //bool

	data->browingOffWorkTime = 100; //ms
	data->browingOffEnabled = 0; // on/off

	data->adjustmentModeTime = 10; //ms

	data->protectiveShieldEnabled = 1;

	data->trialCount = 0;
	data->trialProtection1 = crc8(data->trialCount, 0x14);
	data->trialProtection2 = crc8(data->trialProtection1, 0x14);
	data->trialProtection3 = crc8(data->trialProtection2, 0x14);
}
