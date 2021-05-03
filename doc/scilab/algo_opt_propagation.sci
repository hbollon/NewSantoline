//CORRECTION DANS PROPAGE LISTAUX -> LISTAUXX

// CORRECTION DANS chocdansunecellule 

function MP = MNPLUSUN(M,V)
    //renvoie les coordonnées d'un point sur le bord du carré, le tauperp sortant.
    //Le temps de parcours est actualisé. Si la vitesse fait sortir le point immédiatement, le point ne bouge pas car la cellule de calcul est donné par V([5,6],:).

    V1=V(1);
    V2=V(2);
    X1=M(1);
    X2=M(2);
    IJ=V([5,6],:);
    if V1 > 0.000000000000001 then
        M1=IJ(1)+1;
        dt1=(M1-X1)/V1;
        tauperp1=1;
    elseif V1< -0.000000000000001
        M1=IJ(1);
        dt1=(M1-X1)/V1;
        tauperp1=-1;
    else
        V1=0;
        dt1=10000000000;
        tauperp1=0;
    end
    if V2>0.000000000000001 then
        M2=IJ(2)+1;
        dt2=(M2-X2)/V2;
        tauperp2=1;
    elseif V2<-0.000000000000001
        M2=IJ(2);
        dt2=(M2-X2)/V2;
        tauperp2=-1;
    else
        V2=0;
        dt2=10000000000;
        tauperp2=0;
    end
//    disp(dt1,"dt1",dt2,"dt2");
    if dt1 < dt2 then
            MP(1)=M1;
            MP(2)=X2+dt1*V2;
            MP(3)=M(3)+dt1;
            a1=1;
            a2=0;
    elseif dt1 > dt2
            MP(1)=X1+dt2*V1;
            MP(2)=M2;
            MP(3)=M(3)+dt2;
            a1=0;
            a2=1;
    else
            MP(1)=X1+dt2*V1;
            MP(2)=M2;
            MP(3)=M(3)+dt2;
            a1=1;
            a2=1;
end
MP(4)=a1*tauperp1;
MP(5)=a2*tauperp2;
endfunction



function [LISTFFN,LISTAUXX,INDIC] =supprimecroisement(LISTFF,LISTAUX,MVT,IND)
    //IND indice du point qu'on souhaite prolonger (MT), MVT point qu'on souhaite insérer
    // supprime les points correspondant à des trajectoires moins rapides que 
    // celle de MVT
    // INDIC = 0 pas de croisement ou les trajectoires plus lentes ont été supprimées
    // INDIC = 1 la trajectoire de MVT est plus lente, il ne faudra pas insérer MVT
    // INDIC = 2 deux trajectoires se croisent au même instant, il faudra calculer le choc.
    
//    disp('supprimecroisement',IND,"IND");
    
    LISTFFN=LISTFF;
    LISTAUXX=LISTAUX;
    LISTAUXX(5)=[];


    INDIC=0;
    eps=0.0000000000001;
    //    IJ=MVT(2)(1)([5,6],:); correction
    MT=LISTFF(IND)(1);
    MTP=MVT(1);
    MM=MT([1,2],:);
    MP=MTP([1,2],:);
    TMIN=MT(3);
    TP=MTP(3);

    if norm(MM-MP)<=0.00000000000001 then
        disp(IND,"MIND=MVT, IND=");
        disp(MVT,"MVT",MT,"MT dans supprimecroisement");
//        LISTGRAPH=list(MVT,LISTFF(IND));
//        traceLISTFFNOIRE(LISTGRAPH);
        abort;
    end
    VIT=MVT(2)(1);
    V=VIT([1,2],:);
    indasuppr=[];
    //+++++ debut correction
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
 //   disp(listnum,'listnum');
    for num=listnum
        //num=numero(IJ);//****** correction *****
        [nb,loc]=members([num],LISTAUXX(1));// correction LISTAUX->LISTAUXX
        if nb <> 0 then // **** sinon on rentre dans un nouveau carré
            for INDC=LISTAUXX(2)(num)// correction LISTAUX->LISTAUXX
                MTC=LISTFF(INDC)(1);
                MC=MTC([1,2],:);
                INDOC=MTC(8);
                if norm(MC - MM)> eps & INDOC <> IND then
                    VITC=LISTFF(INDC)(2)(1);
                    VC=VITC([1,2],:);
                    TC=MTC(3);
                    DM=MM-MC;
                    aux=det([VC,V]);
                    if abs(aux) > eps then //les deux vitesses ne sont pas parallèles
                        tesT = TMIN+ det([DM,VC])/aux;
                        if TP >= tesT & tesT >= TMIN // le point d'intersection est dans la cellule 
                            tesTC = TC + det([DM,V])/aux;
                            MCR = MC+(tesTC-TC)*VC;
