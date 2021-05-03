// contient les procédures permettant de rajouter des points
function [LISTFFN,LISTAUXX]=rajoutepoint(LISTFF,LISTAUX,IND,crit)
    // rajoute des points  si la distance entre IND et le suivant calculée au temps le plus grand possible est supérieur à crit et si les deux points n'ont pas la même origine.

    //    disp("on rentre dans rajoutepoint");
    LISTFFN=LISTFF;
    LISTAUXX=LISTAUX;

    TMIN=LISTFF(LISTAUX(4))(1)(3);
    MT=LISTFF(IND)(1);
    M=MT([1,2],:);
    T=MT(3);
    VITM=LISTFF(IND)(2);
    VIT=VITM(1);
    INDS=MT(7);
    MTS=LISTFF(INDS)(1);
    if MTS(8)<>MT(8) & MTS(9)+MT(9)<> 3 then // sinon les points M et MS proviennent du même point, ou sont de part et d'autre d'une reflexion.
        VITMTS=LISTFF(INDS)(2);
        VITS=VITMTS(1);
        TS=MTS(3);
        MS=MTS([1,2],:);
        VS=VITS([1,2],:);
        V=VIT([1,2],:);
        IJ=VIT([5,6],:);
        IJS=VITS([5,6],:);
        if TS>=T then
            MS=MS+(T-TS)*VS;
            DM=MS-M;
            MM=M;
        else
            MM=M+(TS-T)*V;
            MS=MTS([1,2],:);
            DM=MS-MM;
        end

        if norm(IJ-IJS)<>0 then
            testS=norm(DM);
            testmemcell=%F;
        else
            testS=abs(det([V,DM]))/norm(V)+abs(det([VS,DM]))/norm(VS);
            testmemcell=%T;
        end
        //    if IND==423 then
        //        disp(INDS,'INDS =');
        //        disp(testS,'IND==423 testS =' );
        //        pause;
        //    end
        //    if IND==422 then
        //        disp(INDS,'INDS =');
        //        disp(testS,'IND==422 testS =' );
        //        pause;
        //    end
        if testS >=crit then
            //disp(["rajoutepoint : on doit rajouter un point","MIS(1)=",string(MS(1)),"MIS(2)=",string(MS(2))]);


            MM=M+(TMIN - T)*V;
            MS=MTS([1,2],:)+(TMIN-TS)*VS;
            MIS=(MS+MM)/2;
            IJ = round(MIS);
            if (MIS(1)-IJ(1))*(MIS(2)-IJ(2))==0 then
                disp('MIS est sur un bord');
                MIS=(2*MS+MM)/3;
            end
            //                DIR=(VITS([3,4],:)+VIT([3,4],:))/2;

            IJ = floor(MIS);
            DIR=MS-MM;
            testSS=norm(DIR);
            if testSS>=crit/5 then
                VITMIS=vitessea0(0,DIR,IJ);

                MIS=[MIS;TMIN];
                MNP=MNPLUSUN(MIS,VITMIS);
                MVT=list([MNP;0;0;0;0],list(VITMIS));
                TEST=testaretebrulee(LISTFF,LISTAUX,MVT);
                if TEST<>%T then
                    //                    [LISTFFN,LISTAUXX,INDICS] =supprimecroisement(LISTFF,LISTAUX,MVT,IND);
                    [LISTFFN,LISTAUXX,INDICS] =supprimecroisementraj(LISTFF,LISTAUX,MVT,TMIN);
                    if INDICS==0 then
                        [LISTFFN,LISTAUXX] = insere(LISTFFN,MVT,IND,%T,LISTAUXX);
                        //                        point(MNP,2,2);
                        //                        traceLISTFF(list(MVT));
                        disp(["on a rajouté",string(size(LISTFFN))]);
                    end
                end
            end
        else
            //            disp("rajoutepoint : M et MS proviennent du même point");
            //            pause;
        end
    end

endfunction


