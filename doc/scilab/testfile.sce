
//if getos() == 'Windows' then 
//    //getd("H:\Mes documents\Bureau\SCILAB\propagfeu_algo3_2");
//    getd("D:\SCILAB\propagfeu_algo_opt");
//    path="D:\SCILAB\propagfeu_algo_opt\";
//    nomfichier="D:\SCILAB\propagfeu_algo_opt\resultats\";
//else 
//    getd("/Users/pierrebaras/Desktop/propagfeu_algo_opt");
//    path="/Users/pierrebaras/Desktop/propagfeu_algo_opt/";
//    nomfichier="/Users/pierrebaras/Desktop/propagfeu_algo_opt/resultats/";
//end
//A=[];
//fd_w = mopen(path+'write.txt', 'wt');
//for i=1:10
//    for j=1:10
//        IJ=[i;j];
//        E=ellipse(IJ);
//// write a line in fd_w
//E=[IJ',E];
//A=[A;E];
//end
//end
////disp(A,'A');
//mfprintf(fd_w,A);
////A =fscanfMat(path+'write.txt')
////disp(A);
//
//mclose(fd_w);
//
//// read text
////fd_r2 = mopen(path+'write.txt', 'rt');
////A =read(path+'/write.txt',-1,3)
////disp(A);
////mgetl(fd_r2)
//mclose(fd_r2);

RZ(1,1)= 3;
RZ(1,2)= 4;
disp(RZ);
