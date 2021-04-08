
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


%% Lire les signaux et l'afficher
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
for k=2:length(filteredSignalRedAC)-1
    if ( filteredSignalRedAC(k)> filteredSignalRedAC(k-1) && filteredSignalRedAC(k)>filteredSignalRedAC(k+1) && filteredSignalRedAC(k)>1000)
        position(nbre_battements+1)=k;
        nbre_battements=nbre_battements + 1; % ...donc on a pic
        if (nbre_battements>=2) % si on a plus qu'un seul pic 
            % Maintenant , on veut calculer le ratio R
            R(nbre_battements)=((filteredSignalRedAC(k))./(filteredSignalRedDC(k)))./((filteredSignalIRAC(k))./(filteredSignalIRDC(k)));
            % Maintenant, on veut calculer le SpO2 
            SpO2=-50*R.^2 + 5*R + 117;
            % Je calcule la fréquence cardiaque 
            freq_card (nbre_battements) = 60/(k-ancienk)*Fs;
        end
        ancienk=k;
    end
end

%  Pour comparer mon signal de la fréquence cardiaque avec le signal du fichier exemple
%  x2=linspace(1,nbre_battements-1,length(freq_card));
%  figure 
% hold on 
% plot(x2,freq_card,'r');
% plot(signal.HeartRate);
% hold off

% Pour comparer mon SpO2 avec celui du fichier exemple 
% x3 = linspace(1,nbre_battements-1, length(SpO2)); 
% figure (4)
% hold on 
% plot(SpO2);
% plot(signal.SpO2)
% hold off
% 