//                            if IND==3228   then
//                                    disp(["INDC =",string(INDC),"TMIN",string(TMIN),"tesTC",string(tesTC),"tesT",string(tesT),'INDOC =',string(INDOC)]);
//                                    clf(2);
//                                    scf(2);
//                                     point(MCR,5,4);
//                                     tr(INDC,LISTFFN);
//                                     pause;
//                            end
                            if  norm(MCR-MM)>= eps & tesTC >= 0 then 
                                if tesT+eps <tesTC & TC >= TMIN then
                                    //                                disp(['on supprime ',string(INDC)]);
                                    //                               disp(["INDC =",string(INDC),'tesT =', string(tesT),'TC =', string(TC),'TMIN =', string(TMIN)]);
                                    //point(MCR,4,1);
                                    indasuppr=[indasuppr,INDC];
                                elseif tesT >tesTC+eps 
                                    //                                disp(['tesT =', string(tesT),'TC =', string(TC),'TMIN =', string(TMIN)]);
                                    //point(MCR,5,1);
                                    INDIC=1;
                                    return;
                                elseif  abs(tesTC-tesT) <= eps then
                                    //*************** correction************************
                                    //  on a un choc, deux trajectoires de croisent au même instant
                                    if MVT(1)(9)==2 then // MVT est un sommet, la vitesse est sur le bord le choc est sur un bord, on garde MVT, et on supprime INDC
                                        indasuppr=[indasuppr,INDC];
                                        disp(INDC,'INDC MVT(1)(9)==2');
                                    elseif MTC(9)==2  // MTC est un sommet, la vitesse VC est sur le bord le choc est sur un bord, on garde MTC, et on n'inserera pas MVT
                                        //point(MCR,5,1);
                                        INDIC=1;
                                        return;
                                        //****** debut correction ************
                                    else
                                        [nbre,loc]=members([INDC],LISTAUX(3)([1],:));
                                        if nbre==0 then // INDC n'est plus actif donc pas optimal, MVT non plus
                                            INDIC=1;
                                            return;
                                            //********* fin correction
                                        else
                                            INDIC=2;
                                            //point(MCR,5,1);
                                            //on stocke les infos nécessaires à chocdansunecellule
                                            LISTAUXX(5)=[LISTAUXX(5),[num;INDC;MCR;tesT]];
                                        end
                                        //LISTAUXX(6)($+1)=MVT(2);
                                    end
                                    //*************** fin correction************************
                                end
                            end
                        end
                    end
                end
            end

        end
    end
    indasuppr=unique(indasuppr);
    //disp(indasuppr,"indasuppr");
    if size(indasuppr,"c")  > 0 then
        for INDC =indasuppr
            [LISTFFN,LISTAUXX]=supprime(LISTFFN,INDC,LISTAUXX);
            //***** correction****** on supprime de LISTAUXX(5)les chocs liés à des points supprimés
            [nbre,loc]=members([INDC],LISTAUXX(5)(2,:));
            if nbre==1 then
                LISTAUXX(5)=[LISTAUXX(5)(:,1:loc-1),LISTAUXX(5)(:,loc+1:$)];
            end
            //***** fin correction ********
        end
    end

endfunction

function [LISTFFN,LISTAUXX,INDIC]=propage(LISTFF,LISTAUX,IND,VITS)
    // propage le point d'indice IND avec les vitesses VITS
    // met à jour LISTAUX
    // INDIC =%T le point a été propagé
    // INDIC =%F aucune vitesse n'a pu propager le point.
   // disp('propage');
    LISTFFN=LISTFF;
    LISTAUXX=LISTAUX;
    MT=LISTFF(IND)(1);
    compt=1;
    INDIC=%F;
