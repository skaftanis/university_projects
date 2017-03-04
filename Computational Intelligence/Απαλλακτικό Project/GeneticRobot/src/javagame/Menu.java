package javagame;

import org.lwjgl.input.Mouse;
import org.newdawn.slick.*;
import org.newdawn.slick.state.*;


public class Menu extends BasicGameState {
	
	public Menu(int state) {
		
	}

	@Override
	public void init(GameContainer arg0, StateBasedGame arg1) throws SlickException {
	}

	@Override
	public void render(GameContainer arg0, StateBasedGame arg1, Graphics g) throws SlickException {
		g.fillOval(75, 100, 100, 100);
		g.drawString("Click the cirle and wait...",80, 70);
		g.drawString("30000 Generations, 1000 members on population",80, 220);
		g.drawString("P(crossover) = 20%, P(Mutation) = 40%",80, 235);


		
		
	}

	@Override
	public void update(GameContainer gc, StateBasedGame sbg, int arg2) throws SlickException {
		Input input = gc.getInput();
		int xpos = Mouse.getX();
		int ypos = Mouse.getY();
		if ( xpos>75 && xpos<175 && ypos>160 && ypos<260) {
			if (input.isMouseButtonDown(0)) {
				sbg.enterState(1);
				
			}
		}
	}

	@Override
	public int getID() {
		return 0;
	}
	
	

}
