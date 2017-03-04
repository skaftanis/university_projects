from nltk.stem import PorterStemmer
from nltk.tokenize import RegexpTokenizer
import os

'''
δέχεται σαν όρισμα τη κατηγορία που ανήκει το κείμενο και το όνομα του κειμένου και επιστέφει το περιεχόμενο
του κειμένου 
'''
def get_file_content(category, filename, test):
	path = "20news-bydate/20news-bydate-train/" + str(category) + "/" + str(filename)
	if test:
		path = "20news-bydate/20news-bydate-test/" + str(category) + "/" + str(filename)

	text_file = open(path, "r", encoding = "ISO-8859-1")
	readable = text_file.read()
	text_file.close()
	return readable

print("Διάβασμα training αρχείων...")

#στο categories υπάρχουν όλες οι κατηγορίες τραβηγμένες από τα ονόματα των αρχείων 
directory="20news-bydate/20news-bydate-train/"
categories_path = []
for filename in os.listdir(directory):
	categories_path.append(os.path.join(directory, filename))

categories = []
for i in range(len(categories_path)):
	categories.append( categories_path[i].split("/")[2] )

files_in_folder=[]
for i in categories:
	path="20news-bydate/20news-bydate-train/" + str(i)
	for filename in os.listdir(path):
		files_in_folder.append(os.path.join(path, filename))

#παράλληλες λίστες που δείχνουν σε ποια κλάση (κατηγορία) αντιστοιχεί το κάθε training αρχείο 
all_Files=[]
all_Categories=[]
for i in range(len(files_in_folder)):
	all_Files.append(files_in_folder[i].split("/")[3])
	all_Categories.append(files_in_folder[i].split("/")[2])


print("Tokenizing training data...")
#tokenizing σε όλα τα κείμενα
tokenized_Files=[]
tokenizer = RegexpTokenizer(r'\w+')

for i in range(len(files_in_folder)):
	tokenized_Files.append( tokenizer.tokenize(get_file_content(all_Categories[i], all_Files[i], test=False).lower()) )
	for j in range(len(tokenized_Files[i])):
		#για το bug της nltk με το oed (https://github.com/nltk/nltk/issues/1581)
		if tokenized_Files[i][j] == "oed":
			tokenized_Files[i][j] = "oed0"


print("Stemming training data...")
ps = PorterStemmer()

#για αφαίρεση των stopwords (δηλαδή συνδέσμων κτλ, που είναι άχρηστα για τη κατανόηση του κειμένου)
#στην ουσία για μη εισαγωγή τους στη stemming list 
from nltk.corpus import stopwords
stop_words = set(stopwords.words("english"))

stemmed_Files=[]
for i in range(len(tokenized_Files)):
	stemed_file = [] 
	for w in tokenized_Files[i]:
		if not w in stop_words:
			stemed_file.append(ps.stem(w))
	stemmed_Files.append(stemed_file)

#ελευθέρωση μνήμης από αυτά που δε χρειάζονται πλέον
tokenized_Files=[]

#όλες οι (διαφορετικές) λέξεις από όλα τα κείμενα
all_words_in_all_docs = []
all_words_in_training_data = []
all_words_in_testing_data = []
#αυτό το κομμάτι δημιουργεί το αρχείο allDifferentWords.txt το οποίο περιέχει όλες τις διαφορετικές
#λέξεις. Εκτελείται μόνο όταν ο χρήστης το επιθυμεί. Το αρχείο έχει δημιουργηθεί από πριν οπότε ο μόνος
#λόγος για να εκτελεστεί ξανά αυτό το κομμάτι είναι αν συμβεί αλλαγή στο dataset (δηλαδή στα κείμενα)
ans = input("Θέλετε να δημιουργηθεί το αρχείο allDifferentWords.txt? Επιλέξτε 'Y' εάν έχει γίνει κάποια αλλαγή στα κείμενα [Y/N] : ")
#ans = 'n'
if ans == 'Y' or ans == 'y':
	print("Δημιουργία αρχείου με όλες τις διαφορετικές λέξεις...")
	for i in range(len(stemmed_Files)):
		for j in range(len(stemmed_Files[i])):
			if not stemmed_Files[i][j] in all_words_in_training_data:
				all_words_in_training_data.append(stemmed_Files[i][j])

	thefile = open('allDifferentWords.txt', 'w')
	for item in all_words_in_training_data:
 		 thefile.write("%s\n" % item)
