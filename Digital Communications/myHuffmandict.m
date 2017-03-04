function [ symbols, representation, avglen ] = myHuffmandict (alphabet, p)

%lengthx2Sorted: Ταξινομηένο κατά φθίνουσα σειρά των πιθανοτήτων 
Alen=length(alphabet);
IDs=[1:Alen];
C=cell(length(alphabet),2);
for i=1:length(alphabet)
    C{i}=p(i);
    C{i,2}=alphabet(i);
end


avglen=0;

%φθίνουσα ταξινόμηση με βάση τις πιθανότητες.
Sorted=sortrows(C,[-1,1]); 
StartSorted=Sorted;


%το διάνυμσμα αυτό περιλαμβάνει τον προσωρινό κώδικα την κάθε λέξης
%(παράλληλο  διάνυμα με το ταξινιμημένο μητρώο Sorted. 
%Αρχική τιμή =-1 -> δεν έχει προσπελαθεί ακόμα
for i=1:Alen Coding{i}=''; end

for i=Alen:-1:2
 
    if Sorted{i,2}(1) > Sorted{i-1,2}(1) 
        for j=1:length(Sorted{i,2})
            Coding{Sorted{i,2}(j)}=['1' Coding{Sorted{i,2}(j)}];            
        end
        for j=1:length(Sorted{i-1,2})
            Coding{Sorted{i-1,2}(j)}=['0' Coding{Sorted{i-1,2}(j)}];
        end            
    else
        for j=1:length(Sorted{i,2})
         Coding{Sorted{i,2}(j)}=['0' Coding{Sorted{i,2}(j)}];
        end
        for j=1:length(Sorted{i-1,2})
            Coding{Sorted{i-1,2}(j)}=['1' Coding{Sorted{i-1,2}(j)}];
        end
    end
    
    newProb=Sorted{i,1}+Sorted{i-1,1}; %άθροισμα των δύο μικρότερων πιθανοτήτων
    Sorted{i,1}=0; %μηδενισμός τελευταίας γραμμής (αφαίρεση)
    Sorted{i-1}=newProb; %το προηγούμενο άθροισμα στέκεται σαν μια νέα πιθανότητα στον πίνακα
    Sorted{i-1,2}=[Sorted{i-1,2} Sorted{i,2}];
    
    Sorted=sortrows(Sorted, [-1,1]); %εκ νέου ταξινόμηση
    
    %ταξινόμηση των cells (για σωστή διαχείρηση των 0 και 1
    for j=1:Alen
        Sorted{j,2}=sort(Sorted{j,2});
    end
        
    
end

representation=Coding';
symbols=[1:Alen]';

%υπολογισμός του μέσου μήκους κώδικα
sum=0;
for i=1:Alen
    sum=sum+numel(Coding{i})*StartSorted{i,1}; %Σ(p*l)
end
avglen=sum;

end