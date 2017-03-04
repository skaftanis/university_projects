package javagame;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Random;
import java.util.concurrent.TimeUnit;

import org.newdawn.slick.*;
import org.newdawn.slick.geom.Circle;
import org.newdawn.slick.geom.Rectangle;
import org.newdawn.slick.geom.Shape;
import org.newdawn.slick.state.*;

import javagame.Tree.Node;

public class Sim extends BasicGameState {

	int POP_SIZE = 1000;
	int MAXGENS = 30000;
	final float PXOVER = (float) 0.2; // Πιθανότητα διασταύρωσης
	final float PMUTATION = (float) 0.4; //πιθανότητα μετάλλαξης

	int Generation = 1;

	int robotX = 65;
	int robotY = 125;

	int oneTime = 0;

	// η συντεταγμένες που φτάνει τελικά ο αλγόριθμος
	int finalX;
	int finalY;


	//το καλύτερο genotype από κάθε γενιά
	ArrayList<Genotype> BestGenotypes = new ArrayList<>();

	int algCounter = 0;

	static ArrayList<Genotype> Genotypes = new ArrayList<Genotype>();

	private long counter = 0;

	// το ρομπότ
	Shape robot = new Circle(robotX, robotY, 15, 15);

	// συντεταγμένες στις οποίες βρίσκεται έγγυρος δρόμος. Όλα τα άλλα
	// θεωρούνται "τοίχοι" (παράλληλα διανύσματα)
	int validX[] = { 65, 125, 125, 125, 185, 245, 245, 245, 305, 305, 365, 365, 365, 425 };
	int validY[] = { 125, 125, 185, 245, 245, 245, 185, 125, 125, 245, 125, 185, 245, 185 };

	int globali = 0;
	
	//μεταβλητές για updata
	int genNo=0;
	int stepNo=0;

	public Sim(int state) {

	}

	@Override
	public void init(GameContainer arg0, StateBasedGame arg1) throws SlickException {
		if (globali++ == 0) {
			// εδώ γίνονται όλες οι διεργασίες. Στο render και στο update
			// γίνεται απλά η απεικόνηση.

			// -----------------------------INITIALIZE &
			// EVALUATE--------------------------

			// δημιουργία πληθυσμού

			for (int i = 0; i < POP_SIZE; i++) {
				Tree<String> rt = createRandomTree(1);

				Genotype gen = new Genotype(); // δημιουργία νέου γενότυπου
				gen.genTree = rt; // εκχώρηση του δέντρου που δημιουργήθηκε στην
									// αντίστοιχη μεταβλητή
				// η aglorithm της genotype θα έχει μέσα τον "αλγόριθμο" για
				// αυτό το δέντρο"
				gen.algorithm = getAlgorithm(rt);
				// οι συντεταγμένες που έφτασε ο αλγόριθμος.
				gen.arrX = finalX;
				gen.arrY = finalY;
				gen.evaluate();
				Genotypes.add(gen);

			}

		}

	}

