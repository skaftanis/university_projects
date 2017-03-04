%Ερώτημα 2B (σε μορφή script)

clear
clc
%δημιουργία αλφαβήτου. Οι αριθμοί 1-26 αντιστοιχούν στα 26 γράμματα του
%αλφαβήτου
alphabet=[1:26];

%οι πιθανότητες που αντοιστοιχούν στο κάθε γράμμα του αλφαβήτου
p=[8.167/100,1.492/100,2.782/100,4.253/100,12.702/100,2.228/100,2.015/100,6.094/100,6.966/100,0.153/100,0.772/100,4.025/100,2.406/100,6.749/100,7.507/100,1.929/100,0.095/100,5.987/100,6.327/100,9.056/100,2.758/100,0.978/100,2.361/100,0.150/100,1.974/100,0.074/100];

%πηγή που προέρχεται από το αρχείο kwordsER2.txt
text = fileread('kwordsER2.txt');
text = strrep(text,sprintf('\n'),''); %αφαίρεση new lines
disp('Starting text be kwordsER2.txt')
text

disp('Please Wait...');

tempout=convert2numbers(text); %μετατροπή σε αριθμούς
%μετατροπή σε πίνακα αριθμών (out) 
out=convert2NumberVector(tempout,  29096);
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
convert2letters(dsig)

disp('strcmp result:');
strcmp(convertVector2Letters(out), convert2letters(dsig))

disp('Average code length');
avglen

disp('Complete')