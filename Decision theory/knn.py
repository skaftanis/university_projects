import numpy as np 
from math import sqrt
from random import randint
from random import sample
from random import shuffle


def KNN_Prediction(data, data_to_predict,  k=15):

	classes,I0,PA500,HFS,DA,Area,ADA,MaxIP,DR,P = data



	#το data_to_predict είναι το δεδομένο το οποίο θα κατηγοριοποιηθεί
	#είναι ένα vector με 9 χαρακτηριστικά καθένα από τα οποίο αντιπροσωπεύει τις μετρήσεις I0,PA500,HFS,DA,Area,ADA,MaxIP,DR,P αντίστοιχα
	#(1x9 vector) 

	#υπολογισμός απόστασης του data_to_predict από κάθε data που υπάρχει στο dataset
	distance=[]
	#για κάθε απόσταση κρατάμε τη κλάση στην οποία ανήκει η αρχική τιμή του data, έτσι ώστε να ξέρουμε για ποια κλάση θα μετρηθεί η ψήφος
	class_belongs=[]

	for i in range (len(classes)):
		distance.append(  sqrt( (I0[i]-data_to_predict[0])**2 + (PA500[i]-data_to_predict[1])**2 + (HFS[i]-data_to_predict[2])**2 + (DA[i]-data_to_predict[3])**2
		 				+ (Area[i]-data_to_predict[4])**2 + (ADA[i]-data_to_predict[5])**2 + (MaxIP[i]-data_to_predict[6])**2 + (DR[i]-data_to_predict[7])**2  
		 				+ (P[i]-data_to_predict[8])**2) )

		class_belongs.append(classes[i])


	#εύρεση των k  πιο κοντινών στοιχείων

	#αύξουσα ταξινόμηση των στοιχείων (παράλληλα με τη λίστα που δείχνει την κλάση που ανήκει)
	distance, class_belongs = zip(*sorted(zip(distance, class_belongs)))

	#επιλογή των k μικρότερων (δηλαφή των k πρώτων)
	#στη λίστα votes φαίνονται οι "ψήφοι" που πήρε η επιλογής της κλάσης 1 έως 6 αντίστοιχα
	votes = [0,0,0,0,0,0] 
	for i in range(k):
		votes[class_belongs[i]-1] = votes[class_belongs[i]-1] + 1


	#print("confidence = ", max(votes)/9)


	#επέστρεψε τη κλάση στην οποία κατηγοροιποιείται το data_to_predict
	return votes.index(max(votes)) + 1 


def calculate_accuracy(data, k=15):

	#all data (ordered)
	classes_or,I0_or,PA500_or,HFS_or,DA_or,Area_or,ADA_or,MaxIP_or,DR_or,P_or = data


	#5-folds cross validation
	fold_size = int (len(classes_or) / 5)
	fold_start=0
	fold_end=fold_size

	#υπολογισμός αθροίσματος των accuracies για να βγει ο μέσος όρος
	acc_sum = 0 

	for step in range(5):

		#print("start")
		#print(fold_start)
		#print("end")
		#print(fold_end)

		#πρώτα πρέπει να ανακατέψουμε τα δεδομένα
		numbers=[i for i in range(len(classes_or))]
		shuffle(numbers)

		classes=[]
		I0=[]
		PA500=[]
		HFS=[]
		DA=[]
		Area=[]
		ADA=[]
		MaxIP=[]
		DR=[]
		P=[]

		#ανακατεμένα δεδομένα
		for i in numbers:
			classes.append(classes_or[i])
			I0.append(I0_or[i])
			PA500.append(PA500_or[i])
			HFS.append(HFS_or[i])
			DA.append(DA_or[i])
			Area.append(ADA_or[i])
			ADA.append(ADA_or[i])
			MaxIP.append(MaxIP_or[i])
			DR.append(DR_or[i])
			P.append(P_or[i])


		#(τα train_data είναι τα data που θα στείλουμε στη KNN_Prediction)
		#(τα test_data είναι τα data_to_predict

		#ορισμός των δεδομένων για δοκιμή 
		test_classes = []
		test_I0 = []
		test_PA500 = []
		test_HFS = []
		test_DA = []
		test_Area = []
		test_ADA = []
		test_MaxIP = []
		test_DR = []
		test_P = []
		#τα test data είναι το [fold_start,fold_end]
		for i in range(fold_start, fold_end):
			test_classes.append(classes[i])
			test_I0.append(I0[i])
			test_PA500.append(PA500[i])
			test_HFS.append(HFS[i])
			test_DA.append(DA[i])
			test_Area.append(Area[i])
			test_ADA.append(ADA[i])
			test_MaxIP.append(MaxIP[i])
			test_DR.append(DR[i])
			test_P.append(P[i])


		#ορισμός των δεδομένων εκπαίδευσης
		train_classes = []
		train_I0 = []
		train_PA500 = []
		train_HFS = []
		train_DA = []
		train_Area = []
		train_ADA = []
		train_MaxIP = []
		train_DR = []
		train_P = []
		#τα train data είναι αυτά που δεν είναι test data
		if fold_start != 0:
			for i in range(0, fold_start):
				train_classes.append(classes[i])
				train_I0.append(I0[i])
				train_PA500.append(PA500[i])
				train_HFS.append(HFS[i])
				train_DA.append(DA[i])
				train_Area.append(Area[i])
				train_ADA.append(ADA[i])
				train_MaxIP.append(MaxIP[i])
				train_DR.append(DR[i])
				train_P.append(P[i])
		for i in range(fold_end, len(classes)):
			train_classes.append(classes[i])
			train_I0.append(I0[i])
			train_PA500.append(PA500[i])
			train_HFS.append(HFS[i])
			train_DA.append(DA[i])
			train_Area.append(Area[i])
			train_ADA.append(ADA[i])
			train_MaxIP.append(MaxIP[i])
			train_DR.append(DR[i])
			train_P.append(P[i])


		


		#(τα train_data είναι τα data που θα στείλουμε στη KNN_Prediction)
		#(τα test_data είναι τα data_to_predict


		training_data = train_classes,train_I0,train_PA500,train_HFS,train_DA,train_Area,train_ADA,train_MaxIP,train_DR,train_P
		testing_data = test_classes,test_I0,test_PA500,test_HFS,test_DA,test_Area,test_ADA,test_MaxIP,test_DR,test_P

		test_now = []
		corrects=0
		for j in range(fold_size):
			for i in range(1,len(testing_data)):		
				test_now.append(testing_data[i][j])
			#αν η πρόβλεψη είναι ίδια με τη πραγματική τιμή
			if KNN_Prediction(training_data, test_now ,k ) == testing_data[0][j]:
				corrects=corrects+1
			test_now=[]

		#(debug) εμφανίζει το accuracy του κάθε fold
		#print(corrects/fold_size)
		acc_sum=acc_sum+(corrects/fold_size)

		fold_start=fold_start+fold_size
		fold_end=fold_end+fold_size

	return(acc_sum/5)
