
#include "project.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define I2C_ADDRESS 0X57 // 0b1010111 in the data sheet ( page 29)
#define I2C_WRITE_ADDR 0x57
#define I2C_READ_ADDR 0x57

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
               uint8_t data, data we want to read
    This function is used to read a single data, on a specific register of the I2C.

*/
void ReadMAX30102(uint8_t reg_adress,uint8_t data);


/*
    ReadBurstMax30102

parameters :   uint8_t reg_adress, adress of the register we want to write in
               uint8_t *data, data we want to read
               uint8_t lenght, lenght of the data we want to read

    This function is used to read multiple data on a specific register of the I2C,
following the process explained on the MAX30102 DataSheet p30.

*/
void ReadBurstMAX30102 ( uint8_t reg_adress, uint8_t *data, uint8_t lenght);


/*
    WriteMax30102
 
 parameters:   uint8_t reg_adress, adress of the register we want to write in
               uint8_t data, data we want to write in

    This function is used to write a single data, on a specific register of the I2C.


*/
void WriteMAX30102(uint8_t reg_adress,uint8_t data);


/*
    WriteBurstMax30102
 parameters:   uint8_t reg_adress, adress of the register we want to write in
               uint8_t data, data we want to write in
               uint8_t lenght, lenght of the data we want to write

    This function is used to write multiple data consecutively, on a specific register of the I2C. 
    The register increments automaticly after writing data except on the FIFO_DATA register


*/
void WriteBurstMAX30102(uint8_t reg_adress,uint8_t *data, uint8_t lenght);