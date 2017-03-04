#include "Map.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <sstream>
#include "Simulate.h"
#include "Explorer.h"

using namespace std;

Map::Map () {
LoadMap("images/spritesheet.png");
SetUpMap();
for (int i=0; i<24; i++)
    for (int j=0; j<32; j++) {
        isDanger[i][j]=false;
        flagPos[i][j]=0;
        NeedHelp[i][j]=0;
    }

}


void Map::LoadMap(std::string name) {

    if (!image.loadFromFile(name) ) {
        filename="";
        isLoaded=false;
    }
    else {
        filename=name;
        sprite.setTexture(image);
        isLoaded=true;
    }

}

void Map::DrawMap(sf::RenderWindow& window) {

    if (isLoaded)
            window.draw(sprite);

}

void Map::SetUpMap() {

    srand (static_cast <unsigned> (time(0)));

   //srand( time(NULL) );
   for (int i=0; i<24; i++)
        for (int j=0; j<32; j++) {
            Leukoxrisos[i][j]=  0+static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(99-0)));
            Palladio[i][j]= 0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(99-0)));
            Iridio[i][j] = 0+static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(99-0)));
           // do{
            DangerLevel[i][j] = 0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.9-0)));
            DangerLevel[i][j] = 0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.9-0)));
           // } while ( DangerLevel[i][j] == 1 );
        }
//base is empty and it's no danger
for (int i=11; i<=12; i++)
    for (int j=14; j<=16; j++) {
        Palladio[i][j]=0;
        Iridio[i][j]=0;
        Leukoxrisos[i][j]=0;
        DangerLevel[i][j]=0;
    }
}

int Map::GetPalladio(int x, int y) {
    return Palladio[x][y];
}

int Map::GetIridio(int x, int y) {
    return Iridio [x][y];
}

int Map::GetLeukoxrisos (int x, int y) {
    return Leukoxrisos[x][y];
}

float Map::GetDangerLevel (int x, int y) {
    return DangerLevel[x][y];
}

void Map::SetDanger(int x, int y){
    isDanger[x][y]=true;
}

void Map::SetPalladio(int x, int y, int neo) {
    Palladio[x][y]=neo;
}

void Map::SetIridio(int x, int y, int neo) {
    Iridio[x][y]=neo;
}

void Map::SetLeukoxrisos(int x, int y, int neo) {
    Leukoxrisos[x][y]=neo;
}

//void Map::Add2BaseI(int x){
//    Base [0][0] += x;
    //std::string str = toString(Base[0][0]);
    //Simulate::BaseIridio.setString(str);
//}

//void Map::Add2BaseP (int x){
//    Base[0][1] += x;
//}

//void Map::Add2BaseL(int x) {
//    Base[0][2] += x;
//}


std::string Map::toString(int x) {
            std::stringstream ss;
            ss << x;
            std::string str = ss.str();
            return str;

}

bool Map::hasFlag(int x, int y) {
    if (  flagPos[x][y] == 1 )
        return true;
    return false;
}

int Map::flagPos[24][32];
int Map::NeedHelp[24][32];
//VisibleGameObject* Map::Vihicles[24][32];
