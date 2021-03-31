
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"
#include "Accelerometre.h"

static  struct bmi160_dev bmi160Dev; // interface point to the bmi160

/* BMI 160Write écris une seul valeur*/
 void BMI160Write ( uint8_t dev_addr, uint8_t reg_addr, uint8_t data) // check autre chose que burst pour ecrire une par une
{
    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context); // sent the register we want to write in
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,data,0,&I2C_1_context); // writing  the byte
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context); // stop
    // procedure comme celle Data sheet du BMI page 94
   


}
/* Ecris plusieur valeur consécutives, car l'adresse s'incremente automatiquement de 1*/
 void BMI160BurstWrite ( uint8_t dev_addr, uint8_t reg_addr, uint8_t *data,uint16_t lenght) // check autre chose que burst pour ecrire une par une
{
    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context); // sent the register we want to write in
    for(int i =0;i<lenght;i++)
    {
        Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,data[i],0,&I2C_1_context); // writing  the byte 
    }
    
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context); // stop
    // procedure comme celle Data sheet du BMI page 94
   

}
/* Lis une seul valeur*/
 void BMI160Read (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data)
{

    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // start
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context);// write the adress where we want to read
    Cy_SCB_I2C_MasterSendReStart(I2C_1_HW,dev_addr,CY_SCB_I2C_READ_XFER,0,&I2C_1_context);//send I2C restart 
    Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_NAK,data,0,&I2C_1_context); //read with NAK saying its done reading
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context);//I2C stop
    
}
/* Lis plusieur valeurs consécutives car le registre s'incrémente automatiquement de 1*/
 void BMI160BurstRead (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t lenght) // Burst read possible car le registre est automatiquement incrémenter de 1
{

    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // start
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context);// write the adress it want to read
    Cy_SCB_I2C_MasterSendReStart(I2C_1_HW,dev_addr,CY_SCB_I2C_READ_XFER,0,&I2C_1_context);//send I2C restart
    for ( int i = 0; i<lenght-1; i++ ) // I2C read within ACK until its done reading
    {
        Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_ACK,&data[i],0,&I2C_1_context); 
    }
    Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_NAK,&data[lenght-1],0,&I2C_1_context); // final read with NAK saying it's done reading
    
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context);//I2C stop
    // procedure comme celle du DataSheet du BMI 160 page 95
}

 void bmi160Init (void)
{
    vTaskDelay(100);
    
 /* BMI160*/
    
bmi160Dev.read = (bmi160_read_fptr_t) BMI160BurstRead; 
bmi160Dev.write = (bmi160_write_fptr_t)BMI160BurstWrite; 
bmi160Dev.delay_ms = (bmi160_delay_fptr_t)vTaskDelay;
bmi160Dev.id = BMI160_I2C_ADDR; //I2C adress

bmi160_init(&bmi160Dev); // initialize the device

bmi160Dev.gyro_cfg.odr = BMI160_GYRO_ODR_800HZ;
bmi160Dev.gyro_cfg.range = BMI160_GYRO_RANGE_125_DPS;
bmi160Dev.gyro_cfg.bw = BMI160_GYRO_BW_OSR4_MODE;

    /* Select the power mode of Gyroscope sensor */

bmi160Dev.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;
bmi160Dev.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
bmi160Dev.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
bmi160Dev.accel_cfg.bw = BMI160_ACCEL_BW_OSR4_AVG1;
bmi160Dev.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    /* Set the sensor configuration */

    bmi160_set_sens_conf(&bmi160Dev);
    bmi160Dev.delay_ms(50);

}
//32768 = 2g

#define MAXACCEL (32768/2)

void motionTask (void *arg) 
{
    (void)arg;
    I2C_1_Start();
    bmi160Init();
    struct bmi160_sensor_data acceleration;
    
    float gx,gy,gz;
    
    for(;;)
    {

        bmi160_get_sensor_data(BMI160_ACCEL_ONLY, &acceleration, NULL, &bmi160Dev); // on veut uniquement l'acceleration
        
        gx = (float)acceleration.x/MAXACCEL; // display des valeurs pour vérifier la lecture et l'ecriture.
        gy = (float)acceleration.y/MAXACCEL;
        gz = (float)acceleration.z/MAXACCEL;
        printf( "x=%1.2f y=%1.2f z=%1.2f\r\n", gx, gy, gz);
        
        vTaskDelay(200);
    }

}
