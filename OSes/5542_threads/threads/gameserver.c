#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>	/* basic system data types */
#include <sys/socket.h>	/* basic socket definitions */
#include <errno.h>	 /* for the EINTR constant */
#include <sys/wait.h>	/* for the waitpid() system call */
#include <sys/un.h>	 /* for Unix domain sockets */
#include <pthread.h>
#include <linux/in.h>

#define UNIXSTR_PATH "/tmp/unix.str"
/* Size of the request queue. */
#define LISTENQ 20

#define NUM_THREADS 2
//handler για kill
void singint_handler (int sig_num) {
	signal(SIGINT,singint_handler);
	kill(getpid(), SIGKILL);
}

//αρχικοποίηση global μεταβλητών για πρόβαση και από το κύριο πρόγραμμα και από τον server
int Quantity[20]; //περιέχει τα Quantity για κάθε Resource Name
char ResourceNames[20][20];  //περιέχει τα Resoudrce Names
int quota; //αποθηκεύει τον αριθμό των quota που δίνεται σαν όρισμα στη main
int serverLines; //το πλήθος των γραμμών του Server Inventory
int players; //ο αριθμός των παιχτών που μπορούν να μπουν
int onlinePlayers=0; // oι online παίχτες αυτή τη στιγμή
int startCheck=0; 

pthread_mutex_t lock; //αρχικοποίηση μεταβλητής mutex

typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
} connection_t;

