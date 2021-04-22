
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "bmi160.h"
#include "Accelerometre.h"
#include "Declaration.h"
static  struct bmi160_dev bmi160Dev; // interface point to the bmi160


/* BMI 160Write écris une seul valeur*/

 void BMI160Write ( uint8_t dev_addr, uint8_t reg_addr, uint8_t data) 
{
    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context); // sent the register we want to write in
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,data,0,&I2C_1_context); // writing  the byte
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context); // stop
    // procedure comme celle Data sheet du BMI page 94
   
}
/* BMI 160Write écris une seul valeur*/
 void BMI160BurstWrite ( uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t lenght) // check autre chose que burst pour ecrire une par une
{
    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // start sent by master
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context); // sent the register we want to write in
    for ( int i = 0 ; i<lenght; i++){
        
        Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,data[i],0,&I2C_1_context); // writing  the byte
        }
    
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context); // stop
    // procedure comme celle Data sheet du BMI page 94
   


}
/* Lis une seul valeur*/
 void BMI160Read (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data)
{

    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // START
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context);// Écris le registre ou l'on veut lire
    Cy_SCB_I2C_MasterSendReStart(I2C_1_HW,dev_addr,CY_SCB_I2C_READ_XFER,0,&I2C_1_context);//Envoyer I2C restart 
    Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_NAK,data,0,&I2C_1_context); //Lire et envoyer NAK (non aknowledge) pour qu'il qu'il a fini de lire 
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context);//I2C stop
    
}
/* Lis plusieur valeurs consécutives car le registre s'incrémente automatiquement de 1*/
 void BMI160BurstRead (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t lenght) // Burst read possible car le registre est automatiquement incrémenter de 1
{

    Cy_SCB_I2C_MasterSendStart(I2C_1_HW,dev_addr,CY_SCB_I2C_WRITE_XFER,0,&I2C_1_context); // start
    Cy_SCB_I2C_MasterWriteByte(I2C_1_HW,reg_addr,0,&I2C_1_context);// Écris l'adresse du registre ou l'on veut écrire 
    Cy_SCB_I2C_MasterSendReStart(I2C_1_HW,dev_addr,CY_SCB_I2C_READ_XFER,0,&I2C_1_context);//Envoyer I2C restart
    for ( int i = 0; i<lenght-1; i++ ) // I2C lis tant qu'il y a un ACK envoyé 
    {
        Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_ACK,&data[i],0,&I2C_1_context); 
    }
    Cy_SCB_I2C_MasterReadByte(I2C_1_HW,CY_SCB_I2C_NAK,&data[lenght-1],0,&I2C_1_context); // derniere lecture et envoie un NAK pour dire que c'est la derniere valeur à lire    
    Cy_SCB_I2C_MasterSendStop(I2C_1_HW,0,&I2C_1_context);//I2C stop
    // procedure comme celle du DataSheet du BMI 160 page 95
}

 void bmi160Init (void)
{
    vTaskDelay(100);
    
 /* BMI160*/
    
bmi160Dev.read = (bmi160_com_fptr_t) BMI160BurstRead; // assigner fonction de lecture au BMI
bmi160Dev.write = (bmi160_com_fptr_t)BMI160BurstWrite; // assigner fonction d'écriture au BMI
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
    
     /*Set any motion interrupt*/
    
    uint8_t rslt = 0;
    struct bmi160_int_settg int_config;
    

    int_config.int_channel = BMI160_INT_CHANNEL_1; // na - Select the interrupt channel
    int_config.int_type = BMI160_ACC_ANY_MOTION_INT; // na - choosing Any Motion Interrupt

  // na - the following configuration is written to registers (0x53) INT_OUT_CTRL & (0x54) INT_LATCH  see datasheet pg71 section 2.11.20
  int_config.int_pin_settg.output_en = BMI160_ENABLE;             // na - Enabling interrupt pin as output -> register (0x53)
  int_config.int_pin_settg.output_mode = BMI160_DISABLE;          // na - Selecting push-pull mode for interrupt pin -> register (0x53)
  int_config.int_pin_settg.output_type = BMI160_ENABLE;           // na - Setting interrupt pin to active high -> register (0x53)
  int_config.int_pin_settg.edge_ctrl = BMI160_ENABLE;             // na - Enabling edge trigger -> register (0x53)
  int_config.int_pin_settg.input_en = BMI160_DISABLE;             // na - Disabling interrupt pin to act as input -> register (0x54)
  int_config.int_pin_settg.latch_dur = BMI160_LATCH_DUR_2_56_SEC; // na - non-latched output -> register (0x54)

  // na - Select the Any Motion Interrupt parameter
  int_config.int_type_cfg.acc_any_motion_int.anymotion_en = BMI160_ENABLE; // na - 1- Enable the any-motion, 0- disable any-motion
  int_config.int_type_cfg.acc_any_motion_int.anymotion_x = BMI160_ENABLE;  // na - Enabling x-axis for any motion interrupt - monitor x axis
  int_config.int_type_cfg.acc_any_motion_int.anymotion_y = BMI160_ENABLE;  // na - Enabling y-axis for any motion interrupt - monitor y axis
  int_config.int_type_cfg.acc_any_motion_int.anymotion_z = BMI160_ENABLE;  // na - Enabling z-axis for any motion interrupt - monitor z axis
  int_config.int_type_cfg.acc_any_motion_int.anymotion_dur = 0;            // na - any-motion duration. This is the consecutive datapoints -> see datasheet pg32 section 2.6.1 <int_anym_dur> and pg78
  int_config.int_type_cfg.acc_any_motion_int.anymotion_thr = 1;           // na - An interrupt will be generated if the absolute value of two consecutive accelarion signal exceeds the threshold value -> see datasheet pg32 section 2.6.1 <int_anym_th> and pg78 INT_MOTION[1]
                                                                           // na - (2-g range) -> (anymotion_thr) * 3.91 mg, (4-g range) -> (anymotion_thr) * 7.81 mg, (8-g range) ->(anymotion_thr) * 15.63 mg, (16-g range) -> (anymotion_thr) * 31.25 mg

  rslt = bmi160_set_int_config(&int_config, &bmi160Dev); // na - Set Any-motion interrupt
  printf("rslt: %d", rslt);

  if (rslt != BMI160_OK) { // Verifie si la configuration de l'ANYMOTION interrupt se fait correctement
    printf("BMI160 Any-motion interrupt configuration failure!\n");
  } else {
    printf("BMI160 Any-motion interrupt configuration done!\n");
  }

    
    

}
//32768 = 2g