	@Override
	public void render(GameContainer arg0, StateBasedGame arg1, Graphics g) throws SlickException {

		if (oneTime == 0) {

			int bingo = 0;
			while (Generation < MAXGENS) {
				Generation++;

				Selection();
				Mutate();
			    Crossover();
				Reproduction();
				
				//αντί για crossover (που δε δουλευέι καλά)
				//αρχικοποιούμε τυχαία ένα από τα γονίδια! 
				Random rand = new Random();
				int pos = rand.nextInt(POP_SIZE); 
				Tree randomTree = createRandomTree(3);
				Genotypes.get(pos).genTree=randomTree;
				Genotypes.get(pos).algorithm=getAlgorithm(randomTree);
				Genotypes.get(pos).arrX = finalX;
				Genotypes.get(pos).arrY = finalY;
				Genotypes.get(pos).evaluate();
				

				// find the best genotype
				double max = Genotypes.get(0).Fitness;
				int maxPos = 0;
				for (int i = 1; i < POP_SIZE; i++) {
					if (Genotypes.get(i).Fitness > max) {
						max = Genotypes.get(i).Fitness;
						maxPos = i;
					}
				}
				BestGenotypes.add(Genotypes.get(maxPos));

			}

			for (int i = 0; i < POP_SIZE; i++) {
				System.out.println(BestGenotypes.get(i).Fitness);
				System.out.println(BestGenotypes.get(i).algorithm);

			}
			
			oneTime = 1;


		}

		// κατασεκυή χάρτη

		g.setColor(Color.white);

		// για κάθε στήλη

		for (int j = 0; j < 7; j++) {

			for (int i = 0; i < 5; i++) {
				if (j == 0 && i != 1)
					g.fillRect(40, 40 + i * 60, 50, 50);
				if (j == 1 && (i == 0 || i == 4))
					g.fillRect(100, 40 + i * 60, 50, 50);
				if (j == 2 && i != 3)
					g.fillRect(160, 40 + i * 60, 50, 50);
				if (j == 3 && (i == 0 || i == 4))
					g.fillRect(220, 40 + i * 60, 50, 50);
				if (j == 4 && i != 1 && i != 3)
					g.fillRect(280, 40 + i * 60, 50, 50);
				if (j == 5 && (i == 0 || i == 4))
					g.fillRect(340, 40 + i * 60, 50, 50);
				if (j == 6 && i != 2)
					g.fillRect(400, 40 + i * 60, 50, 50);
			}
		}

		// the white line
		g.setColor(Color.white);
		g.drawRect(490, 0, 1, 360);

		g.setColor(Color.green);
		robot = new Circle(robotX, robotY, 15, 15);
		g.draw(robot);
		g.fill(robot);
		
		g.setColor(Color.white);
		g.drawString("Generation "+ Integer.toString(genNo), 500, 50);


	}

	
	//για το GUI
	@Override
	public void update(GameContainer arg0, StateBasedGame arg1, int delta) throws SlickException {
		arg0.setTargetFrameRate(12);

		//εάν έχουν ολοκληρωθεί οι γενιές
		if (oneTime == 1) {
			String step = BestGenotypes.get(genNo).algorithm.get(stepNo);
			if (step.equals("move_forward"))
				robotX+=60;
			else if (step.equals("move_right"))
				robotY+=60;
			else if (step.equals("mode_left") ) 
				robotY-=60;
			else if (step.equals("mode_bak")) 
				robotX-=60;
			
			if ( stepNo < BestGenotypes.get(genNo).algorithm.size()-1 ) {
				stepNo++;
			}
			else {
				stepNo=0;
				genNo++;
				//System.out.println("TREE");
				System.out.println(genNo);
				BestGenotypes.get(genNo).genTree.printTree(BestGenotypes.get(genNo).genTree.root);
				System.out.println(BestGenotypes.get(genNo).algorithm);
				System.out.println(BestGenotypes.get(genNo).Fitness);
				robotX=65;
				robotY=125;
			}
		}
		

	}

	@Override
	public int getID() {
		return 1;
	}

	// move the robot forward
	private void move_forward() {
		robotX += 60;

	}

	// move the robot right
	private void move_right() {
		robotY += 60;

	}

	// move the robot left
	private void move_left() {
		robotY -= 60;

	}

	// move the robot back
	private void move_back() {
		robotX -= 60;
	}

	/*
	 * Επιστρέφει true εάν υπάρχει τοίχος (όπως φαίνεται ο χάρτης) προς το
	 * direction To direction δείχνει προς τα που γίνεται ο έγεγχος για το αν
	 * υπάρχει τοίχος. Διαθέσιμες επιλογές: {forward,left,right}
	 */
	private boolean is_wall(String direction) {

		int wannabeX = robotX;
		int wannabeY = robotY;

		if (direction.equals("forward")) {
			wannabeX = robotX + 60;
		} else if (direction.equals("left")) {
			wannabeY = robotY - 60;
		} else if (direction.equals("right")) {
			wannabeY = robotY + 60;
		}

		for (int i = 0; i < validX.length; i++) {
			if ((validX[i] == wannabeX) && (validY[i] == wannabeY)) {
				return false;
			}
		}

		return true;

	}

