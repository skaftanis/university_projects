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
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;



public class Tamias {
	
	static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
	static String DB_URL = "jdbc:mysql://localhost/Restaurant";
	static String USER = "root";
	static String PASS = "171517";
	
	
	private JFrame frame;
	private JComboBox combo;
	private JFrame apodeixi; 
	
	//staff from db
	private String date;
	private String time; 
	private String id;
	private ArrayList<String> onomaPiatwn = new ArrayList<String>();
	private ArrayList<String> onomaPotwn = new ArrayList<String>();
    private String curDate;
    private String curTime;
    private ArrayList<String> timesPiatwn = new ArrayList<String>();
    private ArrayList<String> timesPotwn = new ArrayList<String>();
    
    JComboBox cardField;
    
    ArrayList<String> tablesWithPeople = new ArrayList<String>();
    ArrayList<String> peopleWithCard = new ArrayList<String>();
    ArrayList<String> peoplePoints = new ArrayList<String>();
    ArrayList<String> parageliesMina = new ArrayList<String>();

    


	
	public Tamias() {
		
		
		if ( Main.settingsChanged == true) {
			DB_URL= Main.dbNameField.getText();
			USER = 	Main.usernameField.getText();
			PASS =  Main.passwordField.getText();
		}
		
	    frame = new JFrame();
        frame.setSize(310,100);
        frame.setLocationRelativeTo(null);
        frame.setTitle("Italian Restaurant - Cashier");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        
        //mysql connection to read tables with customers to appear in the list----
        Connection conn = null;
        Statement stmt = null;
      	   try{
      	      //STEP 2: Register JDBC driver
      	      Class.forName("com.mysql.jdbc.Driver");

      	      //STEP 3: Open a connection
      	      conn = DriverManager.getConnection(DB_URL,USER,PASS);

      	      //STEP 4: Execute a query
      	      System.out.println("Creating statement...");
      	      stmt = conn.createStatement();
      	      String sql;
      	      sql = "select trapezi from PARAGELIA;";
      	      ResultSet rs = stmt.executeQuery(sql);

      	      //STEP 5: Extract data from result set
      	      while(rs.next()){
      	    	tablesWithPeople.add(rs.getString("trapezi"));	
      	      }
      	      
      	      sql = "select onoma,pontoi,paragelies_mina from PELATIS;";
    	      rs = stmt.executeQuery(sql);

    	      //STEP 5: Extract data from result set
    	      peopleWithCard.add("No card");
    	      while(rs.next()){
    	    	  peopleWithCard.add(rs.getString("onoma"));
    	    	  peoplePoints.add(rs.getString("pontoi"));
    	    	  parageliesMina.add(rs.getString("paragelies_mina"));
    	      }
    	      
      	            	           	      
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
        //------------------------
        
      	 //only tables with people can appear here
      	 String[] items = new String[tablesWithPeople.size()];
      	 items = tablesWithPeople.toArray(items);

        
        JButton next = new JButton("Next");
        next.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				
				frame.setVisible(false);
				frame.dispose();
				
				apodeixi = new JFrame();
				apodeixi.setSize(400,400);
				apodeixi.setLocationRelativeTo(null);
				apodeixi.setTitle("Italian Restaurant - Cashier");
				apodeixi.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				
		        BoxLayout box = new BoxLayout(apodeixi.getContentPane(),BoxLayout.Y_AXIS);
		        apodeixi.setLayout(box);
		        				
			    JTextArea textArea = new JTextArea(600,500);
			    textArea.setEditable(false);
				
				//##SQL queries to execute
				//select id, imerominia, wra from PARAGELIA where trapezi=X;
				//select onoma_piatou from PERILAMVANEI_PIATA where id_paragelias=X;
				//select onoma_potou from PERILAMVANEI_POTA where id_paragelias=X;
				//select CURTIME();
				//select CURDATE();
				//select timi from PIATA where onoma="X";
				//select timi from POTA where onoma="X";
				//delete from PARAGELIA where id=X; (ενημερώνει αυτόματα και τα υπόλοιπα) 
				
				
		        int tableId = Integer.parseInt( String.valueOf( combo.getSelectedItem() ) ) ; 

				
				//GET all the plates from database and people with card----------------
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
			   	      sql = "select id, imerominia, wra from PARAGELIA where trapezi=" + tableId + ";";
			   	      ResultSet rs = stmt.executeQuery(sql);
		   	      
			   	      //STEP 5: Extract data from result set
			   	      while(rs.next()){
			   	         //Retrieve by column name
			   	         id = rs.getString("id");
			   	         date = rs.getString("imerominia");
			   	         time = rs.getString("wra");
		 	         
			   	         //System.out.println(id+","+time+","+date);

			   	      }
			   	      
			   	     sql = "select onoma_piatou from PERILAMVANEI_PIATA where id_paragelias="+id+";";
			   	     rs = stmt.executeQuery(sql);
		   	      
			   	      //STEP 5: Extract data from result set
			   	      while(rs.next()){
			   	         //Retrieve by column name
	 			   	      onomaPiatwn.add(rs.getString("onoma_piatou"));		 	         
			   	      }
			   	      
			   	      //for (int i=0; i<onomaPiatwn.size(); i++)
			   	    	 // System.out.println(onomaPiatwn.get(i));
			   	      
			   	      
			   	      
			   	    sql = "select onoma_potou from PERILAMVANEI_POTA where id_paragelias="+id+";";
			   	    rs = stmt.executeQuery(sql);
		   	      
			   	      //STEP 5: Extract data from result set
			   	      while(rs.next()){
			   	         //Retrieve by column name
	 			   	      onomaPotwn.add(rs.getString("onoma_potou"));		 	         
			   	      }
			   	      
			   	    sql = "select CURTIME();";
			   	    rs = stmt.executeQuery(sql);
		   	      
			   	      //STEP 5: Extract data from result set
			   	      while(rs.next()){
			   	         //Retrieve by column name
	 			   	      curTime=rs.getString("CURTIME()");	 	         
			   	      }
			   	      
			   	    
			   	      
			   	   sql = "select CURDATE();";
			   	   rs = stmt.executeQuery(sql);
		   	      
			   	      //STEP 5: Extract data from result set
			   	      while(rs.next()){
			   	         //Retrieve by column name
	 			   	      curDate=rs.getString("CURDATE()");	 	         
			   	      }
			   	   
			   	  //get prize for every plate
			   	   for (int i=0; i< onomaPiatwn.size(); i++) {
			   	 	   sql = "select timi from PIATA where onoma=\""+onomaPiatwn.get(i) + " \";";
				   	   rs = stmt.executeQuery(sql);
			   	      
				   	      //STEP 5: Extract data from result set
				   	      while(rs.next()){
				   	         //Retrieve by column name
		 			   	      timesPiatwn.add(rs.getString("timi"));	         
				   	      }			   		   
			   	   } //endfor
			   	   
			   	 //get prize for every drink  
			   	 for (int i=0; i< onomaPotwn.size(); i++) {
			   	 	   sql = "select timi from POTA where onoma=\""+onomaPotwn.get(i) + " \";";
				   	   rs = stmt.executeQuery(sql);
			   	      
				   	      //STEP 5: Extract data from result set
				   	      while(rs.next()){
				   	         //Retrieve by column name
		 			   	      timesPotwn.add(rs.getString("timi"));	         
				   	      }			   		   
			   	   } //endfor
			   	  
			   	    //delete the order from Database
			    	 sql="delete from PARAGELIA where id="+id+";";
			    	 stmt.executeUpdate(sql);
			   	 

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
		        
			   	JLabel recLabel = new JLabel("Receipt");
		        recLabel.setAlignmentX(Component.CENTER_ALIGNMENT);
		        
		        JButton okButton = new JButton("OK");
		        okButton.setAlignmentX(Component.CENTER_ALIGNMENT);
		        okButton.addActionListener(new ActionListener() {

		 			@SuppressWarnings("deprecation")
		 			@Override
		 			public void actionPerformed(ActionEvent e) {
		                 apodeixi.setVisible(false); 
		                 apodeixi.dispose();               
		 			}
		         	
		         });
		        
			   	apodeixi.add(recLabel);
				apodeixi.add(textArea);
				apodeixi.add(okButton);
				
				
				
				String forTextArea = "Date: " + curDate + "  Time: " + curTime + "\n";		
				if (id == null) {
					forTextArea += "Failed to laod this order. Try a different table\n\n";
				}
				else 
					forTextArea += "Id: "+ id + "\n\n";
				
				
				forTextArea += "Plates Ordered: \n";
				for (int i=0; i < onomaPiatwn.size(); i++) {
					forTextArea += onomaPiatwn.get(i)+" - " ;
					forTextArea += timesPiatwn.get(i) + "euros " +  "\n\n";
				}
				
				forTextArea += "Drinks Ordered: \n";
				for (int i=0; i < onomaPotwn.size(); i++) {
					forTextArea += onomaPotwn.get(i)+" - " ;
					forTextArea += timesPotwn.get(i) + "euros" +  "\n";
				}
				
				int sum=0;
				for (int i=0; i<timesPiatwn.size();i++) {
					sum += Integer.parseInt(timesPiatwn.get(i));
				}
				for (int i=0; i<timesPotwn.size(); i++) {
					sum += Integer.parseInt(timesPotwn.get(i));
				}
				
				forTextArea+="\n";
				forTextArea += "Sum: " + sum +" euros\n\n";
				
			 	 //if there is smth writen in the card's field
			   	 if ( ! cardField.getSelectedItem().equals("No card") ) {
			   		 peopleWithCard.remove(0); //remove the "No card"
			   		 for (int i=0; i<peopleWithCard.size(); i++)
			   			 if (cardField.getSelectedItem().equals(peopleWithCard.get(i)) ) {
			   				 forTextArea += "Points: " + peoplePoints.get(i) + "\n";
			   				 forTextArea += "Orders this month: " + parageliesMina.get(i) + "\n";
			   				 if ( Integer.parseInt(peoplePoints.get(i)) > 200 ) {
			   					 forTextArea += "You won 20 euros discount for your next order! \n";
			   				 }
			   				 if ( Integer.parseInt(parageliesMina.get(i)) > 10 ) {
			   					 forTextArea += "You won a 10 euros coupon for a next order! \n ";
			   				 }
			   			 }
			   		 
			   		 			   		 
			   	 } 
		   	   
			   	textArea.setText(forTextArea);   
			   	   		
				apodeixi.setVisible(true);
			}
        	
        });
              
        GridLayout grid = new GridLayout(3,2);
        frame.setLayout(grid);
        
        cardField = new JComboBox(peopleWithCard.toArray());
        combo = new JComboBox(items); 
        
        frame.add(new JLabel("No Of Table"));
        frame.add(combo);
        
        frame.add(new JLabel("People With Card"));
        frame.add(cardField);
        
        frame.add(next);
        

        frame.setVisible(true);
	
	}

}
