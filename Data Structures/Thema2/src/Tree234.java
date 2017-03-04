import java.util.ArrayList;
import java.util.List;


public class Tree234 {
//	int noOfNodes=1;
	int noOfElements;
	private Node root = new Node ();
	
	public boolean contains (int key){
		Node curNode=root;
		int childNumber;
		while (true) {
			if ((childNumber=curNode.findItem(key)) != -1)
				return true;
			else if ( curNode.isLeaf())
				return false;
			else
				curNode=getNextChild(curNode,key);
		}
	}
	
	public Node find (int key){  //same with "contains" but it return the whole node when we found the key
		Node curNode=root;
		int childNumber;
		while (true) {
			if ((childNumber=curNode.findItem(key)) != -1)
				return curNode;
			else if ( curNode.isLeaf())
				System.out.print("");
			else
				curNode=getNextChild(curNode,key);
		}
	}
	
	public void insert (int dValue) {
		noOfElements++;
		Node curNode = root;
		DataItem tempItem = new DataItem(dValue);
		while (true) {
			if (curNode.isFull()) {
				split(curNode);
				curNode = curNode.getParent();
				curNode = getNextChild(curNode, dValue);
			}
			else if (curNode.isLeaf())
				break;
			else
				curNode = getNextChild(curNode, dValue);
		}
		curNode.insertItem(tempItem);
		
	}
	
	public void split (Node thisNode) {
		DataItem itemB, itemC;
		Node parent, child2, child3;
		int itemIndex;
		itemC=thisNode.removeItem();
		itemB=thisNode.removeItem();
		child2=thisNode.disconnectChild(2);
		child3=thisNode.disconnectChild(3);
		Node newRight = new Node();
		if (thisNode == root) {
			root= new Node();
			parent = root;
			root.connectChild(0, thisNode);
		}
		else
			parent = thisNode.getParent();
		itemIndex = parent.insertItem(itemB);
		int n = parent.getNumItems();
		for ( int j=n-1; j>itemIndex; j--) {
			Node temp = parent.disconnectChild(j);
			parent.connectChild(j+1,temp);
		}
		parent.connectChild(itemIndex+1, newRight);
		newRight.insertItem(itemC);
		newRight.connectChild(0,child2);
		newRight.connectChild(1, child3);
		
	}
	
	public Node getNextChild (Node theNode, int theValue) {
		int j;
		int numItems = theNode.getNumItems();
		for (j=0;j<numItems;j++)
			if (theValue < theNode.getItem(j).dData)
				return theNode.getChild(j);
		return theNode.getChild(j);
	}
	
	public void displayTree(){
		recDisplayTree(root,0,0);
	}
	
	private void recDisplayTree (Node thisNode, int level, int childNumber) {
		System.out.print("level= "+level+" child= "+childNumber+" ");
		thisNode.displayNode();
		int numItems=thisNode.getNumItems();
		for (int j=0; j<numItems+1; j++) {
			Node nextNode = thisNode.getChild(j);
			if (nextNode != null) {
				recDisplayTree(nextNode, level+1, j);
			}
			else {
				return;
			}
		}
		
	}
	
	
	
	public boolean empty () {
		if (root.isLeaf() && root.getItem(0)==null && root.getItem(1)==null && root.getItem(2)==null)
			return true;
		else
			return false;
		
	}
	