else:
	with open('allDifferentWords.txt') as f:
		all_words_in_training_data = f.readlines()

	#αφαίρεση των newlines
	for i in range(len(all_words_in_training_data)):
		all_words_in_training_data[i]=all_words_in_training_data[i].rstrip()


print("Διάβασμα testing αρχείων...")

#στο categories υπάρχουν όλες οι κατηγορίες τραβηγμένες από τα ονόματα των αρχείων 
directory="20news-bydate/20news-bydate-test/"
test_categories_path = []
for filename in os.listdir(directory):
	test_categories_path.append(os.path.join(directory, filename))

test_categories = []
for i in range(len(test_categories_path)):
	test_categories.append( test_categories_path[i].split("/")[2] )

test_files_in_folder=[]
for i in test_categories:
	path="20news-bydate/20news-bydate-test/" + str(i)
	for filename in os.listdir(path):
		test_files_in_folder.append(os.path.join(path, filename))

#παράλληλες λίστες που δείχνουν σε ποια κλάση (κατηγορία) αντιστοιχεί το κάθε testing αρχείο 
test_all_Files=[]
test_all_Categories=[]
for i in range(len(test_files_in_folder)):
	test_all_Files.append(test_files_in_folder[i].split("/")[3])
	test_all_Categories.append(test_files_in_folder[i].split("/")[2])

print("Tokenizing test data...")
#tokenizing σε όλα τα κείμενα
tokenizer = RegexpTokenizer(r'\w+')

for i in range(len(test_files_in_folder)):
	tokenized_Files.append( tokenizer.tokenize(get_file_content(test_all_Categories[i], test_all_Files[i], test=True).lower()) )
	for j in range(len(tokenized_Files[i])):
		#για το bug της nltk με το oed (https://github.com/nltk/nltk/issues/1581)
		if tokenized_Files[i][j] == "oed":
			tokenized_Files[i][j] = "oed0"


print("Stemming test data...")
ps = PorterStemmer()

#για αφαίρεση των stopwords (δηλαδή συνδέσμων κτλ, που είναι άχρηστα για τη κατανόηση του κειμένου)
#στην ουσία για μη εισαγωγή τους στη stemming list 
stop_words = set(stopwords.words("english"))

test_stemmed_Files=[]
for i in range(len(tokenized_Files)):
	stemed_file = [] 
	for w in tokenized_Files[i]:
		if not w in stop_words:
			stemed_file.append(ps.stem(w))
	test_stemmed_Files.append(stemed_file)


#ελευθέρωση μνήμης από αυτά που δε χρειάζονται πλέον
tokenized_Files=[]


#όλες οι (διαφορετικές) λέξεις από όλα τα testing κείμενα
#αυτό το κομμάτι δημιουργεί το αρχείο allDifferentTestWords.txt το οποίο περιέχει όλες τις διαφορετικές
#λέξεις. Εκτελείται μόνο όταν ο χρήστης το επιθυμεί. Το αρχείο έχει δημιουργηθεί από πριν οπότε ο μόνος
#λόγος για να εκτελεστεί ξανά αυτό το κομμάτι είναι αν συμβεί αλλαγή στο dataset (δηλαδή στα κείμενα)
ans = input("Θέλετε να δημιουργηθεί το αρχείο allDifferentTestWords.txt? Επιλέξτε 'Y' εάν έχει γίνει κάποια αλλαγή στα testing κείμενα [Y/N] : ")
#ans = 'n'
if ans == 'Y' or ans == 'y':
	print("Δημιουργία αρχείου με όλες τις διαφορετικές λέξεις...")
	for i in range(len(test_stemmed_Files)):
		for j in range(len(test_stemmed_Files[i])):
			if not test_stemmed_Files[i][j] in all_words_in_testing_data:
				all_words_in_testing_data.append(test_stemmed_Files[i][j])

	thefile = open('allDifferentTestWords.txt', 'w')
	for item in all_words_in_testing_data:
 		 thefile.write("%s\n" % item)
