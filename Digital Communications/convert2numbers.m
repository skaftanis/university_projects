function [ numbers ] = convert2numbers( letters )

    realAlphabet=['a':'z'];

    numbers='';
    for i=1:length(letters)
         numbers=strcat(numbers, num2str(find(realAlphabet==letters(i))) );
         numbers=strcat(numbers, '|'); %διαχωριστικό για να ξέρουμε που αλλάξει γράμμα
    end


end

