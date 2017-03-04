#include "Analyzer.h"
#include "VisibleGameObject.h"
#include <cstdlib>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>
#include <ctime>
#include <iostream>


Analyzer::Analyzer()
{
    MaxLoad = 2000;
    Load("images/analyzer.png");
    GenerateStartedPosition();
    GenerateAccessAbility();
    SetPosition(currentPositionX, currentPositionY);
    PalladioLoad=0;
    IridioLoad=0;
    LeukoxrisosLoad=0;
    Damages=0;
    GenerateTarget();

    Type="Analyzer";

}

Analyzer::~Analyzer()
{
    //dtor
}

void Analyzer::Draw(sf::RenderWindow& window) {
    VisibleGameObject::Draw(window);
}

//float Analyzer::GetSpeed() const {
 //   return position;
//}

void Analyzer::Update () {



if  (isDamaged(currentPositionX,currentPositionY) == false ) {
    damageSetted=1;
  if ( PalladioLoad + IridioLoad + LeukoxrisosLoad == MaxLoad ) {
        times++;
        if (times == 1)
            Simulate::Analyzers2Base++;
        currentPositionX=14*32;
        currentPositionY=12*32;
        SetPosition((14+RobotCode-1)*32,12*32);
        Simulate::map.SetPalladio(12,(14+RobotCode-1), PalladioLoad);
        Simulate::map.SetIridio(12,(14+RobotCode-1), IridioLoad);
        Simulate::map.SetLeukoxrisos(12,(14+RobotCode-1), LeukoxrisosLoad);

        //Simulate::map.BaseTotal += MaxLoad;

        Load("images/analyzer.png");
    }

    else if (currentPositionY >= targetY && currentPositionX < targetX) {
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

    steps++;
// --------------------------------------


    if (targetX == currentPositionX && targetY == currentPositionY)
        GenerateTarget();


    if (isForMining(currentPositionX/32,currentPositionY/32) && Simulate::map.flagPos[currentPositionY/32][currentPositionX/32] == 0) {
         Mining(currentPositionX/32, currentPositionY/32);
         //std::cout << Simulate::map.GetLeukoxrisos(12,14);
         //std::cout << "x = " << currentPositionX <<std::endl;
         //std::cout << "y= " << currentPositionY <<std::endl;

    } else
        Load("images/analyzer.png");


   } else { //damages = true
        Load("images/analyzerdamaged.png");
        Simulate::map.NeedHelp[currentPositionX/32][currentPositionY/32] = 1;
        if (damageSetted == 1) {
            Damages++;
            damageSetted=0;
       }

   }




}

void Analyzer:: GenerateStartedPosition () {
    srand (time(NULL));
    int x,y;
    do{
        x= (rand()  ) % 100 ;
        y= (rand ()) % 100;
    } while ( (x>24-1 || y>32-1) || (x==12 && y==14) || (x==12 && y==15) || (x==12 && y==16) ||
                (x==13 && y==14) || (x==13 && y==15) || (x==13 && y==16) );

    currentPositionX= y *32;
    currentPositionY= x *32;
}

int Analyzer::getPositionX() {
    return currentPositionX;
    //sf::Vector2f vec = GetSprite().getPosition();
    //return vec.x;
}

int Analyzer::getPositionY() {
    return currentPositionY;
    //sf::Vector2f vec = GetSprite().getPosition();
    //return vec.y;
}

int Analyzer::GetRundomDirection() {
    srand (time(NULL));
    int x;
    do {
        x = rand() % 10;
    } while (x > 3);
    return x;
}





int Analyzer::getPalladioLoad() const {
    return PalladioLoad;
}

int Analyzer::getIridioLoad() const {
    return IridioLoad;
}

int Analyzer::getLeukoxrisosLoad() const {
    return LeukoxrisosLoad;
}




bool Analyzer::isForMining(int x, int y) {
    for (int i=0; i<Simulate::Analyzers2Base; i++)
        if ( x == 12 && y == 14+i) { //if is ina base
            return false;
        }
    if (Simulate::map.GetIridio(y,x) ==0 && Simulate::map.GetLeukoxrisos(y,x)==0 && Simulate::map.GetPalladio(y,x) ==0 ) {
        return false;
    }
    if ( Simulate::map.GetIridio(y,x) >=95 || Simulate::map.GetPalladio(y,x)>=95 || Simulate::map.GetLeukoxrisos(y,x)>=95) {
        return true;
    }
    if ( Simulate::map.GetIridio(y,x) >=40 && Simulate::map.GetPalladio(y,x) >=40 && Simulate::map.GetLeukoxrisos(y,x) >=40 ) {
        return true;
    }
    return false;

}

void Analyzer::Mining (int x, int y) {
    int prev_load;
    Load("images/analyzermining.png"); //load new mining picture (red inside)
    //palladio mining

    if ( PalladioLoad + IridioLoad + LeukoxrisosLoad + Simulate::map.GetPalladio(y,x) <= MaxLoad ) {
        PalladioLoad += Simulate::map.GetPalladio(y,x);
        Simulate::map.SetPalladio(y,x,0);
        //Simulate::map.SetPalladio(12,14,Simulate::map.GetPalladio(12,14)); //add to base
        end=false;
    } else {
    prev_load=PalladioLoad;
    PalladioLoad += MaxLoad - ( PalladioLoad + IridioLoad + LeukoxrisosLoad );
    Simulate::map.SetPalladio(y,x, Simulate::map.GetPalladio(y,x) - ( MaxLoad - ( PalladioLoad + IridioLoad + LeukoxrisosLoad) ) );
    //Simulate::map.SetPalladio(12,14,Simulate::map.GetPalladio(12,14)+MaxLoad-(prev_load + IridioLoad + LeukoxrisosLoad));   //add to base

    end=true;
    }


   //iridio mining
 if (end == false) {
    if ( PalladioLoad + IridioLoad + LeukoxrisosLoad + Simulate::map.GetIridio(y,x) <= MaxLoad ) {
        IridioLoad += Simulate::map.GetIridio(y,x);
        Simulate::map.SetIridio(y,x,0);
        //Simulate::map.SetIridio(12,14,Simulate::map.GetIridio(12,14)+Simulate::map.GetIridio(y,x)); //add to base
    } else {
    prev_load=IridioLoad;
    IridioLoad += MaxLoad - ( PalladioLoad + IridioLoad + LeukoxrisosLoad );
    Simulate::map.SetIridio(y,x, Simulate::map.GetIridio(y,x) - ( MaxLoad - ( PalladioLoad + IridioLoad + LeukoxrisosLoad) ) );
    //Simulate::map.SetIridio(12,14,Simulate::map.GetIridio(12,14) + MaxLoad - ( PalladioLoad + prev_load + LeukoxrisosLoad )); //add to base
    end=true;
    }
 }
    //leukoxrisos mining
  if ( end == false ) {
    if ( PalladioLoad + IridioLoad + LeukoxrisosLoad + Simulate::map.GetLeukoxrisos(y,x) <= MaxLoad ) {
        LeukoxrisosLoad += Simulate::map.GetLeukoxrisos(y,x);
        Simulate::map.SetLeukoxrisos(y,x,0);
        Simulate::map.SetLeukoxrisos(12,14,Simulate::map.GetLeukoxrisos(12,14)+Simulate::map.GetLeukoxrisos(y,x)); //add to base
    } else {
    prev_load=LeukoxrisosLoad;
    LeukoxrisosLoad += MaxLoad - ( PalladioLoad + IridioLoad + LeukoxrisosLoad );
    Simulate::map.SetLeukoxrisos(y,x, Simulate::map.GetLeukoxrisos(y,x) - ( MaxLoad - ( PalladioLoad + IridioLoad + LeukoxrisosLoad) ) );
    //Simulate::map.SetLeukoxrisos(12,14,Simulate::map.GetLeukoxrisos(12,14)+ MaxLoad - ( PalladioLoad + IridioLoad + prev_load ));
    end=true;
    }
  }
}
 int Analyzer::getMaxLoad() {
    return MaxLoad;
}

void Analyzer::setCode (std::string str) {
    code=str;
}

std::string Analyzer::getCode () {
    return code;
}



void Analyzer::GenerateTarget() {
    srand (rand());
    int x,y;
    do{
        x= (rand() ) % 100 ;
        y= (rand ()  ) % 100;
    } while ( x>24-1 || y>32-1 );

    targetX= y * 32;
    targetY= x * 32;
}

int Analyzer::GetSteps() {
    return steps;
}

int Analyzer::GetDamages() {
    return Damages;
}

int Analyzer::GetTotalFlags() {
    return 0;
}

int Analyzer::GetTotalSaves() {
    return 0;
}