//function LISTFF=rajoutepointbis(LISTFF,LISTAUX,IND,crit)
//    // rajoute des points  si la distance entre IND et le suivant calculée au temps le plus grand possible est supérieur à crit.
//  
////    disp("on rentre dans rajoutepoint");
//    eps=0.0000001;
//
//    TMIN=LISTFF(LISTAUX(14))(1)(3);
//    MT=LISTFF(IND)(1);
//    M=MT([1,2],:);
//    T=MT(3);
//    VITM=LISTFF(IND)(2);
//    VIT=VITM(1);
//    INDS=MT(7);
//    MTS=LISTFF(INDS)(1);
//    VITMTS=LISTFF(INDS)(2);
//    VITS=VITMTS(1);
//    TS=MTS(3);
//    if MT(8) == 2 then
//        disp('on est sur un sommet');
//        return;
//    end
//    if MTS(8) == 2 then
//    end
//    if MT(6)==3 & norm(VITM(4)-VITS([7,8],:))==0  then // dans le cas d'un sommet, on ne rajoute pas de point du coté de la reflexion
//       return;
//    end
//    if MTS(6)==3 & norm(VITMTS(4)-VIT([7,8],:))==0  then // dans le cas d'un sommet, on ne rajoute pas de point du coté de la reflexion
//       return 
//    end
//    MS=MTS([1,2],:);
//    VS=VITS([1,2],:);
//    V=VIT([1,2],:);
//    IJ=VIT([7,8],:);
//    IJS=VITS([7,8],:);
//    if TS>=T then
//        MS=MS+(T-TS)*VS;
//        DM=MS-M;
//        MM=M;
//    else
//        MM=M+(TS-T)*V;
//        MS=MTS([1,2],:);
//        DM=MS-MM;
//
//end
//if norm(IJ-IJS)<>0 then
//    testS=norm(DM);
//    testmemcell=%F;
//else
//    testS=abs(det([V,DM]))/norm(V)+abs(det([VS,DM]))/norm(VS);
//    testmemcell=%T;
//end
//    //    testS=abs(det([V,DM]))/norm(V);
////testS=norm(DM);    
////disp(norm(DM),"norm(DM)",testS,"testS",abs(det([V,DM]))/norm(V),"abs(det([V,DM]))/norm(V)",crit,"crit");
//    if testS >=crit then
//        //disp(["rajoutepoint : on doit rajouter un point","MIS(1)=",string(MS(1)),"MIS(2)=",string(MS(2))]);
//
//        if size(VITM)+size(VITMTS)==7 then // cas où MT ou MTS est un sommet, on ne fait rien
////            if size(VITM==4) then
////                disp("MT est un sommet et MTS une reflexion");
////            else
////                disp("MTS est un sommet et MT une reflexion");
////            end
//
//        elseif MTS(7)==MT(7) then // les points M et MS proviennent du même point
//            
//            MIS=(MS+MM)/2;
//            MO=LISTFF(2)(MT(7));
//            DIR=MIS-MO([1,2],:);
//            MNS=MNPLUSUN3(MO,DIR,0);
//            IJ= floor((MO([1,2],:)+MNS([1,2],:))/2);
//            VITMIS=vitessemaxA3(DIR,IJ);
//            coul=5;
//            TMIN=MO(3);
//            //MIS=MO([1,2],:);
//            INDO=MT(7);
//            VITS=list(list(VITMIS));
////            disp("------");
////            disp(MS,"MS",MM,"MM");
////            disp("------");
//            LISTFF=propagebis(LISTFF,INDO,IND,VITS);
////            disp(VS,"VS",V,"V",MTS,"MTS",MT,"MT",MO,"MO",MIS,"coucou1 MIS");
//        else // les points M et MS proviennent de points différents
//            //                TMIN=min(T,TS);
//            //                hermite(LISTFF,IND ,INDS,TMIN,5);
//            MM=M+(TMIN - T)*V;
//            MS=MTS([1,2],:)+(TMIN-TS)*VS;
//            MIS=(MS+MM)/2;
//
//            IJ = round(MIS);
//            if (MIS(1)-IJ(1))*(MIS(2)-IJ(2))==0 then
//                disp(INDS,"INDS",IND,"IND",VITS,"VITS",VIT,"VIT",MM,"MM",MTS,"MTS",MT,"MT",MIS,"erreur dans rajoute, MIS est sur un bord");
////                clf(4);
////                scf(4);
////                tracefronttempmin(LISTFF,6);
////                abort;
//                MIS=(2*MS+MM)/3;
//            end
//            //                DIR=(VITS([3,4],:)+VIT([3,4],:))/2;
//            IJ = floor(MIS);
//            DIR=MS-MM;
//            testSS=norm(DIR);
//            if testSS>=crit/5 then
//                VITMIS=vitesseA3(0,DIR,IJ);
//                MIS=[MIS;TMIN;0;1;7;0];
//                [LISTFF,INDICINS]=INSERE(LISTFF,MIS,IND,list(VITMIS),%F,%T,0);
////                               disp(MIS,"on a inséré MIS");
//                INDO=size(LISTFF(2));
//                VITS=list(list(VITMIS));
//                //                scf(7);
//                //                clf(7);
//                //                tracedanscellule(LISTFF,INDO);
////                disp(INDO,"INDO",VITS,"VITS",kmax,"kmax");
////                disp("avant propage1 dans rajoutepoint");
//                [LISTFF,PROP]=propage1(LISTFF,INDO,VITS,0,kmax);
////                if PROP==1 then
////                     scf(7);
////                     clf(7);
////                     tracedanscellule(LISTFF,IND);
////                     MP=MIS([1,2],:)+VITMIS([1,2],:)/4;
////                     point(MIS,2,2);
////                     dessellipse(IJ,6);
////                     tracedanscellulenum(LISTFF,numero(IJ),TMIN);
////                     dessvect(MIS,MP,5);
////                     tracefronttempmin(LISTFF,5,label,%T);
////                     disp("PROP=1 dans rajoutepoint");
////                   //  pause;
////                end
//            end  
//        end
//
//    end
//endfunction
//



