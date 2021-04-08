
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"
#include "max30102.h"


uint8_t ReadMAX30102(uint8_t reg_adress)
{
    I2C_2_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); // start sent by master
    I2C_2_MasterWriteByte(reg_adress,100); // sent the register we want to write in
    I2C_2_MasterSendReStart(I2C_ADDRESS,CY_SCB_I2C_READ_XFER,100);
    uint8_t data = 0;
    I2C_2_MasterReadByte(CY_SCB_I2C_NAK,(uint8_t*)&data,100); // reading the data
    
    I2C_2_MasterSendStop(100); // stop
     
    return data;
    
}
uint8_t ReadBurstMAX30102 ( uint8_t reg_adress, uint8_t *data, uint8_t lenght){

    I2C_2_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); // start sent by master
    I2C_2_MasterWriteByte(reg_adress,100); // sent the register we want to write in
    I2C_2_MasterSendReStart(I2C_ADDRESS,CY_SCB_I2C_READ_XFER,100);
    
    for ( int i = 0; i<lenght-1;i++)
    {
        I2C_2_MasterReadByte(CY_SCB_I2C_ACK,(uint8_t*)&data[i],100); // reading the data
    }
    
    I2C_2_MasterReadByte(CY_SCB_I2C_NAK,(uint8_t*)&data[lenght-1],100);
    I2C_2_MasterSendStop(100); // stop
return 0;
}  
    
uint8_t WriteMAX30102(uint8_t reg_adress,uint8_t data)
{
    I2C_2_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); // start sent by master
    I2C_2_MasterWriteByte(reg_adress,100); // sent the register we want to write in
    I2C_2_MasterWriteByte(data,100); // reading the data
    I2C_2_MasterSendStop(100); // stop
return 0;
}

uint8_t WriteBurstMAX30102(uint8_t reg_adress,uint8_t *data, uint8_t lenght)
{
    I2C_2_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); // start sent by master
    I2C_2_MasterWriteByte(reg_adress,100); // sent the register we want to write in
     for ( int i = 0; i<lenght-1;i++)
    {
        I2C_2_MasterWriteByte(data[i],100); // reading the data
    }
  
    I2C_2_MasterSendStop(100); // stop
return 0;
}



void GetValue_LedIr_RedLed ( uint32_t *RedLed, uint32_t *LedIr){ 


uint8_t stockage[6];

ReadBurstMAX30102( REG_FIFO_DATA,stockage,6);

*RedLed = ((((0b00000011&stockage[0])<<8)+stockage[1])<<8)+stockage[2];

*LedIr = (((0b00000011&stockage[3]<<8)+stockage[4])<<8)+stockage[5];

};


uint16_t GetCurrent(void){
    
    uint8_t stockage[2];
    uint8_t CurrentValue = ReadBurstMAX30102(REG_MODE_CONFIG,stockage,2);
    return CurrentValue;
}

void ChangeCurrent ( uint16_t *CurrentWanted ){

    uint8_t upperByte = *CurrentWanted >> 8;
    uint8_t lowerByte = *CurrentWanted <<8 ;
            lowerByte = lowerByte >> 8;
    uint8_t buffer[2];
    
    buffer[0] = upperByte;
    buffer[1]= lowerByte;
    
WriteBurstMAX30102 (REG_LED1_PA,buffer,2);
WriteBurstMAX30102 (REG_LED2_PA,buffer,2);
}


void TaskReadingSample(void*arg){
    (void)arg;
    
    static uint32_t IR_stockage,Red_stockage; // static type because those value will be use for the signal treatment.
    uint32_t *ptrRED=&Red_stockage,
    *ptrIR=&IR_stockage;
    for(;;){
        
        GetValue_LedIr_RedLed(ptrRED,ptrIR);
        IR_stockage = *ptrIR;
        Red_stockage = *ptrRED;
        
       printf( "IR=%lu RED=%lu \r\n",(unsigned long)IR_stockage,(unsigned long) Red_stockage);
       
    }
}

void Max30102Init(void){
    
    // initialising the MAX30102



}

