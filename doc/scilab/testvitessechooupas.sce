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
IJ=[1;1];
E=ellipse(IJ);
disp(E,"E");
c=E(:,[1,2]);
e=E(:,[3,4]);
a=E(5);
b=E(6);
d=0.01;
x=[c(1)-d,c(1),c(1)+d,c(1)-d];
y=[c(2)-d,c(2)+d,c(2)-d,c(2)-d];

plot2d(x,y,5);

    
x=[];
y=[];

for t = 0:0.1:2*%pi;
x=[x,a*cos(t)*e(1)+b*sin(t)*e(2)];
y=[y,a*cos(t)*e(2)-b*sin(t)*e(1)];
end

plot2d(x,y,5);



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//test vitessechocoupas
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
VP=[5;4];
VS=[-3;2];


VIP=vitessemax(VP,IJ);
VIS=vitessemax(VS,IJ);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
x=[c(1),c(1)+VIP(1)];
y=[c(2),c(2)+VIP(2)];
plot2d4(x,y,1);
x=[c(1),c(1)-VIP(3)];
y=[c(2),c(2)-VIP(4)];
plot2d(x,y,1);
x=[c(1),c(1)+VIS(1)];
y=[c(2),c(2)+VIS(2)];
plot2d4(x,y,2);
x=[c(1),c(1)+VIS(3)];
y=[c(2),c(2)+VIS(4)];
plot2d(x,y,2);
kmax=50;

[VITC,INDIC]=vitessechocoupas(VIP,VIS)
COUL=1;
for V=VITC

disp(V,"V");
V=V(1);
x=[c(1),c(1)+V(1)];
y=[c(2),c(2)+V(2)];

plot2d4(x,y,COUL);

COUL=COUL+1;
end
disp(VITC,"VITC");
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// on permute VIP et VIS
disp('+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++');
disp("on permute VIP et VIS");

VIPP=VIS;
VIS=VIP;
VIP=VIPP;
clf(2);
scf(2);

x=[c(1)-d,c(1),c(1)+d,c(1)-d];
y=[c(2)-d,c(2)+d,c(2)-d,c(2)-d];

plot2d(x,y,5);

    
x=[];
y=[];

for t = 0:0.1:2*%pi;
x=[x,a*cos(t)*e(1)+b*sin(t)*e(2)];
y=[y,a*cos(t)*e(2)-b*sin(t)*e(1)];
end

plot2d(x,y,5);

x=[c(1),c(1)+VIP(1)];
y=[c(2),c(2)+VIP(2)];
plot2d4(x,y,1);
x=[c(1),c(1)-VIP(3)];
y=[c(2),c(2)-VIP(4)];
plot2d(x,y,1);
x=[c(1),c(1)+VIS(1)];
y=[c(2),c(2)+VIS(2)];
plot2d4(x,y,2);
x=[c(1),c(1)+VIS(3)];
y=[c(2),c(2)+VIS(4)];
plot2d(x,y,2);

[VITC,INDIC]=vitessechocoupas(VIP,VIS)
COUL=1;
for V=VITC
V=V(1);
x=[c(1),c(1)+V(1)];
y=[c(2),c(2)+V(2)];

plot2d4(x,y,COUL);

COUL=COUL+1;
end
disp(VITC,"VITC");
