import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;



public class Mageiras {
		
	static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
	static  String DB_URL = "jdbc:mysql://localhost/Restaurant";
	static  String USER = "root";
	static  String PASS = "171517";
	private JList list = null;
	private JList platesJList;
	private JFrame frame;
	private JFrame platesFrame;
	private int selectedItem=0;
	private int selectedPlate=0;
	private ArrayList<Integer> idList;
	
	private ArrayList<String> platesList;
	private ArrayList<Boolean> platesDone;
	private int time=0;
	

	
	public Mageiras() {
		
		
		if ( Main.settingsChanged == true) {
			DB_URL= Main.dbNameField.getText();
			USER = 	Main.usernameField.getText();
			PASS =  Main.passwordField.getText();
		}
	
        //Create and set up the window.
        frame = new JFrame();
        frame.setSize(600,500);
        frame.setLocationRelativeTo(null);
        frame.setTitle("Italian Restaurant - Cook");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        //layout setup
        BoxLayout box = new BoxLayout(frame.getContentPane(),BoxLayout.Y_AXIS);
        frame.setLayout(box);
        
        
        //label setup
        JLabel label = new JLabel("Select the order to prepare. First is the older(recommended)");
        label.setAlignmentX(Component.CENTER_ALIGNMENT);
        frame.getContentPane().add(label);
        
        createList(frame);


        JButton nextButton = new JButton ("Next");
        nextButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        nextButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
                frame.setVisible(false); 
                frame.dispose(); 
                
        	    platesDone = new ArrayList<Boolean>();
                platesList();
			}
        	
        }); 
        
        
        frame.getContentPane().add(nextButton);
  
        //Display the window.
        frame.setVisible(true);
    }
	
	//reads staff from database and save them to a ArrayList
	private void createList(JFrame frame) { 
    idList = new ArrayList<Integer>();
    ArrayList<String> messageList = new ArrayList<String>();

	 Connection conn = null;
   	 Statement stmt = null;
   	   try{
   	      //STEP 2: Register JDBC driver
   	      Class.forName("com.mysql.jdbc.Driver");

   	      //STEP 3: Open a connection
   	      System.out.println("Connecting to database...");
   	      conn = DriverManager.getConnection(DB_URL,USER,PASS);

   	      //STEP 4: Execute a query
   	      System.out.println("Creating statement...");
   	      stmt = conn.createStatement();
   	      String sql;
   	      sql = "select * from PERILAMVANEI_PIATA inner join PARAGELIA on PARAGELIA.id=PERILAMVANEI_PIATA.id_paragelias order by imerominia,wra;";
   	      ResultSet rs = stmt.executeQuery(sql);

   	      //STEP 5: Extract data from result set
   	      while(rs.next()){
   	         //Retrieve by column name
   	    	 int id = rs.getInt("id_paragelias");
   	         
   	         if (idList.size() >= 1) {
   	        	 if (idList.get(idList.size()-1) != id)
   	        		idList.add(id);
   	         }
   	         else
   	            	idList.add(id);
        
   	      }
   	      
   	      
   	      
   	      for (int i=0; i<idList.size(); i++)
   	    	messageList.add("Order with number: "+ idList.get(i));
   	      
   	      list=new JList(messageList.toArray());
   	      frame.add(new JScrollPane(list));
   	      
   	      list.addListSelectionListener( 
   	    		  new ListSelectionListener(){
   	    			  public void valueChanged(ListSelectionEvent event) {
   	    				  setSelected(list.getSelectedIndex());
   	    			  }
   	    		  }
   	    		  );
   	      
   	      //STEP 6: Clean-up environment
   	      rs.close();
   	      stmt.close();
   	      conn.close();
   	   }catch(SQLException se){
   	      //Handle errors for JDBC
   	      se.printStackTrace();
   	   }catch(Exception e){
   	      //Handle errors for Class.forName
   	      e.printStackTrace();
   	   }finally{
   	      //finally block used to close resources
   	      try{
   	         if(stmt!=null)
   	            stmt.close();
   	      }catch(SQLException se2){
   	      }// nothing we can do
   	      try{
   	         if(conn!=null)
   	            conn.close();
   	      }catch(SQLException se){
   	         se.printStackTrace();
   	      }//end finally try
   	   }//end try
		
	}
	
	private void setSelected(int no){
		selectedItem=no;
	}
	
	private int getSelected(){
		return selectedItem;
	}
	
	private void platesList(){

		
	    platesList = new ArrayList<String>();

	    ArrayList<Integer> posotitaList = new ArrayList<Integer>();
	    ArrayList<Integer> kommatiaList = new ArrayList<Integer>();
	    ArrayList<String> messageList = new ArrayList<String>();
	    ArrayList<Integer> xronosList = new ArrayList<Integer>();
	    ArrayList<String> odigiesList = new ArrayList<String>();

		//Create and set up the window.
		platesFrame = new JFrame();
		platesFrame.setSize(1000,300);
		platesFrame.setLocationRelativeTo(null);
		platesFrame.setTitle("Italian Restaurant - Cook");
		platesFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		BoxLayout box = new BoxLayout(platesFrame.getContentPane(),BoxLayout.Y_AXIS);
		platesFrame.setLayout(box);
	        
		
	   	 JLabel label = new JLabel("Select the plate to prepare");
	     label.setAlignmentX(Component.CENTER_ALIGNMENT);
	     platesFrame.getContentPane().add(label);   
		
		 Connection conn = null;
	   	 Statement stmt = null;
	   	   try{
	   	      //STEP 2: Register JDBC driver
	   	      Class.forName("com.mysql.jdbc.Driver");

	   	      //STEP 3: Open a connection
	   	      System.out.println("Connecting to database...");
	   	      conn = DriverManager.getConnection(DB_URL,USER,PASS);

	   	      //STEP 4: Execute a query
	   	      System.out.println("Creating statement...");
	   	      stmt = conn.createStatement();
	   	      String sql;
	   	      String sql2;
	   	      sql = "select onoma_piatou from PERILAMVANEI_PIATA  where id_paragelias="+idList.get(getSelected())+";";
	   	      ResultSet rs = stmt.executeQuery(sql);


	   	      //STEP 5: Extract data from result set
	   	      while(rs.next()){
	   	         //Retrieve by column name
	   	         String onoma_piatou = rs.getString("onoma_piatou");
 	         
	   	         platesList.add(onoma_piatou);

	   	      }

	   	      //setup platesListDone if we are in the first time (all plates unready)
	   	      if (time == 0 ) {
		   	      for (int i=0; i<platesList.size(); i++)
		   	    	  platesDone.add(false);
		   	      time=1;
	   	      }
	 
	   	      
	   	      
	   	//   platesFrame.add(new JScrollPane(platesJList));

	   	      //STEP 6: Clean-up environment
	   	      rs.close();
	   	      stmt.close();
	   	      conn.close();
	   	   }catch(SQLException se){
	   	      //Handle errors for JDBC
	   	      se.printStackTrace();
	   	   }catch(Exception e){
	   	      //Handle errors for Class.forName
	   	      e.printStackTrace();
	   	   }finally{
	   	      //finally block used to close resources
	   	      try{
	   	         if(stmt!=null)
	   	            stmt.close();
	   	      }catch(SQLException se2){
	   	      }// nothing we can do
	   	      try{
	   	         if(conn!=null)
	   	            conn.close();
	   	      }catch(SQLException se){
	   	         se.printStackTrace();
	   	      }//end finally try
	   	   }//end try
		
	   	 int doneCounter=0;
	   	 for (int i=0; i<platesList.size(); i++)
	   		 if (platesDone.get(i) == true ) {
	   			 platesList.set(i, platesList.get(i)+" READY");
	   			 doneCounter++;
	   		 }
	   	 
	     JButton nextButton = new JButton ("Next");
	       nextButton.setAlignmentX(Component.CENTER_ALIGNMENT);
	       nextButton.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent e) {
	                platesFrame.setVisible(false); 
	                platesFrame.dispose(); 
	                cookInfos(getSelectedPlate());
				}
	        	
	        }); 
	   	 
	     //if all the plates are ready
	   	 if (doneCounter == platesDone.size() ) {
	   		 label.setText("Order is ready. Waiter is now informed about that after you press the button below!");
	   		 nextButton.setVisible(false);
	   	 	 JButton backButton = new JButton ("Back to orders & Inform Waiter");
	   	 	 platesFrame.add(backButton);
	   		 backButton.setAlignmentX(Component.CENTER_ALIGNMENT);
	   		 backButton.addActionListener(new ActionListener() {
	   	     
					@Override
					public void actionPerformed(ActionEvent e) {
		                platesFrame.setVisible(false); 
		                platesFrame.dispose(); 
		                
		                //###delete the order from database and notify the waiter
		             Connection conn = null;
		       	   	 Statement stmt = null;
		       	   	   try{
		       	   	    //STEP 2: Register JDBC driver
		       	        Class.forName("com.mysql.jdbc.Driver");

		       	        //STEP 3: Open a connection
		       	        conn = DriverManager.getConnection(DB_URL, USER, PASS);
		       	        

		       	   	      //STEP 4: Execute a query
		       	   	      stmt = conn.createStatement();
		       	   	      String sql;
		       	   	      String sql2;
		       	   	      sql="insert into ENIMEROSH_SERVITORWN values("+idList.get(getSelected())+");";
		       	   	     // System.out.println(sql);
		       	   	      stmt.executeUpdate(sql);
		       	   	     
		       	   	}catch(SQLException se){
		       	      //Handle errors for JDBC
		       	      se.printStackTrace();
		       	   }catch(Exception ee){
		       	      //Handle errors for Class.forName
		       	      ee.printStackTrace();
		       	   }finally{
		       	      //finally block used to close resources
		       	      try{
		       	         if(stmt!=null)
		       	            conn.close();
		       	      }catch(SQLException se){
		       	      }// do nothing
		       	      try{
		       	         if(conn!=null)
		       	            conn.close();
		       	      }catch(SQLException se){
		       	         se.printStackTrace();
		       	      }//end finally try
		       	   }//end try

		       	   	   
		       	   	  
		            //###inform the waiter
		       	   	 Connection conn2 = null;
		       	   	 Statement stmt2 = null;
		       	   	   try{
		       	   	    //STEP 2: Register JDBC driver
		       	        Class.forName("com.mysql.jdbc.Driver");

		       	        //STEP 3: Open a connection
		       	        conn2 = DriverManager.getConnection(DB_URL, USER, PASS);
		       	        

		       	   	      //STEP 4: Execute a query
		       	   	      stmt2 = conn2.createStatement();
		       	   	      String sql;
		       	   	      String sql2;
		       	   	      sql="delete from PARAGELIA where id="+idList.get(getSelected())+";";
			       	      stmt2.executeUpdate(sql);
		       	   	   //   stmt.executeUpdate(sql);

		       	   	      //#notify the waiter
		       	   	      //#delete from database
		       	   	     
		       	   	}catch(SQLException se){
		       	      //Handle errors for JDBC
		       	      se.printStackTrace();
		       	   }catch(Exception ee){
		       	      //Handle errors for Class.forName
		       	      ee.printStackTrace();
		       	   }finally{
		       	      //finally block used to close resources
		       	      try{
		       	         if(stmt2!=null)
		       	            conn2.close();
		       	      }catch(SQLException se){
		       	      }// do nothing
		       	      try{
		       	         if(conn2!=null)
		       	            conn2.close();
		       	      }catch(SQLException se){
		       	         se.printStackTrace();
		       	      }//end finally try
		       	   }//end try

		       	   	 //show the remaining orders using another instance on mageiras
		             Mageiras m1 = new Mageiras();   
					}
		        	
		        }); 
	   		 
	   	 }

	  	 platesJList=new JList(platesList.toArray());
	   	 platesFrame.add(new JScrollPane(platesJList));
	   	 platesJList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
	   	    
	   	 platesJList.addListSelectionListener( 
  	    		  new ListSelectionListener(){
  	    			  public void valueChanged(ListSelectionEvent event) {
  	    				  setSelectedPlate(platesJList.getSelectedIndex());	    				  
  	    			  }
  	    		  }
  	    		  );
	   	   	   
	   	
	  
	        
	        
	       platesFrame.getContentPane().add(nextButton); 	   
		
	       platesFrame.setVisible(true);

		
     }
	
	private void cookInfos(int selected){
		
	    ArrayList<String> resultsList = new ArrayList<String>();

		//Create and set up the window. 
        frame = new JFrame();
        frame.setSize(600,500);
        frame.setLocationRelativeTo(null);
        frame.setTitle("Italian Restaurant - Cook");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        //layout setup
        BoxLayout box = new BoxLayout(frame.getContentPane(),BoxLayout.Y_AXIS);
        frame.setLayout(box);
        
        
        //label setup
        JLabel label = new JLabel("Informations about this plate");
        label.setAlignmentX(Component.CENTER_ALIGNMENT);
        frame.getContentPane().add(label);
        
        //TextArea setup
        JTextArea textArea = new JTextArea(600,500);
        textArea.setEditable(false);
        

	      
        
		 Connection conn = null;
	   	 Statement stmt = null;
	   	   try{
	   	      //STEP 2: Register JDBC driver
	   	      Class.forName("com.mysql.jdbc.Driver");

	   	      //STEP 3: Open a connection
	   	      System.out.println("Connecting to database...");
	   	      conn = DriverManager.getConnection(DB_URL,USER,PASS);

	   	      //STEP 4: Execute a query
	   	      System.out.println("Creating statement...");
	   	      stmt = conn.createStatement();
	   	      String sql;
	   	      String sql2;
	   	      sql = "select sistatiko from SISTATIKA_PIATWN where onoma_piatou=\""+ platesList.get(getSelectedPlate()) + "\" union select odigies from PIATA where onoma=\"" + platesList.get(getSelectedPlate()) + "\" union select xronos from PIATA where onoma=\""+ platesList.get(getSelectedPlate()) +"\";";

	   	      ResultSet rs = stmt.executeQuery(sql);


	   	      //STEP 5: Extract data from result set
	   	      while(rs.next()){
	   	         //Retrieve by column name
	   	    	  String result = rs.getString("sistatiko");
	   	          resultsList.add(result);

	   	      }
	   	      //η resultList περιλαμβάνει τα συστατικά στην αρχή και στις 2 τελευταίες θέσεις τις οδηγίες και το χρόνο.
	   	      
	   	    String tempMessage = "Instructions: "+ resultsList.get(resultsList.size()-2);
	   	    tempMessage += "\n";
	   	    tempMessage += "Time: " + resultsList.get(resultsList.size()-1);
	   	    tempMessage += "\n";
	   	    tempMessage += "Ingredients: ";
	   	    for (int i=0; i<resultsList.size()-2; i++) 
	   	    	tempMessage += resultsList.get(i) + ", ";
	   	    
	   	    
	   	    textArea.setText(tempMessage);
	        frame.add(textArea);
	   	      	
	     	//   platesFrame.add(new JScrollPane(platesJList));

	   	      //STEP 6: Clean-up environment
	   	      rs.close();
	   	      stmt.close();
	   	      conn.close();
	   	   }catch(SQLException se){
	   	      //Handle errors for JDBC
	   	      se.printStackTrace();
	   	   }catch(Exception e){
	   	      //Handle errors for Class.forName
	   	      e.printStackTrace();
	   	   }finally{
	   	      //finally block used to close resources
	   	      try{
	   	         if(stmt!=null)
	   	            stmt.close();
	   	      }catch(SQLException se2){
	   	      }// nothing we can do
	   	      try{
	   	         if(conn!=null)
	   	            conn.close();
	   	      }catch(SQLException se){
	   	         se.printStackTrace();
	   	      }//end finally try
	   	   }//end try

        JButton doneButton = new JButton ("Done");
        doneButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        doneButton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
                platesDone.set(getSelectedPlate(), true);
                frame.setVisible(false); 
                frame.dispose(); 
                platesList();
                
			}
        	
        }); 
		frame.add(doneButton);
        frame.setVisible(true);
		//sto DONE Button -> platesDone.set(platesJList.getSelectedIndex(), true);
	}
	
	private void setSelectedPlate (int no) {
		selectedPlate=no;
	}
	private int getSelectedPlate () {
		return selectedPlate;
	}
	

}
