package DataSructures;

public class BinarySearch {
	
	public static int search ( int [] A, int target ) {
		
		int low = 0;
		int high = A.length-1;
		int test;
		int steps=0; 
		
		while ( low + 1 < high) {
			steps++;
			test=(low+high)/2;
			if (A[test] > target ) 
				high=test;
			else 
				low=test;
			
		}
		if (A[low] == target ) {
			System.out.print("Found in " +steps + " steps\n" );
			return low;
		}
		else
			return -1;	
	}
	
}
