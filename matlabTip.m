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

%% Creation des filtres 

Fc_high = 0.9;       %frequence de coupure inférieure à 1 Hz
Fs = 250;      %frequence d'échantillonage 
Wn_high=Fc_high/(Fs/2);  %fréquence de coupure normalisée
[b,a] = butter(6,Wn_high,'high');
figure (1);
freqz(b,a)

Fc_low=20;
Wn_low=Fc_low/(Fs/2);
[d,c] = butter(6,Wn_low,'low');
figure (2);
freqz(d,c)


%% Lire le signal 
figure(1)
signal=loadSignal('SpO2_signal_high_BPM');
filteredSignal=filter(b,a,signal.SpO2Red);
filteredSignal=filter(c,d,filteredSignal);
x=linspace(0,length(filteredSignal)/Fs,length(filteredSignal));
plot(x,signal.SpO2Red,x,filteredSignal);
legend('Signal','filtered Signal');



% %% Enlever la tendance des données ( EST CE QUE C'EST NECESSAIRE DE FAIRE CELA ?)
% 
% detrendedSignal = detrend(filteredSignal,5); % on enlève la tendance polynomiale du 5ème degré , des données du signal filtré 
% 
% % Visualisation des résultats
% figure(2);
% x1=linspace(0,length(detrendedSignal)/Fs,length(detrendedSignal));
% plot(x,filteredSignal,x1,detrendedSignal); % visualisation du signal filtré et du signal filtré après la supression de la tendance polynomiale
% legend ('Filtered Signal','Detrended Signal');
% xlabel('temps en millisecondes')
% ylabel('tension en millivolt')

%% Calcul de la fréquence cardiaque 

% On veut compter les peaks dominant du signal ( correspondant aux battements
% cardiaques ) ; les peaks sont definis comme étant des echantillons plus
% grands que deux de leurs plus proches voisins et plus grand que 1 

nbre_battements=0;
for k=2:length(filteredSignal)-1
    if ( filteredSignal(k)> filteredSignal(k-1) && filteredSignal(k)>filteredSignal(k+1) && filteredSignal(k)>1)
        nbre_battements=nbre_battements + 1;
    end
end
    

freq_card=(nbre_battements*Fs/length(filteredSignal))*60;
disp(freq_card);













