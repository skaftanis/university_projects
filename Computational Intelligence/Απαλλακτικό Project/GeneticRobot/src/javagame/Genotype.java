package javagame;

import java.util.ArrayList;


public class Genotype {
	
	//συντεταγμένες στις οποίες βρίσκεται έγγυρος δρόμος. Όλα τα άλλα θεωρούνται "τοίχοι"  (παράλληλα διανύσματα) 
	int validX[]={65,125,125,125,185,245,245,245,305,305,365,365,365,425};
	int validY[]={125,125,185,245,245,245,185,125,125,245,125,185,245,185};

	
	double Fitness; //η αξιολόγηση του γενότυπου
	Tree<String> genTree; //το δέντρο που το αναπαριστά
	
	ArrayList<String> algorithm = new ArrayList<String>(); //οι κινήσεις που εκτελεί τελικά (μετά την εκτέλεση των συναρτήσεων)
	//οι συντεταγμένες που τελικά έφτασε ο αλγόριθμος
	int arrX;
	int arrY;
	
	//οι συντεταγμένες του σημείου τερματισμού
	final int targetX=425;
	final int targetY=185;
	
	
	
	
	public void evaluate() { 
		
		double Euclidean_distance = Math.sqrt(  Math.pow( arrX-targetX , 2 ) + Math.pow( arrY-targetY , 2)  );
		int length = algorithm.size();
		this.Fitness=-Euclidean_distance-calculatePenalties()-length;
		
	}
	
	private int calculatePenalties() {
		
		int penalty=0;
		int X=65;
		int Y=125;
		
		for (int i=0; i<algorithm.size(); i++) {
			if ( algorithm.get(i).equals("move_forward")) 
				X+=60;

			else if (algorithm.get(i).equals("move_left" ) ) 
				Y-=60;

			else if (algorithm.get(i).equals("move_right")) 
				Y+=60;
			else if (algorithm.get(i).equals("move_back")) 
				X-=60;
			
			for (int j=0; j<validX.length; j++) {
				if ( !(X==validX[j] && Y==validY[j]) ) {
					penalty+=100;
					break;
				}
				else  {
					//penalty-=100;
				}
			}

		}
		
		return penalty;
	}
	
}
