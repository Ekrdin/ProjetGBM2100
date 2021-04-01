%% Liste de fonctions intéressantes
%
%clear all   %Supprime tous les variables du workspace
%close all   %Ferme toutes les figures
%clc         %nettoie la command window

%help butter %créer un filtre numérique
%help filter %appliquer le filtre numérique
%help max %
%help min % 
%help find %fonction pour trouvez des éléments, retourne les indices
%help fft %effectuer la transformée de fourrier, on l'utiliser souvent comme
% exemple :abs(fftShift(fft(monSignal))); l'axe des x doit être : -Fs/2 à Fs/2
%ici Fs est la fréquence d'échantillonage.
%help linspace %Créer un vecteur, par exemple linspace(-1,1,100) créé un vecteur
% de 100 éléments entre -1 et 1
%help abs %l'absolu d'un vecteur
%help length %retourne la longueur du vecteur
clc 
clear all
%% Creation des filtres 
% Filtre passe-haut pour avoir la composante AC
Fc_high = 0.9;       %frequence de coupure inférieure à 1 Hz
Fs = 250;      %frequence d'échantillonage 
Wn_high=Fc_high/(Fs/2);  %fréquence de coupure normalisée
[b,a] = butter(6,Wn_high,'high');
figure (1);
freqz(b,a)
% Filtre passe-bas pour avoir la composante DC 
Fc_low=6;
Wn_low=Fc_low/(Fs/2);
[d,c] = butter(6,Wn_low,'low');
figure (2);
freqz(d,c)


%% Lire le signal et l'afficher
figure(3)
signal=loadSignal('SpO2_signal_normal_BPM');
filteredSignalRedDC=filter(d,c,signal.SpO2Red);
filteredSignalIRDC=filter(d,c,signal.SpO2Infrared);
filteredSignalRedAC=filter(b,a,filteredSignalRedDC);
filteredSignalIRAC=filter(b,a,filteredSignalIRDC);
x=linspace(0,length(filteredSignalRedDC)/Fs,length(filteredSignalRedDC));
plot(x,signal.SpO2Red,x,filteredSignalRedDC,x,filteredSignalIRAC);
legend('Signal before filtering','filtered Signal for high frequencies','filtered Signal to obtain the AC component','Location','Best');

%% Calcul de la fréquence cardiaque et du % de SpO2

% On veut compter les peaks dominant du signal ( correspondant aux battements
% cardiaques ) ; les peaks sont definis comme étant des echantillons plus
% grands que deux de leurs plus proches voisins et plus grand que 1 

nbre_battements=0;
freq_card=zeros(33,1);
pics(1)=0;
for k=2:length(filteredSignalRedAC)-1
    if ( filteredSignalRedAC(k)> filteredSignalRedAC(k-1) && filteredSignalRedAC(k)>filteredSignalRedAC(k+1) && filteredSignalRedAC(k)>1000)
        position(nbre_battements+1)=k;
        nbre_battements=nbre_battements + 1; % ...donc on a pic
        if (nbre_battements>=2) % si on a plus qu'un seul pic 
            lowpulse(nbre_battements-1,1)=min(filteredSignalRedDC(position(nbre_battements-1):position(nbre_battements))); % on calcule le plus bas point entre deux pics
            lowpulse(nbre_battements-1,2)=min(filteredSignalIRAC(position(nbre_battements-1):position(nbre_battements)));
            % Maintenant , on veut calculer le ratio R
            R=log(lowpulse(nbre_battements-1,2)/filteredSignalIRAC(position(nbre_battements-1)))/log(lowpulse(nbre_battements-1,1)/filteredSignalRedDC(position(nbre_battements-1)));
            % Maintenant, on veut calculer le SpO2 (j'ai besoin des coeff
            % de calibration)
            spo2(position(nbre_battements-1))=1.5*R.^2+(-34.*R)+112
            % Je calcule la fréquence cardiaque 
            freq_card (nbre_battements) = 60/(k-ancienk)*Fs;
        end
        ancienk=k;
    end
end

% Pour comparer mon signal de la fréquence cardiaque avec le signal du fichier exemple
 x2=linspace(1,nbre_battements-1,length(freq_card));
 figure 
hold on 
plot(x2,freq_card,'r');
plot(signal.HeartRate);
hold off







