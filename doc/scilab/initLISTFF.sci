// CORRECTION DANS initLISTFF 


function [LISTFF,LISTAUX]=initLISTFF(LISTPOINT)
// initialise LISTFF à partir d'une liste de points (M_X,M_Y)

// on complète LISTPOINT pour avoir au moins un point dans chaque cellule traversée
// on déplace un peu les points pour ne pas avoir de point sur un bord
LISTPOINT=raffine(LISTPOINT,crit);

for M=LISTPOINT
    //    disp(M );
    point(M,5,3);
end

// initialisation de la liste des indices précédent, suivant, pére(ici père=fils))

for k=2:size(LISTPOINT)-1
    LISTPOINT(k)=[LISTPOINT(k);0;0;0;k-1;k+1;k;0];
end
LISTPOINT(1)=[LISTPOINT(1);0;0;0;size(LISTPOINT);2;1;0];
LISTPOINT(size(LISTPOINT))=[LISTPOINT(size(LISTPOINT));0;0;0;size(LISTPOINT)-1;1;size(LISTPOINT);0];

// premier calcul d'une vitesse par point, celle du segment de front qui suit le point

LISTVIT=list();
LISTVIT(1)=list();
IND=1;
INDS=LISTPOINT(IND)(7);
while INDS <> 1
    LISTVIT(INDS)=list();
    M=LISTPOINT(IND);
    MS=LISTPOINT(INDS);
    plot2d([M(1),MS(1)],[M(2),MS(2)],3);
    DS=MS-M;

    if norm(DS([1,2],:))<>0 then
        // calcul de l'indice de la cellule

        IJ=floor(M([1,2],:));
        //  calcul de la vitesse 
        V=vitessea0(0,DS,IJ);

        IJS=floor(MS([1,2],:));
        //            //disp(IJS,"IJS",IND,"IND",M,"M");
        VS=vitessea0(0,DS,IJS);

        LISTVIT(IND)($+1)=V;
        LISTVIT(INDS)($+1)=VS;

        IND=INDS;
        INDS=LISTPOINT(INDS)(7);
    else
        INDS=LISTPOINT(INDS)(7);
    end
end

LISTVIT(1)=list(LISTVIT($)(1),LISTVIT(1)(1));
LISTVIT($)=list(LISTVIT(1)(1),LISTVIT($)(1));



LISTFF=list();
LISTAUX=list();
LISTAUX(1)=[];
LISTAUX(2)=list();
LISTAUX(3)=[];
LISTAUX(6)=list();
LISTAUX(7)=[];
for IND=1:size(LISTPOINT)
    VIT=LISTVIT(IND);
    LISTVIT(IND)=vitessechocoupas(VIT(1),VIT(2));
    LISTFF(IND)=list();
    LISTFF(IND)(1)=LISTPOINT(IND);
end

for IND = 1:size(LISTPOINT)
    VITS=LISTVIT(IND);

    [LISTFF,LISTAUX,INDIC]=propage(LISTFF,LISTAUX,IND,VITS);
    // DEBUT CORRECTION
    if INDIC == %F then
        INDP=LISTFF(IND)(1)(6);
        INDS=LISTFF(IND)(1)(7);
        LISTFF(INDP)(1)(7)=INDS;
        LISTFF(INDS)(1)(6)=INDP;
    end
    // FIN CORRECTION
    //disp(LISTPOINT(IND),VITS,IND);
end

//**** correction rajout****
IND0=LISTAUX(4);
IND=LISTFF(IND0)(1)(7);
while IND<>IND0
    LISTAUX=actualisesommet(LISTFF,LISTAUX,IND);
    IND=LISTFF(IND)(1)(7);
end
LISTAUX=actualisesommet(LISTFF,LISTAUX,IND0);
endfunction



function LISTPOINTN=raffine(LISTPOINT,crit)

    M=LISTPOINT(1);
    LISTPOINTN=list();
    LISTPOINT($+1)=M;
    for k=2:size(LISTPOINT)
        MP=LISTPOINT(k);
        dist=norm(MP([1,2],:)-M([1,2],:));
        if dist > 0.0000000000001 then
            test =floor(dist/crit);
            for k=1:test+1
                MK=M+(k-1)*(MP-M)/(test+1);
                MKI=round(MK);
                if (MK(1)-MKI(1))*(MK(2)-MKI(2))==0 then // on modifie pour ne pas avoir de point sur un bord
                    MK=MK+0.01*(MP-M)/(test+1);
                end
                LISTPOINTN($+1)=MK;
            end
            M=MP;
        end
    end

endfunction

