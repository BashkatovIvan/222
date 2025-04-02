/*
 * SystemErrors.h
 *
 *  Created on: 11 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef INCLUDE_SYSTEMERRORS_H_
#define INCLUDE_SYSTEMERRORS_H_

enum SystemInfo
{
	NO_INFO = 0,
	N31_LUBRICANT_ON_STARTED = 31, //Запущена прокачка смазки при включении станка
	N32_START_MAIN_MOTOR = 32, //Запуск главного привода. Ожидайте
	N33_MODE_ADJUSTMENT_START_MAIN_MOTOR = 33, //Режим "Наладка". Запуск главного привода. Ожидайте 10 секунд
	N34_MODE_ONE_STEP_START_MAIN_MOTOR = 34, //Режим "Одиночный ход". Запуск главного привода. Ожидайте 10 секунд
	N35_MODE_CONTINUOUS_START_MAIN_MOTOR = 35, //Режим "Непрерывные хода". Запуск главного привода. Ожидайте 10 секунд
	N36_MODE_MANUAL_STOP_MAIN_MOTOR = 36, //Режим "Ручной проворот". Останов главного привода. Ожидайте 2 минуты
	N37_MODE_ADJUSTMENT = 37, //Режим "Наладка".
	N38_MODE_ONE_STEP = 38, //Режим "Одиночный ход".
	N39_MODE_CONTINUOUS = 39, //Режим "Непрерывные хода".
	N40_MODE_MANUAL = 40, //Режим "Ручной проворот".
};

enum SystemWarning
{
	NO_WARNING = 0,
	N09_LUBRICANT_OIL_LEVEL_W = 9, //низкий уровень масла (SL должен быть = 0)
	N12_LUBRICANT_PRESSURE_ERRROR_W = 12, //в течение 30с включена смазка, а ответа нет (SP2 должен =1)
	N13_LUBRICANT_PRESSURE_ERRROR_W = 13, //выключена смазка (SP2 должен быть =0)
	N50_SB5_PRESSED = 50, //была нажата кнопка SB5 «стоп непрерывных ходов»(SB5=0)
	N69_PEDAL_RELEASED = 69, //была нажата кнопка SB5 «стоп непрерывных ходов»(SB5=0)
	N78_KEYBOARD_IS_LOCKED = 78, //клавиатура заблокирована (SA =0)
};

enum SystemError
{
	NO_ERROR = 0,
	N01_TWO_BUTTONS_CONTROL_HAS_ONLY_ONE_BUTTON = 1, //вместо двух кнопок SB3, SB4 используется одна. Устранить!
	N02_SQ3_MUSTBE_1_IF_SQ2_SWITCH_TO_0 = 2, // SQ3 должен быть =1 в момент, когда SQ2 стал = 0
	N03_SQ3_SWITCH_0_BEFORE_SQ1_SQ2 = 3, // датчик SQ3 сработал (стал = 0) раньше датчиков SQ1, SQ2
	N05_SQ13_LIMIT_OVER = 5, //датчик SQ13 сработал более 99 раз в секунду
	N08_SP1_MUST_BE_1 = 8, //нет воздуха (SP1 должен быть =1)
	N09_LUBRICANT_OIL_LEVEL = 9, //низкий уровень масла (SL должен быть = 0)
	N10_PRESS_OVERLOAD = 10, //перегрузка (SQ16 должен быть =1)
	N11_SQ11_IS_OFF = 11, // датчик согласованной работы клапанов(SQ11 должен быть =1)
	N12_LUBRICANT_PRESSURE_ERRROR = 12, //в течение 30с включена смазка, а ответа нет (SP2 должен =1)
	N13_LUBRICANT_PRESSURE_ERRROR = 13, //выключена смазка (SP2 должен быть =0)
	N14_CLUTCH_TRANSISTOR_BREAKAGE = 14, //неисправны выходные ключи муфты-тормоза (обрыв)
	N15_CLUTCH_TRANSISTORS_SHORT_CIRCUIT = 15, //неисправны выходные ключи муфты-тормоза (пробиты)
	N16_CLUTCH_TRANSISTOR_SHORT_CIRCUIT = 16,  //пробит выходной ключ муфты-тормоза
	N17_EXCEEDING_BRAKING_DISTANCE = 17, //превышение тормозного пути
	N20_STM32_FREEZE = 20, //зависание микроконтроллера
	N21_BLOCK_SENSOR_EVENT = 21, // сработали блокировки (SQ15 должен быть =1)
	N22_NOT_SQ1_1_AND_NOT_SQ2_1 = 22, //не было SQ1=SQ2=1
	N23_NOT_SQ1_0_AND_NOT_SQ2_0 = 23, //не было SQ1=SQ2=0
	N24_HAS_SQ1_0_AND_NOT_SQ2_0 = 24, //было SQ1=0, но не было SQ2=0
	N25_HAS_SQ1_0_AND_HAS_SQ2_0_LATER = 25, // было SQ1=0, затем SQ2=0, но не одновременно
	N26_HAS_SQ2_0_AND_NOT_SQ1_0 = 26, //было SQ2=0, но не было SQ1=0
	N27_HAS_SQ2_0_AND_HAS_SQ1_0_LATER = 27, //было SQ2=0, затем SQ1=0, но не одновременно
	N29_CLUTCH_ENABLED_AND_SQ2_10S_TIMEOUT = 29, //в течение 10 секунд включена муфта-тормоз, а SQ2 не стал =0
	N30_SQ1_MUST_BE_1_IF_SQ3_SWITCH_0 = 30, //SQ1 должен быть =1 в момент, когда SQ3 стал = 1
	N51_PROTECTIVE_SHIELD_IS_OFF = 51, //не опущен экран (SQ14 должен быть =1)
	N52_CONTROL_PEDAL_MUST_BE_ENABLED = 52, //не нажата педаль (SB6.1 должен быть =1, а SB6.2 = 0)
	N53_CONTROL_PEDAL_MUST_BE_DISABLE = 53, //не отжата педаль (SB6.1 должен быть =0, а SB6.2 = 1)
	N59_SLIDE_BLOCK_IS_NOT_STARTED_POSITION = 59, //ползун не в исходном положении (SQ3 должен быть =0)
	N60_CONTROL_BUTTONS_MUST_BE_DISABLED = 60, //кнопки «пуск цикла» должны быть отжаты (SB3.1 и SB4.1 должны быть =0, а SB3.2 и SB4.2 =1)
	N61_CONTROL_BUTTONS_MUST_BE_ENABLED = 61, //кнопки «пуск цикла» должны быть нажаты (SB3.1 и SB4.1 должны быть =1, а SB3.2 и SB4.2 =0)
	N68_PEDAL_CONTROL_OVER_TIME = 68, //медленно нажата педаль (SB6.1 и SB6.2 должны сработать с интервалом не более 0,5 секунд)
	N70_PROTECTIVE_SHIELD_WAS_OFF = 70, //был поднят защитный экран (SQ14 был =0)
	N71_TWO_BUTTONS_CONTROL_OVER_TIME = 71, //медленно нажата кнопка SB3 (SB3.1 и SB3.2 должны сработать с интервалом не более 0,5 секунд)
	N79_KM1_IS_OFF = 79, //не включен главный привод (КМ1.1 должен быть =1)
	N80_KM1_IS_ON = 80, //не выключен главный привод (КМ1.1 должен быть =0)
};


#endif /* INCLUDE_SYSTEMERRORS_H_ */
