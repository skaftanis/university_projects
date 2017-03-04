function [ comp ] = myHuffmanenco ( actualsig, representation, symbols )

   %σε κάθε επανάληψη στο comp προστίθεται η κωδικοποίηση του κάθε συμβόλου
   %Στο τέλος προκύπει η αναπαράσταση όλου του κειμένου
   comp='';
   for i=1:length(actualsig)
       comp=strcat(comp,representation(find(symbols==actualsig(i))));
   end

   
end

