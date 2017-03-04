function [ letters ] = convertVector2Letters( numbers )


      realAlphabet=['a':'z'];
      realNumbers=[1:26];
      
      letters='';
      for i=1:length(numbers)
          letters=strcat(letters, realAlphabet(find(realNumbers==numbers(i))));
      end

end

