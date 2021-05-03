if getos() == 'Windows' then 
    //getd("H:\Mes documents\Bureau\SCILAB\propagfeu_algo3_2");
    getd("D:\SCILAB\propagfeu_algo_opt");
    path="D:\SCILAB\propagfeu_algo_opt\";
    nomfichier="D:\SCILAB\propagfeu_algo_opt\resultats\";
    nomfichieracomparer="D:\SCILAB\propagfeu_algo_opt\resultats-ref\";
else 
    getd("/Users/pierrebaras/Desktop/propagfeu_algo_opt");
    path="/Users/pierrebaras/Desktop/propagfeu_algo_opt/";
    nomfichier="/Users/pierrebaras/Desktop/propagfeu_algo_opt/resultats/";
    nomfichieracomparer="/Users/pierrebaras/Desktop/propagfeu_algo_opt/resultats-ref/front";
end

IJ=[2;3];
R=[0.1+IJ(2)/3;0.2+IJ(1)/3;1];
    epsilon=0.9;
    alpha=IJ(1)+3*IJ(2);
    phi=IJ(1)+IJ(2);
N=[sin(phi)*sin(alpha);-cos(phi)*sin(alpha);cos(alpha)];
test=R'*N;

if test <> 0 then
    disp("R n est pas dans le plan, on modifie R_z");
    R(3)=R(3)-test/cos(alpha);
end

disp(R,"R modifié");
C=-(epsilon/(1+epsilon))*R;
x=[C(1)];
y=[C(2)];
z=[C(3)];

clf(1);
scf(1);
param3d(x,y,z, 30, 72,"X@Y@Z",[4,4]);
curve = gce();
curve.mark_mode = "on";
curve.mark_style = 10;
curve.mark_foreground = color("black");



x=[];
y=[];
z=[];
for t = 0:0.1:2*%pi;
    VIT=vitesse_opt(t,R,epsilon,alpha,phi);
    x=[x,VIT(1)];
    y=[y,VIT(2)];
    z=[z,VIT(3)];
end



param3d(x,y,z, 30, 72,"X@Y@Z",[4,4]);
curve = gce();
curve.mark_mode = "on";
curve.mark_style = 10;
curve.mark_foreground = color("magenta");

x=[C(1)];
y=[C(2)];
z=[0];

param3d(x,y,z, 30, 72,"X@Y@Z",[4,4]);
curve = gce();
curve.mark_mode = "on";
curve.mark_style = 10;
curve.mark_foreground = color("blue");

x=[];
y=[];
z=[];
for t = 0:0.1:2*%pi;
    VIT=vitesse_opt(t,R,epsilon,alpha,phi);
    x=[x,VIT(1)];
    y=[y,VIT(2)];
    z=[z,0];
end


param3d(x,y,z, 30, 72,"X@Y@Z",[4,4]);
curve = gce();
curve.mark_mode = "on";
curve.mark_style = 10;
curve.mark_foreground = color("red");

E=parametres_ellipse_proj(R,epsilon,alpha,phi);
//disp(E,"E");
c=E(:,[1,2]);
e=E(:,[3,4]);
a=E(5);
b=E(6);

x=[c(1)];
y=[c(2)];
z=[0];


param3d(x,y,z, 30, 72,"X@Y@Z",[4,4]);
curve = gce();
curve.mark_mode = "on";
curve.mark_style = 10;
curve.mark_foreground = color("green");


x=[];
y=[];
z=[];
for t = 0:0.1:2*%pi;
x=[x,a*cos(t)*e(1)+b*sin(t)*e(2)];
y=[y,a*cos(t)*e(2)-b*sin(t)*e(1)];
z=[z,0];
end



param3d(x,y,z, 30, 72,"X@Y@Z",[4,4]);
curve = gce();
curve.mark_mode = "on";
curve.mark_style = 10;
curve.mark_foreground = color("green");
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//test vitesse
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

