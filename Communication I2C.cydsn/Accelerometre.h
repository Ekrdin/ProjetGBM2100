
#include "project.h"


/*
    BMI160Write
    
    parametres :  uint8_t dev_addr, adresse de l'I2C
               uint8_t reg_addr, adresse du registre ou l'on veut ecrire of the register we want to write in
               uint8_t data, data que l'on souhaite écrire
    
    Cette fonction sert a écrire un seul byte de data sur un registre specific de l'appareil
*/
void BMI160Write ( uint8_t dev_addr, uint8_t reg_addr, uint8_t data);




/*
    BMI160Read
    
    parametres :  uint8_t dev_addr, Adresse de l'I2C
                uint8_t reg_addr, Adress du registre ou l'on ecris 
                uint8_t *data, pointeur vers la ou l'on veut stocker la valeur à lire

    Cette fonction sert à lire un seul byte sur un registre spécifique
*/
 void BMI160Read (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data);


/*
    BMI160BurstRead
    
    parametres :  uint8_t dev_addr, Adresse de l'I2C
               uint8_t reg_addr,Adresse du registre ou l'on veut écrire
               uint8_t *data, data que l'on veut ecrire
               uint8_t lenght, nombre de bytes que l'on veut ecrire
    
Cette fonction est utiliser pour lire plusieur byte de data sur un registre specifique suivant la procedure expliquer
dans la page 97 de la datasheet du BMI160.


*/
 void BMI160BurstRead (uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t lenght);


/*
    bmi 160Init
    
    parametre :  Aucun
    
    Cette fonction sert a initialiser le capteur BMI, on initialise également l'interruption ANYMOTION
    permettant de detecter un mouvement
*/
  void bmi160Init (void);

/*
    motionTask
    
    parametre :  *arg, void pointeur

    This function is used get the acceleration data from the BMI160, and display it to be sure that
    its working properly.

    Cette tache sert a savoir si le capteur BMI160 est en mouvement à l'aide d'une interruption,
    un booléen est modifié en conséquence
*/
void GetInterruptStatus (void *arg) ;


/*
    motionTask
    
    parametre :  *arg, void pointeur

    Cette tache a servi de test pour s'assurer que l'on était bien capable de voir la modification des valeur 
    d'accélération en temps réel
*/
void motionTask (void *arg);