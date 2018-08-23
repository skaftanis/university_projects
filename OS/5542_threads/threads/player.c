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
#include <netdb.h>
#include <pthread.h>


#define UNIXSTR_PATH "/tmp/unix.str"
#define SEM_ID    250	 /* ID for the semaphore.               */
#define LISTENQ 20

//δημιουργία μεταβλητών socket
int port=5555;
int sock = -1;
struct sockaddr_in address;
struct hostent * host;
int len;

void *sendThread( void *ptr) {	
	printf("send Thread\n");
	char buffer[40];
	char send;
	int check=0;
	 while( 1 ) {
              send = getchar();	
              write(sock, &send , sizeof(char)); 
	}
        pthread_exit( NULL );
}

void *getThread( void *ptr) {
	printf("Get Thread\n");
	char buffer[40];	
	while( 1 ) {
		// pthread_mutex_lock(&lock);
		 read(sock, buffer,5);
	         printf("%s", buffer);
		 //pthread_mutex_unlock(&lock);
	} 


}

int main( int argc, char **argv )
{
	//ΜΕΡΟΣ1 - Έλεγχος ορισμάτων και αποθήκευση των τιμών τους
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

/* create socket */

 	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
		return -3;
	}

	/* connect to server */
	address.sin_family = AF_INET;
	address.sin_port = htons(5555);
	host = gethostbyname("localhost");
	if (!host)
	{
		fprintf(stderr, "%s: error: unknown host %s\n", argv[0], argv[1]);
		return -4;
	}
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
	{
		fprintf(stderr, " error: cannot connect to host\n");
		return -5;
	}


	//Μέρος 4 - Δημιουργία ενός string αποστολής
	/* Δημιουργεί ένα string (allStrings) το οποίο περιέχει όλα τα strings που χρειάζονται έτσι ώστε 
	να σχηματιστεί το inventory με τη μορφή που ήταν και στο αρχείο. 
	*/

	//Υπολογισμός του πλήθους των γραμμάτων που περιέχουν όλα τα ονόματα των Resource
	int reChars=0;
	for (i=0; i<noOfWords/2; i++) {
		reChars+=strlen(ResourceNames[i]);
	}
	len = strlen(playerName);
	//υπολογισμος του συνολικού μεγέθους του string allStrings και αποθήκευσή του στη μεταβλητή len2. 
	int len2=strlen(playerName)+reChars+noOfWords*2;

	//δήλωση μεταβλητών
	char dig[3];
	int j;
	char* allStrings;

	//Διαδικασία δημιουργίας του string αποστολής 
	for (i=0; i<noOfWords/2; i++) { 
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
		if (i==0) { //στην πρώτη επανάληψη δεσμεύουμε τον κατάλληλο χώρο για την allString (1η γραμμή) 
			allStrings=malloc(strlen(tempStr) + 2 + strlen(playerName) );
			allStrings[0]='\0';
			strcat(allStrings,playerName);
			strcat(allStrings, "\n");

		}
		else //στις επόμενες απλά αυξάνουμε τον χώρο αυτό έτσι ώστε να χωράνε όλα τα tempStrings
			allStrings=realloc(allStrings, strlen(allStrings)+strlen(tempStr) + 2);
		//σε κάθε επανάληψη προσαρτούμε στο allString το νέο tempStr και ένα new line. 
		//Έτσι, στο τέλος του loop στo allString θα βρίσκεται να string που θα έχει ότι ακριβώς έχει 
		//το inventory αυτή τη συκγεκριμένη στιγμή.  

		strcat(allStrings, tempStr);
		strcat(allStrings, "\n");
	}
	strcat(allStrings, "\n");

	//αποστολή inventory στον server
	write(sock, &len2, sizeof(int)); //Καθορισμός μεγέθους (len2) 
	write(sock, allStrings, len2);	 //Αποστολή αίτησης


	char send;
	int check=0;


	printf("Type your messages here: \n");
	//while loop για την αποστολή μηνυμάτων.
	while( 1 ) {
              send = getchar();	
              write(sock, &send , sizeof(char));
	}



	/* close socket */
	close(sock);

	
		


}
