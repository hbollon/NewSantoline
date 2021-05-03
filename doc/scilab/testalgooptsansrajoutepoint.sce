//LISTAUX(1): tableau des cellules actives
//LISTAUX(2): liste de tableaux : LISTAUX(2)(num) est le tableau des indices de MVT dans la cellule num
//LISTAUX(3): tableau de colonne [IND,T] où T est le temps d'ignition du MVT IND, ne contient que les MVT actifs 
//LISTAUX(4): indice du MVT ayant le temps d'ignition minimum, tous les temps de LISTAUX(3) lui sont supérieur ou égal.
//LISTAUX(5):transmet les infos de supprimecroisement à chocdansunecellule pour traiter les chocs
//LISTAUX(6):
//LISTAUX(7): tableaux dont les colonnes sont [numsommet,tempsignitsommet,indice du point ayny allumé le sommet, ....] permet de determiner les arêtes brulées ou pas.








clear;
//clf(0);
clf(1);
clf(2);
clf(3);
if getos() == 'Windows' then 
    //getd("H:\Mes documents\Bureau\SCILAB\propagfeu_algo3_2");
    getd("D:\SCILAB\propagfeu_algo_opt");
    path="D:\SCILAB\propagfeu_algo_opt\";
    nomfichier="D:\SCILAB\propagfeu_algo_opt\resultats\";
else 
    getd("/Users/pierrebaras/Desktop/propagfeu_algo_opt");
    path="/Users/pierrebaras/Desktop/propagfeu_algo_opt/";
    nomfichier="/Users/pierrebaras/Desktop/propagfeu_algo_opt/resultats/";
end




nom='LISTFFLISTAUX';
scf(0);
// variables globales
NUM=100;
kmax=36;
eps=0.000000000001;

crit=0.1;
TMAX=3;
comptmax= 20000;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//test initLISTFF
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
rep=x_choose(['avec DI';'reprendre avec le resultat enregistré'],['']);
//disp(rep,'rep');
//pause;
if rep==2 then
    load(nomfichier+'L','LISTFF');
    load(nomfichier+'X','LISTAUX');
    T=LISTFF(LISTAUX(4))(1)(3);
else
IJ=[0;0];
LISTFF = list();

contourinit = list([0.1;0.1],[0.7;0.1],[0.6;0.8],[0.5;0.3],[0.1;0.1]);
dessellipse(IJ);

disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
disp('debut d une propagation des points avec TEMPMIN');
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
contourinit = list([0.1;0.1],[0.7;0.1],[0.6;0.8],[5;0.3],[0.1;8]);
x=[];
y=[];

for M=contourinit
    x=[x,M(1)];
    y=[y,M(2)];   
end
plot2d(x,y,1);
[LISTFF,LISTAUX] = initLISTFF(contourinit);
//disp(LISTAUX(4),"LISTAUX(4)");
//pause;
T=LISTFF(LISTAUX(4))(1)(3);
while T < 0
    [LISTFF,LISTAUX]=uneiteration(LISTFF,LISTAUX,LISTAUX(4));
    T=LISTFF(LISTAUX(4))(1)(3);
end
//LISTAUX(6)=list();
//pause;
//scf(1);
end
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
disp('debut d iteration');
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
NMAX=size(LISTFF); //**** correction rajout *****
compt=0;
TOLD=0;
while compt <= comptmax & T<=TMAX

IND = LISTAUX(4);

T=LISTFF(LISTAUX(4))(1)(3);
if TOLD==T then
    disp('TOLD=T');
//    pause;
    end
TOLD=T;
disp('------------------------------------------------------------');
disp(['compt =',string(compt),'IND =',string(IND),'T = ',string(T),'size',string(size(LISTFF))]);

//if IND==7263 then
//    scf(2);
//    tr(IND);
//    pause;
//end
[LISTFF,LISTAUX]=uneiteration(LISTFF,LISTAUX,IND);

//disp(LISTAUX(7),'LISTAUX(7)');
NMAXNEWN=size(LISTFF);
NMAXNEW=NMAX;
while (NMAXNEWN>NMAXNEW) & (NMAXNEWN-NMAX<=20) then
for k=NMAXNEW+1:NMAXNEWN
[LISTFF,LISTAUX] = rajoutepoints(LISTFF,LISTAUX,k,crit);
end
NMAXNEW=NMAXNEWN;
NMAXNEWN=size(LISTFF);
end
NMAXNEW=size(LISTFF);
for k=NMAX+1:NMAXNEW
LISTAUX=actualisesommet(LISTFF,LISTAUX,k);
end
NMAX=NMAXNEW;
//verifactif(LISTFF,LISTAUX,3380);
//if size(LISTFF)>=16000 then
////disp(size(LISTFF),"size(LISTFF)");
//  verifpoint(LISTFF,LISTAUX);
//end
MVT=LISTFF(IND);
//if MVT(1)(2)==8 then
//    scf(2);
//    tracevitcellule([0;8])
//    pause;
//end
compt=compt+1;
end

disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
disp('fin d iteration');
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
//disp(LISTFF);
//abort;
tracefront(LISTFF,LISTAUX(4));
//traceLISTFFactif(LISTFF);    
//dessellipseactif(LISTAUX);
if rep ==1 then
save(nomfichier+'L','LISTFF');
save(nomfichier+'X','LISTAUX');
end