else:
	with open('allDifferentTestWords.txt') as f:
		all_words_in_testing_data = f.readlines()

	#αφαίρεση των newlines
	for i in range(len(all_words_in_testing_data)):
		all_words_in_testing_data[i]=all_words_in_testing_data[i].rstrip()


############################# TF IDF
all_words_in_all_docs =  all_words_in_training_data  + list(set(all_words_in_testing_data) - set(all_words_in_training_data))
all_words_in_training_data = []
all_words_in_testing_data = []

#έχουμε έτοιμο εδω το all_words_in_all_docs με τον ένα ή τον άλλο τρόπο
#wordDictA = dict.fromKeys(all_words_in_all_docs,0)
WordDictZero = {}
for i in range(0, len(all_words_in_all_docs)):
	WordDictZero[all_words_in_all_docs[i]] = 0

print("Υπολογισμός Tf-Idf για τα training data...")

#περιέχει το πόσες φορές εμφανίζεται κάθε λέξη σε ΟΛΑ τα κείμενα (για το idf)
WordDict = {}
for i in range(0, len(all_words_in_all_docs)):
	WordDict[all_words_in_all_docs[i]] = 1

for doc in range(len(stemmed_Files)):
	for word in stemmed_Files[doc]:
		WordDict[word]+=1

#το WordDictZero αποθηκεύει προσωρινά το dictionary με τη συχνότητα εμφάνισης
#κάθε λέξης στο ΣΥΓΚΕΚΡΙΜΕΝΟ κείμενο (με τη σειρά)
#Στο TfIDf για κάθε κείμενο θα υπάρχει μία λίστα με το tfidf διάνυσμα
import math
tfidf = []
for doc in range(len(stemmed_Files)):
	for word in stemmed_Files[doc]:
		#πόσες φορε υπάρχει η κάθε λέξη στο stemmed_Files[doc]
		WordDictZero[word]+=1
	localTF_IDF = []
	for word, val in WordDictZero.items():
		#tf x idf
		#πόσες φορές εμφανίζεται στο κείμενο της/πλήθος λέξεων στο κείμενο x πλήθος όλων των αρχείων / συχνότητα λέξης σε όλα
		localTF_IDF.append(val/len(WordDictZero) *  math.log((len(all_Files) / WordDict[word] )))
	tfidf.append(localTF_IDF)
	#αρχικοποίηση με μηδενίκή του dictionary για το επόμενο loop
	WordDictZero=dict.fromkeys(WordDictZero,0)

#αφού έχουμε τα tfidf κάθε κειμένου από το training set, υπολογίζουμε την ίδια τιμή με τον ίδιο τρόπο για το
#testing set 

#έχουμε έτοιμο εδω το all_words_in_all_docs με τον ένα ή τον άλλο τρόπο
#wordDictA = dict.fromKeys(all_words_in_all_docs,0)
WordDictZero = {}
for i in range(0, len(all_words_in_all_docs)):
	WordDictZero[all_words_in_all_docs[i]] = 0


print("Υπολογισμός Tf-Idf για τα testing data...")

#περιέχει το πόσες φορές εμφανίζεται κάθε λέξη σε ΟΛΑ τα κείμενα (για το idf)
WordDict = {}
for i in range(0, len(all_words_in_all_docs)):
	WordDict[all_words_in_all_docs[i]] = 1

for doc in range(len(test_stemmed_Files)):
	for word in test_stemmed_Files[doc]:
		WordDict[word]+=1

#ελευθέρωση μνήμης
all_words_in_all_docs = []

