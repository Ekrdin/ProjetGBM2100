
#include "TraitementSignal.h"
#include "CAPSENS.h"
#include "DISPLAY.h"



uint32_t blockSize=BLOCK_SIZE; // nombre d'échantillons à traiter 


// Fonction de filtre Passe-bas afin d'avoir la composante DC du signal à l'aide de la librairie CMSIS 
void FiltreLowPass(float* buffer ,float* filteredSignal) {
    
    //uint32_t compteur; // c'était un compteur que j'avais mis si je voulais filtrer l'ensemble de l'échantillon 
    arm_fir_instance_f32 S; // pointe vers une instance de la structure du filtre 
    arm_fir_init_f32(&S,NUM_TAPS,(float32_t *) &firCoeffs32_LowPass[0],&firStateF32[0],blockSize);// initialise la structure du filtre  en prenant les positions 0 pour les coefficients du filtre passe-bas et pour l'état du filtre 
    arm_fir_f32(&S,buffer,filteredSignal,blockSize);// traite le filtre "finite impulse response" en pointant vers le buffer qui contient les données d'entrées ( valeurs brutes) et vers filtered signal qui contiendra les données de sorties  (valeurs filtrées) 
    
}
    
// Fonction de filtre Passe-haut afin d'avoir la composante AC du signal à l'aide de la librairie CMSIS (fait la même chose que le passe-bas mais avec différentes coefficients ( celui du passe-haut) )
void FiltreHighPass(float* buffer ,float* filteredSignal) {
    
    //uint32_t compteur;
    arm_fir_instance_f32 S;
    arm_fir_init_f32(&S,NUM_TAPS,(float32_t *) & firCoeffs32_HighPass[0],&firStateF32[0],blockSize);
    arm_fir_f32(&S,buffer,filteredSignal,blockSize);

}

// Fonction qui trouve les zéros afin de calculer la fréquence cardiaque 
void trouverZero(int* ZeroI, int* ZeroF, int* nZero){
    // Elle va trouver les zeros sur l'ensemble du signal 
    for ( int i= 0; i < BUFFER_LENGTH-1; i++){
     // Si la position du signal rouge AC est négative et la position qui suit est positive, elle passe par l'axe des abscisses et donc par 0 
    if ( dataSpo2[RED][AC][i] <0 && dataSpo2[RED][AC][i+1] > 0){
        
        (*nZero)++; // l'index du nombre de zero s'incrémente afin de passer par les zeros de tout le signal 
       
            if((*nZero) == 1){
               
                *ZeroI = i; // le premier zero trouvé pour le pulse qui passe par 0 prend la position du compteur 
            }
            if( (*nZero) > 1){
                
                *ZeroF = i; // le dernier zero trouvé pour le pulse qui passe par 0 prend la position du compteur
            }
            // Ainsi on trouve tous les zeros et on pourra appliquer cette fonction dans le calcul de la fréquence cardiaque 
        }
    }
}
// On calcule la fréquence cardiaque en appelant la fonction qui trouve les zeros . 
 uint8_t calculBPM(void){
    
    int ZeroIni = 0,
        ZeroFin = 0,
        nbrZero = 0;
    
        trouverZero(&ZeroIni,&ZeroFin,&nbrZero);// Une fois les zeros trouvés, il a vu tous les pulses ( equivalent a trouver les pics des pulses locaux du signal)
        
         BPM = round (60*(nbrZero-1)*FREQUENCE_ECH/(ZeroFin-ZeroIni));// Une fois les pulses trouvés, il peut calculer la fréquence cardiaque en multipliant par le nombre de periode du signal ( donc le nombre de zeros -1 ) et par 60 ( convertir en minute pour avoir un BPM)et par la fréquence d'échantillonage et en divisant par (la position du dernier zero-la position du premier zero ) 
       return BPM;// retourne la valeur la plus proche du BPM trouvé 
}

