function E=parametres_ellipse_proj(R,epsilon,alpha,phi)
//E=[cx,cy,ex,ey,a,b] c point d'ignition, e vecteur unitaire du grand axe
// verification de la cohérence
test=R'*N;
if test <> 0 then
//    disp("R n est pas dans le plan, on modifie R_z");
    R(3)=R(3)-test/cos(alpha);
end    
    
// calcul de la normale unitaire au plan physique
N=[sin(phi)*sin(alpha);-cos(phi)*sin(alpha);cos(alpha)];
// calcul du petit axe de l'ellipse physique
RVN=cross(R,N);
// calcul du point d'ignition
C=-(epsilon/(1+epsilon))*R;
// de sa projectio
c=C([1,2],:);
//vecteurs auxiliaires projection des grand-axe et petit axe de l'ellipse physique
rn=sqrt((1-epsilon)/(1+epsilon))*RVN([1,2],:);
r=1/(1+epsilon)*R([1,2],:);
//calcul des axes de l'ellipse projetée
aux1=rn'*rn-r'*r;
aux2=rn'*r;
if abs(aux1)<=0.00000000000001 then // on a à faire à un cercle
    normr=norm(rn);
    E=[0,0,1,0,normr,normr];
else
    theta=atan(aux2/aux1)/2;
    ax1=cos(theta)*r+sin(theta)*rn;
    ax2=-sin(theta)*r+cos(theta)*rn;
    normax1=norm(ax1);
    normax2=norm(ax2);
    ax1=ax1/normax1;
    ax2=ax2/normax2;
    if normax1 > normax2 then
       E=[c',ax1',normax1,normax2]; 
    else
        E=[c',ax2',normax2,normax1];
    end  
end
//disp(rn,"rn",r,"r",normax1*ax1,"ax1",normax2*ax2,"ax2");
endfunction


function E=ellipse(IJ)
    
    R=[0.1+IJ(2)/3;0.2+IJ(1)/3-IJ(2);1];
    epsilon=0.9;
    alpha=(IJ(1)-IJ(2))/(abs(IJ(1))+abs(IJ(2))+1);
    phi=IJ(1)+IJ(2);

    N=[sin(phi)*sin(alpha);-cos(phi)*sin(alpha);cos(alpha)];
    test=R'*N;

    if test <> 0 then
//        disp("R n est pas dans le plan, on modifie R_z");
        R(3)=R(3)-test/cos(alpha);
    end

    E=parametres_ellipse_proj(R,epsilon,alpha,phi);
    
endfunction






function dessellipse(IJ)// dessine une ellipse
    coul=color(155,155,155);
    ech=1;
    E=ellipse(IJ);
    c=E(1,[1,2])';
    e=E(1,[3,4])';
    a= E(5);
    b= E(6);
    i=IJ(1);
    j=IJ(2);

    e1=e(1);
    e2=e(2);
    a= a/ech;
    b= b/ech;
    c1= c(1)/ech;
    c2= c(2)/ech;
    t=[0:0.01:2*%pi];
    plot2d(i+0.5-c1+a*cos(t)*e1-b*sin(t)*e2,j+0.5-c2+a*cos(t)*e2+b*sin(t)*e1,coul);
    plot2d([i,i+1,i+1,i,i],[j,j,j+1,j+1,j],4) ;
    plot2d(i+0.5+0.005*cos(t),j+0.5+0.005*sin(t),coul);
endfunction



function tracefront(LISTFF,IND)
    
MTV=LISTFF(IND);
MT=MTV(1);


M=MT([1,2],:);
TMIN=MT(3);
MM=M
MM0=MM;
INDS=MT(7);
disp(["IND =",string(IND),"TMIN =",string(TMIN)]);
compt=0;
while INDS<>IND & compt<=size(LISTFF)
//    disp(INDS,'INDS');
    MTV=LISTFF(INDS);
    MT=MTV(1);
    M=MT([1,2],:);
    TM=MT(3);
    V=MTV(2)(1)([1,2],:);
    MMS=M+(TMIN-TM)*V;
    x=[MM(1),MMS(1)];
    y=[MM(2),MMS(2)];
    plot2d(x,y,1);
    point(MM,3,1);
    MM=MMS;
    INDS=MT(7);
    compt=compt+1;
end
    x=[MMS(1),MM0(1)];
    y=[MMS(2),MM0(2)];
plot2d(x,y,5);    
endfunction
    


function traceLISTFF(LISTFF)
// on représente toute la liste LISTFF    
   for MTV=LISTFF
//    disp(MTV,"MTV");
    MT=MTV(1);
    VIT=MTV(2)(1);
    x=[MT(1)-VIT(1),MT(1)];
    y=[MT(2)-VIT(2),MT(2)];
    if size(MTV(2))==1 then 
        coul=3;
    elseif size(MTV(2))==2
        coul=2;
    else
        coul=5;
    end
    plot2d4(x,y,coul);
    end 
endfunction

function traceLISTFFNOIRE(LISTFF)
// on représente toute la liste LISTFF    
   for MTV=LISTFF
//    disp(MTV,"MTV");
    MT=MTV(1);
    VIT=MTV(2)(1);
    x=[MT(1)-VIT(1),MT(1)];
    y=[MT(2)-VIT(2),MT(2)];
    plot2d4(x,y,1);
    end 
endfunction


