
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bmi160.h"
#include "max30102.h"
#include "Declaration.h"

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




void GetValue_FIFO ( uint32_t *RedLed, uint32_t *LedIr){ 


uint8_t stockage[6];
ReadBurstMAX30102( REG_FIFO_DATA,stockage,6);
*RedLed = ((((0b00000011&stockage[0])<<8)+stockage[1])<<8)+stockage[2];

*LedIr = (((0b00000011&stockage[3]<<8)+stockage[4])<<8)+stockage[5];

};


uint8_t GetCurrent(void){
    
    
    uint8_t CurrentValue = ReadMAX30102(REG_LED1_PA); // since both LED have the same current, reading one is enough
    return CurrentValue;
}

void ChangeCurrent ( uint8_t CurrentWanted ){

WriteMAX30102 (REG_LED1_PA,CurrentWanted);
WriteMAX30102 (REG_LED2_PA,CurrentWanted);

}

void Max30102Init(void){
    
    // initialising the MAX30102
    // Interrupt setting
    
    WriteMAX30102(REG_INTR_ENABLE_1,0b01000000);
    
    WriteMAX30102(REG_INTR_ENABLE_2,0b00000000);
   
    
    // FIFO_WR_PTR
    WriteMAX30102(REG_FIFO_WR_PTR,0b00000000);
  
    // OVF_COUNTER[4;0]
    WriteMAX30102(REG_OVF_COUNTER,0b00000000);
   
    
    // FIFO_RD_PTR[4;0]
    WriteMAX30102(REG_FIFO_RD_PTR,0b00000000);
    ;
    
    // Fifo Config : Sample AVG =0, FIFO rollover = false, fifo almsot full = 18
    WriteMAX30102(REG_FIFO_CONFIG,0b00001110);
    
    
    // Mode config :0b010 for Red Only, 0b011 for SpO2 mode 0b111 multimode LED
    WriteMAX30102(REG_MODE_CONFIG, 0b00000011);
    
    
    //SpO2 config 
    WriteMAX30102(REG_SPO2_CONFIG,0b01100011);
    
    
    // LED1&2 at 7.2 mA 
    WriteMAX30102(REG_LED1_PA,0x24);
    WriteMAX30102(REG_LED2_PA,0x24);
   
}

void TaskReadingSample(void*arg){
    
    (void)arg;
    I2C_2_Start();
    Max30102Init();
    
    int indexBuffer = 0;
    
    uint32_t IR_stockage,Red_stockage; 
    uint32_t *ptrRED=&Red_stockage,
             *ptrIR=&IR_stockage;
    
    
     uint32_t IR_Buffer[100],// 100 = 2 sec of data at 50 SPS ,static type because those buffer will be use for the signal treatment.
                    Red_Buffer[100];
    for(;;){
        GetValue_FIFO(ptrRED,ptrIR);
        IR_stockage = *ptrIR;
        Red_stockage = *ptrRED;
        
        dataSpo2[RED][RAW][indexBuffer] = *ptrIR;
        dataSpo2[IR][RAW][indexBuffer] = *ptrRED;
        
        indexBuffer++;
        /*
        Red_Buffer[indexBuffer] = Red_stockage;
        IR_Buffer[indexBuffer] = IR_stockage;
        
        */
        
        if( indexBuffer == BUFFER_LENGTH){
            indexBuffer = 0 ;
        }
        printf( "IR=%lu RED=%lu \r\n",(unsigned long)IR_stockage,(unsigned long) Red_stockage);
       vTaskDelay(pdMS_TO_TICKS(1)); 
    }
}
    

void resetMax(void){
 WriteMAX30102(REG_MODE_CONFIG,0b01000000);
UART_1_PutString("reset done \r\n");
}