#το WordDictZero αποθηκεύει προσωρινά το dictionary με τη συχνότητα εμφάνισης
#κάθε λέξης στο ΣΥΓΚΕΚΡΙΜΕΝΟ κείμενο (με τη σειρά)
#Στο TfIDf για κάθε κείμενο θα υπάρχει μία λίστα με το tfidf διάνυσμα
tfidf_test = []
for doc in range(len(test_stemmed_Files)):
	for word in test_stemmed_Files[doc]:
		#πόσες φορε υπάρχει η κάθε λέξη στο test_stemmed_Files[doc]
		WordDictZero[word]+=1
	localTF_IDF = []
	for word, val in WordDictZero.items():
		#tf x idf
		#πόσες φορές εμφανίζεται στο κείμενο της/πλήθος λέξεων στο κείμενο x πλήθος όλων των αρχείων / συχνότητα λέξης σε όλα
		localTF_IDF.append(val/len(WordDictZero) *  math.log((len(all_Files) / WordDict[word] )))
	tfidf_test.append(localTF_IDF)
	#αρχικοποίηση με μηδενίκή του dictionary για το επόμενο loop
	WordDictZero=dict.fromkeys(WordDictZero,0)


#---------------------------------------------------------

#μετρική cosine
def cosine(v1, v2):
	import numpy as np
	from numpy import linalg as la
	return np.dot(v1,v2) / ( la.norm(v1) * la.norm(v2) )


from math import*
def jaccard_similarity(x,y):
	intersection_cardinality = len(set.intersection(*[set(x), set(y)]))
	union_cardinality = len(set.union(*[set(x), set(y)]))
	return intersection_cardinality/float(union_cardinality)


while True:

	metric = 3
	while metric != "1" and metric != "2" and metric != "-1":
		metric = input("Ποια μετρική θέλετε να χρησιμοποιήσετε [1:Cosine, 2:Jaccard -1:Τερματισμός]:")
		if metric != "1" and metric != "2" and metric !="-1":
			print("Επιλέξτε κάποια από τις διαθέσιμες επιλογές")

	if metric == "-1":
		import sys
		sys.exit()

	selected = len(test_all_Files)+100
	while selected >= len(test_all_Files): 
		selected = int(input("Ποιο αρχείο θέλετε να κατηγοριοποιήσετε [0-" + str(len(test_all_Files)-1) +"]:"))
		if selected > len(test_all_Files)-1:
			print("Επιλέξτε κάποιο διαθέσιμο test data")
		else:
			break

	if int(metric) == 1: 
		maxCosine = 0
		pos = 0 
		for j in range(len(tfidf)):
	 		cos = cosine(tfidf[j], tfidf_test[selected])
	 		if cos > maxCosine:
	 			maxCosine = cos
	 			pos = j
		print("Cosine Similarity = ", maxCosine )
		print("Κατηγοριοποιήθηκε σαν: ", all_Categories[pos])
		print("Στη πραγματικότητα ανήκε στη κατηγορία: ", test_all_Categories[selected])

	if int(metric) == 2:
		maxJaccard = 0
		pos = 0 
		for j in range(len(tfidf)):
	 		cos = jaccard_similarity(tfidf[j], tfidf_test[selected])
	 		if cos > maxCosine:
	 			maxJaccard = cos
	 			pos = j
		print("Jaccard Similarity = ", maxJaccard )
		print("Κατηγοριοποιήθηκε σαν: ", all_Categories[pos])
		print("Στη πραγματικότητα ανήκε στη κατηγορία: ", test_all_Categories[selected])


#calculate system accuracy code
# correct = 0
# for i in range(len(tfidf_test)):
# 	maxCosine = 0
# 	pos = 0 
# 	for j in range(len(tfidf)):
# 		cos = jaccard_similarity(tfidf[j], tfidf_test[i])
# 		if cos > maxCosine:
# 			maxCosine = cos
# 			pos = j
# 	if all_Categories[pos] == test_all_Categories[i]:
# 		print(maxCosine)
# 		correct += 1 

# print("accuracy: ", correct / len(tfidf_test))