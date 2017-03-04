#ifndef EXPLORER_H
#define EXPLORER_H
#include "VisibleGameObject.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Explorer : public VisibleGameObject
{
    public:
        Explorer();
        ~Explorer();

        void Update();
        void Draw (sf::RenderWindow& window);

        int getPositionX() ;
        int getPositionY() ;


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

        int GetRundomDirection();
        bool isForFlag(int, int);

        sf::Texture texture;

        int count;

        sf::Sprite flags[768];

        int startedPosX;
        int startedPosY;

        int prosimo;

        int damageSetted;

        int steps=0;
        int Damages=0;
        int totalFlags=0;
};

#endif // EXPLORER_H
