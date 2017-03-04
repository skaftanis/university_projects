#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>	/* basic system data types */
#include <sys/socket.h>	/* basic socket definitions */
#include <errno.h>	 /* for the EINTR constant */
#include <sys/wait.h>	/* for the waitpid() system call */
#include <sys/un.h>	 /* for Unix domain sockets */

#include <signal.h>
#include <unistd.h>  

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>     /* semaphore functions and structs.    */
#include <sys/wait.h>    /* wait(), etc.                        */


#define UNIXSTR_PATH "/tmp/unix.str"
/* Size of the request queue. */
#define LISTENQ 20
#define SEM_ID    250	 /* ID for the semaphore.               */
#define SEM_ID2	  300

/* Η συνάτηση αυτή χρησιμοποιείται για να γίνει εγγραφή όλων των τιμών που βρίσκονται στους πίνακες
*  ResourceNames, Quantity στην Shared Memory καθώς και για τον αριθμό των χρηστών που είναι 
*  συνδεδεμένοι αυτοί τη στιγμή. Η υλοποίησή της βρίσκεται μετά τη main.
*/ 
void write2SM (char ResourceNames[][20], int Quantity[], int length, int users, int quotaPerPlay);
void writeMessage2SM2 (char,int);
void writeMessage2SM3 (char* name, char* msg, int pos);

int ReadDataFromSM(int);
union semun {
        int val;                    /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
};

/*
The use of this functions avoids the generation of
"zombie" processes.
*/
void sig_chld( int signo )
{
       pid_t pid;
       int stat;

       while ( ( pid = waitpid( -1, &stat, WNOHANG ) ) > 0 ) {
              printf( "Child %d terminated.\n", pid );
       }
}


void singint_handler (int sig_num) {
	signal(SIGINT,singint_handler);
	kill(getpid(), SIGKILL);
}