	/*
	 * Δημιουργεί ένα τυχαίο δέντρο max επιπέδων. Στο δέντρο υπάρχουν οι
	 * διαθέσιμες συναρτήσεις και στα φύλλα τα τερματικά, βαλμένα τυχαία με τη
	 * μέθοδο της επαύξησης
	 */
	private Tree createRandomTree(int max) {

		Queue queue = new LinkedList();

		Random rand = new Random();
		int noOfNodes = rand.nextInt(max) + 1;
		boolean random;

		if (noOfNodes % 2 != 0)
			noOfNodes++;

		Tree<String> rt = new Tree<>(generateRandomFunction());

		queue.add(rt.getRoot());

		for (int i = 0; i < max; i++) {

			Node tempRoot = (Node) queue.remove();

			Node<String> n1 = new Node();
			Node<String> n2 = new Node();
			n1.data = Integer.toString(i);
			n2.data = Integer.toString(i);

			rt.new_edge(tempRoot, n1);
			rt.new_edge(tempRoot, n2);

			queue.add(n1);
			queue.add(n2);

		}

		for (int i = 0; i < rt.getAllNodes(rt.root).size(); i++) {
			rt.setData(rt.getAllNodes(rt.root).get(i), generateRandomFunction());
		}

		// μετατροπή φίλων σε τερματικά
		for (int i = 0; i < rt.getAllNodes(rt.root).size(); i++) {
			if (rt.getAllNodes(rt.root).get(i).children.size() == 0) {
				rt.setData(rt.getAllNodes(rt.root).get(i), generateRandomTerminal());

			}
		}

		return rt;

	}

	// παράγει μία από τις διαθέσιμες συναρτήσεις τυχαία
	private String generateRandomFunction() {
		Random rand = new Random();

		int random = rand.nextInt(3) + 1;

		if (random == 1)
			return "if_wall_forward";
		else if (random == 2)
			return "if_wall_right";
		else
			return "if_wall_left";

	}

	// παράγει ένα από τα διαθέσιμα τερματικά τυχαία
	private String generateRandomTerminal() {

		Random rand = new Random();
		int random = rand.nextInt(4) + 1;

		if (random == 1)
			return "move_forward";
		else if (random == 2)
			return "move_right";
		else if (random == 3)
			return "move_left";
		else
			return "move_back";

	}

	// algotithm-----------------------------------
	// Επιστρέφει ένα List με τον αλγόριθμο (βήματα) που τελικά εκτελούνται
	private ArrayList<String> getAlgorithm(Tree tree) {

		ArrayList<String> algorithm = new ArrayList<String>();

		String command = null;
		Node<String> nextNode = tree.root;

		for (int i = 0; i < 10; i++) {

			// αρχικά μπαίνει η root
			nextNode = commandOfFunction(tree, tree.root);

			while (isFunction(nextNode.data)) {
				nextNode = commandOfFunction(tree, nextNode);
			}

			command = nextNode.data;

			// System.out.println(command);

			if (command.equals("move_forward")) {
				algorithm.add("move_forward");
				move_forward();
			} else if (command.equals("move_right")) {
				algorithm.add("move_right");
				move_right();
			} else if (command.equals("move_left")) {
				algorithm.add("move_left");
				move_left();
			} else if (command.equals("move_back")) {
				algorithm.add("move_back");
				move_back();
			}

		}

		// ενημερώνει την τελική θέση του ρομπότ
		finalX = robotX;
		finalY = robotY;
		// μόλις τελειώσει επιστρέφει τις τιμές robotX και robotY στην αφετηρία
		// επειδή τις χρησιμοποιεί η update για τα γραφικά
		robotX = 65;
		robotY = 125;

		return algorithm;

	}

	// ελέγχει αν η εντολή είναι συνάρτηση ή τερματικό
	private boolean isFunction(String command) {
		if (command.equals("if_wall_forward") || command.equals("if_wall_right") || command.equals("if_wall_left"))
			return true;
		else
			return false;
	}