//main thread - Δημιουργείται ένα τέτοιο για κάθε client που συνδεέεται
void *process( void *ptr) {	

	//δημιουργία σύνδεσης με socket. 
        char* buffer;
	char line;
	int len;
	connection_t * conn;
	long addr = 0;

	if (!ptr) pthread_exit(0); 
	conn = (connection_t *)ptr;

	/* read length of message */
	read(conn->sock, &len, sizeof(int));
	
	//Έλεγχος για τον αριθμό των online Players και εμφάνιση κατάλληλων μηνυμάτων. Το onlinePlayers αυξάνεται τα διαφορετικό thread 	έτσι ώστε να αποφεύγονται συνθήκες ανταγωνισμού. 	
	while (onlinePlayers < players ) { 
		if (startCheck == 0 ) { 
			printf("Waiting for players\n");
			sleep(5); 
		}
 	}  	

	//Έλεγχος ορίου παιχτών
	if ( onlinePlayers>players ) { 
		printf("This player can't be added: Player limit reached\n"); 
		pthread_exit( NULL );
	}

	//Εμφάνιση μηνύματος έναρξης παρτίδας
	if (startCheck==0) {
		 printf("START\n");
		 startCheck=1;
	}

	if (len > 0) 
	{       
		addr = (long)((struct sockaddr_in *)&conn->address)->sin_addr.s_addr;
		buffer = (char *)malloc((len+1)*sizeof(char));
		buffer[len] = 0;

		/* read message */
		//o client στέλενει το inventory του και ο server το διαβάζει. 
		//το αίτημα αποθηκεύευται στον πίνακα re έτσι όπως διαβάζεται. 

	        char re[20][20]; //εδώ θα αποθηκευτεί το αίτημα από τον client
		//μετρητές για ορθή αποθήκευση

		//μεταβλητές που δείχνουν στο κατάλληλο σημείο του πίνακα που θα γίνει η αποθήκευση
	        int counterChar=0; 
	        int counterWord=0;
		int newLineinRow=0;

		while  (read( conn->sock, &line, sizeof(char) ) > 0 ) {
			re[counterWord][counterChar]=line; 
		        counterChar++;
		        if ( line == '\n' ) {  //ενημέρωση μεταβλητών σε περίπτωση που δισβαστεί new line
				counterWord++; 
				counterChar=0;
				newLineinRow++; 
			}
		//η τελευταία γραμμή του αιτήματος είναι ένα new line. Όταν διαβαστούν 2 συνεχώμενα new Lines συμαίνει ότι έχει 
		//τελειώσει η ανάγνωση του μηνύματος, οπότε γίνεται break στο loop.
			if ( newLineinRow == 2 ) 
				break;
			if ( line != '\n' ) 
				newLineinRow=0;
			//putchar(line);
		}
		int noOfLines=counterWord-2; 

	//Αποθήκευση τιμών από το re (αίτημα client) σε πίνακες----------
	
	//αρχικοποίηση πινάκων οι οποίοι θα περιέχουν τα στοιχεία που έχει δώσει ο client στο αίτημά του 
       	char ClientResourceNames[noOfLines][20];
	char ClientQuantityChar[noOfLines][20]; //για λόγους ευκολίας αρχικά είναι πίνακας χαρακτήρων. μετατρέπεται αργότερα.
	int i;
	int count=0; //μετρητής έτσι ώστε να γίνεται ο έλεγχος για περιττή η άρτια επανάληψη
	int savePosSQ=0; //δείκτης αποθήκευσης για πίνακα Quantity
	int savePosSR=0; //δείκτης αποθήκευσης για πίνακα ResourceNames


	int resWordCounter=0; //δείχνει τη θέση κάθε νέας λέξης στους πίνακες ClientResourceNames, ServerQuantity
	int resCharCounter=0; //δείχενι τη θέση κάθε νέου χαρακτήρα στον κάθε πίνακα χαρακτήρων στον ClientResourceNames 
	int quaCharCounter=0;  //δείχενι τη θέση κάθε νέου χαρακτήρα στον κάθε πίνακα χαρακτήρων στον ClientQuantityChar
	int flag=0; //0 --> resourse, 1 --> quantity, προσδιορίζει το που γράφουμε-αναφερόμαστε σε κάθε επανάληψη
	int lengths2[noOfLines]; //αποθηκεύει το πλήθος των χαρακτήρων για κάθε γραμμή 
	int lineCounter=noOfLines; 

	//μετρητές για να δείχουν σε ποιον ακριβώς χαρατήρα του s αναφερόματε
	int sWordCounter=1; //ξεκινάει από 1 γιατί στην s[0][i] υπάρχει το username. δείχνει λέξεις  s->[i][j]
	int sCharCounter=0; //δείχνει χαρακτήρες μέσα στη λέξη s[i][j]<-



	while (lineCounter != 0)  { //σε κάθε επανάληψη αναφερόμαστε σε έναν χαρακτήρα re το οποίο αυξάνεται κατά ένα σε κάθε loop. X
		//Αν είναι ένας απλός χαρακτήρας και βρισκόμαστε σε flag=0 (resourse) γίνεται είσοδος τιμής στον ClientResourceNames
		if ( re[sWordCounter][sCharCounter] != '\t' && re[sWordCounter][sCharCounter] != '\n' && flag==0 ) {
			ClientResourceNames[resWordCounter][resCharCounter]=re[sWordCounter][sCharCounter];
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


	//έλεγχοι
	//υπολογισμός πλήθους αντικειμένων που θέλει ο παίχτης
	int wantedItemsNo=0;
	for (i=0; i<noOfLines; i++) { 
		wantedItemsNo+=ClientQuantity[i];
	}
	


	//έλεγχος για το αν ξεπαρνάνε τα quota ή όχι 
	char dig[3]; 
	int j;
	if ( wantedItemsNo > quota ) {
		printf("You can't use this inventory to this game. [Quota Error]\n"); 
		printf("Exiting...\n");		
		sleep(2);
		exit(0);
		}
	else {  //εάν δεν υπάρχει πρόβλημα με το quota, τότε γίνεται o έλεγχος αποθεμάτων
		printf("\"Quota\" is fine!\n"); 
		//έλεγχος αποθεμάτων------

                pthread_mutex_lock(&lock);  //MUTEX LOCK. Όταν κάποιο thread μπαίνει στη κρίσημη αυτή περιοχή, μπλοκάρεται το mutex 
		//οπότε κανένα άλλο δε μπορεί να μεπι εδώ. Μόνο ένα thead μπορεί να εξυπηρετεί έναν client έτσι ώστε να ελέγχει τα 
		//σωστά στοιχεί και να μπορεί να καταλώνει. 

		int check=0; //γίνεται 1 εάν βρεθεί κάποιο καινό σε κάποια επανάληψη. 
		//για κάθε συναδιασμό από τα στοιεία του player inventory και του server inventory
		for (i=0; i<noOfLines; i++) {
			for (j=0; j<serverLines; j++) {
			//Αν βρεθεί κάποιο ίδιο (με το ίδιο όνομα) 
			if ( strcmp(ClientResourceNames[i],ResourceNames[j]) == 0   ) {
				check=1;
				//εάν δεν υπάρχει καθόλου απόθεμα εμφανίζεται το κατάλληλο μήνυμα και  η συνδεση διακόπτεται. 
				if (  Quantity[j] == 0 ) {  
					
					printf("Error: There it not %s available\n", ClientResourceNames[i] );
					sleep(2);
				}

				//..ελέγχεται αν η ποσότητά του player είναι μεγαλύτερη από αυτή του server
				if ( Quantity[j] < ClientQuantity[i] ) {
					//..εάν είναι, εμφανίζεται το κατάλληλο προειδοποιητικό μήνυμα και δίνονται οι υπόλοιποι πόροι
					printf("There is not enought %s available\n", ClientResourceNames[i] ); 
					sleep(1);
					printf("You will take %d instead of %d\n", Quantity[j], ClientQuantity[i]);
					ClientQuantity[i]=Quantity[j];
				}
				
			} 
			//εάν είμαστε στην τελευταία επανάληψη του j και δεν έχει βρεθεί κάποιο κοινό (check=0) τότε αυτό σημαίνει ότι
			//πάμε να δώσουμε στον server άγνωστο αντικείμενο, οπότε ο client τερματίζεται με το ανάλογο μήνυμα. 
			if ( j == (serverLines)-1 && check==0) {
				printf("Unknown object %s \n", ClientResourceNames[i]);
				printf("Exiting...\n");
				sleep(2);
			}		 
			}
			check=0; //σε κάθε εξωτερική for το check προφανώς αρχικοποιείται και πάλι με 0. 
		}
	


	//κατανάλωση στοιχείων και εμφάνιση των παλαιών και των καινούριων τιμών. 
	int j;
	for (i=0; i<noOfLines; i++)
		for (j=0; j<serverLines; j++) 
			if ( strcmp(ClientResourceNames[i],ResourceNames[j]) == 0 ) {
				printf("before: %s - %d | ", ClientResourceNames[i], Quantity[j]);
				Quantity[j]-=ClientQuantity[i]; //αφαίρεση από το quantity του server
				printf("after: %s - %d \n", ClientResourceNames[i], Quantity[j]);
			}
	
	}
               pthread_mutex_unlock(&lock); //Το mutex γίνεται unloak, οπότε το thread που περιένει μπορεί να μπει στη κρίσημη περιοχή του. 

	}

	//Messages area. Ο server διαβάζει και εμφανίζει τα μηνύματα που έρχονται από τους clients. 
	char msg[40];
	int pos=0;

	if ( players==onlinePlayers )  { //για να εμφανιστεί μόνο μία φορά το συγκεκριμένο μήνυμα
		printf("Messaging Area:\n");
	}
	while ( read( conn->sock, &line, sizeof( char ) ) > 0 ) {
			 if ( line == '\n' ) {
				msg[pos]='\0';
				pos=0;
				//write(conn->sock,msg, 5);
			} else {
			msg[pos]=line;
			pos++;
			}  
                        putchar( line );
	  } 

       
	/* close socket and clean up */
	close(conn->sock);
	free(conn);
	pthread_exit(0);
}

//thread το οποίο χρησιμοποιείται για την αύξηση της μεταβλητής onlinePlayers. 
void *countUsers( void *arg ) {
	onlinePlayers++;
        pthread_exit( NULL );
}



int main (int argc, char **argv) {
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
		
		int i; 

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
		
		quota=quotaPerPlay; //αποθήκευση της τιμή του quta στη global μεταβλητή για πρόσβαση από το thread
		players=noOfPlayers; //ομοίως
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

		serverLines=noOfWords/2; //αποθήκευση της τιμής στη global μεταβλητή για χρήση στο thread. 
	   
		 /*Με δεδεομένο τον αριθμό των λέξεων του αρχείου δημιουργούνται οι παρακάτω παράλληλοι πίνακες
		 ResourseNames: Περιέχει όλα τα ονόματα των δυνάμεων
		 Quantity: Περιέχει την αντίστοιχη ποσότητά τους.*/

		// char ResourceNames[noOfWords/2][20];
		// int Quantity[noOfWords/2]; 

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



	//MEΡΟΣ 3
	//socket construction
	int sock = -1;
	struct sockaddr_in address;
	int port;
	connection_t * connection;
	pthread_t thread;
	pthread_t thread2;



	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		fprintf(stderr, " error: cannot create socket\n");
		return -3;
	}

	/* bind socket to port */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(5555);
	if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
	{
		fprintf(stderr, "error: cannot bind socket.  Please try again in minute\n");
		return -4;
	}

	/* listen on port */
	if (listen(sock, 5) < 0)
	{
		fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
		return -5;
	}

	printf("%s: ready and listening\n", argv[0]);
	
	while (1)
	{
		/* accept incoming connections */
		connection = (connection_t *)malloc(sizeof(connection_t));
		connection->sock = accept(sock, &connection->address, &connection->addr_len);
		if (connection->sock <= 0)
		{
			free(connection);
		}
		else
		{
			/* start  new threads but do not wait for it */
			pthread_create(&thread, 0, process, (void *)connection); //δημιουργία process thread
			pthread_create(&thread2, 0,  countUsers, (void *)connection); //δημιουργία countUsers thresd
			pthread_mutex_init(&lock, NULL); 
		        pthread_mutex_destroy(&lock);
			pthread_detach(thread);
		}

	}


}


