#include "stm32f1xx_hal.h"
#ifndef INC_FUNTION_H_
#define INC_FUNTION_H_
#endif /* INC_FUNTION_H_ */

/*FUNTION DISPLAY TIME (us)*/
void DELAY_US(uint32_t time);
/*FUNTION DISPLAY TIME (S) HAVE UPDATE TEMPERATURE*/
void DELAY_COUNT(uint8_t time);
/*FUNTION DISPLAY LCD*/
void DISPLAY_LCD();
/*FUNTION DISPLAY READY*/
void READY();
/*FUTION SET PIN OUTPUT*/
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
/*FUTION SET PIN INPUT*/
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/*********************************** DS18B20 BOILER FUNCTIONS ****************************************/
uint8_t DS18B20_BOILER_Start (void);
void DS18B20_BOILER_Write(uint8_t data);
uint8_t DS18B20_BOILER_Read(void);


/*FUNTION INSPECT TEMPORATURE OF BOILER*/
float TEMPORATURE_OF_BOILER();
/*FUNTION INSPECT WATER LEVEL OF BOILER*/
void INSPECT_WATER_LEVEL_OF_BOILER();
/*FUNTION INSPECT TEMPORATURE OF BOILER*/
void INSPECT_TEMPORATURE_OF_BOILER(uint8_t Tem_std);

/*FUNTION INSPECT TEMPORATURE OF BOILER*/
void INSPECT_TEMPORATURE_OF_TANK(uint8_t Tem_std);
/*FUNTION OPEN VALVE*/
void OPEN_VALVE();
/*FUNTION CLOSE VALVE*/
void CLOSE_VALVE();
/*FUNTION OPEN PUMP RINSE*/
void OPEN_PUMP_RINSE();
/*FUNTION CLOSE PUMP RINSE*/
void CLOSE_PUMP_RINSE();
/*FUNTION OPEN PUMP WASH*/
void OPEN_PUMP_WASH();
/*FUNTION CLOSE PUMP WASH*/
void CLOSE_PUMP_WASH();
/*FUNTION OPEN BOILER HEATER*/
void OPEN_BOILER_HEATER();
/*FUNTION CLOSE BOILER HEATER*/
void CLOSE_BOILER_HEATER();
/*FUNTION OPEN TANK HEATER*/
void OPEN_TANK_HEATER();
/*FUNTION CLOSE TANK HEATER*/
void CLOSE_TANK_HEATER();
/*FUNTION STOP EVERY THING*/
void STOP();
/*FUNTION DISPLAY_TEMPORATURE OF BOILER*/
void DISPLAY_TEMPORATURE_BOILER(float Temp);
/*FUNTION DISPLAY_TEMPORATURE OF TANK*/
void DISPLAY_TEMPORATURE_TANK(float Temp);
/*PRESS POWER BUTTON*/
void CHECK_PRESS_POWER_BUTTON();
/*UPDATE TEMPERATURE OF BOILER AND TANK*/
void UPDATE_TEMPERATURE();
/*FUNTION CHECK WATER LEVEL BOILER AND TANK AND DOOR*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
/*FUNTION BUZZER*/
void SET_BUZZER();
/*FUNTION OF MODE WASH STANDARD*/
void WASH_MODE_STD();
/*FUNTION OF MODE WASH HIGHTEMP*/
void WASH_MODE_HIGH();
/*FUNTION RESET*/
void resetSTM32();
/*FUNTION SELECT MODE*/
void SELECT_MODE(uint8_t Tem_std_Boiler,uint8_t Tem_std_Tank);
/*FUNTION INSPECT DOOR CLOSE TO WASH*/
void CLOSE_DOOR_TO_WASH(uint8_t Tem_std_Boiler,uint8_t Tem_std_Tank);
/*FUNTION TIMER INTERRUPT*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
/*FUNTION OPEN PUMP MECHICAL WASH*/
void OPEN_PUMP_MECHICAL_WASH();
/*FUNTION OPEN PUMP MECHICAL RINSE*/
void OPEN_PUMP_MECHICAL_RINSE();
/*FUNTION CLOSE PUMP MECHICAL WASH*/
void CLOSE_PUMP_MECHICAL_WASH();
/*FUNTION OPEN PUMP MECHICAL RINSE*/
void CLOSE_PUMP_MECHICAL_RINSE();