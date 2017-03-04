function [ dsig ] = myhuffmandeco( comp, representation, symbols )

   chars='';
   dsig='';
   tempValue=0;
   exist=0;
   counter=1;
   while 1     
       if counter > length(comp{1})
           dsig=strcat(dsig,num2str(tempValue));
            dsig=strcat(dsig, '|'); 
           break
       end
       chars=strcat(chars, comp{1}(counter));
       for j=1:length(symbols)
         if length(representation{j}) >= length(chars)
             if strcmp(representation{j}(1:length(chars)), chars)
                 exist=1; 
             end
         end
       end
       if exist == 1 
           exist=0;
           tempValue=symbols(find(strcmp(representation,chars))); 
           counter=counter+1;
       else
           dsig=strcat(dsig,num2str(tempValue));
           dsig=strcat(dsig, '|'); 
           chars='';
       end

      
             
   end


end