#define MAXACCEL (32768/2)

void GetInterruptStatus (void*arg) 
{
    (void)arg;
    union bmi160_int_status int_status;
    uint8_t rslt;
    //initialisation
    I2C_1_Start();
    bmi160Init();
    
    for(;;){
        
      rslt = bmi160_get_int_status(BMI160_INT_STATUS_0,&int_status,&bmi160Dev);
       if ( int_status.bit.anym == 1){
        printf(" ANYMOTION interrupt \r\n");
         AlarmeBMI160 = true; // Signal de l'alarme on
        }
    else if ( int_status.bit.anym == 0){
        printf(" NO ANYMOTION interrupt \r\n");
         AlarmeBMI160 = false; // Signal de l'alarme off
        }
    
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
         
    void motionTask (void *arg) // tache de test pour s'assurer que la config a bien été faite et que les valeurs sont modifié en temps réel
{
    (void)arg;
    I2C_1_Start();
    bmi160Init();
    struct bmi160_sensor_data acceleration;
    
    float gx,gy,gz;
    
    for(;;)
    {

        bmi160_get_sensor_data(BMI160_ACCEL_ONLY, &acceleration, NULL, &bmi160Dev); // on veut uniquement l'acceleration
        
        gx = (float)acceleration.x/MAXACCEL; // display des valeurs pour le test
        gy = (float)acceleration.y/MAXACCEL;
        gz = (float)acceleration.z/MAXACCEL;
        printf( "x=%1.2f y=%1.2f z=%1.2f\r\n", gx, gy, gz);
        
        vTaskDelay(200);
    }
}