	public Tree234 delete ( int number) {
		
		Node theNode;
		Node Parent;
		int EmptyChildPos = 0;
		if (contains(number)) {
			theNode = find(number); //theNode is the node where we found the key
			if (!theNode.isLeaf()) 
				theNode=SwapWithInorderSuccessor(number);
				

			if ( theNode.getNumItems() > 1 ) //if node n doenn't contain another element
				theNode.deleteItem(number);
			 else {
			
				//redistribution
				Parent = theNode.getParent();
				
				
				
				int childsWithOneorTwoItems=0;
				for (int i=0; i<Parent.getNoofClids(); i++){
					if (Parent.getChild(i).getNumItems() == 0 ||Parent.getChild(i).getNumItems() == 1 ) 
						childsWithOneorTwoItems++;
				}
				if (childsWithOneorTwoItems < Parent.getNoofClids()) { //έλεγχος για το αν υπάρχει τουλάχιστον ένας κόβμος με 2+ στοιχεία
					EmptyChildPos = Parent.getChildNum(theNode); //pos of empty node
					System.out.print(EmptyChildPos);
					if (EmptyChildPos == 0 ){ //κυκλικά αριστερή αναδιανομή
						System.out.print("ARISTERA");
						theNode.insertItem(Parent.getItem(0));
						Parent.setItem(0, Parent.getChild(1).getItem(0).dData); //messy code
						Parent.getChild(1).deleteItem(Parent.getChild(1).getItem(0).dData);
					}
					else { //δεξια
						
						
						int move=Parent.getItem(EmptyChildPos-1).dData;
						DataItem item = new DataItem(move);
						//BUG HERE
						//Parent=theNode.getParent();
						theNode.insertItem(item);
					//	theNode.insertItem(Parent.getItem(EmptyChildPos-1));
					//	DataItem test = new DataItem(54);
					
						//theNode.insertItem(test);
						
						theNode.deleteItem(number);
						
						Parent.deleteItem(Parent.getItem(EmptyChildPos-1).dData);
						
						
						Node LeftChild = Parent.getChild(EmptyChildPos-1);
						LeftChild.deleteItem(EmptyChildPos-1);
						
						int LeftNodeItems = Parent.getChild(EmptyChildPos-1).getNumItems();
						Parent.insertItem(Parent.getChild(EmptyChildPos-1).getItem(LeftNodeItems) );
				//		System.out.print("***" + Parent.getItem(EmptyChildPos-1).dData);
					//	Parent.deleteItem(23);
		//*				if (Parent.getChild(EmptyChildPos-1).getNumItems() == 1)
		//*					Parent.deleteItem(Parent.getItem(EmptyChildPos-1).dData    );
							//Parent.getChild(EmptyChildPos-1).deleteItem(getChild(EmptyChildPos-1).getNumItems()-1);
						
						
					//	Parent.setItem(EmptyChildPos, getChild(EmptyChildPos-1).getItem(getChild(EmptyChildPos-1).getNumItems()-1).dData );
					//	
					} 
				}
				else { //merging time
					
				//	System.out.print("merging time");
					theNode.deleteItem(number);
					
					Node LeftChild = Parent.getChild(EmptyChildPos-1);
					LeftChild.insertItem(Parent.getItem(EmptyChildPos-1));	
				}
			 }

			
		}
		return this;
		
	}
	

//	private Node MergeNodes( int number) {
//		
//		Node theNode = null;
//		Node Parent;
//		if (contains(number)) 
//			theNode = find(number); 
//		Parent=theNode.getParent();
		
		
		
		
//	}
	
	
	
	private Node getChild(int i) {
		// TODO Auto-generated method stub
		return null;
	}

	public void test (int number) {
		Node theNode;
		Node Parent;
		if (contains (number)) {
			theNode=find(number);
			//Parent = theNode.getParent();
			theNode.deleteItem(20);
			//System.out.print(theNode.getNoofClids()); 
		}
		
	}
	
	
	
	
	private Node SwapWithInorderSuccessor(int number) {
		Node nextNumberNode = null;
		if (contains(number)) {
			Node theNode = find(number); 
		
		//find the inorder successor in the sorted list of all elements
		int nextNumber = 0;
		List<Integer> List = new ArrayList<Integer>();
		List = toIntArray (root); //get the sorted array
		for (int i=0; i<List.size();i++)  {
			if (List.get(i) == number) 
				nextNumber=List.get(i+1);
		}
		//System.out.print(nextNumber);
		
		//find the node where next Number belongs
		nextNumberNode = find(nextNumber);
		
		//make the swap
		DataItem old = new DataItem(number);
		DataItem with = new DataItem(nextNumber);
		
		//System.out.print("Old= " + old.dData + "new= " + with.dData);
		
		theNode.replace(old, with);	
		nextNumberNode.replace(with,old);
		
		}
		return nextNumberNode;
				
	}
	
	
	public List<Integer> toIntArray (Node curNode) {
		
		
	List<Integer> list = new ArrayList<Integer>();
	list=curNode.getSortedElements();
	
	int childs  = curNode.getNumItems() + 1;
	Node next;
	for (int i=0; i<childs; i++) {
		next = curNode.getChild(i);
		if ( !next.isLeaf() )
			list=merge(list,toIntArray(next));
	}
	return list;
	
	}
	
	private List<Integer> merge ( List<Integer> First , List<Integer> Second) { //merge two lists in one
		
		//remove same values from second list
		for (int i=0; i<First.size(); i++) 
			for (int j=0; j<Second.size(); j++) {
				if (First.get(i) == Second.get(j)) 
					Second.remove(Second.get(j));
			}
		
		//merge (like MergeSort!) but with arrayList
		
		List<Integer> Final = new ArrayList<Integer>();
         
     	int i; // Left Array Pointer
 		int j; // Right Array Pointer
 		i=0;  j=0;
 		while ( i < First.size() && j < Second.size() ) {
 			if ( First.get(i) < Second.get(j) ) {
 				Final.add( First.get(i++));
 			}
 			else {
 				Final.add(Second.get(j++));
 			}
 		}
 		while ( i < First.size() ) {
 			Final.add(First.get(i++));
 		}
 		while ( j < Second.size() ) {
 			Final.add(Second.get(j++));
 		}
 		
 		return Final; 
		
	}
	
	public int NumberOfNodes( Node curNode) { //return NumberOfNodes.
		//calculate the number of children
		int children=0;
		for (int i=0; i<4; i++) 
			if (curNode.getChild(i) != null )
				children++;
		
		for (int i=0; i<children; i++) {
			children+=NumberOfNodes(curNode.getChild(i));
		}
		return children;
		
	}
	
	
	public Node getRoot () {
		return root;
	}


}
