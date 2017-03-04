#ifndef PLAYERPADDLE_H
#define PLAYERPADDLE_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "VisibleGameObject.h"
#include <iostream>

//class VisibleGameObject;

class Analyzer : public VisibleGameObject
{
    public:
        Analyzer();
        ~Analyzer();

        void Update ();
        void Draw (sf::RenderWindow& window);

        int getPositionX();
        int getPositionY();

        int getPalladioLoad() const;
        int getIridioLoad() const;
        int getLeukoxrisosLoad() const;
        int getMaxLoad();
        void GenerateStartedPosition ();
        int GetSteps();
        int GetDamages();
        int GetTotalFlags();
        int GetTotalSaves();


        void setCode(std::string);
        std::string getCode();

        int RobotCode;

        int poreia;


    protected:
    private:
        const int speed = 1; //1 box per lap

        int PalladioLoad;
        int IridioLoad;
        int LeukoxrisosLoad;
        int MaxLoad;

        int DamageChance;

        int GetRundomDirection();
        int currentPositionX;
        int currentPositionY;


        //mining staff
        //returns true if robot can do mining here. (one meterial>=90 or all>=50 (υπόθεση)
        bool isForMining (int, int);
        void Mining (int,int);

        std::string code;

        //int Gone[24][32]; //1 means that it has gone there before
        //bool hasGone(int,int);

        bool end;

        void GenerateTarget();
        int targetX;
        int targetY;

        int times=0;

        int damageSetted;

        int steps;
        int Damages;
        int totalFlags;


};

#endif // PLAYERPADDLE_H
