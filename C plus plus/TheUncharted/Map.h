#ifndef MAP_H
#define MAP_H
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#include "VisibleGameObject.h"


class Map
{
    public:
        Map();
        void LoadMap(std::string name);
        void DrawMap (sf::RenderWindow& window);

        int GetPalladio (int x, int y);
        int GetIridio (int x, int y);
        int GetLeukoxrisos (int x, int y);

        float GetDangerLevel (int x, int y);
        bool GetisDanger (int x, int y);
        void SetDanger (int x, int y);

        void SetPalladio (int x, int y, int neo);
        void SetIridio (int x, int y, int neo);
        void SetLeukoxrisos (int x, int y, int neo);

        //int Base [1][3]; // iridio | palladio | leukoxrisos
        //void Add2BaseI(int); //add iririo to base
        //void Add2BaseP(int);
        //void Add2BaseL(int);

        static bool hasFlag(int,int);
        static int flagPos[24][32];
        static int NeedHelp[24][32]; //1 when there is a damaged analyzer there
        //static VisibleGameObject* Vihicles[24][32];

        int BaseTotal;

    private:

        void SetUpMap ();

        sf::Sprite sprite;
        sf::Texture image;
        std::string filename;
        bool isLoaded;

        int Palladio[24][32];
        int Iridio [24][32];
        int Leukoxrisos [24][32];

        float DangerLevel [24][32];

        bool isDanger [24][32];

        std::string toString(int);
};

#endif // MAP_H
