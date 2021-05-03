// CORRIGER vitesse_new_1 pour le transformer en vitesse_new_2
// UNE ERREUR A CORRIGER DANS vitessechocoupas

//RAJOUTER LISTV dans vitesse_sommet_new kmax


function V=vitesse_opt(theta,R,epsilon,alpha,phi) // fonction qui permet de dessiner l'ellipse 3D

// calcul de la normale unitaire au plan physique
N=[sin(phi)*sin(alpha);-cos(phi)*sin(alpha);cos(alpha)];


// calcul du petit axe de l'ellipse physique
RVN=cross(R,N);

aux= 1-epsilon*cos(theta);
auxR=(cos(theta)-epsilon)/(aux*(1+epsilon));
auxRVN=(1-epsilon)*sin(theta)/aux;
V=auxR*R+auxRVN*RVN;  

endfunction


function V=vitessea0(a0,tau,IJ)
    // calcule la vitesse V et la tangente au front de flamme au sein d'une cellule IJ 
    // a0 coefficient d'entrée dans la cellule,
    // pour obtenir la vitesse correspondant à un front de flamme de tangente tau, faire a0=0,
    // tau vecteur tangent au front de flamme (ou à la face d'entrée dans la cellule si a0 n'est pas égal à 0), ATTENTION, IL DOIT ETRE DANS LE SENS DES AIGUILLES D'UNE MONTRE.
    E=ellipse(IJ);
    c=E(1,[1,2])';
    e=E(1,[3,4])';
    a= E(5);
    b= E(6);
    //   //disp(E,"ellipse =");
    //   //disp(tauperp,"tauperp=");
    tau=tau([1,2],:);// A CORRIGER 
    tau=tau/norm(tau);
    tauperp(1)=-tau(2);
    tauperp(2)=tau(1);
    eperp=[-e(2);e(1)];
// calcul des coordonnées de tau-a0*c dans le repère de l'ellipse
    W0=tau+a0*c;
    W01=W0'*e;
    W02=W0'*eperp;
    AW0=(W01/a^2)*e+(W02/b^2)*eperp;
    W0AW0=W0'*AW0;
    delta=W0AW0-a0^2;
    //   //disp(W0AW0,delta,"delta=");
    if (delta>=0) then
        W1(1)=-AW0(2);
        W1(2)=AW0(1);
        test=W1'*tauperp;
        //       //disp(test,"test=");
        if test <0 then
            test= 1;
        else
            test=-1;
        end
        aux1=a0/W0AW0;
        aux2=a*b*sqrt(delta)/W0AW0;
        V=-c+aux1*W0+test*aux2*W1;
        TFF= aux1*W1- (test*aux2/(a^2*b^2))*W0;
        normTFF=norm(TFF);
        if det([V,TFF])<=0 then
            TFF=-TFF;
            //disp("det([V,TFF])<=0");
        end;
        V(3)=TFF(1)/normTFF;
        V(4)=TFF(2)/normTFF;
        INDIC=0;
    else
        V=[0;0;0;0];
        INDIC=10;
    end
    V=[V([1:4]);IJ;INDIC];
endfunction


function VOPT=vitessemax(VD,IJ) //calcule la vitesse optimale dans la direction VD dans la cellule IJ

    E=ellipse(IJ);
    c=E(1,[1,2])';
    e=E(1,[3,4])';
    a= E(5);
    b= E(6);
//           //disp(E,"E=");
VD=[VD(1);VD(2)];

if norm(VD)<0.000000001 then
     disp("erraur dans vitessemax, VD=O");
     abort;