	// επιστρέφει τον κόβμο που θα εκτελέσει τελικά η συνάρτηση που δίνεται σαν
	// όρισμα (που βρίσκεται στο node που δίνεται)
	private Node commandOfFunction(Tree tree, Node node) {

		// Node<String> rootNode = tree.root;
		String command = null;
		Node<String> nextNode = null;
		String funcName = (String) node.data;

		if (funcName.equals("if_wall_forward")) {

			// εύρεση εντολής. αν υπάρχει τοίχος μπροστά αριστερό παιδί(0)
			// αλλιώς δεξί1)
			if (is_wall("forward")) {
				nextNode = tree.getClild(node, 0);
			} else {
				nextNode = tree.getClild(node, 1);
			}

		} else if (funcName.equals("if_wall_right")) {

			// εύρεση εντολής. αν υπάρχει τοίχος μπροστά αριστερό παιδί(0)
			// αλλιώς δεξί1)
			if (is_wall("right")) {
				nextNode = tree.getClild(node, 0);

			} else {
				nextNode = tree.getClild(node, 1);

			}

		}

		else if (funcName.equals("if_wall_left")) {

			// εύρεση εντολής. αν υπάρχει τοίχος μπροστά αριστερό παιδί(0)
			// αλλιώς δεξί
			if (is_wall("left")) {
				nextNode = tree.getClild(node, 0);
			} else {
				nextNode = tree.getClild(node, 1);

			}

		}

		return nextNode;

	}

	// -----------------------------------------------GENETICS--------------------------------------------------

	private void Selection() {

		double Total_Fitness;
		ArrayList<Genotype> newPopulation = new ArrayList<>();
		newPopulation.clear();
		// Total Fitness του πληθυσμού
		Total_Fitness = 0;
		
		for (int i=0; i<Genotypes.size(); i++) {
			Total_Fitness += Genotypes.get(i).Fitness;
		}

		// πιθανότητα επιλογής για κάθε genotype
		ArrayList<Double> Selection_Prob = new ArrayList<>();
		for (int i = 0; i < POP_SIZE; i++) {
			Selection_Prob.add(Genotypes.get(i).Fitness / Total_Fitness);
			// Genotypes.get(i).Selection_Prob=Genotypes.get(i).Fitness/Total_Fitness;
		}


		// αθροιστικές πιθανότητες για κάθε άτομο
		ArrayList<Double> Cumulative_Prob = new ArrayList<>();
		Cumulative_Prob.add(Selection_Prob.get(0));


		for (int i = 1; i < POP_SIZE; i++) {
			Cumulative_Prob.add(Cumulative_Prob.get(i - 1) + Selection_Prob.get(i));
		}

		// select survivors using cumulative probability and add it to the new
		// population
		Random rand = new Random();
		for (int spin_num = 0; spin_num < POP_SIZE; spin_num++) {
			boolean found = false;
			double roulette = rand.nextDouble();

			if (roulette < Cumulative_Prob.get(0))
				newPopulation.add(Genotypes.get(0));
			else if (roulette > Cumulative_Prob.get(POP_SIZE - 1))
				newPopulation.add(Genotypes.get(POP_SIZE - 1));
			else
				for (int mem = 1; mem < POP_SIZE && found == false; mem++)

					if ((roulette > Cumulative_Prob.get(mem - 1)) && (roulette <= Cumulative_Prob.get(mem))) {
						newPopulation.add(Genotypes.get(mem));
						found = true;
					}

		}


		// copy the temp population to global
		Genotypes.clear();
		Genotypes = newPopulation;


	}

