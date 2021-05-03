clear;
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





scf(1);
// variables globales
NUM=100;
kmax=20;
eps=0.000000000001;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//test initLISTFF
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
IJ=[0;0];
LISTFF = list();
contourinit = list([0.1;0.1],[0.7;0.1],[0.6;0.8],[0.5;0.3],[0.1;0.1]);
dessellipse(IJ);

disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
disp('debut d une propagation des points avec TEMPMIN');
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
x=[];
y=[];

for M=contourinit
    x=[x,M(1)];
    y=[y,M(2)];   
end
plot2d(x,y,1);
LISTFF = initLISTFF(contourinit);
LISTAUX=initlistaux(LISTFF);


disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
disp('debut d iteration');
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
compt=0;
while compt <1000
    compt=compt+1;
    IND = LISTAUX(4);
//    disp(IND,"IND");
T=LISTFF(LISTAUX(4))(1)(3);
disp(['IND =',string(IND),'T',string(T)])
    MTV = LISTFF(IND);
    MT=MTV(1);
    VITS=MTV(2);
    MM=MT([1,2],:);
    tauperp=MT([4,5],:);
    if norm(MM-round(MM))<= eps then
        disp(MM,'on est sur un sommet');
        [LISTFF,LISTAUX]=supprime(LISTFF,IND,LISTAUX);
        else
    
    //    disp(tauperp,'tauperp');
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CALCUL DES VITESSES DE PROPAGATION
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if MT(9)==0 then
        [VITS,INDIC]=vitesse_new_1(VITS,tauperp)
    elseif MT(9)==1
        [VITS,INDIC]=vitesse_choc_new(VITS,tauperp);
    else
        disp("erreur dans MT(9)");
        abort;
    end
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SUPPRESSION DU POINT SI PAS DE VITESSE CALCULEE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if INDIC==%F then
        disp(IND,'on supprime IND');
        [LISTFF,LISTAUX]=supprime(LISTFF,IND,LISTAUX);
    else
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PROPAGATION DU POINT
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        [LISTFF,LISTAUX,INDIC]=propage(LISTFF,LISTAUX,IND,VITS);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// SUPPRESSION DU POINT SI PAS DE PROPAGATION POSSIBLE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if INDIC==%F then
        disp(IND,'on supprime IND');
        [LISTFF,LISTAUX]=supprime(LISTFF,IND,LISTAUX);
        end
    end
    
//pause;

    //    pause;
    //    disp(V-VIT,"V-VIT");
//    IJ=LISTFF($)(2)(1)([5,6],:);
//    dessellipse(IJ);
    
    tracefront(LISTFF,LISTAUX(4));
//    disp(LISTAUX(3));
//    pause;

end
end
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
disp('fin d iteration');
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++');
//disp(LISTFF);
//abort;
    IJ=LISTFF($)(2)(1)([5,6],:);
    dessellipse(IJ);
    T=LISTFF(LISTAUX(4))(1)(3);
    tracefront(LISTFF,LISTAUX(4));
    
traceLISTFF(LISTFF);

traceLISTFFactif(LISTFF);    

