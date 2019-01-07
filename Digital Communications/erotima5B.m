%Ερώτημα 5B (σε μορφή script)

clear
clc

text = fileread('kwords.txt');
text = strrep(text,sprintf('\n'),''); %αφαίρεση new lines
disp('Starting text be kwords.txt')
text

diffSymbols=unique(text);
p=zeros(length(diffSymbols),1);

alphabet=[1:length(diffSymbols)];

disp('Please Wait...');


startAlphabet=alphabet;



%Για τη δεύτερης τάξης επέκταση, θέλουμε όλους τους πιθανούς συνδιασμούς
%ανά δύο. Αυτό δίνεται από το αποτέλεσμα της combnk συν τα αντίστροφά + το
%καθένα με τον εαυτό του. 

first=combnk(startAlphabet,2);
second=fliplr(combnk(startAlphabet,2));
final=[first;second];
for i=1:length(diffSymbols)
    final=[final;[i,i]]; 
end

p=zeros(length(final),1);

%νέες πιθανότητες (ζευγών χαρακτήρων) υπολογισμένες από το αρχείο *****************
for i=1:length(final)
    %πόσες φορές υπάρχει στο text ο κάθε συνδιασμός / όλους τους
    %συνδισμούς (στη σειρά) 
    p(i)=(length(strfind(text,strcat(diffSymbols(final(i,1)),diffSymbols(final(i,2))))))/(length(text)-1); 
end
p=p';

alphabet=[1:length(final)];

%μετατροπή σε αριθμούς
tempout=convert2numbersB(text,diffSymbols); 
%μετατροπή σε πίνακα αριθμών (out) 
out=convert2NumberVector(tempout,  length(text));
out=out';

%δημιουργία αναπαράστασης
[sym, repr, avglen] = myHuffmandict (alphabet, p);

%κωδικοποίηση
[comp] = myHuffmanenco (out, repr, sym); 


disp('avglen=');
avglen

disp('Coded Message in Binary');
comp{1}




disp('Complete')
