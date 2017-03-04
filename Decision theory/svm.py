import numpy as np
import cvxopt
import cvxopt.solvers
import random


def linear_kernel(x1, x2):
    return np.dot(x1, x2)

def RBF_kernel(x,y, gamma=1):
    return np.exp(-gamma*np.linalg.norm(x-y)**2)

class SVM(object):

    def __init__(self, kernel='linear_kernel', C=None):
        self.kernel = kernel
        self.C = C
        if self.C is not None: self.C = float(self.C)

    def fit(self, X, y, gamma):

        cvxopt.solvers.options['show_progress'] = False

        n_samples, n_features = X.shape

        # υπολογισμός των kernels 
        K = np.zeros((n_samples, n_samples))
        for i in range(n_samples):
            for j in range(n_samples):
            	if self.kernel == 'linear_kernel':
               		K[i,j] = linear_kernel(X[i], X[j])
            	else:
               		K[i,j] = RBF_kernel(X[i], X[j], gamma=gamma)
 

        #δημιουργία απαραίτητων μητρώων που χρειάζονται για τη λύση του quadratic problem
        P = cvxopt.matrix(np.outer(y,y) * K)
        q = cvxopt.matrix(np.ones(n_samples) * -1)
        A = cvxopt.matrix(y, (1,n_samples))
        b = cvxopt.matrix(0.0)


        if self.C is None:
            G = cvxopt.matrix(np.diag(np.ones(n_samples) * -1))
            h = cvxopt.matrix(np.zeros(n_samples))
        else:
            tmp1 = np.diag(np.ones(n_samples) * -1)
            tmp2 = np.identity(n_samples)
            G = cvxopt.matrix(np.vstack((tmp1, tmp2)))
            tmp1 = np.zeros(n_samples)
            tmp2 = np.ones(n_samples) * self.C
            h = cvxopt.matrix(np.hstack((tmp1, tmp2)))

        # επίληση QP
        solution = cvxopt.solvers.qp(P, q, G, h, A, b)

        # υπολογισμός πολλαπλασιατων Lagrange
        a = np.ravel(solution['x'])

        sv = a > 1e-50
        ind = np.arange(len(a))[sv]
        self.a = a[sv]
        self.sv = X[sv]
        self.sv_y = y[sv]

        #υπολογιμός bias
        self.b = 0
        for n in range(len(self.a)):
            self.b += self.sv_y[n]
            self.b -= np.sum(self.a * self.sv_y * K[ind[n],sv])
        self.b /= len(self.a)

        # υπολογισμός w 
        if self.kernel == linear_kernel:
            self.w = np.zeros(n_features)
            for n in range(len(self.a)):
                self.w += self.a[n] * self.sv_y[n] * self.sv[n]
        else:
            self.w = None

    def predict(self, X):
        if self.w is not None:
            #υπολογισμός w στη περίπτωση που έχουμε non linear kernel
            project = np.dot(X, self.w) + self.b
        else:
            y_predict = np.zeros(len(X))
            #υπολογισμός προβολής του u στο w
            for i in range(len(X)):
                s = 0
                for a, sv_y, sv in zip(self.a, self.sv_y, self.sv):
                	if self.kernel == "linear_kernel":
                   		s += a * sv_y * linear_kernel(X[i], sv)
                	else:
                   		s += a * sv_y * RBF_kernel(X[i], sv)
                y_predict[i] = s
            project = y_predict + self.b
            #η κλάση (-1 ή 1) δίνεται απλά από το πρόσημο της προβολής
        return (np.sign(project))


#η συνάρτηση αυτή παίρνει σαν όρισμα τα δεδομένα και τα χωρίζει σε ένα 2D "πίνακα" (Χ) που περιέχει όλα τα features (x1,x2,x3...)
# και σε  ένα διάνυσμα y με τα αντίστοιχα labels
#εκτελεί 5-folds cross validation τεστάρει τα δεδομένα χρησιμοποιώντας τη κλάση SVM και επιστρέφει την accuracy 
def calculate_accuracy(all_data, kernel='RBF_kernel', C=10, gamma=3 ):
    #classes,I0,PA500,HFS,DA,Area,ADA,MaxIP,DR,P = all_data
    #all_data[0] = classes τα υπόλοιπα είναι τα δεδομένα
    classes=all_data[0]

    X=[]
    row = []
    #δημιουργία του Χ
    for i in range(len(classes)):
        for j in range(1,len(all_data)):
            row.append(all_data[j][i])
        X.append(row)
        row=[]

         #επιλογή 
    fold_size = int (len(classes) / 5)
    fold_start = 0
    acc_sum = 0


    #One Versul All strategy
    #λίστα test_data_class (θα περιέχει το νούμερο της κλάσης όταν τελικά βρεθεί. Μέχρι να βρεθεί θα έχει το 0)
    test_data_class = np.zeros(fold_size)

    #ανακάτεμα
    c = list(zip(X, classes))
    random.shuffle(c)
    random.seed()
    X, classes = zip(*c)

    classes=np.array(classes)

    noBugClasses = []
    for i in range(len(classes)):
        noBugClasses.append(classes[i])



    #δημιουργία του y (το classes πρέπει να μείνει σταθερό )
    #y=classes[:]
    y=classes[:]
    for i in range(len(y)):
        #χωρίσμός της κλάσης 1 και της κλάσης -1 (1ο βήμα)
        if y[i] != 1:
            y[i]=-1
        else:
            y[i]=1

    #one versus all loop 
    for onevsall in range(1,9):

        X=np.array(X)
        y=np.array(y)

      
        ### test 1o loop
        X_test = []
        y_test = []
        X_train = []
        y_train = []

        X_test = X[fold_start:(fold_start+fold_size)]
        y_test = y[fold_start:(fold_start+fold_size)] 

        if fold_start != 0:
            X_train.append(X[0:fold_start])
            y_train.append(y[0:fold_start])

        X_train.append(X[(fold_start+fold_size):len(classes)])
        y_train.append(y[(fold_start+fold_size):len(classes)])

        X_train=np.array(X_train[0])
        y_train=np.array(y_train[0])

        X_train=X_train.astype(np.double)
        y_train=y_train.astype(np.double)

        clf = SVM(kernel= kernel, C=C)
        clf.fit(X_train, y_train, gamma=gamma)

        y_predict = clf.predict(X_test)

        for predCounter in range(fold_size):
            #αν είναι στη θετική κλάση σημαίνει ότι είναι σε κάποια από τις ONEs που εξετάζονται
            if y_predict[predCounter] == 1:
                test_data_class[predCounter] = onevsall
            #στον τελευταίο έλεγχο αν κάποιο δεν έχει μπει τόση ώρα κάπου ανήκει στη κλάση 9
            if onevsall == 8:
                if test_data_class[predCounter] == 0:
                    test_data_class[predCounter]=9

        #αλλαγή των y (επόμενο ONE και επόμενα REST)
        y=noBugClasses[:]

        for i in range(len(y)):
            #χωρίσμός της κλάσης onevsall+1 και της κλάσης -1 (1ο βήμα)
            if y[i] != onevsall+1:
                y[i]=-1
            else:
                y[i] = 1

        X_test = []
        y_test = []
        X_train = []
        y_train = []

    #στο τέλος στο test_data_class έχω τη τελική πρόβλεψη κλάσης (multiclass)
    correct = np.sum(test_data_class == noBugClasses[fold_start:(fold_start+fold_size)])
    fold_accuracy = correct/fold_size
    return fold_accuracy
