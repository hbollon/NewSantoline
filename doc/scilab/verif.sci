function verifpoint(LISTFF,LISTAUX)
    
    // on verifie que les points successifs du front ne sont pas confondus
    // on teste si ils sont très éloignés
    // on verifie que ça boucle bien
    TAB = LISTAUX(3);
    IND=LISTAUX(4);
    MVT=LISTFF(IND);
    INDS=MVT(1)(7);
    IND0=IND;
    compt=0
    while INDS<>IND0 & compt<= size(LISTFF);
        MVTS=LISTFF(INDS);
        M=MVT(1)([1,2],:);
        MS=MVTS(1)([1,2],:)
        test=norm(MS-M);
        if test<=0.000001 then
            disp(['les points suivants sont confondus',string(IND),string(INDS)]);
        end
        if test>=3 then
            disp(['les points suivants sont distants de plus de 3 :',string(IND),string(INDS)]);
                x=[M(1),MS(1)];
                y=[M(2),MS(2)];
                plot2d4(x,y,5);
        end

        [nbre,loc]=members([INDS],TAB([1],:));
         if nbre==0 then
            disp([string(INDS),' n est pas actif, predecesseur =',string(IND)]);
            test1=%F;
            abort;
        end
        IND=INDS;
        INDS=MVTS(1)(7);
        MVT=MVTS;
        compt=compt+1;
    end
    if compt== size(LISTFF)+1 then
        disp('compt==size(LISTFF)');
        // on cherche l'erreur
        IND=LISTAUX(4);
        MVT=LISTFF(IND);
        INDS=MVT(1)(7);
        TABIND=[IND];
        nbre=0;
        while nbre==0
            [nbre,loc]=members([INDS],TABIND);
            TABIND=[TABIND,INDS];
            INDS=LISTFF(INDS)(1)(7);
        end
        disp(loc,'loc',INDS,'erreur en ');
        disp(TABIND);
        abort;
    end
endfunction

function verifactif(LISTFF,LISTAUX,IND)
    // verifie qu'un point est actif
    if size(LISTFF)<IND then
        return;
    end
TAB = LISTAUX(3);
[nbre,loc]=members([IND],TAB([1],:));
if nbre==1 then
//    disp([string(IND),' est actif']);
    test1=%T;
else
    disp([string(IND),' n est pas actif']);
    test1=%F;
    pause;
end
if size(LISTFF)>=IND then

IJ=LISTFF(IND)(2)(1)([5,6],:);
TAB=LISTAUX(2)(numero(IJ));
[nbr,lc]=members([IND],TAB);
if nbr==1 then
//    disp([string(IND),' est dans LISTAUX(2)']);
    test2=%T;
else
//    disp([string(IND),' n est pas dans LISTAUX(2)']);
    test2=%F;
end
if test1==%F & test2==%T then
//    disp([string(IND),' n est pas actif mais il est dans LISTAUX(2)']);
//    abort;
end
end
endfunction

function vit(IND)
     MVT = LISTFF(IND);
    MT=MVT(1);
    VITS=MVT(2);
    MM=MT([1,2],:);
    tauperp=MT([4,5],:);
    if norm(MM-round(MM))<= eps then
        disp(MVT,'on est sur un sommet');
        //[LISTFF,LISTAUX]=supprime(LISTFF,IND,LISTAUX);
        //pause;
    end

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// CALCUL DES VITESSES DE PROPAGATION
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    if MT(9)==0 then
        disp('vitesse_new_2');
        [VITS,INDIC]=vitesse_new_2(VITS,tauperp)
    elseif MT(9)==1
        disp('vitesse_choc_new');
        [VITS,INDIC]=vitesse_choc_new(VITS,tauperp);
    elseif MT(9)==2
        disp('vitesse_sommet_new');
        [VITS,INDIC,LISTAUX]=vitesse_sommet_new(VITS,tauperp,LISTAUX);//********* correction **************
    elseif MT(9)==3 // cas d'un père de choc artificiel cf 'chocdansunecellule'
        VITS=list(LISTFF(IND)(2));
        INDIC=%T;
    else
        disp("erreur dans MT(9)");
        abort;
    end
    disp(VITS);
endfunction

function saveinitFF(nom)
if getos() == 'Windows' then 
    nomfichier=path+"init\ffi_"+nom+'.dat';
else 
    nomfichier=path+"init/ffi_"+nom+'.dat';
end

save(nomfichier,'LISTFF');
endfunction

function initdata(nom,crit,kmax,N)
    if getos() == 'Windows' then 
    nomfichier=path+"init\init_"+nom+'.dat';
else 
    nomfichier=path+"init/init_"+nom+'.dat';
end
datainit=list(crit,kmax,N);
save(nomfichier,'datainit');
endfunction

function datainit=loaddata(nom)
    if getos() == 'Windows' then 
    nomfichier=path+"init\init_"+nom+'.dat';
else 
    nomfichier=path+"init/init_"+nom+'.dat';
end
load(nomfichier,'datainit');
endfunction

function test(LISTFF)
    // on représente les points successifs en commençant par le dernier inséré

    MTV=LISTFF($);
    MT=MTV(1);
    IND=size(LISTFF);
    INDS=MT(7);
    coul=1;
    compt=0;
    disp(IND,'IND');
    while INDS<>IND & compt<=IND+2
        //disp(IND);
        MTV=LISTFF(INDS);
        MT=MTV(1);
        INDS=MT(7);
        compt=compt+1;
    end
    disp(compt,"S compt");
    if compt==size(LISTFF)+2 then
        disp('erreur 1 dans traceLISTFFactif, compt==size(LISTFF)+2') ; 
    end

    MTV=LISTFF($);
    MT=MTV(1);
    IND0=size(LISTFF);
    IND=MT(6);
    coul=1;
    compt=0;
//    disp(IND0,'IND0 sens P');
    while IND<>IND0 & compt<=IND0+2
        //disp(IND);
        MTV=LISTFF(IND);
        MT=MTV(1);
        IND=MT(6);
        compt=compt+1;
    end
    disp(compt,"P compt");
    if compt==size(LISTFF)+2 then
        disp('erreur 2 dans traceLISTFFactif, compt==size(LISTFF)+2') ; 
    end
    
    for k=1:size(LISTFF)
        MTV=LISTFF(k);
        INDP=MTV(1)(6);
        INDS=MTV(1)(7);
        if (INDP-k)*(INDS-k)==0 then
            disp(k,'erreur dans MTV IND =');
            abort;
        end
        
    end
endfunction
