function [ letters ] = convert2lettersB ( numbers, realAlphabet )


    realNumbers=[1:length(realAlphabet)];
    letters='';
    chars=''; %temp
    for i=1:length(numbers)
        if numbers(i) == '|' 
            letters=strcat(letters,realAlphabet(find(realNumbers==numbers(i))));
            letters=strcat(letters,realAlphabet(str2num(chars)));
            chars='';
        else
             chars=strcat(chars,numbers(i));
        end
    end
        


end