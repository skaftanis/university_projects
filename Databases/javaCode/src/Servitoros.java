import java.awt.Component;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;


public class Servitoros {

	static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
	static String DB_URL = "jdbc:mysql://localhost/Restaurant";
	static String USER = "root";
	static String PASS = "171517";
	

	private JFrame frame;
	private JFrame platesListFrame;
	private JFrame drinksiListFrame;
	
	private int selectedPlates[]; 
	private int selectedDrinks[];
	JLabel platesLabel;
	JLabel drinksLabel;
	
	private int people;
	private int table;
	JComboBox combo;
	JComboBox combo2;
	
	ArrayList<String> allThePlates;
	ArrayList<String> allTheDrinks;
	int latestOrderId; //The next order must have id = latestOrderId+1
	
	JTextField infoFromCook; //ready orders from cook
	ArrayList<String> readyID = new ArrayList<String>(); //ready orders (id)
	
	JButton done;

	public Servitoros(){
		
		if ( Main.settingsChanged == true) {
			DB_URL= Main.dbNameField.getText();
			USER = 	Main.usernameField.getText();
			PASS =  Main.passwordField.getText();
		}
		
		calculatelatestOrderId();
		latestOrderId++; //now is the new
		
	    //Create and set up the window.
        frame = new JFrame();
        frame.setSize(360,450);
        frame.setLocationRelativeTo(null);
        frame.setTitle("Italian Restaurant - Waiter's App");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        
        String[] items = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
        String[] items2 = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

        
        combo = new JComboBox(items); 
        combo2 = new JComboBox(items2);
        
        JTextField field = new JTextField("");
        
        infoFromCook = new JTextField(""); //interaction with cook 
        infoFromCook.setEditable(false);
        updateInfoFromCook();
        if ( readyID.isEmpty() )
        	infoFromCook.setText("There is no ready order from cook");
        else {
        	String temp="Orders to receive: ";
        	for (int i=0; i<readyID.size(); i++){
        		temp+=readyID.get(i);
        		temp+=",";
        	}
        	infoFromCook.setText(temp);
        }
        
        JButton platesButton = new JButton("Show Plates");
	    platesButton.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent e) {
					
					allThePlates = new ArrayList<String>(); 
					
					platesListFrame = new JFrame();
					platesListFrame.setSize(400,400);
					platesListFrame.setLocationRelativeTo(null);
					platesListFrame.setTitle("Italian Restaurant - Waiter's App");
					platesListFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	                
			        BoxLayout box = new BoxLayout(platesListFrame.getContentPane(),BoxLayout.Y_AXIS);
			        platesListFrame.setLayout(box);
			        

			        JLabel label = new JLabel("Please select the plates (use Ctrl for many)");
			        label.setAlignmentX(Component.CENTER_ALIGNMENT);
			        platesListFrame.add(label);
			        
			        //GET all the plates from database----------------
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
				   	      sql = "select onoma from PIATA;";
				   	      ResultSet rs = stmt.executeQuery(sql);


				   	      //STEP 5: Extract data from result set
				   	      while(rs.next()){
				   	         //Retrieve by column name
				   	         String onoma_piatou = rs.getString("onoma");
			 	         
				   	         allThePlates.add(onoma_piatou);

				   	      }
				   	      //-----------------------------
				   	      			
				   	      
				   	      //STEP 6: Clean-up environment
				   	      rs.close();
				   	      stmt.close();
				   	      conn.close();
				   	   }catch(SQLException se){
				   	      //Handle errors for JDBC
				   	      se.printStackTrace();
				   	   }catch(Exception e3){
				   	      //Handle errors for Class.forName
				   	      e3.printStackTrace();
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
			        
			        
			        
			        //list with plates creation and setup
			        final JList list = new JList(allThePlates.toArray()); 
			        platesListFrame.add(new JScrollPane(list)); 
				   	frame.dispose();
				   	frame.setVisible(false);
				    list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
				    
				   	list.addListSelectionListener( 
			  	    		  new ListSelectionListener(){
			  	    			  public void valueChanged(ListSelectionEvent event) {			  	    				
			  	    				  selectedPlates = list.getSelectedIndices();
			  	    			  }
			  	    		  }
			  	    		  );
				   	
				    JButton backButton = new JButton("OK");
				    backButton.setAlignmentX(Component.CENTER_ALIGNMENT);
				    
				    backButton.addActionListener(new ActionListener() {

						@Override
						public void actionPerformed(ActionEvent e) {
	  	    				  frame.setVisible(true);
	  	    				  platesListFrame.dispose();
	  	    				  platesListFrame.setVisible(false);
	  	    				  platesLabel.setText("Plates: (Setted)");
						}
			        	
			        }); 
				    
				    platesListFrame.add(backButton);

					
				   	platesListFrame.setVisible(true);
	                
				}
	        	
	        }); 
        
        //SHOW DRINKS STAFF
        JButton drinks = new JButton("Show Drinks");
        drinks.addActionListener(new ActionListener() {

    				@Override
    				public void actionPerformed(ActionEvent e) {
    					allTheDrinks = new ArrayList<String>(); 
    					
    					drinksiListFrame = new JFrame();
    					drinksiListFrame.setSize(400,400);
    					drinksiListFrame.setLocationRelativeTo(null);
    					drinksiListFrame.setTitle("Italian Restaurant - Waiter's App");
    					drinksiListFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    	                
    			        BoxLayout box = new BoxLayout(drinksiListFrame.getContentPane(),BoxLayout.Y_AXIS);
    			        drinksiListFrame.setLayout(box);
    			        

    			        JLabel label = new JLabel("Please select the drinks (use Ctrl for many)");
    			        label.setAlignmentX(Component.CENTER_ALIGNMENT);
    			        drinksiListFrame.add(label);
    			        
    			        //GET all the plates from database----------------
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
    				   	      sql = "select onoma from POTA;";
    				   	      ResultSet rs = stmt.executeQuery(sql);


    				   	      //STEP 5: Extract data from result set
    				   	      while(rs.next()){
    				   	         //Retrieve by column name
    				   	         String onoma_piatou = rs.getString("onoma");
    			 	         
    				   	          allTheDrinks.add(onoma_piatou);
 
    				   	      }
    				   	      //-----------------------------
    				   	      			   	      
  				   	      
    				   	      //STEP 6: Clean-up environment
    				   	      rs.close();
    				   	      stmt.close();
    				   	      conn.close();
    				   	   }catch(SQLException se){
    				   	      //Handle errors for JDBC
    				   	      se.printStackTrace();
    				   	   }catch(Exception e3){
    				   	      //Handle errors for Class.forName
    				   	      e3.printStackTrace();
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
    			        
    			        
    			        
    			        //list with plates creation and setup
    			        final JList list = new JList(allTheDrinks.toArray()); 
    			        drinksiListFrame.add(new JScrollPane(list)); 
    				   	frame.dispose();
    				   	frame.setVisible(false);
    				    list.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION);
    				    
    				   	list.addListSelectionListener( 
    			  	    		  new ListSelectionListener(){
    			  	    			  public void valueChanged(ListSelectionEvent event) {			  	    				
    			  	    				 selectedDrinks = list.getSelectedIndices();
    			  	    			  }
    			  	    		  }
    			  	    		  );
    				   	
    				    JButton backButton = new JButton("OK");
    				    backButton.setAlignmentX(Component.CENTER_ALIGNMENT);
    				    
    				    backButton.addActionListener(new ActionListener() {

    						@Override
    						public void actionPerformed(ActionEvent e) {
    	  	    				  frame.setVisible(true);
    	  	    				  drinksiListFrame.dispose();
    	  	    				  drinksiListFrame.setVisible(false);
    	  	    				  drinksLabel.setText("Drinks: (Setted)");
    						}
    			        	
    			        }); 
    				    
    				    drinksiListFrame.add(backButton);

    					
    				    drinksiListFrame.setVisible(true);
    				}	
    	        	
    	        });
        
        
        done = new JButton("Send Order");
        done.addActionListener(new ActionListener() {
        			
    				@Override
    				public void actionPerformed(ActionEvent e) {
    					
    					done.setText("Send Order (DONE)");
    					
    					//Get the values from combos 
    			        people = Integer.parseInt( String.valueOf( combo.getSelectedItem() ) ) ; 
    			        table = Integer.parseInt( String.valueOf( combo2.getSelectedItem() ) ) ; 

    			      //  System.out.println(latestOrderId);
    			          			        
    					//Update table PARAGELIA
    					//Update table PERILAMVANEI_PIATA
    					//Update table PERILAMVNEI_POTA
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
    		       	   	      String sql2; //#TODO
    		      	   	      sql="insert into PARAGELIA values ("+latestOrderId +","+table+", CURDATE(),"+people+", CURTIME() )";
    		       	   	     // System.out.println(sql);
    		       	   	      stmt.executeUpdate(sql);
    		       	   	      
    		       	   	      for (int i=0; i<selectedPlates.length; i++) {
		    		      	   	  sql="insert into PERILAMVANEI_PIATA values(NULL," +latestOrderId + ",\""+ allThePlates.get(selectedPlates[i]) +"\",1,\"1\");";
				       	   	      //System.out.println(sql);
				       	   	      stmt.executeUpdate(sql);
    		       	   	     }
    		       	   	      
	    		       	   	  for (int i=0; i<selectedDrinks.length; i++) {
		    		      	   	  sql="insert into PERILAMVANEI_POTA values("+latestOrderId+ " ,\""+ allTheDrinks.get(selectedDrinks[i]) + "\",1);";
				       	   	     // System.out.println(sql);
				       	   	      stmt.executeUpdate(sql);
			       	   	     }
    		       	   	      

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
    				}
    	        	
    	        });
        
        GridLayout grid = new GridLayout(6,2);
        frame.setLayout(grid);
        
        frame.add(new JLabel("Number Of Table:"));
        frame.add(combo2);
        
        frame.add(new JLabel("Number Of People"));
        frame.add(combo);
        platesLabel = new JLabel("Plates");
        frame.add(platesLabel);
        frame.add(platesButton);
        drinksLabel = new JLabel("Drinks");
        frame.add(drinksLabel);
        frame.add(drinks); 
        frame.add(new JLabel("Details"));
        frame.add(field);
        frame.add(done);
        frame.add(infoFromCook);
        
        frame.setVisible(true);

        
	}
	
	
	private void calculatelatestOrderId() {
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
	   	      sql = "select MAX(id) from PARAGELIA;";
	   	      ResultSet rs = stmt.executeQuery(sql);


	   	      //STEP 5: Extract data from result set
	   	      while(rs.next()){
	   	         //Retrieve by column name
	   	         String latest = rs.getString("MAX(id)");
	         
	   	         latestOrderId=Integer.parseInt(latest);
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
		
		
	}
	
	
	private void updateInfoFromCook() {
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
	   	      sql = "select * from ENIMEROSH_SERVITORWN;";
	   	      ResultSet rs = stmt.executeQuery(sql);


	   	      //STEP 5: Extract data from result set
	   	      while(rs.next()){
	   	         //Retrieve by column name
	   	         String latest = rs.getString("id_paragelias");
	   	         readyID.add(latest);
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
		
	}
	

}
