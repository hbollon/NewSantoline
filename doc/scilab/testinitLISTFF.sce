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




clf(1);
scf(1);
IJ=[0;0];
dessellipse(IJ);



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//test initLISTFF
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LISTFF = list();
contourinit = list([0.1;0.1],[0.7;0.1],[0.6;0.8],[0.5;0.3],[0.1;0.1]);
x=[];
y=[];
for M=contourinit
x=[x,M(1)];
y=[y,M(2)];   
end
plot2d(x,y,1);

LISTFF = initLISTFF(contourinit);
for MTV=LISTFF
//    disp(MTV,"MTV");
    MT=MTV(1);
    VIT=MTV(2)(1);
    x=[MT(1)-MT(3)*VIT(1),MT(1)];
    y=[MT(2)-MT(3)*VIT(2),MT(2)];
    if size(MTV(2))==1 then 
        coul=3;
    else
        coul=5;
    end
    plot2d4(x,y,coul);
end
//disp(LISTFF,'LISTFF');