int main (int argc, char **argv) {

signal(SIGINT,singint_handler); 


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

sem_val.val = 1;  //αρχικά up
    rc = semctl(sem_set_id, 0, SETVAL, sem_val);
    if (rc == -1) {
	perror("main: semctl");
	exit(1);
    }

//semaphore2-------------------
int sem_set_id2;
int rc2;		      /* return value of system calls.  */
union semun sem_val2;
struct sembuf sem_op2;
/* first we create a semaphore set with a single semaphore, */
/* whose counter is initialized to '0'.                     */
sem_set_id2 = semget(SEM_ID2, 1, IPC_CREAT | 0600);
if (sem_set_id2 == -1) {
    perror("semget");
    exit(1);
}

sem_val2.val = 1;  //αρχικά up
    rc2 = semctl(sem_set_id2, 0, SETVAL, sem_val2);
    if (rc2 == -1) {
	perror("main: semctl");
	exit(1);
    }




//ΜΕΡΟΣ1 - Έλεγχος ορισμάτων και αποθήκευση των τιμών τους
/*
Μετά το μέρος αυτό έχουμε στις μεταβλητές:
#noOfPlayers: τον αριθμό των παιχτών
#quotaPerPlay: τον αριθμό των quota ανά παιχνίδι
#gameInventory: το όνομα του αρχείου που περιέχει το inventory
*/
	//Αποθηκεύουν τις τιμές: Αριθμός παιχτών, Quota ανά παιχνίδι και όνομα αρχείου GameInventory αντίστοιχα
	int noOfPlayers, quotaPerPlay;
	char* gameInventory;

	//χρησιμοποιούνται για τον έλεγχο ύπαρξης των 3ων παραπάνω τιμών.
	int NOPCheck=0;
	int QPPCheck=0;
	int GICheck=0;
	


	//Μήνυμα λάθους και έξοδος εάν τα ορίσματα δεν είναι 7.
	if (argc != 7 && argc != 2) {
		printf("Wrong Syntax. Type ./gameserver help for more info\n");
		exit(0);
	}

	//Περίπτωση όπου ο χρήστης ζητήσεις βοήθεια (./gameserver help)
	if ( argc == 2 && strcmp(argv[1],"help") == 0 ) {
		printf("Usage: ./gameserver -p <num_of_players> -i <game_inventory> -q <quota_per_player>\n");  
		exit(0);
	}

	int i; 
	//έλεγχος για τα ορίσματα 1,3,5 (αυτά που περιέχουν τα indexes)
	//Όταν βρεθεί κάποιο από τα 3 πιθανά indexes, τότε η κατάλληλη μεταβλητή παίρνει την τιμή του επόμενου ορίσματος
	//και το Check του καθενός γίνεται 1, πράγμα που συμαίνει ότι όντως βρέθηκε index αυτής της μορφής στη κλήση του server.
	for (i=1; i<=5; i+=2) {
		if ( strcmp(argv[i],"-p") == 0 ) {  
			noOfPlayers=atoi(argv[i+1]);
			NOPCheck=1;
			continue;
		}
		else if ( strcmp(argv[i],"-i") == 0 ) {
			gameInventory=argv[i+1];
			GICheck=1;
			continue;
		}
		else if ( strcmp(argv[i],"-q") == 0  ) {
			quotaPerPlay=atoi(argv[i+1]);
			QPPCheck=1;
			continue;
		}
		else {
			printf("Wrong Syntax. Type ./gameserver help for more info\n");
			exit(0);
		}
	}

	//Αν κάποιο από τα xxxCheck δεν έγινε 1, τότε κάποιο όρισμα δε δώθηκε οπότε ο server τερματίζεται με μήνυμα λάθους. 
	if ( NOPCheck == 0 || QPPCheck == 0 || GICheck == 0)
		printf("Wrong Syntax. Type ./gameserver help for more info\n");

	//ΜΕΡΟΣ2 - Τράβηγμα δεδομένων από το αρχείο gameInventory
	/*
	Μετά το μέρος αυτό έχουμε τους πίνακες
	#ResourceNames και #Quantity με τα στοιχεία του αρχείου
	*/

	//ορισμός μεταβλητών για τη πρόβαση στο αρχείο
	FILE *fp;
	char oneword[20]; 
	char c;
	int noOfWords=0;

	fp=fopen(gameInventory, "r"); /*Άνοιγμα αρχείου gameInventory για ανάγνωση  */
	//έλεγχος για  το αν υπάρχει το αρχείο
	if (fp == NULL) { 
 		 fprintf(stderr, "Error! Can't open input game inventory file!\n");
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

	//νέο άνοιγμα του αρχείου για τα πραγματικά δεδομένα
	fp=fopen(gameInventory, "r");

	int count=0; //μετρητής έτσι ώστε να γίνεται ο έλεγχος για περιττή η άρτια επανάληψη
	int savePosQ=0; //δείκτης αποθήκευσης για πίνακα Quantiry
	int savePosR=0; //δείκτης αποθήκευσης για πίνακα ResourceNames

	c = fscanf(fp,"%s",oneword); /* got one word from the file */

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

  	}

   fclose(fp);

    //αποθήκευση των αρχικών στοιχείων στην shared memory
    write2SM(ResourceNames,Quantity, noOfWords/2,0, quotaPerPlay);

//MEΡΟΣ 3
	

	//Αρχικοποίηση socket για να ακούει
       int listenfd, connfd; /* Socket descriptors. */
       //char* line;
       pid_t childpid;
       socklen_t clilen;
       struct sockaddr_un cliaddr, servaddr; /* Structs for the client and server socket addresses. */

       signal( SIGCHLD, sig_chld ); /* Avoid "zombie" process generation. */

       listenfd = socket( AF_LOCAL, SOCK_STREAM, 0 ); /* Create the server's endpoint */

       unlink( UNIXSTR_PATH ); /* Remove any previous socket with the same filename. */

       bzero( &servaddr, sizeof( servaddr ) ); /* Zero all fields of servaddr. */
       servaddr.sun_family = AF_LOCAL; /* Socket type is local (Unix Domain (local)). */
       strcpy( servaddr.sun_path, UNIXSTR_PATH ); /* Define the name of this socket. */

        /* Create the file for the socket and register it as a socket. */
       bind( listenfd, ( struct sockaddr* ) &servaddr, sizeof( servaddr ) );

       listen( listenfd, LISTENQ ); /* Create request queue. */

       


       for ( ; ; ) {  //loop για accept
	      char line; //θα αποθηκευτεί ένα χαρακτήρα από το αίτημα του (κάθε) client που συνδέεται στον server 
              clilen = sizeof( cliaddr );

	      /* Copy next request from the queue to connfd and remove it from the queue. */
              connfd = accept( listenfd, ( struct sockaddr * ) &cliaddr, &clilen );

              if ( connfd < 0 ) {
                     if ( errno == EINTR ) /* Something interrupted us. */
                            continue;	 /* Back to for(ace)... */
                     else {
                            fprintf( stderr, "Accept Error\n" );
                            exit( 0 );
                    }
              }
			

             childpid = fork(); /* Spawn a child. */
	    	 

              if ( childpid == 0 ) {	/* Child process. */
			      close( listenfd );	/* Close listening socket. */

				

				sem_op2.sem_num = 0;
				sem_op2.sem_op = -1;  
				sem_op2.sem_flg = 0;
				semop(sem_set_id2, &sem_op2, 1);

			
				sem_op.sem_num = 0;
			       	sem_op.sem_op = -1;  
			   	sem_op.sem_flg = 0;
			   	semop(sem_set_id, &sem_op, 1);

				printf("writing to SM...\n");

        if ( ReadDataFromSM(1) == 0 ) {  write2SM(ResourceNames,Quantity, noOfWords/2, 0 , quotaPerPlay); }


	 int shmid;
	 key_t key;
	 char *shm;
	 char* s;
	 key = 5679; //κλειδί σύνδεσης shared memory. 
	 int i;
	 char* allStrings;  //στο τέλος θα έχουν αποθηκευτεί όλες οι τιμές στο format του inventory.

	//Δημιουργία σύνδεσης shared memory
	if ((shmid = shmget(key, 1, IPC_CREAT | 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }

	 if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
 		exit(1);
 	}
	s = shm; //το s δείχνει στην shared memory


		
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



	//Αποθήκευση τιμών από Shared Memory σε πίνακες -----
	
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
			ServerResourceNames[resWordCounter][resCharCounter]='\0';
			ServerQuantityChar[resWordCounter][quaCharCounter]='\0';
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


 write2SM(ServerResourceNames,ServerQuantity, ServernoOfWords/2, ReadDataFromSM(1)+1 , quotaPerPlay);


//------------------

				//todo comments (αυτό το γράφει ΠΡΙΝ ΛΑΒΕΙ ΤΟ ΑΙΤΗΜΑ) 
				
				sem_op.sem_num = 0;
				sem_op.sem_op = 1;
				sem_op.sem_flg = 0;
				semop(sem_set_id, &sem_op, 1); //τέλος κρίσημης περιοχής



				char re[20][20]; //εδώ θα αποθηκευτεί το αίτημα από τον client
				//μετρητές για ορθή αποθήκευση
				int counterChar=0; 
				int counterWord=0;
				int newLineinRow=0;
				 while  (read( connfd, &line, sizeof(char) ) > 0 ) {
				 	re[counterWord][counterChar]=line; 
					counterChar++;
					if ( line == '\n' ) { 							
						counterWord++; 
						counterChar=0;
						newLineinRow++; 
					}
					if ( newLineinRow == 2 ) 
						break;
					if ( line != '\n' ) 
						newLineinRow=0;
					//putchar(line);
				}
				//close(connfd);
				//fflush(stdout);

				//οι γραμμές που αναφέρονται στο inventory είναι όσα τα new lines (counterWord) μείως 2 
				//ένα για τη new line που μπαίνει στο τέλος της αίτησης και ένα για αυτή που είναι μετά το όνομα χρήστη
				int noOfLines=counterWord-2; 




	//Αποθήκευση τιμών από το re σε πίνακες----------
	
	//αρχικοποίηση πινάκων οι οποίοι θα περιέχουν τα στοιχεία που έχει δώσει ο client στο αίτημά του 
       	char ClientResourceNames[noOfLines][20];
	char ClientQuantityChar[noOfLines][20]; //για λόγους ευκολίας αρχικά είναι πίνακας χαρακτήρων. μετατρέπεται αργότερα.

	 count=0; //μετρητής έτσι ώστε να γίνεται ο έλεγχος για περιττή η άρτια επανάληψη
	 savePosSQ=0; //δείκτης αποθήκευσης για πίνακα Quantiry
	 savePosSR=0; //δείκτης αποθήκευσης για πίνακα ResourceNames


	 resWordCounter=0; //δείχνει τη θέση κάθε νέας λέξης στους πίνακες ClientResourceNames, ServerQuantity
	 resCharCounter=0; //δείχενι τη θέση κάθε νέου χαρακτήρα στον κάθε πίνακα χαρακτήρων στον ClientResourceNames 
	 quaCharCounter=0;  //δείχενι τη θέση κάθε νέου χαρακτήρα στον κάθε πίνακα χαρακτήρων στον ClientQuantityChar
	 flag=0; //0 --> resourse, 1 --> quantity, προσδιορίζει το που γράφουμε-αναφερόμαστε σε κάθε επανάληψη
	int lengths2[noOfLines]; //αποθηκεύει το πλήθος των χαρακτήρων για κάθε γραμμή 
	 lineCounter=noOfLines; 

	//μετρητές για να δείχουν σε ποιον ακριβώς χαρατήρα του s αναφερόματε
	int sWordCounter=1; //ξεκινάει από 1 γιατί στην s[0][i] υπάρχει το username. δείχνει λέξεις  s->[i][j]
	int sCharCounter=0; //δείχνει χαρακτήρες μέσα στη λέξη s[i][j]<-



	while (lineCounter != 0)  { //σε κάθε επανάληψη αναφερόμαστε σε έναν χαρακτήρα s το οποίο αυξάνεται κατά ένα σε κάθε loop. X
	//	printf("--%c-- ", s[sWordCounter][sCharCounter]); 
		//Αν είναι ένας απλός χαρακτήρας και βρισκόμαστε σε flag=0 (resourse) γίνεται είσοδος τιμής στον ClientResourceNames
		if ( re[sWordCounter][sCharCounter] != '\t' && re[sWordCounter][sCharCounter] != '\n' && flag==0 ) {
			//printf("--%c-- ", s[sWordCounter][sCharCounter]); 
			ClientResourceNames[resWordCounter][resCharCounter]=re[sWordCounter][sCharCounter];
			//printf("\n resWo=%d , resChar=%d \n", resWordCounter ,resCharCounter );
			resCharCounter++;
			sCharCounter++;
		}

		//Αν είναι ένας απλός χαρακτήρας και βρισκόμαστε σε flag=1 (quantity) γίνεται είσοδος τιμής στον ServerQuantity
		if ( re[sWordCounter][sCharCounter] != '\t' && re[sWordCounter][sCharCounter] != '\n' && flag==1 )  {

			ClientQuantityChar[resWordCounter][quaCharCounter]=re[sWordCounter][sCharCounter];
			quaCharCounter++;
			sCharCounter++;
			
		}

		//Αν βρούμε tab ή new line γίνεται togle στο flag έτσι ώστε γράψουμε στον κατάλληλο πίνακα την κατάλληλη τιμή
		if ( re[sWordCounter][sCharCounter] == '\t' || re[sWordCounter][sCharCounter] == '\n' ) {
			if (flag == 0) 
				flag=1;
			else
				flag=0;
		}
		
		//Αν είναι συγκεκριμένα new Line
		if ( re[sWordCounter][sCharCounter] == '\n' ) {
			ClientResourceNames[resWordCounter][resCharCounter]='\0'; //σημάνει το τέλος του string (για την αποφυγή σκουπιδιών). 
			ClientQuantityChar[resWordCounter][quaCharCounter]='\0'; 
			sWordCounter++;
			sCharCounter=0;
			lengths2[resWordCounter]=resCharCounter; //σώζουμε το μέγεθος των χαρακτήρων του Resource στον παράλληλο πίνακα 
			resWordCounter++; //δείχνουμε πλέον στην επόμενη θέση του ServerResourceNames & ServerQuantity για τη συνέχεια
			resCharCounter=0; //ο δείκτης χαρακτήρων μηδενίζεται μιας και προετοιμαζόμαστε για νέα επανάληψη
			quaCharCounter=0; //το ίδιο και γι αυτόν τον δείκτη
			lineCounter--; //ο δεικτης γραμμών μειώνεται (για το while loop)
		}

		if ( re[sWordCounter][sCharCounter] == '\t' ) {
			sCharCounter++;
		}
	}

	// ο πίνακας χαρακτήρων ClientQuantityChar μετατρέπτεται σε πίνακα ακεραίων (ServerQuantity) με τη χρήση της atoi() 
	int ClientQuantity[noOfLines];
	for (i=0; i<noOfLines; i++) {
		ClientQuantity[i]=atoi(ClientQuantityChar[i]);
	}

	//Κατανάλωση πόρων από τον server ---------------------

	int j;
	for (i=0; i<noOfLines; i++)
		for (j=0; j<noOfWords/2; j++) 
			if ( strcmp(ClientResourceNames[i],ServerResourceNames[j]) == 0 ) {
				printf("before: %s - %d | ", ClientResourceNames[i], ServerQuantity[j]);
				ServerQuantity[j]-=ClientQuantity[i];
				printf("after: %s - %d \n", ClientResourceNames[i], ServerQuantity[j]);
			}
	
	//ανανέωση shared memory με τις νέες τιμές του inventory
        write2SM(ServerResourceNames,ServerQuantity, ServernoOfWords/2, ReadDataFromSM(1) , quotaPerPlay);
	


	sem_op2.sem_num = 0;
	sem_op2.sem_op = 1;
	sem_op2.sem_flg = 0;
	semop(sem_set_id2, &sem_op2, 1); //τέλος κρίσημης περιοχής


	//έλεγχος για το αν έχουν συπληρωθεί οι παίχτες
	while ( ReadDataFromSM(1) < noOfPlayers ) { 
	       writeMessage2SM2('1', 0 ); //εγγραφή του χαρακτήρα '1' στην SM 
		sleep(2);  //ελέγχει κάθε 2sec για το αν συμπληρώθηκαν οι παίχτες
	}
	//μόλις συμπληρωθεί ο αριθμός των παιχτών φτάφεται το '0' στη SM και η παρτίδα ξεκινά
	  writeMessage2SM2('0', 0 );

	  printf("Game Started\n");
	  printf("Messaging Area: \n");

	 char msg [100];
	 int msgPos=0;

/*	while ( read( connfd, &line, sizeof( char ) ) > 0 ) {
			 msg[msgPos]=line;
                         putchar( line );
			 if (line == '\n') {
				//writeMessage2SM3(re[0], msg, 0 );
			}
			
			// if (line == '\n' )  writeMessage2SM2('\0',msgPos); //msgPos=0;  
			// writeMessage2SM2(line,msgPos);
			 msgPos++;
	  }  
	  close(connfd);
	  write2SM(ServerResourceNames,ServerQuantity, ServernoOfWords/2, ReadDataFromSM(1) - 1 , quotaPerPlay);
	  if ( ReadDataFromSM(1) == 0 ) printf("Game Completed\n");
	  msg[msgPos]='\0';
          putchar( '\n' );
*/

	 
	
          exit( 0 ); /* Terminate child process. */


	 	
             }
		

		
      }

	
return 0; 
}








// ----------------------------write to shared memory function--------------------------------

void write2SM (char ResourceNames[][20], int Quantity[], int length, int users, int quota) {
 	 int shmid;
	 key_t key;
	 char *shm;
	 char* s;
	 key = 5679; //κλειδί σύνδεσης shared memory. 
	 int i;
	 char* allStrings;  //στο τέλος θα έχουν αποθηκευτεί όλες οι τιμές στο format του inventory.

	//Δημιουργία σύνδεσης shared memory
	if ((shmid = shmget(key, 1, IPC_CREAT | 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }

	 if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
 		exit(1);
 	}
	s = shm; //το s δείχνει στην shared memory
	

	for (i=0; i<length; i++) { //length = γραμμές των πινάκων εισόδου. 
		char* tempStr; //σε κάθε επανάληψη έχει μία γραμμή, όπως αυτή του inventory
		char* ResourceLine = ResourceNames[i]; //i-οστή λέξη του ResourceNames
		char QuantityStr[3]; 
		snprintf(QuantityStr, 3 ,"%d", Quantity[i]); //i-οστό Quantity μετατρεπόμενο σε char* από int

		//Με την mallon δεσμεύουμε χώρο ίσο με το άθροισμα των 2 string παραπάνω +2 
		// ένα για το \0 και ένα ακόμα για το \t (tab character)
		if((tempStr = malloc(strlen(ResourceLine)+strlen(QuantityStr)+2)) != NULL ){
		    tempStr[0] = '\0';   // ensures the memory is an empty string
		    //ένωση των 3ων αυτών strings στο tempStr
		    strcat(tempStr,ResourceLine);
		    strcat(tempStr,"\t");
		    strcat(tempStr,QuantityStr);
		} else {
		    //έλεγχος αποτυχίας malloc
		    printf("malloc failed!\n");
		    exit(1);
		}
		if (i==0) //στην πρώτη επανάληψη δεσμεύουμε τον κατάλληλο χώρο για την allString (1η γραμμή)
			allStrings=malloc(strlen(tempStr) + 1);
		else //στις επόμενες απλά αυξάνουμε τον χώρο αυτό έτσι ώστε να χωράνε όλα τα tempStrings
			allStrings=realloc(allStrings, strlen(allStrings)+strlen(tempStr) + 2);
		//σε κάθε επανάληψη προσαρτούμε στο allString το νέο tempStr και ένα new line. 
		//Έτσι, στο τέλος του loop στo allString θα βρίσκεται να string που θα έχει ότι ακριβώς έχει 
		//το inventory αυτή τη συκγεκριμένη στιγμή.  
		strcat(allStrings, tempStr);
		strcat(allStrings, "\n");

	}

		//μετατροπή quota σε string και είσοδος στο allStrings.
		char tempQuota[3];
		snprintf(tempQuota, 3 ,"%d", quota);
		strcat(allStrings, tempQuota);

		strcat(allStrings, "\n");

		//μετατροπή users σε string και είσοδος στο allStrings. (τελευταία γραμμή)
		char tempUsers[3];
		snprintf(tempUsers, 3 ,"%d", users);
		strcat(allStrings, tempUsers);

		
	
		//Όταν ολοκληρωθεί η διαδικασία το allStrings ανατίθεται στο s, άρα το περιεχόμενο αυτό γράφεται
		//στην κοινή μνήμη. 
		for (i=0; i<strlen(allStrings); i++)
			*s++=allStrings[i];
		*s='\0';



}
//Η shared memory 2 χρησιμοποιίται για να αποθηκεύσει ένα μόνο χαρακτήρα. Τον χαρακτήρα '1' εάν βρισκόμαστε σε waiting mode (δεν έχουν συμπληρωθεί οι απαιτούμενοι χρήστες δηλαδή) και τον χαρακτήρα '0' όταν έχουμε φύγει από αυτό το mode. Η παρακάτω συνάρτηση απλώς γράφει τον χαρακτήρα στη shared memory 2 (με key=5690). 
void writeMessage2SM2 (char line, int pos) { 

  	 int shmid;
	 key_t key;
	 char *shm;
	 char* s;
	 key = 5690; //κλειδί σύνδεσης shared memory. 
	 int i;
	 char* allStrings;  //στο τέλος θα έχουν αποθηκευτεί όλες οι τιμές στο format του inventory.

	//Δημιουργία σύνδεσης shared memory
	if ((shmid = shmget(key, 1, IPC_CREAT | 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }

	 if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
 		exit(1);
 	}
	s = shm; //το s δείχνει στην shared memory


	s[pos]=line;
	s[pos+1]='\0';


}

void writeMessage2SM3 (char* name, char* msg, int pos) { 

  	 int shmid;
	 key_t key;
	 char *shm;
	 char* s;
	 key = 5791; //κλειδί σύνδεσης shared memory. 
	 int i;
	 char* inName; 
	 char* inMsg;
	 char* allStrings;
	

	//Δημιουργία σύνδεσης shared memory
	if ((shmid = shmget(key, 1, IPC_CREAT | 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }

	 if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
 		exit(1);
 	}
	s = shm; //το s δείχνει στην shared memory

	for (i=0; i<strlen(msg); i++) {
		*s++=msg[i];
	}
	*s='\0';



}


// ----------------------------read last line of shared memory--------------------------------

//επιστρέφει το πλήθος των χρηστών που είναι online(τελευταία γραμμή της SM για value=1) και το quota (προ τελευταια γραμμή)
//για value=0
int ReadDataFromSM(int value) {
	 int shmid;
	 key_t key;
	 char *shm;
	 char* s;
	 key = 5679; //κλειδί σύνδεσης shared memory. 
	 int i;
	 char* allStrings;  //στο τέλος θα έχουν αποθηκευτεί όλες οι τιμές στο format του inventory.
	 char* returnChar; //η επιτρεφόμενη τιμή όπως διαβάστηκε (πριν τη μετατροπή σε int). 	 

	//Δημιουργία σύνδεσης shared memory
	if ((shmid = shmget(key, 1, IPC_CREAT | 0666)) < 0) {
	 	perror("shmget");
	 	exit(1);
	 }

	 if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
 		exit(1);
 	}
	s = shm; //το s δείχνει στην shared memory

	//μέτρηση γραμμών στην shared memory
	int lines=0;
	 for (s = shm; *s !='\0'; s++) {
		if (*s == '\t')
			lines++;		
	}
	
	int lineCounter=0;
	
	//loop σε όλη τη shared memory όπως και πριν
	for (s = shm; *s !='\0'; s++) {
		if(*s == '\n') //όταν βρεθεί new line αυξάνεται ο μετρητής. New Line εμφανίζεται στο τέλος κάθε σειράς όπως
			       //το ορίσαμε στη write2SM
			lineCounter++;

		if(*s == '\n' && lineCounter==lines+value) //όταν είμαστε στη τελευταία γραμμή (value=1) παίρνουμε τους χαρακτήρες που 								   βρίσκονται εκεί. Ομοίως και στην προτελευταία.
			returnChar=s;
			///printf("---> %s  <-----", s);

	}

	return atoi(returnChar); //μετατροπή του returnChart σε ακέραιο και επιστοφή.

	

}

	