function [LISTFFN,LISTAUXX]=rajoutepoints(LISTFF,LISTAUX,IND,crit)
    // rajoute un point  si la distance entre le suivant ou le précédent calculée au temps le plus grand possible est supérieure à crit.

       MT=LISTFF(IND)(1);

      [LISTFFN,LISTAUXX]=rajoutepoint(LISTFF,LISTAUX,IND,crit);

      INDP=MT(6);

      [LISTFFN,LISTAUXX]=rajoutepoint(LISTFFN,LISTAUXX,INDP,crit);

endfunction

function [LISTFFN,LISTAUXX,INDIC] =supprimecroisementraj(LISTFF,LISTAUX,MVT,TMIN)
    // adaptation de supprimecroisement au cas du rajout de point
    // on rajoute MVT à partir d'un point du front à TMIN
    // supprime les points correspondant à des trajectoires moins rapides que 
    // celle de MVT
    // INDIC=1 si MVT croise une trajectoire plus rapide dans le carré, on n'insérera pas MVT.

    LISTFFN=LISTFF;
    LISTAUXX=LISTAUX;
    LISTAUXX(5)=[];


    INDIC=0;
    eps=0.00000000001;
    //    IJ=MVT(2)(1)([5,6],:); correction
    MTP=MVT(1);
    MP=MTP([1,2],:);
    TP=MTP(3);
    VIT=MVT(2)(1);
    V=VIT([1,2],:);
    indasuppr=[];
    IJ=MVT(2)(1)([5,6],:);
    num=numero(IJ);
    [nb,loc]=members([num],LISTAUXX(1));
    if nb <> 0 then // **** sinon on rajoute un point dans un nouveau carré
        for INDC=LISTAUXX(2)(num)
            MTC=LISTFF(INDC)(1);
            MC=MTC([1,2],:);
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
                    if  norm(MCR-MM)>= eps & tesTC >= 0 then 
                        if tesT+eps < tesTC & TC >= TMIN then
//                                disp(['supprimecroisementraj on supprime ',string(INDC)]);
// disp(['tesT =', string(tesT),'TC =', string(TC),'TMIN =', string(TMIN),'aux',string(aux)]);
                            //point(MCR,4,1);
                            indasuppr=[indasuppr,INDC];
                        elseif tesT >tesTC+eps 
                            //                                disp(['tesT =', string(tesT),'TC =', string(TC),'TMIN =', string(TMIN)]);
                            //point(MCR,5,1);
                            INDIC=1;
                            return;
                        elseif  abs(tesTC-tesT) <= eps then
                            INDIC=1;
                            return;
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
