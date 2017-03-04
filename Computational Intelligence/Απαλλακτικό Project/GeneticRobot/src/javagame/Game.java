package javagame;

import java.util.Random;

import org.newdawn.slick.*;
import org.newdawn.slick.state.*;

import javagame.Tree.Node;

public class Game extends StateBasedGame{

	public static final String simName="Genetic Robot";
	public static final int menu = 0;
	public static final int sim = 1;
	

	public Game(String name) {
		super(simName);
		this.addState(new Menu(menu));
		this.addState(new Sim(sim));
	}
	
	@Override
	public void initStatesList(GameContainer gc) throws SlickException {
		this.getState(menu).init(gc, this);
		this.getState(sim).init(gc, this);
		//screen που ανοίγει στην αρχή (μενού) 
		this.enterState(menu);

	}
	

	
	public static void main(String[] args) {
		
			
		AppGameContainer appgc = null;
		try { 
			appgc = new AppGameContainer(new Game(simName));
			appgc.setDisplayMode(840, 360, false);
			appgc.start();
		} catch(SlickException e) {
			e.printStackTrace();
		}


		
		
		
	}



}
