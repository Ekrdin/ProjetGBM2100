
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bmi160.h"
#include "max30102.h"
#include "Declaration.h"

 bool entree = false;
uint8_t ReadMAX30102(uint8_t reg_adress)
{
    I2C_2_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); // I2C master envoie le départ
    I2C_2_MasterWriteByte(reg_adress,100); // on écris l'adress du registre dans lequel on veut écrire
    I2C_2_MasterSendReStart(I2C_ADDRESS,CY_SCB_I2C_READ_XFER,100); // restart necessaire selon la documentation du MAX30102
    uint8_t data = 0;
    I2C_2_MasterReadByte(CY_SCB_I2C_NAK,(uint8_t*)&data,100); // Lecture de la data
    
    I2C_2_MasterSendStop(100); // stop
     
    return data;
    
}
uint8_t ReadBurstMAX30102 ( uint8_t reg_adress, uint8_t *data, uint8_t lenght){

    I2C_2_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); // I2C master envoie le départ
    I2C_2_MasterWriteByte(reg_adress,100); // on écris l'adress du registre dans lequel on veut écrire
    I2C_2_MasterSendReStart(I2C_ADDRESS,CY_SCB_I2C_READ_XFER,100);
    
    for ( int i = 0; i<lenght-1;i++)
    {
        I2C_2_MasterReadByte(CY_SCB_I2C_ACK,(uint8_t*)&data[i],100); // Lecture des datas et stockage de ceux-ci dans le parametre data
    }
    
    I2C_2_MasterReadByte(CY_SCB_I2C_NAK,(uint8_t*)&data[lenght-1],100);
    I2C_2_MasterSendStop(100); // stop
return 0;
}  
    
uint8_t WriteMAX30102(uint8_t reg_adress,uint8_t data)
{
    I2C_2_MasterSendStart(I2C_ADDRESS,CY_SCB_I2C_WRITE_XFER,100); // Start envoyer par l'I2C maitre
    I2C_2_MasterWriteByte(reg_adress,100); // on écris l'adress du registre dans lequel on veut écrire
    I2C_2_MasterWriteByte(data,100); // on écris la data
    I2C_2_MasterSendStop(100); // stop
return 0;
}




void GetValue_FIFO ( uint32_t *RedLed, uint32_t *LedIr){ 
/* 
    La lecture dans le FIFO_DATA n'incremente pas l'adresse du registre, on peut dont readburst dans
    celui-ci pour obtenir tout les bytes souhaités en une lecture. Chacun des échantillons est codée en 18 bites,
    donc sur 3 bytes avec les 6 bytes du premier bytes mis a 0.
    
    */

uint8_t stockage[6];  // initialisation du vecteur de stockage
ReadBurstMAX30102( REG_FIFO_DATA,stockage,6);// lecture des 6 bytes contenant les echantillons du channel rouge et infrarouge

*RedLed = ((0b00000011&stockage[0])<<16)+(stockage[1]<<8)+stockage[2]; // obtentions des 3 premier bytes avec un masque sur le premier bytes

*LedIr = ((0b00000011&stockage[3])<<16)+(stockage[4]<<8)+stockage[5]; // obtentions des 3 derniers bytes avec un masque sur le 4ème bytes lu.

};


uint8_t GetCurrentRED(void){
    
    uint8_t CurrentValue = ReadMAX30102(REG_LED1_PA); // Lecture dans le registre de la valeur du courant dans la LED rouge
    return CurrentValue;
    
}uint8_t GetCurrentIR(void){
    
    
    uint8_t CurrentValue = ReadMAX30102(REG_LED2_PA); // Lecture dans le registre de la valeur du courant dans la LED infrarouge
    return CurrentValue;
}

void ChangeCurrentLEDRed ( uint8_t CurrentWanted ){
    
WriteMAX30102 (REG_LED1_PA,CurrentWanted); // Écriture dans le registre du courant de la led rouge de la nouvelle valeur
}

void ChangeCurrentLEDIr ( uint8_t CurrentWanted ){

WriteMAX30102 (REG_LED2_PA,CurrentWanted);// Écriture dans le registre du courant de la led rouge de la nouvelle valeur
}
void Max30102Init(void){
    
    // Initialisation du  MAX30102
    //Paramètre d'interruption
    
    // 0bXYZOOOOW -- 1 on - 0 off -- X  A_FULL - Y - PPG_RDY - O - Ne pas écrire dedans - W -  PWR_RDY
    WriteMAX30102(REG_INTR_ENABLE_1,0b01000000); 
    
    // FIFO_WR_PTR
    WriteMAX30102(REG_FIFO_WR_PTR,0b00000000); 
    
    // FIFO_RD_PTR[4;0]
    WriteMAX30102(REG_FIFO_RD_PTR,0b00000000);
    
    // Fifo Config 
    WriteMAX30102(REG_FIFO_CONFIG,0b00011110); 
    //0bXXXYZZZZ -- X SMP_AVE - Y FIFO_ROLLOVER_EN - Z FIFO_A_FULL 
    
    // Mode config 
    WriteMAX30102(REG_MODE_CONFIG, 0b00000011);
    // 0bXYOOOZZZ -- X SHDN - Y - RESET - Z mode
    
    
    //SpO2 config 
    WriteMAX30102(REG_SPO2_CONFIG,0b01101001); // 0bZYYXXXAA - Z(null) - Y (range control) - X (sample rate control) - A (ADC RESOLUTION)
    
    
    // LED1&2 at 19 mA 
    WriteMAX30102(REG_LED1_PA,0x5F);
    WriteMAX30102(REG_LED2_PA,0x5F);
   
}

void TaskReadingSample(void*arg){
    
    (void)arg;
    I2C_2_Start(); // initialisation du I2C
    Max30102Init(); // initialisation du MAX30102
    
    int indexBuffer = 0;
    
    uint32_t IR_stockage,Red_stockage; 
    uint32_t *ptrRED=&Red_stockage,
             *ptrIR=&IR_stockage;
   
    for(;;){
        if(!entree){
            GetValue_FIFO(ptrRED,ptrIR); // lecture des échantillons
          
            dataSpo2[RED][RAW][indexBuffer] = (float) *ptrRED; // stockage des echantillons rouges dans le buffer approprié
            dataSpo2[IR][RAW][indexBuffer] = (float) *ptrIR; // stockage des echantillons infrarouges dans le buffer approprié
            indexBuffer++;
            if( indexBuffer == BUFFER_LENGTH){ // Buffer est rempli
                indexBuffer = 0;// l'index d'ecriture est remis a 0 
                entree= true; //  le signal pour le traitement est enclanché
            } 
        }      
    vTaskDelay(pdMS_TO_TICKS(5));
    }
}
    

void resetMax(void){
    
 WriteMAX30102(REG_MODE_CONFIG,0b01000000);

}







