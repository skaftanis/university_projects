#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include <unistd.h>
#include <sys/socket.h> /* basic socket definitions */
#include <sys/types.h> /* basic system data types */
#include <sys/un.h> /* for Unix domain sockets */

//for shared memory
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include <sys/sem.h>     /* semaphore functions and structs.    */
#include <sys/wait.h>    /* wait(), etc.                        */

#define UNIXSTR_PATH "/tmp/unix.str"
#define SEM_ID    250	 /* ID for the semaphore.               */
#define LISTENQ 20



union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
};

void singint_handler (int sig_num) {
	signal(SIGINT,singint_handler);
	kill(getpid(), SIGKILL);
}

void sig_chld( int signo )
{
       pid_t pid;
       int stat;

       while ( ( pid = waitpid( -1, &stat, WNOHANG ) ) > 0 ) {
              printf( "Child %d terminated.\n", pid );
       }
}



int main( int argc, char **argv )
{

	int sem_set_id;
	int rc;		      /* return value of system calls.  */
	union semun sem_val;
	struct sembuf sem_op;
	/* first we create a semaphore set with a single semaphore, */
	/* whose counter is initialized to '0'.                     */
	sem_set_id = semget(SEM_ID, 1, IPC_CREAT | 0600);
	if (sem_set_id == -1) {
	    perror("semget");
	    exit(1);
	}

//	sem_val.val = 0;
//	    rc = semctl(sem_set_id, 0, SETVAL, sem_val);
//	    if (rc == -1) {
//		perror("main: semctl");
//		exit(1);
//	    }


	//ΜΕΡΟ1 - Έλεγχος ορισμάτων και αποθήκευση των τιμών τους
	/*
	Μετά το μέρος αυτό έχουμε στις μεταβλητές:
	#playerName: το όνομα του παίχτη
	#playerInventory: το inventory του παίχτη
	#serverHost: το hostname (δε χρησιμοποιείται μιας και είναι πάντα localhost)<
	*/

	//Αποθηκεύουν τις τιμές που δίνονται σαν ορίσματα από τον player (clint)
	char* playerName;
	char* playerInventory;
	char* serverHost;
	
	//χρησιμοποιούνται για τον έλεγχο ύπαρξης των 3ων παραπάνω τιμών.
	int nameCheck=0;
	int invCheck=0;


	//Μήνυμα λάθους και έξοδος εάν τα ορίσματα δεν είναι 6.
	if (argc != 6 && argc != 2) {
		printf("Wrong Syntax. Type ./player help for more info\n");
		exit(0);
	}
	
	//Περίπτωση όπου ο χρήστης ζητήσεις βοήθεια (./gameserver help)
	if ( argc == 2 && strcmp(argv[1],"help") == 0 ) {
		printf("Usage: ./player -n <name> -i <inventory> <server_host>\n");  
		exit(0);
	}

	int i; 
	//έλεγχος για τα ορίσματα 1,3 (αυτά που περιέχουν τα indexes)
	//Όταν βρεθεί κάποιο από τα 2 πιθανά indexes, τότε η κατάλληλη μεταβλητή παίρνει την τιμή του επόμενου ορίσματος
	//και το Check του καθενός γίνεται 1, πράγμα που συμαίνει ότι όντως βρέθηκε index αυτής της μορφής στη κλήση του server.
	for (i=1; i<=3; i+=2) {
		if ( strcmp(argv[i],"-n") == 0 ) {  
			playerName=argv[i+1];
			nameCheck=1;
			continue;
		}
		else if ( strcmp(argv[i],"-i") == 0 ) {
			playerInventory=argv[i+1];
			invCheck=1;
			continue;
		}

		else {
			printf("Wrong Syntax. Type ./gameserver help for more info\n");
			exit(0);
		}
	}
	
	//printf("%s", playerName);

	//Τιμή του host
	serverHost=argv[5];

	//Αν κάποιο από τα xxxCheck δεν έγινε 1, τότε κάποιο όρισμα δε δώθηκε οπότε ο server τερματίζεται με μήνυμα λάθους. 
	if ( nameCheck == 0 || invCheck == 0 )
		printf("Wrong Syntax. Type ./player help for more info\n");
	
	//ΜΕΡΟΣ2 - Τράβηγμα δεδομένων από το αρχείο inventory του χρήστη
	/*
	Μετά το μέρος αυτό έχουμε τους πίνακες
	#ResourceNames και #Quantity με τα στοιχεία του αρχείου
	*/

	//ορισμός μεταβλητών για τη πρόβαση στο αρχείο
	FILE *fp;
	char oneword[20]; 
	char c;
	int noOfWords=0;

	fp=fopen(playerInventory, "r"); /*Άνοιγμα αρχείου playerInventory για ανάγνωση  */
	//έλεγχος για  το αν υπάρχει το αρχείο
	if (fp == NULL) { 
 		 fprintf(stderr, "Error! Can't open input player inventory file!\n");
  	exit(0);
	}

	//Μέτρηση λέξεων που περιέρχει το αρχείο
	c = fscanf(fp,"%s",oneword);
	while ( c!= EOF ) {
		noOfWords++;
      	c = fscanf(fp,"%s",oneword);
	 }       
	 fclose(fp); //κλείσιμο αρχείου
   
	 /*Με δεδεομένο τον αριθμό των λέξεων του αρχείου δημιουργούνται οι παρακάτω παράλληλοι πίνακες
	 ResourseNames: Περιέχει όλα τα ονόματα των δυνάμεων
	 Quantity: Περιέχει την αντίστοιχη ποσότητά τους.*/

	 char ResourceNames[noOfWords/2][20];
	 int Quantity[noOfWords/2];

	 int realLenghts[noOfWords/2]; //στον παράλληλο πίνακα αυτό αποθηεκεύνται τα πραγματικά μήκη των αλφαριθμητικών 

	//νέο άνοιγμα του αρχείου για τα πραγματικά δεδομένα
	fp=fopen(playerInventory, "r");

	int count=0; //μετρητής έτσι ώστε να γίνεται ο έλεγχος για περιττή η άρτια επανάληψη
	int savePosQ=0; //δείκτης αποθήκευσης για πίνακα Quantiry
	int savePosR=0; //δείκτης αποθήκευσης για πίνακα ResourceNames

	c = fscanf(fp,"%s",oneword); /* got one word from the file */

	realLenghts[0]=strlen(oneword); //εισάγεται η πρώτη τιμή που είναι το μέγεθος της πρώτης λέξης που διαβάζεται
	int realCount=0; //μετρητής

	while (c!=EOF) { //loop διαβάσματος αρχείου όμοιο με πριν
	      	if (count % 2 != 0) {  //όταν είμαστε σε περιττή επανάληψη στο oneword υπάρχει Quantity (βλέπε μορφή αρχείου)
				Quantity[savePosQ]=atoi(oneword); //Το quantiry μπαίνει στον κατάλληλο πίνακα
				savePosQ++; //Ο δείκτης που δείχνει τη θέση αποθήκευσης αυξάνεται
			}
		 if ( count % 2 == 0  ) { //άρτια επανάληψη -> δύναμη (συμβολοσειρά)
		 	strcpy(ResourceNames[savePosR],oneword); //αποθηεύεται στον πίνακα ResourceNames
			savePosR++; //αύξηση δείκτη
			
		}

		c = fscanf(fp,"%s",oneword); /* διάβασμα επόμενης λέξης στο oneword */ 
		count++;
		//αφού έχει διαβαστεί η επόμενη λέξη και είμαστε σε επανάληψη όπου έχουμε Resource 
		//βάζουμε νέα τιμή στον πίνακα realLenghts
		if ( count % 2 == 0) {
			realCount++;
			realLenghts[realCount]=strlen(oneword);
		}

  	}

   fclose(fp);

	//ΜΕΡΟΣ 3 - Σύνδεση με server
	/*Σε αυτό το κομμάτι σετάρεται το socket έτσι ώστε να μπορεί να γίνει αποστολή  και λήψη
	δεδομένων στον server */

       int sockfd;
       struct sockaddr_un servaddr; /* Struct for the server socket address. */
       int pid;


       printf( "Connecting to server...\n" );

       sockfd = socket( AF_LOCAL, SOCK_STREAM, 0 ); /* Create the client's endpoint. */

       bzero( &servaddr, sizeof( servaddr ) ); /* Zero all fields of servaddr. */
       servaddr.sun_family = AF_LOCAL; /* Socket type is local (Unix Domain). */
       strcpy( servaddr.sun_path, UNIXSTR_PATH ); /* Define the name of this socket. */

      /* Connect the client's and the server's endpoint. */
       connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));


       //ΜΕΡΟΣ 4 - Αποστολή Αίτησης - Έλεγχος εγκυρότητας μέσω κοινής μνήμης

	//Αποστολή του playerName στον server (Αρχικό αίτημα) 
	//char send;
	//printf("READ:: ");
	//send=getchar();


	int shmid;
 	key_t key;
 	char *shm, *s;

	/*
	 * We need to get the segment named
	 * "5679", created by the server.
	 */
	key = 5679;

	 /*
	 * Locate the segment.
	 */
	if ((shmid = shmget(key, 1, 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }
	/*
	 * Now we attach the segment to our data space.
	 */
	 if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		 perror("shmat");
	 	 exit(1);
	 }

	 /*
	 * Now read what the server put in the memory.
	 */

	sleep(2);

	//όταν ο χρήστης διαβάζει δεδεδομένα από τον server o server μπλοκάρεται έτσι ώστε να μην αλλοιώνει τα δεδομένα αυτά. 
	sem_op.sem_num = 0;
	sem_op.sem_op = -1;  //down
	sem_op.sem_flg = 0;
	semop(sem_set_id, &sem_op, 1);


	printf("reading data from server...\n");

	//υπολογισμών γραμμών και λέξεων από το τωρινό inventory του server το οποίο βρίσκεται
	//στη shared memory
	int lines=0;
	int ServernoOfWords=0; 
	 for (s = shm; *s !='\0'; s++) {
		//putchar(*s);
		if (*s == '\t')
			lines++;	
		if (*s == '\t' || *s == '\n' )
			ServernoOfWords++;	
	}

	

	//Αποθήκευση τιμών από Shared Memory σε πίνακες για τον έλεγχο------
	
	//αρχικοποίηση πινάκων οι οποίοι θα περιέχουν τα στοιχεία που έχει ο server αυτή τη στιγμή. 
       	char ServerResourceNames[ServernoOfWords/2][20];
	char ServerQuantityChar[ServernoOfWords/2][20]; //για λόγους ευκολίας αρχικά είναι πίνακας χαρακτήρων. μετατρέπεται αργότερα.

	count=0; //μετρητής έτσι ώστε να γίνεται ο έλεγχος για περιττή η άρτια επανάληψη
	int savePosSQ=0; //δείκτης αποθήκευσης για πίνακα Quantiry
	int savePosSR=0; //δείκτης αποθήκευσης για πίνακα ResourceNames


	int resWordCounter=0; //δείχνει τη θέση κάθε νέας λέξης στους πίνακες ServerResourceNames, ServerQuantity
	int resCharCounter=0; //δείχενι τη θέση κάθε νέου χαρακτήρα στον κάθε πίνακα χαρακτήρων στον ServerResourceNames 
	int quaCharCounter=0;  //δείχενι τη θέση κάθε νέου χαρακτήρα στον κάθε πίνακα χαρακτήρων στον ServerQuantityChar
	int flag=0; //0 --> resourse, 1 --> quantity, προσδιορίζει το που γράφουμε-αναφερόμαστε σε κάθε επανάληψη
	int lengths[lines]; //αποθηκεύει το πλήθος των χαρακτήρων για κάθε γραμμή 
	s=shm; //αρχσικοποίηση s στην αρχή της shared memory
	int lineCounter=lines; 


	while (lineCounter != 0)  { //σε κάθε επανάληψη αναφερόμαστε σε έναν χαρακτήρα (*s το οποίο αυξάνεται κατά ένα σε κάθε loop. X
		//printf("--%c-- ", *s); 
		//Αν είναι ένας απλός χαρακτήρας και βρισκόμαστε σε flag=0 (resourse) γίνεται είσοδος τιμής στον ServerResourceNames
		if ( *s != '\t' && *s != '\n' && flag==0 ) {

			ServerResourceNames[resWordCounter][resCharCounter]=*s;
			resCharCounter++;
		}

		//Αν είναι ένας απλός χαρακτήρας και βρισκόμαστε σε flag=1 (quantity) γίνεται είσοδος τιμής στον ServerQuantity
		if ( *s != '\t' && *s != '\n' && flag==1 )  {
			//#todo - takes only 2-digits values ..fix
			ServerQuantityChar[resWordCounter][quaCharCounter]=*s;
			quaCharCounter++;
			//ServerQuantity[resWordCounter]=*(s-1)-'0';
			//ServerQuantity[resWordCounter]=ServerQuantity[resWordCounter]*10 + *(s)-'0';
			
		}

		//Αν βρούμε tab ή new line γίνεται togle στο flag έτσι ώστε γράψουμε στον κατάλληλο πίνακα την κατάλληλη τιμή
		if ( *s == '\t' || *s == '\n' ) {
			if (flag == 0) 
				flag=1;
			else
				flag=0;
		}
		
		//Αν είναι συγκεκριμένα new Line
		if ( *s == '\n' ) {
			lengths[resWordCounter]=resCharCounter; //σώζουμε το μέγεθος των χαρακτήρων του Resource στον παράλληλο πίνακα 
			resWordCounter++; //δείχνουμε πλέον στην επόμενη θέση του ServerResourceNames & ServerQuantity για τη συνέχεια
			resCharCounter=0; //ο δείκτης χαρακτήρων μηδενίζεται μιας και προετοιμαζόμαστε για νέα επανάληψη
			quaCharCounter=0; //το ίδιο και γι αυτόν τον δείκτη
			lineCounter--; //ο δεικτης γραμμών μειώνεται (για το while loop)
		}
		*s++;	//το s δείχνει στον επόμενο χαρακτήρα για την επόμενη επανάληψη 
	}

	// ο πίνακας χαρακτήρων ServerQuantityChar μετατρέπτεται σε πίνακα ακεραίων (ServerQuantity) με τη χρήση της atoi() 
	int ServerQuantity[ServernoOfWords/2];
	for (i=0; i<ServernoOfWords/2; i++) {
		ServerQuantity[i]=atoi(ServerQuantityChar[i]);
	}

	 //διάβασμα του Quota από shared memory (μία γραμμή κάτω από το inventory και αποθήκευσή του στην μεταβλητή quota
	 //TODO FIX this επιστρέπονται 2 ψηφία μόνο
	 lineCounter=0;
	 int quota;
	 //κλασικό loop στη shared memory μέσω του δείκτη s
	 for (s = shm; *s !='\0'; s++) {
		//αν φτάσουμε στην τελευταια γραμμή του inventory +1  χρησιμοποιούμε αυτό και το επόμενο στοιχείο για να φτιάξουμε
		//τον αριθμό σε ακέραια μορφή όπως και πριν και τερματίζουμε το loop. 
		if (lineCounter==lines) {
			if ( *(s+1) == '\n' ) { //ένα ψηφίο για αριθμός
				quota=*(s)-'0'; 
			} else { 
			quota=*(s)-'0'; 
			quota=quota*10 + *(s+1)-'0';
			}
			break;
		}
		//όταν βρεθεί new line προφανώς ο μετρητής αυξάνεται κατά ένα.
		if ( *s == '\n') lineCounter++;
	 }

	
	//υπολογισμός πλήθους αντικειμένων που θέλει ο παίχτης
	int wantedItemsNo=0;
	for (i=0; i<(noOfWords/2); i++) { 
		wantedItemsNo+=Quantity[i];
	}	
	//έλεγχος για το αν ξεπαρνάνε τα quota ή όχι 
	char dig[3]; 
	if ( wantedItemsNo > quota ) {
		printf("You can't use this inventory to this game. [Quota Error]\n"); 
		printf("Exiting...\n");		
		sleep(2);
		exit(0);
		}
	else {  //εάν δεν υπάρχει πρόβλημα με το quota, τότε γίνεται o έλεγχος αποθεμάτων από την shared memory
		printf("\"Quota\" is fine!\n"); 

		//έλεγχος αποθεμάτων------

		int j;
		int check=0; //γίνεται 1 εάν βρεθεί κάποιο καινό σε κάποια επανάληψη. 
		//για κάθε συναδιασμό από τα στοιεία του player inventory και του server inventory
		for (i=0; i<noOfWords/2 ;i++) {
			for (j=0; j<ServernoOfWords/2; j++) {
			//Αν βρεθεί κάποιο ίδιο (με το ίδιο όνομα) 
			if ( strcmp(ResourceNames[i],ServerResourceNames[j]) == 0   ) {
				check=1;
				//εάν δεν υπάρχει καθόλου απόθεμα εμφανίζεται το κατάλληλο μήνυμα και  η συνδεση διακόπτεται. 
				if (  ServerQuantity[j] == 0 ) {  
					
					printf("Error: There it not %s available\n", ResourceNames[i] );
					printf("Exiting...\n");
					sleep(2);
					exit(0);
				}

				//..ελέγχεται αν η ποσότητά του player είναι μεγαλύτερη από αυτή του server
				if ( ServerQuantity[j] < Quantity[i] ) {
					//..εάν είναι, εμφανίζεται το κατάλληλο προειδοποιητικό μήνυμα και δίνονται οι υπόλοιποι πόροι
					printf("There is not enought %s available\n", ResourceNames[i] ); 
					sleep(1);
					printf("You will take %d instead of %d\n", ServerQuantity[j], Quantity[i]);
					Quantity[i]=ServerQuantity[j];
				}
				
			} 
			//εάν είμαστε στην τελευταία επανάληψη του j και δεν έχει βρεθεί κάποιο κοινό (check=0) τότε αυτό σημαίνει ότι
			//πάμε να δώσουμε στον server άγνωστο αντικείμενο, οπότε ο client τερματίζεται με το ανάλογο μήνυμα. 
			if ( j == (ServernoOfWords/2)-1 && check==0) {
				printf("Unknown object %s \n", ResourceNames[i]);
				printf("Exiting...\n");
				sleep(2);
			}		 
			}
		check=0; //σε κάθε εξωτερική for το check προφανώς αρχικοποιείται και πάλι με 0. 
		}



		sem_op.sem_num = 0;
		sem_op.sem_op = 1; //up
		sem_op.sem_flg = 0;
		semop(sem_set_id, &sem_op, 1);


		//Αποστολή πρώτης γραμμής αιτήματος - Όνομα χρήστη
		for (i=0; i<strlen(playerName); i++) 
			write( sockfd, &playerName[i] , sizeof(char) );
		write( sockfd, "\n" , sizeof(char) );
		
		//αποστολή του inventory διατηρώντας το format του αρχείου
		//(σημείωση) το Quantity μεταφέρεται σαν string, όχι σαν ακέραιος
		for (i=0; i<noOfWords/2; i++) {
			for(j=0; j<realLenghts[i] ; j++) { 
				write( sockfd, &ResourceNames[i][j] , sizeof(char) );
			}

			write( sockfd, "\t" , sizeof(char) );
			sprintf(dig, "%d", Quantity[i]);
			//printf(">>%d<<", Quantity[i]);
			write( sockfd, &dig , 2 );
			write( sockfd, "\n" , sizeof(char) );
		}
		write( sockfd, "\n" , sizeof(char) );

	}

	int shmid2;
	key_t key2;
	char *shm2;
	char* s2;
	key2 = 5690; //κλειδί σύνδεσης shared memory. 
	// int i;
        char returnChar[100]; //η επιτρεφόμενη τιμή όπως διαβάστηκε (πριν τη μετατροπή σε int). 	 

	//Δημιουργία σύνδεσης shared memory
	if ((shmid2 = shmget(key2, 1, IPC_CREAT | 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }

	 if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
		perror("shmat");
 		exit(1);
 	}
	s2=shm2; //το s δείχνει στην shared memory


	int shmid3;
	key_t key3;
	char *shm3;
	char* s3;
	key3 = 5791; //κλειδί σύνδεσης shared memory. 
	// int i;
       // char returnChar[100]; //η επιτρεφόμενη τιμή όπως διαβάστηκε (πριν τη μετατροπή σε int). 	 

	//Δημιουργία σύνδεσης shared memory
	if ((shmid3 = shmget(key3, 1, IPC_CREAT | 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }

	 if ((shm3 = shmat(shmid3, NULL, 0)) == (char *) -1) {
		perror("shmat");
 		exit(1);
 	}
	s3=shm3; //το s δείχνει στην shared memory

        char send;


	int check=0;
	sleep(2); //SEMAPHORES HERE TODO 
	while( 1 ) {
	for (s2 = shm2; *s2 !='\0'; s2++) {	
		while ( *s2 == '1' ) {
			printf("Waiting for players...\n"); 
			sleep(5);
			check=1;
		}
		if (check == 1 ) 
			printf("START\n");
		check=0;
	}
	

             send = getchar();
	    for (s3 = shm3; *s3 !='\0'; s3++) 
			if (send == '\n' )
				printf("%c", *s3);	


	      if (send == '\n') check=0;
              write(sockfd, &send , sizeof(char));

       }


	close(sockfd);





	//for (;;) { }   

}





 
