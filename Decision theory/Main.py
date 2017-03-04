import get_data
import matplotlib.pyplot as plt 
from matplotlib import style
import svm
import knn
import naiveBayes
import numpy as np



classes,I0,PA500,HFS,DA,Area,ADA,MaxIP,DR,P = get_data.get_normalized_data()
data = get_data.get_normalized_data()

#KNN----------------
def call_knn(data):
	print("KNN-----------------")
	print("accuracy: " , knn.calculate_accuracy( data ) )
	#calculate the best k in [3,15]
	accuracies = []
	ks = []
	posCounter=0
	for i in range(3,16):
		accuracies.append(knn.calculate_accuracy( data, i))
		ks.append(i)

	#calculate the best accuracy and print the k
	maxAc = accuracies[0]
	maxK = ks[0]
	for i in range(1,len(accuracies)):
		if (accuracies[i] > maxAc):
			maxAc=accuracies[i]
			maxK=ks[i]
	bestk = maxK
	print("The best k is: ", bestk)

#Bayes (train and predict) ---------------------------------------------------
def call_bayes(data):
	print("NaiveBayes--------------")
	print ("accuracy: ", naiveBayes.calculate_accuracy(data  ) ) 
#------------------------------------------------------------------------

#SVM ---------------------------------------------------
def call_svm(data):
	#use linear kernel and calculate the best accuracy and the C 
	print("SVM-------------------")
	print("please wait a moment...")
	SVMaccuracies=[]
	for i in range(1,205,5):
		SVMaccuracies.append(svm.calculate_accuracy(data, kernel='linear_kernel', C=i))


	maxLSVMacc=max(SVMaccuracies)
	bestC = 5*SVMaccuracies.index(maxLSVMacc) + 1 
	print("Linear Kernel: best C =", bestC , "max accuracy = ", maxLSVMacc)


	#use RBF kernel and calculate the best accuracy with its C and γ
	#με το βέλτισο C του linerr kernel  υπολογίζεται το βέλτιστο γ 
	#το βέλτιστο C είναι διαφορετικό κάθε φορά)

	SVMaccuracies=[]
	for i in np.arange(0.0,10.0,0.5):
		SVMaccuracies.append(svm.calculate_accuracy(data, kernel='RBF_kernel', C=bestC, gamma=i))

	maxRBFSVMacc = max(SVMaccuracies)
	bestgamma = SVMaccuracies.index(maxRBFSVMacc) * 0.5
	print("RBF kernel: best gamma =", bestgamma, "max accuracy = ", maxRBFSVMacc)
#------------------------------------------------------------------------


call_knn(data)
call_bayes(data)
call_svm(data)


# #T-Test----------------------------------------------
import scipy
from scipy import stats
from scipy.stats import ttest_ind, ttest_ind_from_stats
from scipy.stats import t

#υπολογισμός της τιμής της t κατανομής όταν δίνουμε σας είσοδο το p και τον βαθμό ελευθερίας 
def t(prob, df):
    return scipy.stats.t.ppf(1-(prob/2), df)

#υλοποίηση του t_test. επιστρέφει true όταν υπάρχει σχέση μεταξύ των features που γίνονται σας είδοσο
#και False στην αντίθετη περίπτωση
def t_test(a,b):
	df = len(a)+len(b)-2

	meanA = np.mean(a)
	meanB = np.mean(b)
	varA = np.std(a)**2
	varB = np.std(b)**2

	t_value = (np.absolute(meanA-meanB))/(np.sqrt( ( (varA)/len(a) ) + ( (varB)/len(b)) )  )

	_ , p = ttest_ind(a, b, equal_var=False)

	t_test_value=t(p,df)

	if t_value > t_test_value:
		return True
	else:
		return False	

for i in range(1,10):
	for j in range(i+1,10):
		if t_test(data[i],data[j]):
			print("υπάρχει σχέση στο:", i , "με το ", j)

print()
print("SVM μετά το t-test")
new_data = classes,PA500,HFS,Area,P
call_svm(new_data)
