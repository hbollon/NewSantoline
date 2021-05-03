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




//première propagation de tous les points


INDMAX=size(LISTFF);
for IND=1:INDMAX
    MTV=LISTFF(IND);
    MT=MTV(1);
    VITS=MTV(2);
    tauperp=MT([4,5],:);
    //    disp(tauperp,'tauperp');

    if MT(9)==0 then

        tau=[-tauperp(2);tauperp(1)];
        IJN=IJ+tauperp;
        a0=azero(tau,VITS(1));
        V=vitessea0(a0,tau,IJN);
        VITS=list(list(V));
        if norm(V([1,2],:))<>0 then
        INDIC=%T;
    else
        INDIC=%F;
        end
    elseif MT(9)==1

        [VITS,INDIC]=vitesse_choc_new(VITS,tauperp);
        //    disp(VITS,"VITC");
        //    pause;
        if INDIC<>%T then
            disp("INDIC=F");
        end

    else
        disp("erreur dans MT(9)");
        abort;
    end
    if INDIC==%F then
        disp(IND,'on supprime IND');
        LISTFF=supprime(LISTFF,IND);
//    pause;
    else
    MNP=MNPLUSUN(MT,VITS(1)(1));

    MVT=list([MNP;0;0;IND;VITS(1)(1)(7)],VITS(1));

    LISTFF = insere(LISTFF,MVT,IND,%F);
    dessellipse(IJN);
    end
    //    pause;
    //    disp(V-VIT,"V-VIT");
end
//disp(LISTFF);
//abort;
traceLISTFF(LISTFF);

traceLISTFFactif(LISTFF);



