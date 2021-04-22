#include "project.h"

/*
    BMI160Write
    
    parameters :  uint8_t dev_addr, adress of the I2C
               uint8_t reg_addr, adress of the register we want to write in
               uint8_t data, data we want to write in
    This function is used to write a single data, on a specific register of the I2C.
*/
void BMI160Write ( uint8_t dev_addr, uint8_t reg_addr, uint8_t data);




/*
    BMI160Read
    
    parameters :  uint8_t dev_addr, adress of the I2C
               uint8_t reg_addr, adress of the register we want to write in
               uint8_t *data, data we want to write in
    This function is used to read a single data, on a specific register of the I2C.
*/
 void BMI160Read (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data);


/*
    BMI160BurstRead
    
    parameters :  uint8_t dev_addr, adress of the I2C
               uint8_t reg_addr, adress of the register we want to write in
               uint8_t *data, data we want to write in
               uint8_t lenght, lenght of the data we want to write
    This function is used to read multiple data on a specific register of the I2C,
following the process explained on the BMI160 DataSheet p97.

*/
 void BMI160BurstRead (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t lenght);


/*
    bmi 160Init
    
    parameters :  none
    This function is used to initialise the BMI160, 

    We give the specific function for reading and writing bytes to our BMI160, as well as all other parameters
    needed for the the accelerometer, the gyroscope and the sensor.

*/
  void bmi160Init (void);

/*
    motionTask
    
    parameters :  *arg, void pointer

    This function is used get the acceleration data from the BMI160, and display it to be sure that
    its working properly.
*/
void GetInterruptStatus (void *arg) ;


/*
    motionTask
    
    parameters :  *arg, void pointer

    This function is used get the acceleration data from the BMI160, and display it to be sure that
    its working properly.
*/
void motionTask (void *arg);