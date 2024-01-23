#include "stdio.h"
#include"i2c-lcd.h"
#include "main.h"
#include "Funtion.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

extern uint8_t power_press ;
extern uint8_t Temperature_max ;
extern float Temperature_boiler;
extern float  Temperature_tank ;
extern uint8_t Presence ;
extern uint8_t Temp_byte1, Temp_byte2;
extern uint16_t  TEMP;
extern uint8_t BOILER_FULL;
extern uint8_t DOOR ;
extern uint8_t count ;
extern uint8_t a;
extern uint8_t MODE ;
extern uint8_t const Temp_Std_Wash;
extern uint8_t const Temp_Std_Rinse;

/*FUNTION DISPLAY TIME (US)*/
void DELAY_US(uint32_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
}
/*FUNTION DISPLAY TIME (S) HAVE UPDATE TEMPERATURE*/
void DELAY_COUNT(uint8_t time){
	time = time/2;
	while(count < time){
		UPDATE_TEMPERATURE();
	}
}
/*FUNTION DISPLAY LCD*/
void DISPLAY_LCD(){
	lcd_init ();
  lcd_send_string ("CAPTONE PROJECT");
	lcd_put_cur(1,0);
	lcd_send_string("DISHWASHER MACHINE");
  HAL_Delay(3000);
	lcd_clear ();
	HAL_Delay(100);
	lcd_put_cur(0,0);
  lcd_send_string("PRESS P TO START");
}
/*FUNTION READY FOR WASH*/
void READY(){
				lcd_clear();
				lcd_put_cur(0,5);
				lcd_send_string("READY");
				SET_BUZZER();
}
/*FUTION SET PIN INPUT*/
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
/*FUTION SET PIN INPUT*/
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*********************************** DS18B20 BOILER FUNCTIONS ****************************************/
uint8_t DS18B20_BOILER_Start (void)
{
	uint8_t Response = 0;
	Set_Pin_Output(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);   // set the pin as output
	HAL_GPIO_WritePin (DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin, 0);  // pull the pin low
	DELAY_US(480);   // delay according to datasheet
	Set_Pin_Input(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);    // set the pin as input
	DELAY_US (80);    // delay according to datasheet
	if (!(HAL_GPIO_ReadPin (DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = 2;
	DELAY_US(400); // 480 us delay totally.
	return Response;
}
void DS18B20_BOILER_Write(uint8_t data)
{
	Set_Pin_Output(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);  // set as output
	for (int i=0; i<8; i++){
		if ((data & (1<<i))!=0)  // if the bit is high
		{
			Set_Pin_Output(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);  // set as output
			HAL_GPIO_WritePin (DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin, 0);  // pull the pin LOW
			DELAY_US(1);  // wait for 1 us
			Set_Pin_Input(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);  // set as input
			DELAY_US(50);  // wait for 60 us
		}	
		else{  // if the bit is low
			// write 0
			Set_Pin_Output(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);
			HAL_GPIO_WritePin (DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin, 0);  // pull the pin LOW
			DELAY_US(50);  // wait for 50 us
			Set_Pin_Input(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);
		}
	}
}
uint8_t DS18B20_BOILER_Read(void)
{
	uint8_t value=0;
	Set_Pin_Input(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);
	for (int i=0;i<8;i++)
	{
		Set_Pin_Output(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);   // set as output
		HAL_GPIO_WritePin (DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin, 0);  // pull the data pin LOW
		DELAY_US(1);  // wait for > 1us
		Set_Pin_Input(DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin);  // set as input
		if (HAL_GPIO_ReadPin (DS18B20_BOILER_GPIO_Port, DS18B20_BOILER_Pin)){  // if the pin is HIGH
			value |= 1<<i;  // read = 1
		}
		DELAY_US(50);  // wait for 50 us
	}
	return value;
}



/*FUNTION TEMPORATURE OF BOILER*/
float TEMPORATURE_OF_BOILER(){
	Presence = DS18B20_BOILER_Start();
		if(Presence == 2) {
		STOP();
		lcd_clear();
		HAL_Delay(50);
		lcd_put_cur(0,5);
		lcd_send_string("ERROR 1");
		lcd_put_cur(1,0);
		lcd_send_string("PRESS R TO START");
	}
		else {
		HAL_Delay (1);
	  DS18B20_BOILER_Write (0xCC);  // skip ROM
	  DS18B20_BOILER_Write (0x44);  // convert t
	  HAL_Delay (800);
	  Presence = DS18B20_BOILER_Start ();
      HAL_Delay(1);
      DS18B20_BOILER_Write (0xCC);  // skip ROM
      DS18B20_BOILER_Write (0xBE);  // Read Scratch-pad
      Temp_byte1 = DS18B20_BOILER_Read();
	  Temp_byte2 = DS18B20_BOILER_Read();
	  TEMP = (Temp_byte2<<8)|Temp_byte1;
		}
		return  (float)TEMP/16;
}

/*FUNTION OPEN VALVE*/
void OPEN_VALVE(){
			lcd_clear();
			lcd_put_cur(0,1);
			lcd_send_string("FILLING BOILER");
		HAL_GPIO_WritePin(VALVE_INLET_GPIO_Port,VALVE_INLET_Pin,RESET);
}
/*FUNTION CLOSE VALVE*/
void CLOSE_VALVE(){
	HAL_GPIO_WritePin(VALVE_INLET_GPIO_Port,VALVE_INLET_Pin,SET);

}
/*FUNTION OPEN PUMP RINSE*/
void OPEN_PUMP_RINSE(){
		lcd_clear();
		lcd_put_cur(0,2);
		lcd_send_string("DRAIN PUMP");
		HAL_GPIO_WritePin(PUMP_RINSE_GPIO_Port,PUMP_RINSE_Pin,RESET);
	}
/*FUNTION CLOSE PUMP RINSE*/
void CLOSE_PUMP_RINSE(){
	HAL_GPIO_WritePin(PUMP_RINSE_GPIO_Port,PUMP_RINSE_Pin,SET);
	BOILER_FULL =0;
}
/*FUNTION OPEN PUMP WASH*/
void OPEN_PUMP_WASH(){
	HAL_GPIO_WritePin(PUMP_WASH_GPIO_Port,PUMP_WASH_Pin,RESET);
}
/*FUNTION CLOSE PUMP WASH*/
void CLOSE_PUMP_WASH(){
	HAL_GPIO_WritePin(PUMP_WASH_GPIO_Port,PUMP_WASH_Pin,SET);
}
/*FUNTION OPEN BOILER HEATER*/
void OPEN_BOILER_HEATER(){
	lcd_clear();
	lcd_put_cur(0,1);
	lcd_send_string("HEATING BOILER");
	HAL_GPIO_WritePin(BOILER_HEATER_GPIO_Port,BOILER_HEATER_Pin,RESET);
}
/*FUNTION CLOSE BOILER HEATER*/
void CLOSE_BOILER_HEATER(){
	HAL_GPIO_WritePin(BOILER_HEATER_GPIO_Port,BOILER_HEATER_Pin,SET);
}
/*FUNTION OPEN TANK HEATER*/
void OPEN_TANK_HEATER(){
	lcd_clear();
	lcd_put_cur(0,2);
	lcd_send_string("HEATING TANK");
	HAL_GPIO_WritePin(TANK_HEATER_GPIO_Port,TANK_HEATER_Pin,RESET);
}
/*FUNTION CLOSE TANK HEATER*/
void CLOSE_TANK_HEATER(){
	HAL_GPIO_WritePin(TANK_HEATER_GPIO_Port,TANK_HEATER_Pin,SET);
}
/*FUNTION STOP EVERY THING*/
void STOP(){
	CLOSE_BOILER_HEATER();
	CLOSE_PUMP_RINSE();
	CLOSE_PUMP_WASH();
	CLOSE_TANK_HEATER();
	CLOSE_VALVE();
}
/*FUNTION DISPLAY_TEMPORATURE OF BOILER*/
void DISPLAY_TEMPORATURE_BOILER(float Temp){
	char str[20] = {0};
	lcd_put_cur(1,0);
	sprintf (str, "B:%.1f", Temp);
	lcd_send_string(str);
	lcd_send_data('C');
}
/*FUNTION DISPLAY_TEMPORATURE OF TANK*/
void DISPLAY_TEMPORATURE_TANK(float Temp){
	char str[20] = {0};
	lcd_put_cur(1,9);
	sprintf (str, "T:%.1f", Temp);
	lcd_send_string(str);
	lcd_send_data('C');
}
/*PRESS POWER BUTTON*/
void CHECK_PRESS_POWER_BUTTON(){
	if(power_press ==0){
	lcd_put_cur(0, 0);
  lcd_send_string("PRESS P TO START");
	while(power_press ==0 ){
		UPDATE_TEMPERATURE();
		if(HAL_GPIO_ReadPin(POWER_GPIO_Port,POWER_Pin)==0 && power_press ==0){
			power_press=1;
		}
	}
		}
	}
/*UPDATE TEMPERATURE OF BOILER AND TANK*/
void UPDATE_TEMPERATURE(){
			float a,b;
			a =TEMPORATURE_OF_BOILER();
			b =TEMPORATURE_OF_BOILER();
			if(a<150 && b <150){
			Temperature_boiler = a;
			Temperature_tank = b;
			DISPLAY_TEMPORATURE_BOILER(Temperature_boiler);
			DISPLAY_TEMPORATURE_TANK(Temperature_tank);
}
}
/*FUNTION INSPECT TEMPORATURE OF BOILER*/
void INSPECT_TEMPORATURE_OF_BOILER(uint8_t Tem_std){
				if(Temperature_boiler < Tem_std ){
				OPEN_BOILER_HEATER();
			while(Temperature_boiler < Tem_std){		
			UPDATE_TEMPERATURE();
			}
		}
				CLOSE_BOILER_HEATER();
				READY();
}
/*FUNTION INSPECT WATER LEVEL OF BOILER*/
void INSPECT_WATER_LEVEL_OF_BOILER(){
	if(HAL_GPIO_ReadPin(WATER_LEVEL_BOILER_GPIO_Port,WATER_LEVEL_BOILER_Pin)==1){									
	OPEN_VALVE();
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	while(BOILER_FULL ==0){								
	UPDATE_TEMPERATURE();
	}
	}
}
/*FUNTION CHECK WATER LEVEL BOILER AND TANK AND DOOR*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == WATER_LEVEL_BOILER_Pin){
		if(HAL_GPIO_ReadPin(WATER_LEVEL_BOILER_GPIO_Port,WATER_LEVEL_BOILER_Pin)==0){
			DELAY_US(200);
		if(HAL_GPIO_ReadPin(WATER_LEVEL_BOILER_GPIO_Port,WATER_LEVEL_BOILER_Pin)==0 && BOILER_FULL==0  ){
		CLOSE_VALVE();
		BOILER_FULL =1;
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	}
}
}
	else if(GPIO_Pin == RESET_Pin){
		if(HAL_GPIO_ReadPin(RESET_GPIO_Port, RESET_Pin)==0){
		HAL_Delay(500);
		if(HAL_GPIO_ReadPin(RESET_GPIO_Port, RESET_Pin)==0){
		resetSTM32();
		}
	}
		else{}
	}
	else if(GPIO_Pin == MODE_Pin){
				if(HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin)==0){
				DELAY_US(200);
				if(HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin)==0 && MODE ==0){
					a++;
					MODE =1;
					if(a%2==0 && a >0){
			lcd_clear();
			lcd_put_cur(0,4);
			lcd_send_string("MODE HIGH");
					}
					else if(a%2!=0 && a >0){
			lcd_clear();
			lcd_put_cur(0,4);
			lcd_send_string("MODE STD");
					}
				}
				else{
					MODE =0;
				}
				}
}
}
/*FUNTION BUZZER*/
void SET_BUZZER(){
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, RESET);
}

/*FUNTION OF MODE WASH STANDARD*/
void WASH_MODE_STD(){
	lcd_clear();
	lcd_put_cur(0,5);
	lcd_send_string("WASHING");
	SET_BUZZER();
	OPEN_PUMP_WASH();
	OPEN_PUMP_MECHICAL_WASH();
	DELAY_COUNT(3);
	CLOSE_PUMP_MECHICAL_WASH();
	DELAY_COUNT(45);
	CLOSE_PUMP_WASH();
	lcd_clear();
	lcd_put_cur(0,3);
	lcd_send_string("DELAY 5S");
	DELAY_COUNT(6);
	INSPECT_WATER_LEVEL_OF_BOILER();
	INSPECT_TEMPORATURE_OF_BOILER(Temp_Std_Rinse);
	lcd_clear();
	lcd_put_cur(0,5);
	lcd_send_string("RINSING");
	HAL_Delay(100);
	OPEN_PUMP_WASH();
	OPEN_PUMP_MECHICAL_RINSE();
	DELAY_COUNT(3);
	CLOSE_PUMP_MECHICAL_RINSE();
	DELAY_COUNT(10);
	CLOSE_PUMP_WASH();
	lcd_clear();
	HAL_Delay(1000);
	lcd_put_cur(0,3);
	lcd_send_string("COMPLETE");
	HAL_Delay(2000);
	MODE =0;
}
/*FUNTION OF MODE WASH HIGHTEMP*/
void WASH_MODE_HIGH(){
	INSPECT_TEMPORATURE_OF_BOILER(Temp_Std_Wash +5);
	lcd_clear();
	lcd_put_cur(0,5);
	lcd_send_string("WASHING");
	SET_BUZZER();
	OPEN_PUMP_WASH();
	OPEN_PUMP_MECHICAL_WASH();
	DELAY_COUNT(3);
	CLOSE_PUMP_MECHICAL_WASH();
	DELAY_COUNT(120);
	CLOSE_PUMP_WASH();
	lcd_clear();
	lcd_put_cur(0,3);
	lcd_send_string("DELAY 10S");
	DELAY_COUNT(10);
	INSPECT_WATER_LEVEL_OF_BOILER();
	INSPECT_TEMPORATURE_OF_BOILER(Temp_Std_Rinse +10);
	lcd_clear();
	lcd_put_cur(0,5);
	lcd_send_string("RINSING");
	HAL_Delay(100);
	OPEN_PUMP_WASH();
	OPEN_PUMP_MECHICAL_RINSE();
	DELAY_COUNT(3);
	CLOSE_PUMP_MECHICAL_RINSE();
	DELAY_COUNT(50);
	CLOSE_PUMP_WASH();
	lcd_clear();
	HAL_Delay(100);
	lcd_put_cur(0,3);
	lcd_send_string("COMPLETE");
	HAL_Delay(2000);
	MODE =0;
}
/*FUNTION RESET*/
void resetSTM32(){
    NVIC_SystemReset();  
}
/*FUNTION SELECT MODE*/
void SELECT_MODE(uint8_t Tem_std_Boiler,uint8_t Tem_std_Tank){
if(MODE ==0){
				HAL_NVIC_EnableIRQ(EXTI4_IRQn);
				while(HAL_GPIO_ReadPin(CONFIRM_GPIO_Port,CONFIRM_Pin)==1){
				UPDATE_TEMPERATURE();
				if(Temperature_boiler < (Tem_std_Boiler-5)){
					OPEN_BOILER_HEATER();
					while(Temperature_boiler < Tem_std_Boiler){
						UPDATE_TEMPERATURE();
					}
					CLOSE_BOILER_HEATER();
					READY();
				}
				}
				HAL_NVIC_DisableIRQ(EXTI4_IRQn);
				if(a%2!=0 && a >0){
					WASH_MODE_STD();
					power_press =0;
				}
				else if(a%2==0 && a >0){
					WASH_MODE_HIGH();
					power_press =0;
				}
				else{
					WASH_MODE_STD();
					power_press =0;
				}
			}
		}

/*FUNTION INSPECT DOOR CLOSE TO WASH*/
void CLOSE_DOOR_TO_WASH(uint8_t Tem_std_Boiler,uint8_t Tem_std_Tank){
	if(HAL_GPIO_ReadPin(DOOR_GPIO_Port,DOOR_Pin)==1){
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(DOOR_GPIO_Port,DOOR_Pin)==1){
	lcd_clear();
	HAL_Delay(50);
	while(HAL_GPIO_ReadPin(DOOR_GPIO_Port,DOOR_Pin)==1){
	lcd_put_cur(0,0);
	lcd_send_string("CLOSE DOOR-WASH");
		UPDATE_TEMPERATURE();
		if(Temperature_boiler < (Tem_std_Boiler -5)){
				OPEN_BOILER_HEATER();
			while(Temperature_boiler < Tem_std_Boiler){		
				UPDATE_TEMPERATURE();
			}
			CLOSE_BOILER_HEATER();
	}
}
	}
}
	READY();
HAL_Delay(100);
	}
	/*FUNTION TIMER INTERRUPT*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim->Instance == htim3.Instance)
 {
	if(HAL_GPIO_ReadPin(WATER_LEVEL_TANK_GPIO_Port,WATER_LEVEL_TANK_Pin)==1){
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(WATER_LEVEL_TANK_GPIO_Port,WATER_LEVEL_TANK_Pin)==1){
		STOP();
		HAL_Delay(20);
		lcd_clear();
		HAL_Delay(50);
		lcd_put_cur(0,5);
		lcd_send_string("ERROR 3");
		while(1){
		lcd_put_cur(1,0);
		lcd_send_string("PRESS R TO START");
		}
	}
	}
	if(Temperature_boiler >Temperature_max || Temperature_tank >Temperature_max){
		STOP();
		HAL_Delay(20);
		lcd_clear();
		HAL_Delay(50);
		lcd_put_cur(0,5);
		lcd_send_string("ERROR 2");
		while(1){
		lcd_put_cur(1,0);
		lcd_send_string("PRESS R TO START");
		}
	}
	if(HAL_GPIO_ReadPin(PUMP_WASH_GPIO_Port,PUMP_WASH_Pin)==0 && HAL_GPIO_ReadPin(DOOR_GPIO_Port,DOOR_Pin)==1){
		HAL_Delay(50);
			if(HAL_GPIO_ReadPin(PUMP_WASH_GPIO_Port,PUMP_WASH_Pin)==0 && HAL_GPIO_ReadPin(DOOR_GPIO_Port,DOOR_Pin)==1){
			resetSTM32();
			power_press =0;
	}
}
	if(HAL_GPIO_ReadPin(PUMP_WASH_GPIO_Port,PUMP_WASH_Pin)==0 || HAL_GPIO_ReadPin(PUMP_RINSE_GPIO_Port,PUMP_RINSE_Pin)==0){
			count++;
	}
	else{
		count=0;
	}
	if(HAL_GPIO_ReadPin(INFOR_GPIO_Port,INFOR_Pin)==0){
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(INFOR_GPIO_Port,INFOR_Pin)==0){
			HAL_GPIO_WritePin(PUMP_RINSE_GPIO_Port,PUMP_RINSE_Pin,RESET);
		}
 }
	else{
			CLOSE_PUMP_RINSE();
			}
	HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	}
}
/*FUNTION OPEN PUMP MECHICAL WASH*/
void OPEN_PUMP_MECHICAL_WASH(){
	HAL_GPIO_WritePin(MECHICAL_WASH_GPIO_Port,MECHICAL_WASH_Pin,SET);
}
/*FUNTION OPEN PUMP MECHICAL RINSE*/
void OPEN_PUMP_MECHICAL_RINSE(){
	HAL_GPIO_WritePin(MECHICAL_RINSE_GPIO_Port,MECHICAL_RINSE_Pin,SET);
}
/*FUNTION CLOSE PUMP MECHICAL WASH*/
void CLOSE_PUMP_MECHICAL_WASH(){
	HAL_GPIO_WritePin(MECHICAL_WASH_GPIO_Port,MECHICAL_WASH_Pin,RESET);
}
/*FUNTION OPEN PUMP MECHICAL RINSE*/
void CLOSE_PUMP_MECHICAL_RINSE(){
	HAL_GPIO_WritePin(MECHICAL_RINSE_GPIO_Port,MECHICAL_RINSE_Pin,RESET);
}