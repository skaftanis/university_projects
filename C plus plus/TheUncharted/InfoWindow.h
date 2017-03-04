#ifndef INFOWINDOW_H
#define INFOWINDOW_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Analyzer.h"
#include "VisibleGameObject.h"
#include <iostream>

class InfoWindow
{
    public:
        InfoWindow();
        void Start();
        void Show();
        void ShowInfos(int,int);

       static  sf::RenderWindow infowindow;

        template < typename T >
        std::string toString (T);

        sf::Text add;
        sf::Text addE;
        sf::Text addR;
        sf::Text remove;
        sf::Text edit;
        sf::Text show;
        sf::Text exit;



        sf::Text areaInfo;
        sf::Text palladio;
        sf::Text iridio;
        sf::Text leukoxrisos;
        sf::Text palladioValue;
        sf::Text iridioValue;
        sf::Text leukoxrisosValue;
        sf::Text TotalValue;
        sf::Text BaseTitle;
        sf::Text DangerLevel;
        sf::Text DangerLevelValue;

        sf::Text AnalyzerLabel;
        sf::Text PalladioLoaded;
        sf::Text IridioLoaded;
        sf::Text LeukoxrisosLoaded;
        sf::Text PalladioLoadedValue;
        sf::Text IridioLoadedValue;
        sf::Text LeukoxrisosLoadedValue;
        sf::Text TotalLoaded;
        sf::Text TotalLoadedValue;

        sf::Text TotalDamagesText;
        sf::Text TotalDamagesValue;

        sf::Text TotalFlagsText;
        sf::Text TotalFlagsValue;

        sf::Text StepsText;
        sf::Text StepsValue;

        sf::Text DamagesText;
        sf::Text DamagesValue;

        sf::Text ExplorerLabel;
        sf::Text ExplorerFlagsLabel;
        sf::Text ExplorerFlagsValue;

        sf::Text RescueLabel;
        sf::Text RescueSavesText;
        sf::Text RescueSavesValue;


        void showClickedArea();

    private:


        sf::Font font;

        sf::Text title;

        sf::Vector2f spritePos;

        sf::Texture showTexture;

        sf::Sprite Sprites[30];
        int SpriteTablePosition=-1;
        int PrePosition;

};

#endif // INFOWINDOW_H
