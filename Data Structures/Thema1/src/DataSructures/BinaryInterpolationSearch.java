package DataSructures;

public class BinaryInterpolationSearch {
	
	public static int search (int[] A, int target){
		// Returns index of toFind in sortedArray, or -1 if not found
		int steps = 0;
		int low = 0;
		int high = A.length - 1;
		int mid;

		while (A[low] <= target && A[high] >= target) {
			steps++;
			mid = low +	((target - A[low]) * (high - low)) / (A[high] - A[low]);  //out of range is possible  here

			if (A[mid] < target)
				low = mid + 1;
			else if (A[mid] > target)
				// Repetition of the comparison code is forced by syntax limitations.
				high = mid - 1;
			else {
				System.out.println("Steps= " +steps);
				return mid;
			}
		}

		if (A[low] == target) {
			System.out.println("Steps= " +steps);
			return low;
		}
		else
			return -1; // Not found
	}

}
