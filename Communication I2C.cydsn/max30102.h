
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

paramètre:   uint8_t reg_adress, l'adresse du registr ou l'on veut dire
               
    Cette fonction sert a lire un byte de donné dans une adresse précise du capteur grâce a la procédure I2C

*/
uint8_t ReadMAX30102(uint8_t reg_adress);


/*
    ReadBurstMax30102

paramètres :   uint8_t reg_adress, Adresse du registre où  l'on souhaite lire
               uint8_t *data, Pointeur ou l'on veut garder les bytes que l'on a lu
               uint8_t lenght, nombre de bytes que l'on veut lire

    Cette fonction sert a lire plusieur byte d'un registre précis selon la procedure expliquée en page 40 de la 
datasheet du MAX30102


*/
uint8_t ReadBurstMAX30102 ( uint8_t reg_adress, uint8_t *data, uint8_t lenght);


/*
    WriteMax30102
 
 parametres:   uint8_t reg_adress, Adresse du registre où l'on souhaite écrire
               uint8_t data, Data que l'on veut écrire dans le registre
    
Cette fonction sert à écrire un seul byte de data dans un registre spécifique du capteur grâce a l'I2C

*/
uint8_t WriteMAX30102(uint8_t reg_adress,uint8_t data);


/*
    GetValue_FIFO
 parametres:   int *RedLed, Pointeur où sera stocker les valeurs de l'échantillon provenant de la LED rouge
               int * LedIr, Pointeur où sera stocker les valeurs de l'échantillon provenant de la LED infrarouge

    Cette fonction sert a obtenir un échantillon provenant du capteur MAX à partir du registre FIFO_DATA,
    où ils y sont stocker
*/
void GetValue_FIFO( uint32_t *RedLed, uint32_t *LedIr);


/*
    GetCurrentRED
 parametre:    aucun 

    Cette fonction nous rend le courant actuellement utilisé dans la LED Rouge
*/
uint8_t GetCurrentRED(void);

/*
    GetCurrentIr
 parametre:    aucun 


    Cette fonction nous rend le courant actuellement utilisé dans la LED Infrarouge
*/
uint8_t GetCurrentIr(void);


/*
    ChangeCurrentLEDRed
 parametre:    uint8_t CurrentWanted, 

    Cette fonction sert à changer le courant dans la LED Rouge du Max30102 pour la valeur CurrentWanted
*/
void ChangeCurrentLEDRed ( uint8_t CurrentWanted );


/*
    ChangeCurrent
 parametre:    uint8_t CurrentWanted, 

     Cette fonction sert à changer le courant dans la LED infrarouge du Max30102 pour la valeur CurrentWanted
*/
void ChangeCurrentLEDIr ( uint8_t CurrentWanted );
    

/*
    Max30102Init
 parametre:   aucun

Cette fonction sert a initialiser le MAX30102 avec les parametres et configuration souhaitée
*/
void Max30102Init(void);


/*
    TaskReadingSample
 parametre:   aucun

Cette tache sert à lire les valeurs obtenu des channel Rouge et Infrarouge par le MAX30102 et les stock dans un
buffer pour un futur filtrage.
  
*/
void TaskReadingSample(void*arg);


/*
    resetMax
 parametre:   aucun

  Cette fonction sert a reinitialiser le MAX30102
*/
void resetMax(void);
/* [] END OF FILE */


