
#include "project.h"

#define I2C_ADDRESS 0X57 // 0b1010111 in the data sheet ( page 29)
#define I2C_WRITE_ADDR 0xAE
#define I2C_READ_ADDR 0xAF

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

/*
    ReadMax30102

parameters :   uint8_t reg_adress, adress of the register we want to write in
               
    This function is used to read a single data, on a specific register of the I2C.

*/
uint8_t ReadMAX30102(uint8_t reg_adress);


/*
    ReadBurstMax30102

parameters :   uint8_t reg_adress, adress of the register we want to write in
               uint8_t *data, data we want to read
               uint8_t lenght, lenght of the data we want to read

    This function is used to read multiple data on a specific register of the I2C,
following the process explained on the MAX30102 DataSheet p30.

*/
uint8_t ReadBurstMAX30102 ( uint8_t reg_adress, uint8_t *data, uint8_t lenght);


/*
    WriteMax30102
 
 parameters:   uint8_t reg_adress, adress of the register we want to write in
               uint8_t data, data we want to write in

    This function is used to write a single data, on a specific register of the I2C.


*/
uint8_t WriteMAX30102(uint8_t reg_adress,uint8_t data);





/*
    GetValue_FIFO
 parameters:   int *RedLed, Red Led Sample value
               int * LedIr, InfraRed Led sample value

    This function is used to get the sample from the red and Ir channel, in the FIFO_DATA register.


*/
void GetValue_FIFO( uint32_t *RedLed, uint32_t *LedIr);


/*
    GetCurrent
 parameters:    uint16_t *CurrentWanted, 16 bytes representing

    This function is used to get the the current used on the Red and IR Led
*/
uint8_t GetCurrent(void);


/*
    ChangeCurrent
 parameters:    uint16_t CurrentWanted, 8 bites representing the current wanted since its the same for both the LED,
                                        only 8 bites are necessary

    This function is used to change the current from both the Red and IR led.
*/
void ChangeCurrent ( uint8_t CurrentWanted );
    

/*
    Max30102Init
 parameters:   none
This fonction is used to initialize the MAX30102 with the parameters and settings wanted.
*/
void Max30102Init(void);


/*
    TaskReadingSample
 parameters:   none
This fonction is used to read the last value and stock them in a buffer for other usage.
  
*/
void TaskReadingSample(void*arg);


/*
    resetMax
 parameters:   none
This fonction is used to reset the MAX3010
  
*/
void resetMax(void);
/* [] END OF FILE */