sizeold =15620;
    for VIT=VITS
        MNP=MNPLUSUN(MT,VIT(1));
        if size(VIT)==1 then
            INDM=0;
        elseif size(VIT)==2 // on est sur un sommet
            INDM=2;
        elseif size(VIT)==3 // on a un choc
            INDM=1;
        else
            disp(VIT,'erreur sur size(VIT) dans propage VIT=');
            abort;
        end

        MVT=list([MNP;0;0;IND;INDM],VIT);
        TEST=testaretebrulee(LISTFFN,LISTAUXX,MVT); //**** correction rajout *****
        if TEST<>%T then
 
        [LISTFFN,LISTAUXX,INDICS] =supprimecroisement(LISTFFN,LISTAUXX,MVT,IND);
        
        if INDICS==0 & compt==1 then  
            [LISTFFN,LISTAUXX] = insere(LISTFFN,MVT,IND,%F,LISTAUXX);
            compt=compt+1;
            INDIC=%T;
        elseif INDICS==0 & compt > 1
             compt=compt+1;
            INDC=size(LISTFFN);
            [LISTFFN,LISTAUXX] = insere(LISTFFN,MVT,INDC,%T,LISTAUXX);
        elseif INDICS==1
            disp(['on n insère pas MVT']);
//            disp(MVT(1));

        elseif INDICS==2 then
            if compt==1 then 
                INDC=IND;
            else
                INDC=size(LISTFFN);
            end
            [LISTFFN,LISTAUXX,INDIC] = chocdansunecellule(LISTFFN,LISTAUXX,MVT,INDC);// ****correction sur INDIC
//            disp('XXx');
//            verifactif(LISTFFN,LISTAUXX,3380);
//            disp('XXxx');
        else
            disp(INDIC,'erreur 1 dans propage INDIC=');
            abort;
        end
//        if size(LISTFFN)> sizeold then
//        if IND==3228 then    
//                        disp(size(LISTFFN),"disp(size(LISTFFN))");
//                        sizeold=size(LISTFFN);
//                        disp(LISTFFN($)(1),'LISTFFN($)(1)',compt-1,'compt');
//                        pause;
//         end
           else
                   disp('on est sur une arete brulee');
                    //pause;
            end
        end



endfunction

function [LISTFFN,LISTAUXX,INDIC] = chocdansunecellule(LISTFF,LISTAUX,MVT,IND)// **** A CORRIGER IND en argument supplémentaire
    // traite les chocs à l'intérieur d'une cellule
    // disp("on rentre dans chocdansunecellule");
INDIC=%T;
LISTFFN=LISTFF;
LISTAUXX=LISTAUX;
TAB=LISTAUX(5);
//on traite les chocs dans l'ordre des temps d'ignition
// disp(TAB,'TAB');
[tabtemp,ordre] = gsort(TAB(5,:),'c','i');
CHOC=TAB(:,ordre(1));
//IND=MVT(1)(8);// indice du point père de MVT **** A CORRIGER ****
INDC=CHOC(2);

VITS=MVT(2);
VITSC=LISTFF(INDC)(2);
MC=CHOC([3,4,5],:);


V=VITS(1);
VC=VITSC(1);

//T=V([3,4],:);
//TC=VC([3,4],:);

test = det([V([1,2],:),VC([1,2],:)]); // A CORRIGER
//disp(test,'test');
if test < 0 then // pour mettre les vecteurs dans le bon ordre, on sait qu'on a un choc
    
//    INDP=LISTFF(IND)(1)(6);
    if size(VITS)==3 then
        V=VITS(2);
        end
    if size(VITSC)==3 then
        VC=VITSC(3);//********* corriger VITS -> VITSC *************
        end
//    disp(V,"V",VC,"VC");
    VITC=vitessechocoupas(V,VC);
    INDP=LISTFF(IND)(1)(6);
    INDS=LISTFF(INDC)(1)(7);
else
    
    INDP=LISTFF(INDC)(1)(6);
    if size(VITS)==3 then
        V=VITS(3);
    end
    if size(VITSC)==3 then
        VC=VITSC(2);  //********* corriger VITS -> VITSC *************
    end

    [VITC,INDIC]=vitessechocoupas(VC,V); //CORRECTION on rajoute INDIC
    INDP=LISTFF(INDC)(1)(6);
    INDS=LISTFF(IND)(1)(7);
end

