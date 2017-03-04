function [ vector ] = convert2NumberVector( numbers, textLength )

    chars='';
    counter=1;
    pipe=0; %for a bug
    vector=zeros(textLength,1);
    for i=1:length(numbers)
        if numbers(i) == '|'
            if pipe == 0 
                vector(counter)=str2num(chars);
                counter=counter+1;
                chars='';
                pipe=1;
            end
        else
            chars=strcat(chars,numbers(i));
            pipe=0;
        end
    end


end