	private void Crossover() {

		Random rand = new Random();
		int lovers = 0;
		int indiv = 0;
		int parent1;
		int parent2;
		int xover_point1, xover_point2;

		ArrayList<Boolean> mates = new ArrayList<>();

		for (int i = 0; i < POP_SIZE; i++)
			mates.add(false);
	

		// επιλογή κάποιων για μετάλαξη (true στη μεταλβητή Mate)
		for (int i = 0; i < POP_SIZE; i++) {
			double zero_to_one = rand.nextDouble();
			if (zero_to_one < PXOVER) {
				mates.set(i, true);
				//System.out.println("--->" + i);
				lovers++;
			}
		}

		//System.out.println(lovers);


		// αν έχουμε περιττό πλήθος
		if ((lovers % 2) != 0) {
			do {
				indiv = rand.nextInt(POP_SIZE);
			} while (mates.get(indiv) == true);

			mates.set(indiv, true);
			lovers++;
		}


		// αφού έχουμε διεσφαλίσει ότι θα είναι άρτιο πλήθος τα παίρνουμε ανα
		// δύο
		int mem = 0;
		for (int count = 0; count < (lovers / 2); count++) {
			while (mates.get(mem) == false)
				mem++;
			parent1 = mem;
			mem++;
			while (mates.get(mem) == false)
				mem++;
			parent2 = mem;
			mem++;


			// τυχαία επιλογή κόμβου συνάρτησης για το δέντρο που υπάρχει στη
			// θέση parent 1 του genotypes
			xover_point1 = Genotypes.get(parent1).genTree.getRandomValidXpoint();

			// το ίδιο και για το δεύτερο δέντρο
			xover_point2 = Genotypes.get(parent2).genTree.getRandomValidXpoint();


			// τραβάμε τα δέντρα
			Tree selectedTree1 = Genotypes.get(parent1).genTree;
			Tree selectedTree2 = Genotypes.get(parent2).genTree;

			// τραβάμε τους κοβμους
			Node selectedNode1 = (Node) selectedTree1.getAllNodes(selectedTree1.root).get(xover_point1);
			Node selectedNode2 = (Node) selectedTree2.getAllNodes(selectedTree2.root).get(xover_point2);

	
			// αντιμετάθεση data
			String tempdata = (String) selectedNode1.data;
			selectedNode1.data = selectedNode2.data;
			selectedNode2.data = tempdata;

			// αντιμετάθεση children

			ArrayList<Node> tempChildren = (ArrayList<Node>) selectedNode1.children;
			selectedNode1.children = selectedNode2.children;
			selectedNode2.children = tempChildren;

			// αντιμετάθεση parents
			ArrayList<Node> tempParents = (ArrayList<Node>) selectedNode1.parents;
			selectedNode1.parents = selectedNode2.parents;
			selectedNode2.parents = tempParents;

			// αναβάθμιση τιμών fitness
			Genotype temp1 = new Genotype();

			temp1.genTree = selectedTree1;
			temp1.algorithm = getAlgorithm(selectedTree1);
			temp1.arrX = finalX;
			temp1.arrY = finalY;
			temp1.evaluate();

			Genotype temp2 = new Genotype();

			temp2.genTree = selectedTree2;
			temp2.algorithm = getAlgorithm(selectedTree2);
			temp2.arrX = finalX;
			temp2.arrY = finalY;
			temp2.evaluate();

			Genotypes.set(parent1, temp1);
			Genotypes.set(parent2, temp2);



			/*
			 * πιο πολύπλοκος τρόπος που δε δούλεψε τελικά (για την ιστορία)
			 * //τραβάμε τον γονέα του κάθε κόμβου που επιλέχθηκε //και
			 * αποθηκεύουμε τη θέση στην οποία βρίσκεται (αριστερά(0) από τον
			 * πατέρα η δεξιά(1)) Node parentOf1=null; int posOf1=-1; //εάν έχει
			 * γονείς (δλδ δεν είναι root) if (selectedNode1.parents.size() > 0)
			 * { parentOf1=(Node) selectedNode1.parents.get(0); if
			 * (parentOf1.children.get(0) == selectedNode1 ) posOf1=0; else if (
			 * parentOf1.children.get(1) == selectedNode1) posOf1=1; }
			 * 
			 * Node parentOf2=null; int posOf2=-1; if
			 * (selectedNode2.parents.size() > 0) { parentOf2=(Node)
			 * selectedNode2.parents.get(0); if (parentOf1.children.get(0) ==
			 * selectedNode2) posOf2=0; else if ( parentOf1.children.get(1) ==
			 * selectedNode2 ) posOf2=1; }
			 * 
			 * 
			 * 
			 * 
			 * //τραβάμε τα παιδιά του κάθε κόμβου που επιλέχτηκε
			 * ArrayList<Node> childrenOf1=new ArrayList<>(); if (
			 * selectedNode1.children.size() > 0) {
			 * childrenOf1=(ArrayList<Node>) selectedNode1.children; }
			 * 
			 * ArrayList<Node> childrenOf2=new ArrayList<>(); if (
			 * selectedNode2.children.size() > 0) {
			 * childrenOf2=(ArrayList<Node>) selectedNode2.children; }
			 * 
			 * 
			 * 
			 * //ανταλλαγή γονέων if (parentOf2 != null)
			 * selectedNode1.parents.set(0, parentOf2);
			 * 
			 * if (parentOf1 != null) selectedNode2.parents.set(0, parentOf1);
			 * 
			 * //ανταλλαγή παιδιών if (childrenOf2.size() > 0 )
			 * selectedNode1.children=childrenOf2; if ( childrenOf1.size() > 0)
			 * selectedNode2.children=childrenOf1;
			 * 
			 * //update γονεών (έτσι ώστε να δείχνουν στα νέα τους παιδιά) if (
			 * posOf1 != -1 ) parentOf1.children.set(posOf1, selectedNode2);
			 * 
			 * if ( posOf2 != -1 ) parentOf2.children.set(posOf2,
			 * selectedNode1);
			 * 
			 * //update παιδιών (έτσι ώστε να δείχνουν στους νέους γονείς if (
			 * childrenOf1.size() > 0 ) { childrenOf1.get(0).parents.set(0,
			 * selectedNode2); childrenOf1.get(1).parents.set(0, selectedNode2);
			 * }
			 * 
			 * if (childrenOf2.size() > 0) { childrenOf2.get(0).parents.set(0,
			 * selectedNode1); childrenOf2.get(1).parents.set(0, selectedNode1);
			 * 
			 * }
			 * 
			 * String temp = (String) selectedNode1.data;
			 * selectedNode1.data=selectedNode2.data; selectedNode2.data=temp;
			 * 
			 * System.out.println("selected1(updadated) = " +
			 * selectedNode1.data); //System.out.println(
			 * "parent of selected Node1  " + selectedNode1.parents.get(0));
			 * System.out.println("selected2(updadated) = " +
			 * selectedNode2.data); //System.out.println(
			 * "parent of selected Node2  " + selectedNode2.parents.get(0));
			 * 
			 * 
			 * 
			 * 
			 * 
			 * //ανάθεση στα genotypes του αναβαθμισμένου selectedNodei
			 * //Genotypes.get(parent1).genTree.updateChildren(Genotypes.get(
			 * parent1).genTree.getAllNodes(Genotypes.get(parent1).genTree.root)
			 * .get(xover_point1), );
			 * Genotypes.get(parent1).genTree.updateNode(Genotypes.get(parent1).
			 * genTree.getAllNodes(Genotypes.get(parent1).genTree.root).get(
			 * xover_point1), selectedNode1);
			 * Genotypes.get(parent2).genTree.updateNode(Genotypes.get(parent2).
			 * genTree.getAllNodes(Genotypes.get(parent2).genTree.root).get(
			 * xover_point2), selectedNode2);
			 * 
			 * 
			 * 
			 * System.out.println("PARENT1");
			 * Genotypes.get(parent1).genTree.printTree(Genotypes.get(parent1).
			 * genTree.root); System.out.println("PARENT2");
			 * Genotypes.get(parent2).genTree.printTree(Genotypes.get(parent2).
			 * genTree.root);
			 */
			/*
			 * 
			 * 
			 * System.out.println("1---------------"+ parent1);
			 * Genotypes.get(parent1).genTree.printTree(Genotypes.get(parent1).
			 * genTree.root); System.out.println("2---------------" + parent2);
			 * Genotypes.get(parent2).genTree.printTree(Genotypes.get(parent2).
			 * genTree.root); System.out.println("END1---------------");
			 * 
			 * 
			 * 
			 * 
			 * ArrayList<Node> parent1Nodes =
			 * Genotypes.get(parent1).genTree.getAllNodes(Genotypes.get(parent1)
			 * .genTree.root); Node selectedNode1 =
			 * parent1Nodes.get(xover_point1); System.out.println("x_over1 = " +
			 * xover_point1);
			 * 
			 * ArrayList<Node> parent2Nodes =
			 * Genotypes.get(parent2).genTree.getAllNodes(Genotypes.get(parent2)
			 * .genTree.root); Node selectedNode2 =
			 * parent1Nodes.get(xover_point2); System.out.println("x_over2 = " +
			 * xover_point2);
			 * 
			 * 
			 * Node parentOf1 = (Node) selectedNode1.parents.get(0); Node
			 * parentOf2 = (Node) selectedNode2.parents.get(0);
			 * 
			 * //αν είναι το αριστερό ή το δεξί παιδί int child1pos=0,
			 * child2pos=0; if ( parentOf1.children.get(0) == selectedNode1 )
			 * child1pos=0; else if (parentOf1.children.get(1) == selectedNode1)
			 * child1pos=1;
			 * 
			 * if ( parentOf2.children.get(0) == selectedNode2 ) child2pos=0;
			 * else if (parentOf2.children.get(1) == selectedNode2) child2pos=1;
			 * 
			 * parentOf1.children.set(child1pos, selectedNode2);
			 * parentOf2.children.set(child2pos, selectedNode1);
			 * 
			 * selectedNode1.parents.set(0, parentOf2);
			 * selectedNode2.parents.set(0, parentOf1);
			 * //System.out.println("done");
			 * 
			 * 
			 * System.out.println("1---------------"+ parent1);
			 * Genotypes.get(parent1).genTree.printTree(Genotypes.get(parent1).
			 * genTree.root); System.out.println("2---------------" + parent2);
			 * Genotypes.get(parent2).genTree.printTree(Genotypes.get(parent2).
			 * genTree.root); System.out.println("END1---------------");
			 * 
			 */

		}


	}

