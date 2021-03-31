
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"
#include "max30102.h"


void ReadMAX30102(uint8_t reg_adress,uint8_t data)
{
    Cy_SCB_I2C_MasterSendStart(I2C_2_HW,I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,0,&I2C_2_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_2_HW,reg_adress,0,&I2C_1_context); // sent the register we want to write in
    Cy_SCB_I2C_MasterSendReStart(I2C_2_HW,I2C_ADDRESS,CY_SCB_I2C_READ_XFER,0,&I2C_2_context);
    
    Cy_SCB_I2C_MasterReadByte(I2C_2_HW,CY_SCB_I2C_NAK,&data,0,&I2C_1_context); // reading the data
    
    Cy_SCB_I2C_MasterSendStop(I2C_2_HW,0,&I2C_1_context); // stop
    
    
}
void ReadBurstMAX30102 ( uint8_t reg_adress, uint8_t *data, uint8_t lenght){

    Cy_SCB_I2C_MasterSendStart(I2C_2_HW,I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,0,&I2C_2_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_2_HW,reg_adress,0,&I2C_1_context); // sent the register we want to write in
    Cy_SCB_I2C_MasterSendReStart(I2C_2_HW,I2C_ADDRESS,CY_SCB_I2C_READ_XFER,0,&I2C_2_context);
    
    for ( int i; i<lenght-1;i++)
    {
        Cy_SCB_I2C_MasterReadByte(I2C_2_HW,CY_SCB_I2C_ACK,&data[i],0,&I2C_1_context); // reading the data
    }
    
    Cy_SCB_I2C_MasterReadByte(I2C_2_HW,CY_SCB_I2C_NAK,&data[lenght-1],0,&I2C_1_context);
    Cy_SCB_I2C_MasterSendStop(I2C_2_HW,0,&I2C_1_context); // stop

}  
    
void WriteMAX30102(uint8_t reg_adress,uint8_t data)
{
    Cy_SCB_I2C_MasterSendStart(I2C_2_HW,I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,0,&I2C_2_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_2_HW,reg_adress,0,&I2C_1_context); // sent the register we want to write in
    Cy_SCB_I2C_MasterWriteByte(I2C_2_HW,data,0,&I2C_2_context); // reading the data
    Cy_SCB_I2C_MasterSendStop(I2C_2_HW,0,&I2C_2_context); // stop

}

void WriteBurstMAX30102(uint8_t reg_adress,uint8_t *data, uint8_t lenght)
{
    Cy_SCB_I2C_MasterSendStart(I2C_2_HW,I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,0,&I2C_2_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_2_HW,reg_adress,0,&I2C_1_context); // sent the register we want to write in
     for ( int i; i<lenght-1;i++)
    {
        Cy_SCB_I2C_MasterWriteByte(I2C_2_HW,data[i],0,&I2C_2_context); // reading the data
    }
  
    Cy_SCB_I2C_MasterSendStop(I2C_2_HW,0,&I2C_2_context); // stop

}