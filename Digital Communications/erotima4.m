%Ερώτημα 4 (σε μορφή script)


clear
clc

%δημιουργία αλφαβήτου. Οι αριθμοί 1-26 αντιστοιχούν στα 26 γράμματα του
%αλφαβήτου
startAlphabet=[1:26];

%οι πιθανότητες που αντοιστοιχούν στο κάθε γράμμα του αλφαβήτου
startP=[8.167/100,1.492/100,2.782/100,4.253/100,12.702/100,2.228/100,2.015/100,6.094/100,6.966/100,0.153/100,0.772/100,4.025/100,2.406/100,6.749/100,7.507/100,1.929/100,0.095/100,5.987/100,6.327/100,9.056/100,2.758/100,0.978/100,2.361/100,0.150/100,1.974/100,0.074/100];


%Για τη δεύτερης τάξης επέκταση, θέλουμε όλους τους πιθανούς συνδιασμούς
%ανά δύο. Αυτό δίνεται από το αποτέλεσμα της combnk συν τα αντίστροφά + το
%καθένα με τον εαυτό του. 

first=combnk(startAlphabet,2);
second=fliplr(combnk(startAlphabet,2));
final=[first;second];
for i=1:26 
    final=[final;[i,i]]; 
end

p=zeros(length(final),1);
%νέες πιθανότητες
for i=1:length(final)
    p(i)=startP(final(i,1))*startP(final(i,2));
end
p=p';

alphabet=[1:length(final)];


%τυχαία πηγή 5000ων συμβόλων από το συγκεκριμένο αλφάβητο.
out = randsrc(1,10000,[alphabet; p]); %10000 χαρακτήρες=5000 ζεύγη
disp('Random letters created in first place:');
convertVector2Letters(out)

%δημιουργία αναπαράστασης
[sym, repr, avglen] = myHuffmandict (alphabet, p);

%κωδικοποίηση
[comp] = myHuffmanenco (out, repr, sym); 


entropy=0;
%υπολογισμός εντροπίας
for i=1:length(p)
    entropy=entropy+p(i)*log2(1./p(i));
end

disp('efficiency=');
entropy./avglen

disp('Coded Message in Binary');
comp{1}




disp('Complete')