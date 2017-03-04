package DataSructures;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class MainClass {
	
	public static  void main(String args[]) throws IOException {
		int re; //global variable for response
		int [] list = null; //global variable for the list
		
		do{ 
		System.out.println("***Starting Menu***");
		System.out.println("1.Sorting Algorithms");
		System.out.println("2.Search Algorithms");
		System.out.println("3.Exit");
		Scanner keyboard = new Scanner (System.in);
		re = keyboard.nextInt();
		
	    if ( re ==  1 ) { //Sorting Algorithms Staff
	    	System.out.println("Select An Sorting Algorithm");
	    	System.out.println("1.Quick Sort");
	    	System.out.println("2.Merge Sort");
	    	System.out.println("3.Insertion Sort");
	    	int algorithm = keyboard.nextInt();
	    	
	    	System.out.println("Select input method");
	    	System.out.println("1.Custom input");
	    	System.out.println("2.Worst Case scenario");
	    	System.out.println("3.Read Input From File");
	    	int method = keyboard.nextInt();
	    	
	        
	    	
	    	if (method == 1) {
	    		System.out.print("Enter the list of integers. (Seperate them with comma): ");
	    		String arrayList = keyboard.next();
	    		String[] items = arrayList.split(",");
	    		list = new int[items.length];
	    		for (int i=0; i< list.length; i++ )
	    			list[i]=Integer.parseInt(items[i]);
	    	}
	    	
	    	if (method == 2) {
	    		
	    		System.out.print("Enter input length: ");
	    		int length = keyboard.nextInt();
	    		list = new int [length];	
	    		if ( algorithm == 1)
	    			for (int i=0; i< length ; i++)
	    				list[i]=1;
	    		else if (algorithm == 3)
	    			for (int i=0; i< length ; i++)
	    				list[i]=length-1-i;
	    		else
	    			for (int i=0; i< length ; i++)
	    				list[i]=length-1-i;
	    			
	    		
	    	}
	    	
	    	if (method == 3) {

	    		int i=0;
	    		System.out.print("Select DataSet (1 to 5): ");
	    		int dataset = keyboard.nextInt();

	    		//set filename for read and length
	    		String fileName;
	    		int lines;
	    		if (dataset == 1) {
	    			fileName="src/dataset1.txt";
	    			lines=1000;
	    		}
	    		else if (dataset == 2) {
	    			fileName="src/dataset2.txt";
	    			lines=5000;
	    		}
	    		else if (dataset == 3) {
	    			fileName="src/dataset3.txt";
	    			lines=10000;
	    		}
	    		else if (dataset == 4) {
	    			fileName="src/dataset4.txt";
	    			lines=50000;
	    		}
	    		else { 
	    			fileName="src/dataset5.txt";
	    			lines=100000;
	    		}
	    		list=new int [lines];
	    		BufferedReader br = new BufferedReader( new FileReader(fileName)); //1000 elements in the file
	    		String line;
	    		while ((line = br.readLine()) != null) {
	    			try{
	    				list[i]=Integer.parseInt(line);
	    				i++;
	    			} catch (NumberFormatException e) {}
	    		}
	    		br.close();



	    	}

	    	
	    	//System.out.print("Before");
	    	//print(list); 
	    	System.out.println();
	 
	    	//final long startTime = System.currentTimeMillis();
	    	final long startTime =  System.nanoTime();
	    	if (algorithm == 1) QuickSort.sort(list);
	    	if (algorithm == 2)	MergeSort.sort(list);
	    	if (algorithm == 3) InsertionSort.sort(list);
	    	//final long endTime = System.currentTimeMillis();
	    	final long endTime = System.nanoTime();
	    	//System.out.print("After:");
	    	//print(list);
	    	System.out.println();
	    	System.out.println("Total execution time(nanoseconds): " + (endTime - startTime) );
	    	System.out.println("Total execution time(seconds): " + (endTime - startTime)*0.000000001 );

	    		
	    }
	    
	    else if ( re == 2 ) { //Search Algorithms Staff
	    	System.out.println("Select An Search Algorithm");
	    	System.out.println("1.Binary Search");
	    	System.out.println("2.Binary Interpolation Search");
	    	int algorithm = keyboard.nextInt();
	    	
	    	System.out.print("Enter the list of sorted integers. (Seperate them with comma): ");
    		String arrayList = keyboard.next();
    		String[] items = arrayList.split(",");
    		list = new int[items.length];
    		for (int i=0; i< list.length; i++ )
    			list[i]=Integer.parseInt(items[i]);
	    	
    		System.out.print("Enter the target number:");
    		int target = keyboard.nextInt();
    		if ( algorithm == 1) {
    			int result=BinarySearch.search(list,target);
	    		if ( result == -1 ) 
	    			System.out.print("There is not " +target + " in that list");
	    		else
	    			System.out.print("FOUND in position " +result + "\n" ); 
	    	}
    		if (algorithm == 2) {
    			int result=BinaryInterpolationSearch.search(list,target);
    			if (result == -1) 
    				System.out.print("There is not " +target + " in that list\n");
    			else 
    				System.out.print("FOUND in position " +result + "\n" ); 
    		}
	    	
	    	
	    }
	    	
	    
	    }while ( re != 3);
		System.out.println("Application Terminated");
	}
	

	private static void print (int[] list ) {
		System.out.print("["); 
		System.out.print(list[0]);
		for (int i=1; i<list.length; i++) 
			System.out.print(","+list[i]);
		System.out.print("]");

		
	}

}
