package DataSructures;

public class InsertionSort {

	public static void sort ( int [] A ) {
		int compares = 0;
		
		for (int i=1; i< A.length; i++) {
			int temp = A[i];
			int j;
			for (j=i-1; j>=0 && temp < A[j]; j--) { //2 compares here
				
				A[j+1]=A[j];
				compares++;
			}
			A[j+1]=temp;

		}
		System.out.print("Compares=");
		System.out.println(compares*2);
	}
	
	
}
