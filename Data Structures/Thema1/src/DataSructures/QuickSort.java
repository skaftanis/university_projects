package DataSructures;



public class QuickSort {
	
	public static int compares = 0;
	
	//private static <T extends Comparable <T>>
	private static int split(int[] array, int low, int high ) {
		int left=low+1;
		int right=high; 
		int pivot = array[(low+right)/2];
		
		
		while (true) {
			while ( left <= right ) {
				if (array[left] < pivot  )
					left ++;
				else
					break;
				compares++;
			}

			while ( right > left ) {
				if ( array[right] < pivot ) 
					break; 
				else
					right --; 
				compares++;
			}

			if ( left >= right ) {
				compares++;
				break;
			}

			//swap array[left] with array[right]
			int temp = array[left]; 
			array[left]=array[right];
			array[right]=temp;
			left++; right--;
       
		}
		
		array[low] = array[left-1];
		array[left-1]=pivot;
		
		return left-1; //return the split pivot
	}
	
	//private static <T extends Comparable <T>>
	private static void sort (int[] array, int low, int high) {
		compares++;
		if ( (high-low) <= 0  ) 
			return;
		
		int splitPoint = split (array,low,high);
		sort(array, low, splitPoint-1);
		sort (array, splitPoint+1, high); 	
		
		
	}
	
	
	//public static <T extends Comparable <T>>
	public static void sort (int [] array) {
		if (array.length <= 1 ) 
			return;
		sort(array, 0, array.length-1);
		System.out.println("Compares="+compares);
	}

	
	
}