end

    VD=VD/norm(VD);
    
    // coordonnées de VD dans le repère de l'ellipse
        VDE =[e'*VD;det([e,VD])];

    // coordonnées de C dans le repère de l'ellipse
        CE= [e'*c;det([e,c])];
        
    AVDVD=VDE(1)^2/a^2+VDE(2)^2/b^2;
    AVDC=VDE(1)*CE(1)/a^2+VDE(2)*CE(2)/b^2;
    ACC=CE(1)^2/a^2+CE(2)^2/b^2;
    delta=sqrt(AVDC^2-AVDVD*(ACC-1));
    aux1=-AVDC+delta;
    aux1=aux1/AVDVD;
    V=aux1*VD;
    // calcul de la tangente 
    aux3=(VDE(1)*aux1+CE(1))/a^2;
    aux4=(VDE(2)*aux1+CE(2))/b^2;
    TFF=[-aux3*e(2)-aux4*e(1);aux3*e(1)-aux4*e(2)];
    normTFF=norm(TFF);

    if det([V,TFF])<=0 then
       TFF=-TFF;
   end;
   TFF=TFF/normTFF;
   INDIC=0;

VOPT=[V;TFF;IJ;INDIC];

endfunction


function LISTVN=vitessesrajoutes(IJ,V0,V1,kmax)
    
    // rajoute des vitesses optimales entre les directions V0 et V1
    // V0 et V1 doivent être dans la même cellule IJ
    // kmax nombre de vitesses rajoutées
    LISTVN=list();
    E=ellipse(IJ);
    c=E(1,[1,2])';
    e=E(1,[3,4])';
    a= E(5);
    b= E(6);

    VV0=V0([1,2],:);
    VV1=V1([1,2],:);
    VV0=vitessemax(VV0,IJ);
    VV1=vitessemax(VV1,IJ);
    VV0=VV0([1,2],:);
    VV1=VV1([1,2],:);
    // coordonnées dans le repère de l'ellipse
    V00=[VV0'*e,det([e,VV0])];
    V11=[VV1'*e,det([e,VV1])];
    CE= [e'*c;det([e,c])];
    // angle origine au centre de l'ellipse
    theta0=atan((V00(2)+CE(2))/b,(V00(1)+CE(1))/a);
    thetaf=atan((V11(2)+CE(2))/b,(V11(1)+CE(1))/a);

    if thetaf <theta0-0.0000000000001 then
        thetaf=thetaf+2*%pi;
    end
//    disp(e,"e");
//    disp(theta0,"theta0");
//    disp(thetaf,"thetaf");
//   pause; 
    e1=e(1);
    e2=e(2);
    T1=atan(-e1/a,-e2/b);
    if T1 <theta0-0.0000000000001 then
        T1=T1+2*%pi;
    end
    T2=atan(-e2/a,e1/b);
    if T2 <theta0-0.0000000000001 then
        T2=T2+2*%pi;
    end
    T3=atan(e1/a,e2/b);
    if T3 <theta0-0.0000000000001 then
        T3=T3+2*%pi;
    end
    T4=atan(e2/a,-e1/b);
    if T4 <theta0-0.0000000000001 then
        T4=T4+2*%pi;
    end

    listheta=[T1,T2,T3,T4];
    KMAX=floor((thetaf-theta0)*kmax/(2*%pi))+1;
    for k=0:KMAX
        listheta=[listheta,theta0+(thetaf-theta0)*k/KMAX]; 
    end

    listhetab=round(100000*listheta);

    [listhetab,ordre]=unique(listhetab);

    for k=1:size(listhetab,"c")
        t=listheta(ordre(k));
        if t<=thetaf  then
            V(1)= -c(1)+a*cos(t)*e1-b*sin(t)*e2;
            V(2)= -c(2)+a*cos(t)*e2+b*sin(t)*e1;
            V3=-a*sin(t)*e1-b*cos(t)*e2;
            V4=-a*sin(t)*e2+b*cos(t)*e1;
            normTFF=sqrt(V3*V3+V4*V4);
            V(3)=V3/normTFF;
            V(4)=V4/normTFF;
            V(5)=IJ(1);
            V(6)=IJ(2);
            V(7)=0;
            LISTVN($+1)=list(V);
        end
    end
//  disp(LISTVN,"LISTVN"); 
endfunction

function [VITC,INDIC]=vitessechocoupas(VIP,VIS)
// VIP vitesse précédente
// VIS vitesse suivante
// VITC = liste de vitesse
// INDIC = 0 si pas de choc, INDIC =  1 si c'est un choc, INDIC = 2 si VIP=VIS
// on n'a pas besoin de l'ellipse car VIP et VIS contiennent les directions conjuquées (tangentes au front)),

    VP=VIP([1,2],:);
    TP=VIP([3,4],:);
    VS=VIS([1,2],:);
    TS=VIS([3,4],:);
    IJP=VIP([5,6],:);
    IJS=VIS([5,6],:);
    INDICP=VIP(7);
    INDICS=VIS(7);
    test= norm(IJP-IJS)+INDICS+INDICP;
    if test >= 0.0000000000001 then
        disp(test,"erreur dans VIP ou VIS dans vitessechocoupas test=");
        abort;
    end
    test = det([TP,TS]);

    if norm(VP-VS)<0.00000001
        INDIC=2;
        VITC=list(list(VIP));
    elseif test < -0.0000000001 then
        alpha=det([VS-VP,TS])/test;
        VC=VP+alpha*TP;
        VITC=list(list([VC;0;0;IJP;1],VIP,VIS));
        INDIC=1;
    else
        LISTVN=vitessesrajoutes(IJP,VIP,VIS,kmax);  // ERREUR A CORRIGER VERIFIER SI c'est bien IJP
        LISTVN(1)=null();
        LISTVN($)=null();
        VITC=lstcat(list(list(VIP)),LISTVN,list(list(VIS)));
        INDIC=0;
    end
    //    //disp(VITC,"VITC");
endfunction



function a0=azero(tau,VIT)
    // calcul la valeur de a0 pour la vitesse V, la cellule est donnée par V([5,6],:)
    // tau vecteur tangent dans le sens des aiguilles d'une montre (partie brulée à gauche de tau)
    IJ = VIT([5,6],:);
//    disp(IJ,"IJ");
    E=ellipse(IJ);
    c=E(1,[1,2])';
    e=E(1,[3,4])';
    a= E(5);
    b= E(6);
    eperp=[-e(2);e(1)];

    V=VIT([1,2],:);
    // coordonnées de W=C+V et de AW dans le repère de l'ellipse
    W=V+c;
    VE=[e'*W,eperp'*W];
    AW=[VE(1)/a^2,VE(2)/b^2];
   //AW dans le repère de calcul
   AW=AW(1)*e+AW(2)*eperp;
//calcul de a0
    aux1=AW'*tau;
    aux2=AW'*V;
    a0=aux1/aux2;
endfunction



function [VITS,INDIC]=vitesse_choc_new(VITS,TAUPERP)
    // VITS en entrée est une vitesse de choc (une liste de trois vitesses)
    // VITS en sortie est la vitesse de choc (encore une liste de trois vitesses) dans la 'nouvelle' cellule    // l'ancienne cellule est données par IJ=VITS(1)([5,6])
    // la nouvelle cellule est donnée par IJN=IJ+TAUPERP
    // INDIC =%T : tout s'est bien passé
    // INDIC=%F : on a rencontré un probléme 
    //    - on est sur un sommet (TAUPERP=(1,1) par exemple)
    //    - une des vitesses constitutives du choc est réfléchie
    // si INDC =%F, on supprimera le point 
    disp("vitesse_choc_new");
    if size(VITS)<>3 then
        disp(VITS,"erreur 1 dans vitesse_choc_new, VITS =");
        abort;
    end
    if TAUPERP(1)*TAUPERP(2)<>0 then
        disp(VITS,"erreur 2 dans vitesse_choc_new, VITS =");
        abort;
    end
//    disp(VITS,"VITS en entree de vitesse_choc_new");
    VC=VITS(1);
    IJ=VC([5,6],:);
    IJN=IJ+TAUPERP;
    VP=VITS(2);
    VS=VITS(3);
    tau=[-TAUPERP(2);TAUPERP(1)];
    a0s=azero(tau,VS);
    a0p=azero(tau,VP);
    VNS=vitessea0(a0s,tau,IJN);
    VNP=vitessea0(a0p,tau,IJN);
    if VNS(7)==0 & VNP(7)==0 then
        [VITS,INDIC1]=vitessechocoupas(VNP,VNS);
//        disp(VITS,"VITS",INDIC1,"INDIC1");
//        pause;
        if INDIC1<>1 then
            INDIC=%F;
            return;
        else
            INDIC=%T;
            end
        
    else
        INDIC=%F;
        return;
    end
endfunction



function [VITSS,INDIC]=vitesse_new_2(VITS,tauperp)
//    disp("vitesse_new_1");
    VIT=VITS(1);
    IJ=VIT([5,6],:);
    tau=[-tauperp(2);tauperp(1)];
    IJN=IJ+tauperp;
    a0=azero(tau,VIT);
    V=vitessea0(a0,tau,IJN);

    if V(7) == 0 then
        VITSS=list(list(V));
        INDIC=%T;
    else
        // on calcule la vitesse de bord puis la vitesse de reflexion dans le sens de la propagation
        if a0 > 0.00000000001
            VB=vitessemax(tau,IJN);
            b0=1/norm(VB([1,2],:));
            VR=vitessea0(-b0,-tau,IJ);
            [VITSS,INDIC1]=vitessechocoupas(VR,VIT);
            LISTVN=vitessesrajoutes(IJN,tauperp,tau,kmax);
            LISTVN(1)=null();// CORRECTION
            LISTVN($)=null();
            VITSS=lstcat(LISTVN,list(list(VB,VR)),VITSS);//CORRECTION
        elseif a0 < 0.00000000001
            VB=vitessemax(-tau,IJN);
            b0=1/norm(VB([1,2],:));
            VR=vitessea0(b0,-tau,IJ);
            [VITSS,INDIC1]=vitessechocoupas(VIT,VR);
            LISTVN=vitessesrajoutes(IJN,-tau,tauperp,kmax);
            LISTVN(1)=null();
            LISTVN($)=null();//CORRECTION
            VITSS=lstcat(VITSS,list(list(VB,VR)),LISTVN);//CORRECTION
        else
            disp('erreur dans vitesse_new, a0=0');
            abort;
        end
//        disp(VITSS,'VITSS',VR,'VR',VB,'VB');

        // VITS=list(list(V));
        INDIC=%T;
    end
endfunction

function [VITS,INDIC,LISTAUX]=vitesse_sommet_new(VITS,tauperp,LISTAUX) //*****CORRECTION RAJOUTER LISTAUX******
    // VITS a deux éléments une vitesse de bord optimale et la vitesse de reflexion correspondante
    LISTV=LISTAUX(6);// ******** correction ******************
    if size(VITS)<>2 then
        disp("erreur 1 dans vitessesommetnew");
        abort;    
    end
    VB=VITS(1)([1,2],:);
    IJB=VITS(1)([5,6],:);
    VR=VITS(2)([1,2],:);
    IJR=VITS(2)([5,6],:);
    TAU=IJB-IJR;
    if det([VB,TAU])<=0 then
        TAU=-TAU;
        VPO=VITS(1);
        VSO=VITS(2);
        IJPO=IJB;
        IJSO=IJR;
    else
        VPO=VITS(2);
        VSO=VITS(1);
        IJPO=IJR;
        IJSO=IJB;
    end
    TAUPERP=[TAU(2);-TAU(1)];
    IJPN=IJPO+TAUPERP;
    IJSN=IJSO+TAUPERP;
    //++++++++++++++++++++++++++++
    //on traite le bord IJP IJPN
    //++++++++++++++++++++++++++++
    if VPO(7)<>0  then // si VPO n'est pas une vitesse optimale, c'est une vitesse de bord générée par la propagation d'un front, il faut remplacer VPO par la vitesse de ce front
        VPO=LISTV(VPO(7));
    end
    [VITSP,VITBP,LISTV]=vitessebord2(VPO,IJPN,LISTV,"P");
    //++++++++++++++++++++++++++++
    //on traite le bord IJS IJSN
    //++++++++++++++++++++++++++++   
    if VSO(7)<>0 then
        VSO=LISTV(VSO(7));
    end
    [VITSS,VITBS,LISTV]=vitessebord2(VSO,IJSN,LISTV,"S");
    //++++++++++++++++++++++++++++
    //on traite le bord IJPN IJSN
    //++++++++++++++++++++++++++++   
    [VITS,LISTV]=vitessesbord(VITSP,VITSS,LISTV);
    INDIC=%T;
//    disp(LISTV,'LISTV');
//    pause;
    LISTAUX(6)=LISTV; // ******** correction ******************
endfunction

function [VITS,VITB,LISTV]=vitessebord2(VIT,IJN,LISTV,COTE)
    // VITS listes des vitesses 
    // VITB : vitesse de bord (inutilisée ici)
    // LISTV : vitesse de reflexion si reflexion
    //n'est utilisée que dans "vitessesommetnew"
    if VIT(7)<>0 then
        disp(VIT,"VIT n est pas une vitesse optimale dans vitessebord2");
        abort;
    end     
    IJ=VIT([5,6],:);
    TAUPERP=IJN-IJ;// vecteur orthogonal au bord
    TAU=[-TAUPERP(2);TAUPERP(1)];// vecteur tangent au bord de la cellule d'origine le sommet
    if COTE == "P" then
        TAU=-TAU;
    end

    TA=VIT([3,4],:);
    aux=det([TAU,TA]);
    signe=sign(aux);
    VI=VIT([1,2],:);
    auxv=TAUPERP'*VI;// si >0 la vitesse rentre dans le bord
//    disp(auxv,"auxv",COTE,"COTE");
    if abs(aux)> 0.000000000001 then
        VITBN=vitessemax(signe*TAU,IJN);
        if auxv < 0 then //la vitesse ne rentre pas le bord, la demi-arete correspondante est déjà brulée
           // disp(VIT,"VIT",aux,"aux",auxv,"auxv",COTE,"COTE");
            VITBO=vitessemax(signe*TAU,IJ);
            test=signe*(VITBN([1,2],:)-VITBO([1,2],:))'*TAU;
 //           disp(test,"test",COTE,"COTE");
            if test >0.000000000001 then // reflexion
                disp("reflexion.");//****correction******
//                disp(VITBN,'VITBN',VITBO,'VITBO',test,'test');
                V=vitesse(VITBN,IJ);
                VITB=list(list(VITBN,V));
                if COTE == "P" then
                        VITS=VITB;
                    else
                        VITS=VITB;
                end 
//                disp(VITS,"VITS");
//                dessellipse(IJ);
//                dessellipse(IJN);
//                pause;    
            elseif test <-0.000000000001 //IJ enflamme IJN
                V=vitesse(VITBO,IJN);
                VITB=list(list(VITBO,V));
                if COTE == "P" then
                        VITS=list(list(V));
                    else
                        VITS=list(list(V));
                    end            
            else // test=0
                V=vitesse(VITBN,IJ);
                VITB=list(list(VITBN,V));
                if COTE == "P" then
                        VITS=VITB;
                    else
                        VITS=VITB;
                    end                
            end 
 
        else //la vitesse rentre le bord
            auxx=det([VI,TA])/aux;
            VITA=auxx*TAU;
            LISTV($+1)=VIT;
            VB=[VITA;TA;VIT([5,6],:);size(LISTV)];// VB n'est pas optimal, on stocke la vitesse optimale correspondante
//            disp(auxx,"auxx");
            if auxx < 0 then // on se rapproche du sommet
                VBN=vitessemax(-TAU,IJN);
                test=-(VBN([1,2],:)-VITA)'*TAU;
                if test >= 0 then
                    V=vitesse(VBN,IJ);
                    VITB=list(VBN,V);
                    VITS=list(VITB);
                else
                    V=vitesse(VB,IJN);
                    VITB=list(VB,V);
                    VITS=list(list(V));
                end
                VITB=list(VITB);
            else // on s'éloigne du sommet
                test=(VITBN([1,2],:)-VITA)'*TAU;
                
                if test >= 0 then // reflexion
                    V=vitesse(VITBN,IJ);
                    VITB=list(list(VITBN,V));
                    if COTE == "P" then
                        VITC=vitessechocoupas(VIT,V);
                        LISTVN=vitessesrajoutes(IJN,TAU,TAUPERP,kmax);
                        LISTVN(1)=list(VITBN,V);
                        VITS=lstcat(VITC,LISTVN);
//                        disp(VITS,"VITS 3");
                else
                        VITC=vitessechocoupas(V,VIT);
                        LISTVN=vitessesrajoutes(IJN,TAUPERP,TAU,kmax);
                        LISTVN($)=list(VITBN,V);
                        VITS=lstcat(LISTVN,VITC);
//                        disp(VITC,"VITC",V,"V",VIT,"VIT dans vitessebord2");
                    end
                else  //IJ enflamme IJN
                    V=vitesse(VB,IJN);
                    VITB=list(list(VB,V));
                    VITS=list(list(V));          
                end               
            end
        end
    else // le front dans IJ est parallèle au bord
        disp(IJN,"IJN",IJ,"IJ","vitessebord2 : le front dans IJ est parallèle au bord");
        V=vitesse0(IJ,IJN);
        VITS=list(list(V));
        VITB=list();
//disp(VITS,"VITS 4");
    end
endfunction

function V=vitesse(VB,IJ)
    //résoud grad E(V).V=grad E(V).VB, E(V)=1 dans la cellule IJ
    //cellule de la vitesse dans la cellule  IJO =VB([5,6],:) voisine de IJ
    E=ellipse(IJ);
    c=E(1,[1,2])';
    e=E(1,[3,4])';
    a= E(5);
    b= E(6);
    //   //disp(E,"ellipse =");
    //   //disp(tauperp,"tauperp=");
    VV=VB([1,2],:);
    IJO=VB([5,6],:);
    tauperp=IJ-IJO;
    if norm(tauperp)<>1 then
        disp('erreur dans vitesse' );
        abort;
    end
    tau=[tauperp(2);-tauperp(1)];
    a0=1/(VV'*tau);
    if norm(tauperp)<>1 then
        disp('erreur dans vitesse' );
        abort;
    end
    V=vitessea0(-a0,-tau,IJ);
    
endfunction

function [VITS,LISTV]=vitessesbord(VITSP,VITSS,LISTV)
    
    // renvoie une liste contenant la vitesse de propagation le long du bord, la vitesse de reflexion  et la cellule de reflexion, utilisée dans vitessesommet.
    //LISTV contient des vitesses de reflexions qu'on stoke pour un calcul ultérieur
//    disp("on rentre dans vitessesbord");
    INDIC=%T;
    VP=VITSP($)(1);
    VS=VITSS(1)(1);
    LISTAR=list();
    if VS(7)<>0 then
        disp("erreur dans vitessesbord VS(7)<>0");
        abort;
    end
    if VP(7)<>0 then
        disp("erreur dans vitessesbord VP(7)<>0");
        abort;
    end

    IJP=VP([5,6],:);
    IJS=VS([5,6],:);
    TAUPERP=IJS-IJP;// vecteur orthogonal au bord
    TAU=[TAUPERP(2);-TAUPERP(1)];// vecteur tangent au bord de la cellule vers zone non brulée

    TA=VP([3,4],:);
    aux=det([TAU,TA]);      
    VI=VP([1,2],:);
    auxp= VI'*TAUPERP; // test pour savoir si "on rentre" dans la cellule voisine
//   disp(aux,"aux P",test1P,"test1P",TAU,"TAU",TAUPERP,"TAUPERP");
//+++++++++++++++++++++++++++++++++ 
// on regarde comment VITSP se comporte sur l'arete principale
//+++++++++++++++++++++++++++++++++  
    TA=VP([3,4],:);
    aux=det([TAU,TA]); 
    VI=VP([1,2],:);
    auxv=TAUPERP'*VI;// si >0 la vitesse VP rentre dans le bord
    if abs(aux)> 0.000000000001 then
        if auxv < 0 then //la vitesse VP ne rentre pas le bord
            VNBP=vitessemax(TAU,IJP);
            typ="N";// //Bord Pas Brulé par P
        else //la vitesse rentre dans le bord
            auxx=det([VI,TA])/aux;
            VITA=auxx*TAU;
            LISTV($+1)=VP;
            VNBP=[VITA;TA;VP([5:6],:);size(LISTV)];// VB n'est pas ooptimal, on stocke la vitesse optimale correspondante
            if auxx < 0 then // on se rapproche du sommet
                VPS=vitesse(VNBP,IJS);
                typ="BBPP"; //Bord Brulé Par P
            else // on s'éloigne du sommet
                typ="N"; //normal          
            end
        end
    else // le front dans IJ est parallèle au bord
        disp("le front provenant P est parallèle au bord");
        typ='BBPPP'
        if auxv < 0 then //la vitesse VP ne rentre pas le bord
                //Bord Brulé Par P parallèle S Brule P
               VPS=vitesse0(IJS,IJP);
        else //la vitesse rentre dans le bord
 //Bord Brulé Par P parallèle P Brule S
               VPS=vitesse0(IJP,IJS);
        end

        
    end
//+++++++++++++++++++++++++++++++++
// on regarde comment VITSS se comporte sur l'arete principale
//+++++++++++++++++++++++++++++++++    
    TA=VS([3,4],:);
    aux=det([TAU,TA]); 
    VI=VS([1,2],:);
    auxv=-TAUPERP'*VI;// si >0 la vitesse VS rentre dans le bord
    if abs(aux)> 0.000000000001 then
        if auxv < 0 then //la vitesse ne rentre pas le bord
            VNBS=vitessemax(TAU,IJS);
            tys="N";// //Bord Pas Brulé par S
        else //la vitesse rentre dans le bord
            auxx=det([VI,TA])/aux;
            VITA=auxx*TAU;
            LISTV($+1)=VS;
            VNBS=[VITA;TA;VS([5:6],:);size(LISTV)];// VB n'est pas optimal, on stocke la vitesse optimale correspondante
//            disp(VNBS,"VNBS",VS,"VS");
            if auxx < 0 then // on se rapproche du sommet
//                VSP=vitesse(VNBS,IJP);
                tys="BBPS"; //Bord Brulé Par S
            else // on s'éloigne du sommet
                tys="N"; //normal          
            end
        end
    else // le front dans IJ est parallèle au bord
        disp("le front provenant de S est parallèle au bord");
        tys="BBPSP"; //Bord Brulé Par S Parallele
        if auxv < 0 then //la vitesse VS ne rentre pas le bord
                 //Bord Brulé Par S parallèle P Brule S
               VSP=vitesse0(IJP,IJS);
        else //la vitesse rentre dans le bord
                 //Bord Brulé Par P parallèle S Brule P
               VSP=vitesse0(IJS,IJP);
        end

    end
//+++++++++++++++++++++++++++++
//disp(VS,"VS",VP,"VP");
disp(tys,"tys",typ,"typ");

if typ == "N" & tys== "N" then // les deux vitesses traversent l'arete principale, on cherche la plus rapide 
    test =norm(VNBS([1,2],:)) -norm(VNBP([1,2],:));
    disp(test,"test",VNBS,"VNBS",VNBP,"VNBP",VITSP,'VITSP',VITSS,'VITSS');
    if test > 0.000000000000000001 then // La cellule S "allume" la cellule P
            VR=vitesse(VNBS,IJP);
            VNB=list(VNBS,VR);
            [VITC,INDICVITC]=vitessechocoupas(VP,VR);
            if INDICVITC==0 & size(VITSP(1))== 2 then //***** CORRECTION
                disp(VITSP,'VP vitesse de bord');
                VITC(1)=VITSP(1);
            end  // ***** fin correction
//            disp(VITC,"VITC");
            if TAU'*VITC(1)(1)([1,2],:)>=0 then
                VIT= VITSS(1)(1)([1,2],:);
                if det([VNBS([1,2],:),VIT]) >0.0000001 then
                    VITS=lstcat(VITC,list(VNB),VITSS);
                else
                    VITSS(1)=null();
                    VITS=lstcat(VITC,list(VNB),VITSS);
                end
            else // la vitesse VR rentre dans le coté P
               //disp(VR,"VR",IJP,"IJP",TAU,"TAU");
                [VITRR,VITB,LISTV]=vitessebord2(VR,IJP-TAU,LISTV,"S");
                //***** a verifier et corriger********
                if VITRR(1)(1)([1,2],:)'*TAUPERP<0 then
                 VITS=lstcat(VITRR,list(VNB));   
                else
                VITS=lstcat(VITSP,list(VNB));
            end
            //********************************************
            end
            
    elseif test<-0.000000000000000001 // La cellule P "allume" la cellule S
            VR=vitesse(VNBP,IJS);
            VNB=list(VNBP,VR);
            [VITC,INDICVITC]=vitessechocoupas(VR,VS);
             if INDICVITC==0 & size(VITSS(1))==2 then //***CORRECTION
                disp(VITSS,'VS vitesse de bord');
                VITC($)=VITSS(1);
            end  //***fin CORRECTION
            if TAU'*VITC(1)(1)([1,2],:)>=0 then
                VIT= VITSP($)(1)([1,2],:);
                if det([VNBP([1,2],:),VIT])< -0.0000001 then
                    VITS=lstcat(VITSP,list(VNB),VITC);
                else
                    VITSP($)=null();
                    VITS=lstcat(VITSP,list(VNB),VITC);
                end
           else // la vitesse VR rentre dans le coté S
//               disp(VR,"VR",IJS,"IJS",TAUPERP,"TAUPERP");
               [VITRS,VITB,LISTV]=vitessebord2(VR,IJS-TAU,LISTV,"P");
                //***** à verifier et corriger********
                if VITRS(1)(1)([1,2],:)'*TAUPERP>0 then
                 VITS=lstcat(list(VNB),VITRS);
                else
                VITS=lstcat(list(VNB),VITSS);
                
            end
            //********************************************
            end
         //   disp(VITS,"VITS");
    else // égalité
        VR=vitesse(VNBP,IJS);
        VNB=list(VNBP,VR);
        VITC=vitessechocoupas(VR,VS);
//            disp(VR,"VR",VS,"VS",VITC,"VITC");
            if TAU'*VITC(1)(1)([1,2],:)>=0 then
                
                VIT= VITSP($)(1)([1,2],:);
                if det([VNBP([1,2],:),VIT])< -0.0000001 then
                    VITS=lstcat(VITSP,list(VNB),VITC);
                else
                    VITSP($)=null();
                    VITS=lstcat(VITSP,list(VNB),VITC);
                end
           else // la vitesse VR rentre dans le coté S
//               disp(VR,"VR",IJS,"IJS",TAUPERP,"TAUPERP");
               [VITRS,VITB,LISTV]=vitessebord2(VR,IJS-TAU,LISTV,"P");
                VITS=VITRS;
            end
    end
    
elseif typ <> "N" & tys== "N"
        VITS=list();
elseif typ == "N" & tys <> "N"  
       VITS=list();
elseif typ=="BBPPP" & tys=="BBPSP"
    disp(tys,"tys",typ,"typ");
//    disp(VSP,"VSP",VPS,"VPS");
    if norm(VSP-VPS)==0 then
        if norm(VSP([5,6],:)-IJS)==0 then
            VITS=VITSS;
        else
            VITS=VITSP;
        end
    else
        disp('dans vitessesbord, il faut supprimer le point');
        abort;
    end

else
    disp("dans vitessesbord ")
    disp(tys,"tys",typ,"typ");
    disp(VSP,"VSP",VPS,"VPS");
    abort;
end

endfunction

