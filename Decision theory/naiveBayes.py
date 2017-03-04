import numpy as np 
from random import shuffle



ProbOfClass=[]

#δέχεται σαν όρισμα τα δεδομένα και επιστρέφει έναν πίνακα με υπολογισμένες όλες τις πιθανότητες
#που χρειάζονται για να γίνει η πρόβλεψη 
def bayes_train(data):
	classes,I0,PA500,HFS,DA,Area,ADA,MaxIP,DR,P = data

	#υπολογισμός πιθανότητας κάθε class σε ένα 1x6 vector [P(car), P(fad), P(mas)....]
	[ProbOfClass.append(classes.count(i) / len(classes)) for i in range(1,6+1)]

	#υπολογισμός του πόσα στοιχεία υπάρχουν από κάθε κλάση
	ItemsInClass=[]
	[ItemsInClass.append(classes.count(i)) for i in range(1,6+1)]
	#print(ItemsInClass)

	#οι τιμές των χαρακτηριστικών είναι ήδη κανονικοποιημένες στο [-1,1]
	#κάνουμε ομαδοποίηση ανά 0.5 (δηλαδή οι τιμές στο [-1,-0.5) είναι μία κατηγορία στο 
	#[-0.5,0) άλλη κτλ, άρα θα δημιουργηθούν 5 κατηγορίες

	#στο παρακάτω dictionary για κάθε κλάση υπάρχει ένα διάνυσμα το οποίο περιέρχει
	#για καθε μία από τις 20 κατηγορίες μία λίστα με τη πιθανότητα να ανήκει σε κάθε μία 
	#από τις 9 κλάσεις

	RelationCount={'I0': [], 'PA500': [], 'HFS': [] , 'DA': [], 'Area': [],'ADA': [],'MaxIP':[], 
					'DR': [], 'P': [] } 

	RelationProbs={'I0': [], 'PA500': [], 'HFS': [] , 'DA': [], 'Area': [],'ADA': [],'MaxIP':[], 
					'DR': [], 'P': [] } 

	#δημιουργία λιστών 5 θέσεων (για κάθε κατηγορία) στα dictionaries
	for i in range(2):
		RelationCount['I0'].append([0, 0, 0, 0, 0, 0])
		RelationCount['PA500'].append([ 0, 0, 0, 0, 0, 0])
		RelationCount['HFS'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationCount['DA'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationCount['Area'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationCount['ADA'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationCount['MaxIP'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationCount['DR'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationCount['P'].append([ 0, 0, 0, 0, 0, 0 ])

		RelationProbs['I0'].append([ 0, 0, 0, 0, 0, 0])
		RelationProbs['PA500'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationProbs['HFS'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationProbs['DA'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationProbs['Area'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationProbs['ADA'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationProbs['MaxIP'].append([ 0, 0, 0, 0, 0, 0 ])
		RelationProbs['DR'].append([ 0, 0, 0, 0, 0, 0])
		RelationProbs['P'].append([ 0, 0, 0, 0, 0, 0 ])

	#τα όρια που χωρίζουν τις κατηγορίες
	ranges=[-1,0,1]
	#[ranges.append(i) for i in np.arange(-1,1,1)]
	#print("ranges")
	#print(ranges)

	#υπολογισμό του πόσες φορές για κάθε κλάση εμφανίζεται η κάθε κατηγορία (RelationCount)
	for rangePointer in range(2):
		for i in range(len(classes)):
			if I0[i] >= ranges[rangePointer] and I0[i] < ranges[rangePointer+1]:
		 		RelationCount['I0'][rangePointer][classes[i]-1]=RelationCount['I0'][rangePointer][classes[i]-1]+1
			if PA500[i] >= ranges[rangePointer] and PA500[i] < ranges[rangePointer+1]:
		 		RelationCount['PA500'][rangePointer][classes[i]-1]=RelationCount['PA500'][rangePointer][classes[i]-1]+1
			if HFS[i] >= ranges[rangePointer] and HFS[i] < ranges[rangePointer+1]:
		 		RelationCount['HFS'][rangePointer][classes[i]-1]=RelationCount['HFS'][rangePointer][classes[i]-1]+1
			if DA[i] >= ranges[rangePointer] and DA[i] < ranges[rangePointer+1]:
		 		RelationCount['DA'][rangePointer][classes[i]-1]=RelationCount['DA'][rangePointer][classes[i]-1]+1
			if Area[i] >= ranges[rangePointer] and Area[i] < ranges[rangePointer+1]:
		 		RelationCount['Area'][rangePointer][classes[i]-1]=RelationCount['Area'][rangePointer][classes[i]-1]+1
			if ADA[i] >= ranges[rangePointer] and ADA[i] < ranges[rangePointer+1]:
		 		RelationCount['ADA'][rangePointer][classes[i]-1]=RelationCount['ADA'][rangePointer][classes[i]-1]+1
			if MaxIP[i] >= ranges[rangePointer] and MaxIP[i] < ranges[rangePointer+1]:
		 		RelationCount['MaxIP'][rangePointer][classes[i]-1]=RelationCount['MaxIP'][rangePointer][classes[i]-1]+1
			if DR[i] >= ranges[rangePointer] and DR[i] < ranges[rangePointer+1]:
		 		RelationCount['DR'][rangePointer][classes[i]-1]=RelationCount['DR'][rangePointer][classes[i]-1]+1
			if P[i] >= ranges[rangePointer] and P[i] < ranges[rangePointer+1]:
		 		RelationCount['P'][rangePointer][classes[i]-1]=RelationCount['P'][rangePointer][classes[i]-1]+1


	#υπολογισμός του RelationProb δηλαδή στην ουσία μετατροπή των παραπάνω σε πιθανότητες (RelationCounti/πλήθος)
	for category in ranges: 
		for noOfClass in range(6):
			RelationProbs['I0'][category][noOfClass]=RelationCount['I0'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['PA500'][category][noOfClass]=RelationCount['PA500'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['HFS'][category][noOfClass]=RelationCount['HFS'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['DA'][category][noOfClass]=RelationCount['DA'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['Area'][category][noOfClass]=RelationCount['Area'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['ADA'][category][noOfClass]=RelationCount['ADA'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['MaxIP'][category][noOfClass]=RelationCount['MaxIP'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['DR'][category][noOfClass]=RelationCount['DR'][category][noOfClass]/ItemsInClass[noOfClass]
			RelationProbs['P'][category][noOfClass]=RelationCount['P'][category][noOfClass]/ItemsInClass[noOfClass]
	

	#print(RelationProbs)

	#print(RelationCount)
	#print()
	#print(RelationProbs)

	return RelationProbs

def bayes_predict(RelationProbs, data_to_predict):

	#σε ποιο διάστημα ανήκει το κάθε στοιχείο του data_to_predict
	ranges=[-1,0,1]
	#[ranges.append(i) for i in np.arange(-1,1,1)]
	category = []


	#print("LEN")
	#print(len(data_to_predict))

	#το category δηλώνει τη κατηγορία που ανήκει το καθένα απο το data_to_predict
	for i in range(9):
		for rangePointer in range(2):
				if data_to_predict[i] >= ranges[rangePointer] and data_to_predict[i] < ranges[rangePointer+1]: 
					category.append(rangePointer)
					break

	#print("category")
	#print(category)
	#εκτιμήσεις για κάθε δυνατή κλάση appx=(1x6)
	appxs = []

	#print(RelationProbs)

	for i in range(6):
		appxs.append(RelationProbs['I0'][category[0]][i] * RelationProbs['PA500'][category[1]][i] *
		RelationProbs['HFS'][category[2]][i] * 
		RelationProbs['DA'][category[3]][i] * RelationProbs['Area'][category[4]][i]
	 	* RelationProbs['ADA'][category[5]][i] * RelationProbs['MaxIP'][category[6]][i] * RelationProbs['DR'][category[7]][i]
	 	* RelationProbs['P'][category[8]][i])

	#print("appxs")
	#print(appxs)
	#υπολογισμός πιθανοφάνειας (appxs * P(car|fad|..))
	likehood = []
	for i in range(6):
		likehood.append(appxs[i] * ProbOfClass[i])

	sum_of_likehoods = sum(likehood)

	#πραγματικές πιθανότητες να είναι κάποια από τις 6 ασθένειες
	finalProbabilities = []
	for i in range(6):
		if (sum_of_likehoods != 0):
			finalProbabilities.append ( ( appxs[i] *  ProbOfClass[i] ) / ( sum_of_likehoods ) )
		else:
			finalProbabilities.append(0)

	#print("final")
	#print(finalProbabilities)
	#print (finalProbabilities.index(max(finalProbabilities)) + 1 )
	return (finalProbabilities.index(max(finalProbabilities)) + 1 )


def calculate_accuracy(data):
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
			#bayes_predict(bayes_train(training_data) , test_now) 
			if bayes_predict(bayes_train(training_data) , test_now) == testing_data[0][j]:
				corrects=corrects+1
			test_now=[]

		#(debug) εμφανίζει το accuracy του κάθε fold
		#print(corrects/fold_size)
		acc_sum=acc_sum+(corrects/fold_size)

		fold_start=fold_start+fold_size
		fold_end=fold_end+fold_size

	return(acc_sum/5)