function traceLISTFFactif(LISTFF)
    // on représente les points successifs en commençant par le dernier inséré

    MTV=LISTFF($);
    MT=MTV(1);
    VIT=MTV(2)(1);
    IND0=size(LISTFF);
    IND=MT(7);
    coul=1;
    compt=0;
    //disp(IND0,'IND0');
    while IND<>IND0 & compt<=IND0+1
        //    disp(IND);
        MTV=LISTFF(IND);
        MT=MTV(1);
        VIT=MTV(2)(1);
        x=[MT(1)-VIT(1),MT(1)];
        y=[MT(2)-VIT(2),MT(2)];
        if size(MTV(2))<>1 then 
            coul=5;
        end
        plot2d4(x,y,coul);
        IND=MT(7);
        coul=modulo(coul+1,6);
        compt=compt+1;
    end
    

    MTV=LISTFF($);
    MT=MTV(1);
    M=MT([1,2],:);
    TM=MT(3);
    V=MTV(2)(1)([1,2],:);
    MM=M+0.1*V/norm(V);
    IND=MT(7);
    MM0=MM;
    //disp(IND,"IND");
    compt=0;
    while IND<>IND0 & compt<=size(LISTFF)+2
//        disp(IND,'IND');
        MTV=LISTFF(IND);
        MT=MTV(1);
//        disp(MTV,IND);
        M=MT([1,2],:);
        TM=MT(3);
        V=MTV(2)(1)([1,2],:);
        if norm(V)>0.00000000001 then
            MMS=M+0.1*V/norm(V);
            x=[MM(1),MMS(1)];
            y=[MM(2),MMS(2)];
            plot2d4(x,y,1);
        end
        MM=MMS;
        IND=MT(7);
        compt=compt+1;
    end
    x=[MMS(1),MM0(1)];
    y=[MMS(2),MM0(2)];
    plot2d4(x,y,5);
    if compt==size(LISTFF)+2 then
        disp('erreur dans traceLISTFFactif, compt==size(LISTFF)+2') ; 
    end
endfunction

function dessellipseactif(LISTAUX)

    for k=1:size(LISTAUX(3)(1,:),'c')
        IND=LISTAUX(3)(1,k);
        IJ =LISTFF(IND)(2)(1)([5,6],:);
        dessellipse(IJ);
    end
endfunction

function point(M,coul,ep)
    d=0.01;
    X=[M(1)+d,M(1)-d,M(1)-d,M(1)+d,M(1)+d];
    Y=[M(2)+d,M(2)+d,M(2)-d,M(2)-d,M(2)+d];
    plot2d(X,Y,coul);

    a=gca();
    a.isoview='on'; 
    a.children // list the children of the axes.
    // There are a compound made of two polylines and a legend 
    poly1= a.children(1).children(1); //store polyline handle into poly1 
//    poly1.foreground = 1; // another way to change the style...
    poly1.thickness = ep;  // ...and the tickness of a curve.
endfunction

function tracevitcellule(IJ)
// trace tous les éléments de la cellule IJ
    num=numero(IJ);
    TAB=LISTAUX(2)(num);
    TABACTIF=LISTAUX(3)(1,:);
//    disp(TABACTIF,'TABACTIF');
    TMIN=LISTFF(LISTAUX(4))(1)(3);
    LISTF=list();
    for k=1:size(TAB,'c')
        IND=TAB(k);
        MTV=LISTFF(IND);
        MT=MTV(1);
        M=MT([1,2],:);
        TM=MT(3);
        V=MTV(2)(1)([1,2],:);
        MM=M+(TMIN-TM)*V;
        if TMIN>=TM then 
            coul =7;
        else
            [nbre,loc]=members([IND],TABACTIF);
            if nbre == 1 then
                if MT(9)==1 then
                    coul=5
                    disp([string(IND),' est un choc']);
                    else
                   coul=2;
            end
        else
            coul=6;
            end
        end
        point(MM,coul,2);
        LISTF($+1)=MTV;
    end
    traceLISTFF(LISTF);
    disp(TAB);

endfunction

function tr(IND,LISTFF)
    disp(LISTFF(IND));
    IJ=LISTFF(IND)(2)(1)([5,6],:);
    tracevitcellule(IJ);
//    disp(LISTAUX(2)(numero(IJ)));
    disp(['TMIN =',string(LISTFF(LISTAUX(4))(1)(3))])
endfunction

function trajectoire(IND)
  // remonte la trajectoire de IND
    MTV=LISTFF(IND);
    MT=MTV(1);
    M=MT([1,2],:);
    INDO=MTV(1)(8);
    INDOO=IND
    while INDO<INDOO
       MTV=LISTFF(INDO);
       MT=MTV(1);
       MO=MT([1,2],:);
       x=[MO(1),M(1)];
       y=[MO(2),M(2)];
       plot2d4(x,y,5);
       INDOO=INDO;
       INDO=MTV(1)(8);
       M=MO;
    end
    
endfunction

function tracesommet(LISTFF,LISTAUX)
    // tracé des sommets brulés
//    IJT=cellnumero(LISTAUX(13));
//    
//    for k=1:size(LISTAUX(7),'c')
//        M=IJT(:,k);
//        d=0.05;
//        X=[M(1)+d,M(1)-d,M(1)-d,M(1)+d,M(1)+d];
//        Y=[M(2)+d,M(2)+d,M(2)-d,M(2)-d,M(2)+d];
//        plot2d(X,Y,1);
//    end
    // tracé des sommets actualisés
    TMIN=LISTFF(LISTAUX(4))(1)(3);
    IJT=cellnumero(LISTAUX(7)([1],:));
    for k=1:size(LISTAUX(7),'c')
        M=IJT(:,k);
        d=0.1;
        X=[M(1)+d,M(1)-d,M(1)-d,M(1)+d,M(1)+d];
        Y=[M(2)+d,M(2)+d,M(2)-d,M(2)-d,M(2)+d];
        if LISTAUX(7)(2,k)<= TMIN then 
            coul=2;
        else
            coul=5;
        end
        plot2d(X,Y,coul);
    end
endfunction  
