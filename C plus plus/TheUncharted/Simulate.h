#ifndef SIMULATE_H
#define SIMULATE_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SplashScreen.h"
#include "MainMenu.h"
#include "Map.h"
#include <iostream>

class GameObjectManager; //straight declaration of PlayerPaddle (include don't work)

class Simulate
{
    public:
        static void Start(); //creates the basic window and it call the basic GameLoop
        sf::Event GetEvent();

        static  Map map;
        static int lap;

        static sf::RenderWindow mainwindow;

        static sf::Text BaseIridio;
        static sf::Text BasePalladio;
        static sf::Text BaseLeukoxrisos;

        static std::string names[100];

        static GameObjectManager gameobjectmanager;

        static int RobotsAdded;

        static int AnalyzerNumber;
        static int Analyzers2Base;

        static bool infoMode;

    private:
        static bool IsExiting(); //check the gamestate. return true if it's "Exiting"
        static void GameLoop(); //the basic loop with basic events

        static void ShowSplashScreen();
        static void ShowMenu();
        static void ShowPaused();


        static sf::Event Event;
        static sf::Font font;
        static sf::Text text;

        static sf::Text Number;


        static sf::Sprite play;
        static sf::Texture playTexture;

        static sf::Text lapLabel;
        static sf::Text LapNo;

};

#endif // SIMULATE_H
