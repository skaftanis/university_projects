import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


public class MainClass {
	
	public static void main ( String[] args ) throws IOException {
		
		int value;
		Tree234 theTree = new Tree234();

		while (true) {
			System.out.print("Select an option\n");
			System.out.print("0.Display | ");
			System.out.print("1.Insert | ");
			System.out.print("2.Find | ");
			System.out.print("3.Delete | ");
			System.out.print("4.Is Empty | ");
			System.out.print("5.Number Of Nodes | ");
			System.out.print("6. To Int Array ");
			Scanner keyboard = new Scanner (System.in);
			int choice = keyboard.nextInt();
			switch(choice) {
			case 0:
				theTree.displayTree();
				break;
			case 1:
				System.out.print("Enter value to insert ");
				value = getInt();
				theTree.insert(value);
				break;
			case 2:
				System.out.print("Enter value to find ");
				value=getInt();
				boolean found = theTree.contains(value);
				if (found) 
					System.out.println("*Found");
				else 
					System.out.println("*Could not found");
				break;
			case 3:
				System.out.print("Enter value to delete ");
				theTree.delete(keyboard.nextInt());
				break;
			case 4: 
				if (theTree.empty())
					System.out.print("*Is Empty\n");
				else 
					System.out.print("*Is Not Empty\n");
				break;
			case 5: 
				int nodes= theTree.NumberOfNodes(theTree.getRoot())+1;
				System.out.print("*Number Of Nodes " + nodes + "\n");
				break;
			case 6: 
				List<Integer> list = new ArrayList<Integer>();
				list=theTree.toIntArray(theTree.getRoot());
				for (int i=0; i<list.size(); i++)
					System.out.print(list.get(i)+"|");
				System.out.print("\n");
				break;
			default:
				System.out.print("Invalid Input");
		}
	}

  }
	public static String getString() throws IOException {
		InputStreamReader isr = new InputStreamReader(System.in);
		BufferedReader br= new BufferedReader(isr);
		String s = br.readLine();
		return s;
	}
	
	
	public static int getInt() throws IOException {
		String s=getString();
		return Integer.parseInt(s);
	}
	
	
}
	
	