// On calcule le taux de saturation d'oxygène
float calculSPO2(void){
    
       double ratio = 0,
              RedAC=0,
              IrAC=0;
        float32_t MaxRedAC=0.0,
                  MinRedAC=0.0,
                  MaxIrAC=0.0,
                  MinIrAC=0.0,
                  moyenneRedDC=0.0,
                  moyenneIrDC=0.0;
    
        uint32_t indexMaxRedAC=0,
                 indexMaxIrAC=0,
                 indexMinRedAC=0,
                 indexMinIrAC=0;
                  

// Fonctions CMSIS utilisées afin de calculer les maximums et les minimums dans chacun du signal AC rouge et infrarouge 
    arm_max_f32(dataSpo2[RED][AC],BUFFER_LENGTH,&MaxRedAC,&indexMaxRedAC);
    arm_max_f32(dataSpo2[IR][AC],BUFFER_LENGTH,&MaxIrAC,&indexMaxIrAC);
    arm_min_f32(dataSpo2[RED][AC],BUFFER_LENGTH,&MinRedAC,&indexMinRedAC);
    arm_min_f32(dataSpo2[IR][AC],BUFFER_LENGTH,&MinIrAC,&indexMinIrAC);
    
    
    RedAC = MaxRedAC - MinRedAC; // L'AC rouge représente l'amplitude du signal rouge
    IrAC = MaxIrAC - MinIrAC;// L'AC infrarouge représente l'amplitude du signal infrarouge
    
     arm_mean_f32(dataSpo2[RED][DC],BUFFER_LENGTH,&moyenneRedDC); // La moyenne du DC rouge est calculé afin de représenter la "baseline" du signal rouge 
     arm_mean_f32(dataSpo2[IR][DC],BUFFER_LENGTH,&moyenneIrDC);// La moyenne du DC infrarouge est calculé afin de représenter la "baseline" du signal infrarouge
    
 ratio = (RedAC/moyenneRedDC)/(IrAC/moyenneIrDC); // le ratio est calculé en suivant la formule ( présente et expliqué dans le rapport)
                        
 pourcentageSpO2 = round(-45.060*(ratio*ratio) +30.354*ratio + 110);// le pourcentage de SpO2 est calculé en suivant la formule selon des coefficients de calibration specifique (présente et expliqué dans le rapport)
               
        
        return  pourcentageSpO2; // on retourne le Sp02

}
// Fonction qui fait passer le filtre passe bas et passe haut à chacune des channels rouge et infrarouge de l'AC et DC 
void filtrage(uint16_t index){
        
    
        //Filtre le signal rouge
        FiltreLowPass(&dataSpo2[RED][RAW][index],&dataSpo2[RED][DC][index]);// le dataSpO2 [RED][RAW] contient les valeurs brutes et fait passer un index qui va permettre de changer les valeurs brutes en filtrés avec le filtre passe-bas et l'insere dans le channel DC 
        FiltreHighPass(&dataSpo2[RED][DC][index],&dataSpo2[RED][AC][index]);// comme expliqué avant mais à la place de prendre des valeurs brutes , on prend les valeurs filtrés avec le passe bas et on le filtre avec le passe-haut afin d'avoir le signal filtré final rouge 
        
        //Filtre le signal infrarouge
        FiltreLowPass(&dataSpo2[IR][RAW][index],&dataSpo2[IR][DC][index]);// comme avant mais pour le signal IR 
        FiltreHighPass(&dataSpo2[IR][DC][index],&dataSpo2[IR][AC][index]);// comme avant mais pour le signal IR 

}
// Fonction qui permet de faire la translation des signaux filtré de manière à ce que le minimum de ceux-ci soit 0
void LectureDonne( void){
    
        for ( int i =0 ; i< BUFFER_LENGTH; i++){  
                 INFRAROUGE[i]=  ((I32) dataSpo2[IR][AC][i]) + minimum_infrarouge; 
                ROUGE[i] =  ((I32) dataSpo2[RED][AC][i]) + minimum_rouge;
                printf("%d\r\n",INFRAROUGE[i]);
                
            }
    
}
// Cette fonction permet de déterminer la valeur absolue des minimum négatif des signaux filtrés afin de procéder 
// à la translation verticale de ceux-ci 
void determiner_negatif (void) {
    
            // déterminantion de vecteurs contenant la valeur absolue de tous les valeurs négatifs des signaux.
            // Notons qu'il est nécéssaire de faire un CASTING des vecteurs signaux FLOAT de l'expert du traitement signal 
            // en des vecteurs de type uint32_t qui seront affichés par l'expert de l'interface. Ici, on choisi un casting 
            // vers des entiers signés (I32), mais on s'assure que toutes les valeurs sont positifs afin qu'elle puissent 
            // être entrées dans un vecteurs non-signés d'entiers (uint32_t) 
             for ( int i =0 ; i< BUFFER_LENGTH; i++){
                if (dataSpo2[IR][AC][i] < 0 ) {
            INFRAROUGE_minimum_abs[taille_infrarouge] = (I32) (-1*dataSpo2[IR][AC][i]);
            
            taille_infrarouge++; 
            
            }
                if (dataSpo2[RED][AC][i] < 0 ) {
            ROUGE_minimum_abs[taille_rouge] = (I32) (-1*dataSpo2[RED][AC][i]);
            taille_rouge++; 
            }
    }      
            // détermination de la valeur absolue du minimum des signaux. Notons que vous qu'on étudie la valeur absolue
            // de valeurs négatif, on cherchera alors le maximum des vecteurs précédemment établies 
            minimum_infrarouge= maximum_1(INFRAROUGE_minimum_abs, taille_infrarouge);
            minimum_rouge= maximum_1(ROUGE_minimum_abs, taille_rouge);     
         
} 
// Tache qui fait passer toutes les fonctions expliquées plus haut et donc qui filtre le signal,calcule la fréquence cardiaque et le taux de saturation d'oxygène
void TraitementSignal( void*arg){ // cette tache a moins de priorité que la tache d'ecriture pour que l'index soit toujours en retard par rapport a l'ecriture.

    (void)arg;
    uint16_t indexLecture = 0;
     float32_t moyenneRED,
            moyenneIR;
  
    for(;;)
    {
      
        
        //Se fait à chaque 5 seconde (donc a chaque 1000 nouvelles donnees) 
        // Pour l'index de lecture des données brut allant jusqu'à une valeur multiple du buffer afin que la lecture  n'ecrase pas des données de l'ecriture dans le bufferr
        if(((indexLecture)%(BUFFER_LENGTH))==0 && indexLecture < BUFFER_LENGTH && entree == true )
        {
              
                //Filtrage du signal
            
                filtrage(indexLecture); // on filtre le signal selon la position de l'index de lecture 

                //Centrer le signal en 0 puisque la fonction du filtre passe-haut fonctionne mal sur PSoC
                arm_mean_f32(dataSpo2[RED][AC],BUFFER_LENGTH,&moyenneRED); // on utilise la fonction de CMSIS qui calcule la moyenne des données filtrées dans le channel AC du signal rouge
                arm_mean_f32(dataSpo2[IR][AC],BUFFER_LENGTH,&moyenneIR);// on utilise la fonction de CMSIS qui calcule la moyenne des données filtrées dans le channel AC du signal infrarouge

                for ( int i = 0; i < BUFFER_LENGTH; i++){ 
                
                dataSpo2[RED][AC][i] -= moyenneRED; // on soustrait la moyenne obtenue précédemment des valeurs filtrées et le signal rouge est centré en 0
                dataSpo2[IR][AC][i] -=  moyenneIR;// on soustrait la moyenne obtenue précédemment des valeurs filtrées et le signal infrarouge est centré en 0
                }
            
            //Calculer la fréquence cardiaque 
            
            BPM = calculBPM();  //  on appelle la fonction qui calcule le BPM
            
            //Calculer  SpO2
            pourcentageSpO2 = calculSPO2();

          
        }
        
            indexLecture++; 
            if(indexLecture == BUFFER_LENGTH){
                indexLecture = 0;  // l'index de lecture retourne a 0 une fois qu'il a traversé tout le buffer pour ne pas écraser des valeurs 
                // À la fin de la lecture de tout le signal, celui-ci est traité afin d'être conforme au code de l'expert de l'interface
            minimum_infrarouge=0; 
            minimum_rouge=0; 
            taille_infrarouge=0; 
            taille_rouge=0; 
            determiner_negatif(); 
            LectureDonne();
            // On met une pause à la tâche de traitement de signal, sachant que les données ont été traités 
            entree = false; 

            }
        
            
        vTaskDelay(pdMS_TO_TICKS(1));
       
    }
    
}

/* [] END OF FILE */
