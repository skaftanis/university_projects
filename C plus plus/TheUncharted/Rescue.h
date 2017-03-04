#ifndef RESCUE_H
#define RESCUE_H
#include "VisibleGameObject.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Rescue : public VisibleGameObject
{
    public:
        Rescue();

        void Update();

        void Draw (sf::RenderWindow& window);

        int getPositionX();
        int getPositionY();

        int GetSteps();
        int GetDamages();
        int GetTotalFlags();
        int GetTotalSaves();


    protected:
    private:
        int currentPositionX;
        int currentPositionY;
        int speed = 1;

        void GenerateStartedPosition ();

        int startedPosX;
        int startedPosY;
        int prosimo;


         sf::Texture texture;

        void checkAndRepair();

        void GenerateTarget();
        int targetX=32*15;
        int targetY=32*11;

        int steps=0;
        int saves=0;

};

#endif // RESCUE_H
