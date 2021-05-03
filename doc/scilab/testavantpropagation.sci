// contient les procédures permettant les tests avant insertion
// on n'insére pas le point sur une arete si les deux sommets sont estimés avoir déja brulé
// le tableaux des sommets avec leur temps d'ignition est LISTAUX(7)


function TEST=testaretebrulee(LISTFF,LISTAUX,MVT) //testaretebruleenew de algo3
    if size(LISTAUX(7),'c')==0 then
        TEST=%F;
        return;
    end
    M=MVT(1);
    IJ = floor(M([1,2],:));
    num=numero(IJ);
    TMIN=LISTFF(LISTAUX(4))(1)(3);
    [nbre,loc]=members([num],LISTAUX(7)([1],:));
    if nbre ==0 then
        TEST=%F;
        return;
    else
        DM=M([1,2],:)-IJ;
        if norm(DM)>0.00000001 then
            IJ1=round(IJ+DM/norm(DM));
            num1=numero(IJ1);
            [nbre,loc1]=members([num1],LISTAUX(7)([1],:));
            if nbre ==0 then
                TEST=%F;
                return;
            else
                IND=LISTAUX(7)(3,loc);
                IND1=LISTAUX(7)(3,loc1);
                TMAX=LISTAUX(7)(2,loc);
                TMAX1=LISTAUX(7)(2,loc1);
                TM=M(3);
                if TMAX<=TMIN & TMAX1<= TMIN then
//                    disp(M,"M",IJ,IJ1,"on veut insérer entre deux sommets déja brulés :");
//                    disp(TMAX1,"TMAX1",TMAX,"TMAX",M(3),"TM",TMIN,"TMIN");
                    TEST=%T; 
                    return;
                end;

                DIJ=LISTAUX(7)([5,6],loc);
                DIJ1=LISTAUX(7)([5,6],loc1);
                if DIJ(1)==0 & DIJ(2)==0 & sum(DIJ1)== 1 & det([DIJ1,IJ1-IJ])==0 then
                    if (TMAX-TM)*(TM-TMAX1)<=0 & TM >= TMAX then
//                        disp(M,"M",IJ,IJ1,"on veut insérer entre deux sommets repertoriés et il y a un point plus performant :");
//                        disp(TMAX1,"TMAX1",TMAX,"TMAX",M(3),"TM",TMIN,"TMIN");
//                         disp(LISTAUX(7)(:,loc),"S",LISTAUX(7)(:,loc1),"S1"); 
//                         point(M,5,1);
                        TEST=%T;
                    else
//                        disp(M,"M",IJ,IJ1,"on veut insérer entre deux sommets repertoriés et il y a un point moins performant :");
//                        disp(TMAX1,"TMAX1",TMAX,"TMAX",M(3),"TM",TMIN,"TMIN");
//                        pause;
                        TEST=%F;
                    end
            else
                TEST=%F;
            end
            end
        else
            
            IND=LISTAUX(7)(3,loc);
            TMAX=LISTAUX(7)(2,loc);
            TM=M(3);
            if TM > TMAX then
                 disp(LISTFF(IND),"déja brulé par ",IJ,"sommet ",M,"M");
            TEST=%T;
//            pause;
        else
            TEST=%F;
            end
        end
    end 
endfunction


function LISTAUXX=actualisesommet(LISTFF,LISTAUX,IND)
//IND est l'indice d'un point 
//actualise les temps d'ignition des sommets dans la prochaine cellule avec des vitessesmax issues de IND)      
LISTAUXX=LISTAUX;
MT=LISTFF(IND)(1);
IJO=LISTFF(IND)(2)(1)([5,6],:);
IJN=IJO+MT([4,5],:);
M=MT([1,2],:);
T=MT(3);
DM=M-round(M);
//    disp(DM,"DM",M,"M")
if norm(DM)<= 0.0000000000000001 then // MT est un sommet
    IJ=round(MT([1,2],:));
    TNEW=MT(3);
    num=numero(IJ);
    [nbre,loc]=members([num],LISTAUX(7)(1,:));
    if MT(7)==5 then
        INDIC=%F;
        LISTFF(2)(IND)(7)=3;
    else
        INDIC=%T;
    end
    if nbre==0 then
        LISTAUXX(7)=[LISTAUXX(7),[num;TNEW;IND;INDIC;0;0]];
    else
        TOLD=LISTAUXX(7)(2,loc);
        if TNEW<=TOLD then
            //                disp(LISTAUX(7),"LISTAUX(7)",loc,"loc");
            LISTAUXX(7)=[LISTAUXX(7)(:,[1:loc-1]),[num;TNEW;IND;INDIC;0;0],LISTAUXX(7)(:,[loc+1:$])];
        end
    end
    return;
else
    listDIJ=list([0;0],[1;0],[1;1],[0;1]);
    for DIJ=listDIJ
        IJ=IJN+DIJ;
        INDIC=%F;
        DIR =IJ - M;
        VIT=vitessemax(DIR,IJN);
        DT=norm(DIR)/norm(VIT([1,2],:));
        TNEW=T+DT;
        num=numero(IJ);
        //            disp(IJ,"IJ",num,"num");
        [nbre,loc]=members([num],LISTAUXX(7)(1,:));
        if nbre==0 then
            LISTAUXX(7)=[LISTAUXX(7),[num;TNEW;IND;INDIC;DIJ]];
        else
            TMAXOLD=LISTAUXX(7)(2,loc);
            INDICOLD=LISTAUXX(7)(4,loc);
            if TNEW<=TMAXOLD & INDICOLD <> 1 then
                //                disp(LISTAUX(7),"LISTAUX(7)",loc,"loc");S1
                LISTAUXX(7)=[LISTAUXX(7)(:,[1:loc-1]),[num;TNEW;IND;INDIC;DIJ],LISTAUXX(7)(:,[loc+1:$])];
            elseif TNEW<=TMAXOLD & INDICOLD == 1
                LISTAUXX(7)=[LISTAUXX(7)(:,[1:loc-1]),[num;TNEW;IND;INDIC;DIJ],LISTAUXX(7)(:,[loc+1:$])];
            end
        end
    end
end
endfunction
