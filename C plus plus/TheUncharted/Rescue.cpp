#include "Rescue.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include "Map.h"

#include <iostream>
Rescue::Rescue()
{
    Load("images/resque.png");
    GenerateStartedPosition();
    prosimo=1;
    SetPosition(currentPositionX,currentPositionY);
    if (!texture.loadFromFile("images/flag.png") )
        return;
    GenerateTarget();
    Type="Rescue";
}



void Rescue::Update() {
  steps++;
  if (currentPositionY >= targetY && currentPositionX < targetX) {
        if ( currentPositionY == targetY ) {
            currentPositionX += 32*speed;
            GetSprite().move(32*speed,0);
            poreia=3;
        } else {
        currentPositionY -= 32*speed;
        GetSprite().move(0,-32*speed);
        poreia=0;
        }
    }

    else if (currentPositionY >= targetY && currentPositionX > targetX) {
        if ( currentPositionY == targetY ){
            currentPositionX -= 32*speed;
            GetSprite().move(-32*speed,0);
            poreia=2;
        } else {
        currentPositionY -= 32*speed;
        GetSprite().move(0,-32*speed);
        poreia=0;
        }
    }

   else  if (currentPositionY <= targetY && currentPositionX < targetX ) {
        if (currentPositionY == targetY ) {
            currentPositionX += 32*speed;
            GetSprite().move(32*speed,0);
            poreia=3;
        }
        currentPositionY += 32 * speed;
        GetSprite().move(0,32*speed);
        poreia=1;
    }


   else  if (currentPositionY <= targetY && currentPositionX > targetX) {
        if (currentPositionY == targetY) {
            currentPositionX -= 32*speed;
            GetSprite().move(-32*speed, 0);
            poreia=2;
        }
        currentPositionY += 32 * speed;
        GetSprite().move(0, 32*speed );
        poreia=1;

    }

    else if (currentPositionX==targetX && currentPositionY > targetY) {
        currentPositionY -= 32*speed;
        GetSprite().move(0,-32*speed);
        poreia=0;

    }

    else if (currentPositionX==targetX && currentPositionY < targetY) {
        currentPositionY += 32*speed;
        GetSprite().move(0,32*speed);
        poreia=1;

    }

    if (targetX == currentPositionX && targetY == currentPositionY) {
        checkAndRepair();
        GenerateTarget();
        steps--;
        }

    checkAndRepair();


}

void Rescue::Draw(sf::RenderWindow& window) {
    VisibleGameObject::Draw(window);
}

void Rescue:: GenerateStartedPosition () {
    currentPositionX=32*15;
    currentPositionY=32*11;

}



void Rescue::checkAndRepair(){
    if ( Simulate::map.NeedHelp[currentPositionX/32][currentPositionY/32] == 1  ) {
       saves++;
       Simulate::map.NeedHelp[currentPositionX/32][currentPositionY/32] = 0;
       VisibleGameObject *robot = new VisibleGameObject();
       for (int i=0; i<Simulate::RobotsAdded; i++) {
            if ( Simulate::names[i] != "NULL")
                robot=Simulate::gameobjectmanager.Get(Simulate::names[i]);
          //  std::cout << "X= " << robot->getPositionX() << " Y= " << robot->getPositionY() << std::endl;
            if (robot->getPositionX() == targetX && robot->getPositionY() == targetY )
                    robot->setDamagedFalse();
            }
        }
}

void Rescue::GenerateTarget() {  //target is where there is a broken robot
    //std::cout << "Generate Target Works" << std::endl;
    int i=0;
    int j=0;
  do {
    do{
        if ( Simulate::map.NeedHelp[i][j] == 1) {
           // std::cout << "NeedHelp = 1 " << std::endl;
            targetX=i*32;
            targetY=j*32;
            //Simulate::map.NeedHelp[i][j]=0;
            break;
            break;
        } else {
            //std::cout << "i=" << i << std::endl;
            i++;
        }
    } while ( i<24 );
    j++;
    i=0;
  } while (j<32);

}

int Rescue::getPositionX() {
    return currentPositionX;
}

int Rescue::getPositionY() {
   return currentPositionY;
}

int Rescue::GetSteps() {
    return steps;
}

int Rescue::GetDamages() { //resque can't damaged
    return 0;
}

int Rescue::GetTotalFlags() { //resque can't place flags
    return 0;
}

int Rescue::GetTotalSaves() {
    return saves;
}

