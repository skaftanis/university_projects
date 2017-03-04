package javagame;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.Random;

public class Tree<T> {
	
    public Node<T> root;

    public Tree(T rootData) {
        root = new Node<T>();
        root.data = rootData;
    }

    public static class Node<T> {
         T data;
         List<Node<T>> parents = new ArrayList<Node<T>>();
         List<Node<T>> children = new ArrayList<Node<T>>();;
    }
    
    //δημιουργία κορυφής από τη μία κορυφή στην άλλη 
    public void new_edge (Node<T> n1, Node<T> n2) {
    	n1.children.add(n2);
    	n2.parents.add(n1);
    }
    
    //επιστρέφει το i-ιστό παιδί του κόμβου n
    public  Node<T> getClild(Node<T> n , int i) {
    	return n.children.get(i);
    }
    
    //επιστρέφει τον αριθμό των παιδιών του n
    public int noOfClildren ( Node<T> n ){
    	return n.children.size();
    }
    
    //επιστρέφει τον αριθμό των γονέων του n
    public int noOfParents ( Node<T> n ){
    	return n.parents.size();
    }
    
    //θέτει τα δεδομένα του κόμβου n 
    public void setData ( Node<T> n, T data){ 
    	n.data=data;
    }
    
    //αλλαγή της λίστας που περιέχει τα παιδιά
    public void updateChildren (Node n, ArrayList newChilder) {
    	n.children=newChilder;
    }
    
    //αλλαγή της λίστας που περιέχει τους γονείς (αν και θα είναι πάντα ή ένας η κανένας) 
    public void updateParents ( Node<T> n, ArrayList newParent) {
    	n.parents=newParent;
    }
    
    public void updateNode (Node<T> oldNode, Node<T> newNode) {
    	oldNode=newNode;
    }
    
    //επιστρέφει τα δεδομένα του κόβμου n
    public T getData ( Node<T> n) {
    	return n.data;
    }
    
    //ελέγχει αν ο κόμβος n είναι τερματικός 
    public boolean isTerminal( Node<T> n ) {
    	if (n.data.equals("move_forward") || n.data.equals("move_right") || n.data.equals("move_left") || n.data.equals("move_back") )
    		return true;
    	else
    		return false;
    }
    
    //επιστρέφει αν ο κόμβος n έχει παιδιά ή όχι
    public boolean hasChildren(Node<String> curNode) {
    	if ( curNode.children.size() == 0 )
    		return false;
    	else return true;
    }
    
    public Node<T> getRoot() {
    	return root;
    }
    

    public void printTree(Node tmpRoot) {

        Queue<Node> currentLevel = new LinkedList<Node>();
        Queue<Node> nextLevel = new LinkedList<Node>();

        currentLevel.add(tmpRoot);

        while (!currentLevel.isEmpty()) {
            Iterator<Node> iter = currentLevel.iterator();
            while (iter.hasNext()) {
                Node currentNode = iter.next();
                if (currentNode.children.size() > 0) {
                    nextLevel.add((Node) currentNode.children.get(0));
                }
                if (currentNode.children.size() > 0) {
                    nextLevel.add((Node) currentNode.children.get(1));
                }
                System.out.print(currentNode.data + " ");
            }
            System.out.println();
            currentLevel = nextLevel;
            nextLevel = new LinkedList<Node>();

        }

    }
    
    public ArrayList<Node> getAllNodes(Node tmpRoot) {
    	
    	 ArrayList<Node> returnList = new ArrayList<>();
    	
    	 Queue<Node> currentLevel = new LinkedList<Node>();
         Queue<Node> nextLevel = new LinkedList<Node>();

         currentLevel.add(tmpRoot);

         while (!currentLevel.isEmpty()) {
             Iterator<Node> iter = currentLevel.iterator();
             while (iter.hasNext()) {
                 Node currentNode = iter.next();
                 if (currentNode.children.size() > 0) {
                     nextLevel.add((Node) currentNode.children.get(0));
                 }
                 if (currentNode.children.size() > 0) {
                     nextLevel.add((Node) currentNode.children.get(1));
                 }
                // System.out.print(currentNode.data + " ");
                 returnList.add(currentNode);
                 
             }
             System.out.println();
             currentLevel = nextLevel;
             nextLevel = new LinkedList<Node>();

         }
         
         return returnList;
    	
    }
    
    
    //Επιστρέφει τα πιθανά XPoint που μπορούν να επιλαγούν για τη μετάλλαξη
    public int getRandomValidXpoint() {
    	
    	Random rand = new Random();
    	
    	ArrayList<Integer> returnList= new ArrayList<>(); 
    	ArrayList<Node> allNodes = this.getAllNodes(root);
    	
    	for (int i=0; i<allNodes.size(); i++) {
    		if ( !isTerminal(allNodes.get(i)) ) 
    				returnList.add(i);
    	}
    	
    	int random = rand.nextInt(returnList.size());
  
    	
    	return returnList.get(random);
     	
    }
    
    
    
    
     
    
    
    
    
    
    
    
}