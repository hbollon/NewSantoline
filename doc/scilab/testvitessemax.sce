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
//test vitesse
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
VD=[1;1];
V=vitessemax(VD,IJ)
disp(V,"VOPT");
x=[c(1),c(1)+V(1)];
y=[c(2),c(2)+V(2)];

plot2d(x,y,2);

x=[c(1)+V(1),c(1)+V(1)+V(3)];
y=[c(2)+V(2),c(2)+V(2)+V(4)];

plot2d(x,y,3);