	private void Mutate() {

		Random rand = new Random();

		if (rand.nextDouble() < PMUTATION) {

			int lengthOfRandomTree = rand.nextInt(5) + 1;
			int noOfSelectedGenotype = rand.nextInt(Genotypes.size());
			if (noOfSelectedGenotype == 0)
				noOfSelectedGenotype = 1;


			Tree selectedTree = Genotypes.get(noOfSelectedGenotype).genTree;



			Tree newTree = createRandomTree(lengthOfRandomTree);
			Node rootOfNewTree = newTree.root;
			Node rootOfOriginalTree = selectedTree.root;

			int randomNodeToChange = rand
					.nextInt(Genotypes.get(noOfSelectedGenotype).genTree.getAllNodes(rootOfOriginalTree).size());

			Node e1 = (Node) selectedTree.getAllNodes(selectedTree.root).get(randomNodeToChange);

			e1.data = rootOfNewTree.data;
			e1.children = rootOfNewTree.children;



			// αναβάθμιση τιμών fitness
			Genotype temp = new Genotype();
			temp.genTree = selectedTree;
			temp.algorithm = getAlgorithm(selectedTree);
			temp.arrX = finalX;
			temp.arrY = finalY;
			temp.evaluate();

			Genotypes.set(noOfSelectedGenotype, temp);

		}

	}

	private void Reproduction() {

		Random rand = new Random();

		double max = Genotypes.get(0).Fitness;
		int maxPos = 0;
		for (int i = 1; i < Genotypes.size(); i++)
			if (Genotypes.get(i).Fitness > max) {
				max = Genotypes.get(i).Fitness;
				maxPos = i;
			}

		double min = Genotypes.get(0).Fitness;
		int minPos = 0;
		for (int i = 0; i < Genotypes.size(); i++)
			if (Genotypes.get(i).Fitness < min) {
				min = Genotypes.get(i).Fitness;
				minPos = i;
			}

		Genotypes.set(minPos, Genotypes.get(maxPos));

	}

}
