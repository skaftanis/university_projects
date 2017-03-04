%Ερώτημα 3 (σε μορφή script)

clear
clc

text = fileread('kwords.txt');
text = strrep(text,sprintf('\n'),''); %αφαίρεση new lines
disp('Starting text be kwords.txt')
text

%εύρεση νέων πιθανοτήτων εμφάνισης
%για κάθε μοναδικό σύμβολο, πόσες φορές έχει εμφανιστεί

diffSymbols=unique(text);
p=zeros(length(diffSymbols),1);

for i=1:length(diffSymbols)
    p(i)=length(strfind(text,diffSymbols(i)))/length(text);
end
p=p';

alphabet=[1:length(diffSymbols)];

disp('Please Wait...');

tempout=convert2numbersB(text,diffSymbols); %μετατροπή σε αριθμούς
%μετατροπή σε πίνακα αριθμών (out) 
out=convert2NumberVector(tempout,  length(text));
out=out';

%δημιουργία αναπαράστασης
[sym, repr, avglen] = myHuffmandict (alphabet, p);

%κωδικοποίηση
[comp] = myHuffmanenco (out, repr, sym); 
disp('Coded Message in Binary');
comp{1}



%αποκωδικοποίηση
[dsig] = myhuffmandeco(comp, repr, sym);
disp('Decoded Message');
convert2lettersB(dsig,diffSymbols)

disp('Average code length');
avglen

disp('Complete')