//****CORRECTION******
VP=VITC(1)(1);   
VM=[-VP([1:4],:);VP([5,6],:)];
MNP=MNPLUSUN(MC,VM);
//****fin CORRECTION*****
// on cherche si le choc correspond à un choc pas encore propagé
TEST=%F;
IJ=VITC(1)(1)([5,6],:);//**********debut  correction
IJC=IJ+MNP([4,5],:);
num=numero(IJC);
listnum=list(numero(IJ),numero(IJC))
for num=listnum 
[nbre,loc]=members([num],LISTAUX(1));
if nbre==1 then
    tabindcelnum=LISTAUX(2)(num);
//    disp(tabindcelnum);
    for INDZ=1:size(tabindcelnum,'c')  //CORRECTION IND-> INDZ

        MVTT=LISTFF(tabindcelnum(INDZ))(1);//*****correction
//        if MVTT(9)==1 then // on cherche les points concaves *****correction
            MVTTT=MVTT([1,2],:);
            if norm(MVTTT-MNP([1,2],:))<=0.0000000001 then
//                INDCHOC=INDZ; // indice du choc pas encore propagé
                TEST=%T;
                INDCHOC=tabindcelnum(INDZ);
            end
//         end *****correction
    end
end
end//**********fin  correction
//On corrige le temps d'ignition et la direction de propagation
 MNP(3)=MC(3)-(MNP(3)-MC(3));
 MNP([4,5],:)=-MNP([4,5],:); //***** CORRECTION*******
 if TEST==%F then //le choc ne correspond pas à un choc pas encore propagé, 

     // on crée l'origine fictive du choc sur le bord 
     MVT=lstcat(list([MNP;INDP;INDS;0;3]),VITC);
 else //le point racine du choc existe déja, cas des reflexions ou de propagation d'un choc *****correction, rajout
//     disp(['le point racine du choc existe INDCHOC  = :',string(INDCHOC),'INDC =',string(INDC)]);
//     disp(['INDS =',string(INDS),'INDP =',string(INDP)]);
     
     //***** correction
     if LISTFFN(INDC)(1)(8)==INDCHOC & LISTFFN(INDC)(1)(9)==1 then // on retombe sur un choc déja calculé, on ne fait rien, on renvoie INDIC=%F pour ne rien insérer et éventuellement supprimer le point à propager
//         disp('INDC choc déja calculé');
         INDIC=%F;
         return;
     elseif MVT(1)(9)==1 & MVT(1)(8) == INDCHOC 
//         disp('MVT choc déja calculé');
         [LISTFFN,LISTAUXX] = supprime(LISTFFN,INDC,LISTAUXX);
         return;
     end
     //***** fin correction
//    scf(2);
//    tr(INDC);
//    point(MC,5,3);
    MVT=lstcat(list([MNP;INDP;INDS;INDCHOC;3]),VITC);

 end
//    disp(MVT,'on insére l origine fictive du choc sur le bord ');
    [LISTFFN,LISTAUXX] = insere(LISTFFN,MVT,IND,%F,LISTAUX);// IND se trouve supprimé des points actifs et dans la cellule ******** ATTENTION ******** CORRECTION INDC remplacé par IND//***** CORRECTION***LISTFF->LISTFFN **
    LISTFFN($)(1)(6)=INDP;
    LISTFFN($)(1)(7)=INDS;
    INDCHOC=size(LISTFFN);
    // on modifie les précédent et suivant en conséquence
//    if size(LISTFFN)>= 573 then
//    disp(LISTFFN(573)(1)(7),'LISTFFN(573)(1)(7) 1');
//end
    LISTFFN(INDS)(1)(6)=INDCHOC;
    LISTFFN(INDP)(1)(7)=INDCHOC;
    
    LISTFFN(IND)(1)(6)=size(LISTFFN);
    LISTFFN(IND)(1)(7)=INDS;
    // on supprime INDC de la liste des points actifs et dans la cellule
//    disp(['INDC a supprimer dans chocdansunecellule :',string(INDC),'IND =',string(IND)]);
//    disp(['INDS =',string(INDS),'INDP =',string(INDP),'INDCHOC =',string(INDCHOC)]);
//    disp(['CHOC(1)',string(CHOC(1))])
    LISTAUXX = supprimeplus(LISTFFN,INDC,LISTAUXX,CHOC(1));
endfunction


