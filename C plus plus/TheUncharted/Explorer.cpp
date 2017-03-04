#include "Explorer.h"
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>

#include <fstream>

Explorer::Explorer()
{
    Load("images/explorer.png");
    GenerateStartedPosition();
    GenerateAccessAbility();
    prosimo=1;
    SetPosition(currentPositionX,currentPositionY);
    if (!texture.loadFromFile("images/flag.png") )
        return;
    //flag.setTexture(texture);
    count=-1;
 //   for (int i=0; i<24; i++)
 //       for (int j=0; j<32; j++)
  //          flagPos[i][j]=0;
    Type="Explorer";
}

Explorer::~Explorer()
{
    //dtor
}

void Explorer::Update(){

if  (isDamaged(currentPositionX,currentPositionY) == false ) {
  damageSetted=1;
  Load("images/explorer.png");
  if (isForFlag(currentPositionX,currentPositionY)) {
        totalFlags++;
        count++;
        sf::Sprite flag;
        flag.setTexture(texture);
        flags[count]=flag;
        //flagPos[currentPositionX/32][currentPositionY/32]=1;
        Simulate::map.flagPos[currentPositionX/32][currentPositionY/32]=1;
        flags[count].setPosition(currentPositionX,currentPositionY);
       // DrawFlags();

    }
    for (int i=0; i<768; i++) {
        Simulate::mainwindow.draw(flags[i]);
    }

    if (startedPosX == 0 && startedPosY == 0 ) { //->
        currentPositionX += 32*speed;
        GetSprite().move(32*speed,0);
        poreia = 3;
        if (currentPositionX == 1024) {
            currentPositionY += prosimo*32;
            startedPosX=1024-32;
            startedPosY=0;
            GetSprite().move(0,32*prosimo);
            poreia=1;
        }

    }
    if ( startedPosX == 1024-32 && startedPosY == 768-32) {   // <-
        currentPositionX -= 32*speed;
        GetSprite().move(-32*speed,0);
        poreia=2;
        if (currentPositionX == -32) {
            currentPositionY -= prosimo*32;
            startedPosX=0;
            startedPosY=768-32;
            GetSprite().move(0,-prosimo*32);
            if (prosimo == 1) poreia=0;
            if (prosimo == -1) poreia=1;
            prosimo=1;

        }

    }

    if (startedPosX == 1024-32 && startedPosY == 0) {
        currentPositionX -= 32*speed;
        GetSprite().move(-32*speed,0);
        poreia=2;
        if (currentPositionX == -32) {
            currentPositionY += 32;
            startedPosX=0;
            startedPosY=0;
            GetSprite().move(0,32);
            poreia=1;
            prosimo=1;

        }

    }

    if (startedPosX == 0 && startedPosY == 768-32) {
        currentPositionX += 32*speed;
        GetSprite().move(32*speed,0);
        poreia=3;
        if (currentPositionX == 1024) {
            currentPositionY -= 32;
            startedPosX=1024-32;
            startedPosY=768-32;
            GetSprite().move(0,-32);
            poreia=0;
            prosimo=1;

        }

    }
steps++;

} else {//damaged=true
    Load("images/explorerdamaged.png");
    Simulate::map.NeedHelp[currentPositionX/32][currentPositionY/32] = 1;
    for (int i=0; i<768; i++) {
        Simulate::mainwindow.draw(flags[i]);
    }
     if (damageSetted == 1) {
            Damages++;
            damageSetted=0;
       }

}

}

void Explorer::Draw(sf::RenderWindow& window) {
    VisibleGameObject::Draw(window);
}

int Explorer::getPositionX() {
      sf::Vector2f vec = GetSprite().getPosition();
      return vec.x;
    //return currentPositionX;
}

int Explorer::getPositionY() {
    //return currentPositionY;
     sf::Vector2f vec = GetSprite().getPosition();
    return vec.y;
}

int Explorer::GetRundomDirection() {
    srand (time(NULL));
    int x;
    do {
        x = rand() % 10;
    } while (x > 3);
    return x;
}

void Explorer:: GenerateStartedPosition () {
    srand (time(NULL));
    int x,y;
    do{ //generates numbers between 0-3 Explorer can start in 4 corers and make a snake move for ther
        x= rand() % 10;
    } while ( x > 3 );

    if ( x == 0 ) {
        currentPositionX=0;
        currentPositionY=0;
        startedPosX=0;
        startedPosY=0;
    }
    if (x == 1 ) {
        currentPositionX=1024-32;
        currentPositionY=0;
        startedPosX=currentPositionX;
        startedPosY=currentPositionY;
    }
    if (x == 2) {
        currentPositionX = 0;
        currentPositionY = 768-32;
        startedPosX=currentPositionX;
        startedPosY=currentPositionY;
    }
    if ( x == 3) {
        currentPositionX=1024-32;
        currentPositionY=768-32;
        startedPosX=currentPositionX;
        startedPosY=currentPositionY;
    }

}

bool Explorer::isForFlag(int x, int y) {
    if (Simulate::map.GetDangerLevel(x/32,y/32) > 0.6)
        return true;
    return false;

}

int Explorer::GetSteps() {
    return steps;
}

int Explorer::GetDamages() {
    return Damages;
}

int Explorer::GetTotalFlags(){
    return totalFlags;
}

int Explorer::GetTotalSaves() {
    return 0;
}

