import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

import java.sql.*;


public class Main implements ActionListener {
	
	  static final String JDBC_DRIVER = "com.mysql.jdbc.Driver";  
	  static String DB_URL = "jdbc:mysql://localhost/Restaurant";
	  static String USER = "root";
	  static String PASS = "171517";
	 
	  //for settings menu
	  static public JFrame settingsFrame; 
	  static public JTextField dbNameField;
	  static public JTextField usernameField;
	  static public JPasswordField passwordField;
	  
	  public static boolean settingsChanged=false;;
	
	private static  void createAndShowGUI() {
        //Create and set up the window.
        final JFrame frame = new JFrame("HelloWorldSwing");
        frame.setSize(350,110);
        frame.setLocationRelativeTo(null);
        frame.setTitle("Italian Restaurant");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        //layout setup
        FlowLayout box = new FlowLayout();
        frame.setLayout(box);
        
        //label setup
        JLabel label = new JLabel("Welcome to our application."+"\n"+"" +
        		"Select your job");
        
        label.setAlignmentX(Component.CENTER_ALIGNMENT);
        
        //buttons & listeners setup
        JButton mageiras = new JButton ("Cook");
        mageiras.addActionListener(new ActionListener() {

			@SuppressWarnings("deprecation")
			@Override
			public void actionPerformed(ActionEvent e) {
                frame.setVisible(false); 
                frame.dispose(); 
                Mageiras m1 = new Mageiras();              
			}
        	
        }); 
        JButton servitoros = new JButton ("Waiter");
        servitoros.addActionListener(new ActionListener() {

     			@SuppressWarnings("deprecation")
     			@Override
     			public void actionPerformed(ActionEvent e) {
                     frame.setVisible(false); 
                     frame.dispose(); 
                     Servitoros s1 = new Servitoros();              
     			}
             	
             }); 
        JButton tamias = new JButton ("Cashier");
        tamias.addActionListener(new ActionListener() {

 			@SuppressWarnings("deprecation")
 			@Override
 			public void actionPerformed(ActionEvent e) {
                 frame.setVisible(false); 
                 frame.dispose(); 
                 Tamias t1 = new Tamias();              
 			}
         	
         }); 
        
        JButton settings = new JButton ("Settings");
        settings.addActionListener(new ActionListener() {

 			@SuppressWarnings("deprecation")
 			@Override
 			public void actionPerformed(ActionEvent e) {
                 frame.setVisible(false); 
                 frame.dispose(); 
                 settingsFrame = new JFrame();
                 settingsFrame.setSize(600,100);
                 settingsFrame.setLocationRelativeTo(null);
                 settingsFrame.setTitle("Italian Restaurant - DB Settings");
                 settingsFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                 
                 GridLayout grid = new GridLayout(4,2);
                 settingsFrame.setLayout(grid);
                 
                 settingsFrame.add(new JLabel("Give: DB_URL"));
                 
                 dbNameField = new JTextField("jdbc:mysql://localhost/Restaurant");
                 settingsFrame.add(dbNameField);
                 
                 settingsFrame.add(new JLabel("DB username: "));
                 usernameField = new JTextField("root");
                 settingsFrame.add(usernameField);
                 
                 settingsFrame.add(new JLabel("DB password: "));
                 passwordField = new JPasswordField("171517");
                 settingsFrame.add(passwordField);
                 
                 JButton back = new JButton("OK");
                 back.addActionListener(new ActionListener() {

          			@SuppressWarnings("deprecation")
          			@Override
          			public void actionPerformed(ActionEvent e) {
          				  settingsFrame.setVisible(false); 
          			      settingsFrame.dispose(); 
          			      
          			      DB_URL = dbNameField.getText();
          			      USER = usernameField.getText();
          			      PASS = passwordField.getText();
          			      
          			      settingsChanged=true;

          			      
          			      createAndShowGUI();              
          			}
                  	
                  }); 
                 
                 settingsFrame.add(back);
                 
                 settingsFrame.setVisible(true);
 			}
         	
         }); 

        
        //add elements to frame
        frame.getContentPane().add(label);
        frame.getContentPane().add(mageiras);
        frame.getContentPane().add(servitoros);
        frame.getContentPane().add(tamias);
        frame.getContentPane().add(settings);
        
        //Display the window.
        frame.setVisible(true);
    }
 
    public static void main(String[] args) {
    	   
    	   createAndShowGUI();
    }

	@Override
	public void actionPerformed(ActionEvent arg0) {
		// TODO Auto-generated method stub
		
	}

}
