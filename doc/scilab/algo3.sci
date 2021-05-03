// VERIFIER LES LISTFFN et LISTFF AINSI QUE LISTAUXX et LISTAUX;
// A CORRIGER, ON INSERE LES SOMMETS DANS LES DEUX CELLULES DONNEES PAR LES VITESSES

// A CORRIGER, DANS supprime, ON VERIFIE QUE LE POINT N'A PAS ETE DEJA SUPPRIME

// pour l'instant, on garde toutes les trajectoires dans LISTAUX(2), même celles detectées par 'supprimecroisement' qui ne sont pas optimales

// A CORRIGER DANS supprimeplus, on supprime systématiquement de LISTAUX(2)

// A CORRIGER DANS insere, on rajoute une mise à jour du précédent pour traiter correctement les chocs en cas de reflexion

function [LISTFFN,LISTAUXX] = insere(LISTFF,MVT,IND,INDIC,LISTAUX)
    // insére le point MVT=(M,LISTV) 'après' le point IND
    // si INDIC=%F, le point MVT a pour père le point IND
    // si INDIC=%T, le point MVT a pour prédecesseur le point IND
    
LISTFFN=LISTFF;
LISTAUXX=LISTAUX;
INDP=LISTFF(IND)(1)(6);
INDS=LISTFF(IND)(1)(7);

if INDIC == %F then // cas où on prolonge la trajectoire
        MVT(1)(6)=INDP;
        MVT(1)(7)=INDS;
        LISTFFN($+1)=MVT;
        Nmax=size(LISTFFN);
        LISTFFN(INDP)(1)(7)=Nmax;
        LISTFFN(INDS)(1)(6)=Nmax;

else
        MVT(1)(6)=IND;
        MVT(1)(7)=INDS;
        LISTFFN($+1)=MVT;
        Nmax=size(LISTFFN);
        LISTFFN(IND)(1)(7)=Nmax;
        LISTFFN(INDS)(1)(6)=Nmax; 
//       disp(LISTFFN(IND)(1)(8),"LISTFFN(IND)(1)(8)",IND,"IND dans insere");
        if LISTFFN(IND)(1)(8)>0 then
                LISTFFN(LISTFFN(IND)(1)(8))(1)(6)=Nmax; // je ne sais plus pourquoi on fait ça, sans doute pour traiter les chocs
        end
end

// mise à jour de LISTAUX

if MVT(1)(9)==2 then
    IJ=MVT(2)(1)([5,6],:);
    num=numero(IJ);
    listnum=list(num);
    IJ=MVT(2)(2)([5,6],:);
    num=numero(IJ);
    listnum($+1)=num;
else
    IJ=MVT(2)(1)([5,6],:);
    num=numero(IJ);
    listnum=list(num);
end
for num=listnum
[nbre,loc]=members([num],LISTAUXX(1));
if nbre==0 then
    LISTAUXX(1)=[LISTAUXX(1),num];
    LISTAUXX(2)(num)=[Nmax];
else
    LISTAUXX(2)(num)=[LISTAUXX(2)(num),Nmax];
end
end

TAB=LISTAUXX(3);

if INDIC ==%F then
    [nbre,loc]=members([IND],TAB(1,:));
    TAB=[TAB(:,1:loc(1)-1),TAB(:,loc(1)+1:$)];
    TAB=[TAB,[Nmax;MVT(1)(3)]];
    else
    TAB=[TAB,[Nmax;MVT(1)(3)]];
end
//disp(TAB,'TAB');

LISTAUXX(3)=TAB;
[tabtemp,ordre]=gsort(TAB(2,:),'c','i');
LISTAUXX(4)=TAB(1,ordre(1));
//disp(LISTAUXX(4),'LISTAUXX(4) dans insere');

endfunction

function [LISTFFN,LISTAUXX] = supprime(LISTFF,IND,LISTAUX)
    // supprime le point d'indice IND
    // attention, on ne fait que modifer les prédecesseurs et les successeurs
LISTAUXX=LISTAUX;
LISTFFN=LISTFF;
disp([' on supprime IND =',string(IND)]);
TAB = LISTAUX(3);
[nbre,loc]=members([IND],TAB([1],:));
if nbre==1 then

TAB=[TAB(:,1:loc(1)-1),TAB(:,loc(1)+1:$)];
//disp(TAB,"TAB");
LISTAUXX(3) = TAB;

[tabtemp,ordre]=gsort(TAB(2,:),'c','i');
LISTAUXX(4)=TAB(1,ordre(1));

INDP=LISTFF(IND)(1)(6);
INDS=LISTFF(IND)(1)(7);

LISTFFN(INDP)(1)(7)=INDS;
LISTFFN(INDS)(1)(6)=INDP;

else
    disp(['supprime',string(IND),' a déja été supprimé ']);
end
endfunction

function LISTAUXX = supprimeplus(LISTFF,IND,LISTAUX,num)
    // supprime le point d'indice IND dans la table LISTAUX(3) et LISTAUX(2)(num)
    
LISTAUXX=LISTAUX;
//if IND==437  then
//    disp([' on supprimeplus IND =',string(IND)]);
////    abort;
//end

TAB = LISTAUX(3);
[nbre,loc]=members([IND],TAB([1],:));
if nbre==1 then

TAB=[TAB(:,1:loc(1)-1),TAB(:,loc(1)+1:$)];
//disp(TAB,"TAB");
LISTAUXX(3) = TAB;

[tabtemp,ordre]=gsort(TAB(2,:),'c','i');
LISTAUXX(4)=TAB(1,ordre(1));
end

TAB=LISTAUX(2)(num);
[nbr,lc]=members([IND],TAB);
if nbr==1 then
    TAB=[TAB(1:lc(1)-1),TAB(lc(1)+1:$)];
    LISTAUXX(2)(num)=TAB;
else
    disp('erreur dans supprimeplus');
    abort;
end


endfunction

function num=numero(IJ)
    num=(IJ(1)+NUM)+(2*NUM+1)*(IJ(2)+NUM);
endfunction

function IJ=cellnumero(num);
    IJ1=modulo(num,2*NUM+1);
    IJ=[IJ1-NUM;(num-IJ1)/(2*NUM+1)-NUM];
    
endfunction



