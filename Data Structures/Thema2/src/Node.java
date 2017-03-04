import java.util.ArrayList;
import java.util.List;


public class Node {
	
	
	private static final int ORDER=4;
	private int numItems;
	private Node parent;
	private Node childArray[] = new Node [ORDER];
	private DataItem itemArray[] = new DataItem [ORDER-1];

	
	public void connectChild(int childNum, Node child) {
		childArray[childNum] = child;
		if (child != null) 
			child.parent=this;
	}
	
	
	public Node disconnectChild (int childNum) {
		Node tempNode = childArray[childNum];
		childArray[childNum]=null;
		return tempNode;
	}
	
	public Node getChild (int childNum) {
		return childArray[childNum];
	}
	
	public int getChildNum ( Node child) { //καλείται από τον πατέρα. επιστρέφει τη θέση του παιδιού στον πίνακα σε σχέση με τον πατέρα
		for (int i=0; i<ORDER-1; i++) {
			if (childArray[i]==child)
				return i;
		}
		return -1;
		
	}
	
	public int getNoofClids () {
		int counter=0;
		for (int i=0; i<ORDER-1; i++)
			if ( childArray[i] != null)
				counter++;
		return counter;
	}
	
	public Node getParent(){
		return parent;
	}
	public boolean isLeaf(){
		return (childArray[0]==null?true:false);
	}
	public int getNumItems(){
		return numItems;
	}
	
	public DataItem getItem (int index) {
		return itemArray[index];
	}
	
	
	
	public void setItem (int index, int number) {
		itemArray[index].dData=number;
	}
	
	public boolean replace (DataItem old , DataItem with ) {
		boolean done=false;
		for (int i=0; i<itemArray.length;i++)
			if (itemArray[i].dData == old.dData) {
				itemArray[i].dData=with.dData;
				done=true;
				break;
			}
		return done;
		
	}
	
	public boolean isFull(){
		return (numItems==ORDER-1)?true:false;
	}
	
	public int findItem (int key) {
		for (int j=0; j<ORDER-1; j++){
			if (itemArray[j]==null)
				break;
			else 
				if (itemArray[j].dData==key)
					return j;
		}
		return -1;
	}
	
	public DataItem removeItem(){ //remove largest item
		DataItem temp = itemArray[numItems-1];
		itemArray[numItems-1]=null;
		numItems--;
		return temp;
	}
	
	public void deleteItem (int number) { //suppose we delete only one

		List<DataItem> list = new ArrayList<DataItem>();
		for (int i=0; i<numItems; i++)
			if (itemArray[i].dData != number)
				list.add(itemArray[i]);
		
		for (int i=0; i< list.size(); i++) 
			itemArray[i].dData=list.get(i).dData;
	    itemArray[list.size()]=null;
	    numItems--;
		
	}
	
	public void displayNode() {
		for (int j=0; j<numItems; j++)
			if (itemArray[j] != null )
				itemArray[j].displayItem();
			System.out.println("/"); 
				
			
	}
	
	public int insertItem (DataItem newItem) {
		//έστω ότι ο κόμβος δεν είναι γεμάτος
		numItems++;
		int newKey= newItem.dData;
		
		for (int j=ORDER-2; j>=0; j--) {
			if (itemArray[j] == null ) 
				continue;
			else {
				int itsKey=itemArray[j].dData;
				if (newKey < itsKey)
					itemArray[j+1]=itemArray[j];
				else {
					itemArray[j+1]=newItem;
					return j+1;
				}
			}
		}
		itemArray[0]=newItem;
		return 0;
	}
	
	public ArrayList  getSortedElements ( ) { //return an array with elements of this node and its child sorted
		
		List<Integer> sortedElements = new ArrayList<Integer>();
		List<Integer> tempList = new ArrayList<Integer>();
		
		//find nodeElements
		int nodeElements = getNumItems();
		for (int i=0; i<=nodeElements; i++) {
			Node tempNode = getChild(i);
			for (int j=0; j<tempNode.getNumItems(); j++)
				tempList.add(tempNode.getItem(j).dData); 
			if (i != nodeElements)
				tempList.add(getItem(i).dData);
		}
		sortedElements=tempList;
		return (ArrayList) sortedElements;
			
		
	}

}
