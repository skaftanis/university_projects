package DataSructures;


public class MergeSort {
	
	public static int compares = 0;

	//Function for merging
	//L is the Left array, R is the Right and A is the started Array
	private static void merge ( int [] L, int[] R,  int [] A  ) {
		int i; // Left Array Pointer
		int j; // Right Array Pointer
		int k; // Basic Array Pointer
		i=0; k=0; j=0;
		while ( i < L.length && j < R.length ) {
			compares++;
			if ( L[i] < R[j] ) {
				
				A[k++] = L[i++];
			}
			else {
				
				A[k++] = R[j++];
			}
		}
		while ( i < L.length ) {
			compares++;
			A[k++] = L[i++];
		}
		while ( j < R.length ) {
			compares++;
			A[k++] = R[j++]; 
		}
	}
	
	//basic function
	public static void sort ( int[] A ){
		if ( A.length < 2 ) return;
		int mid = A.length/2;
		int [] Left = new int[mid];
		int [] Right = new int [A.length-mid];
		
		int i;
		
		for (i=0; i<mid; i++)
			Left[i]=A[i];
		
		for (i=mid; i<A.length; i++ )
			Right[i-mid]=A[i];
		
		sort(Left);
		sort(Right);
		merge(Left,Right,A);
		System.out.println("Compares="+compares);

	}

}
