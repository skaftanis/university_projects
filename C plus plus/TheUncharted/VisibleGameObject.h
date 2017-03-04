#ifndef VISIBLEGAMEOBJECT_H
#define VISIBLEGAMEOBJECT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Simulate.h"
#include "GameObjectManager.h"
#include "Map.h"
//class PlayerPaddle;
class Analyzer;
//this class used to manage the visible robot

class VisibleGameObject
{
    public:
        VisibleGameObject();
        virtual ~VisibleGameObject();

        virtual void Load (std::string filename);
        virtual void Draw (sf::RenderWindow& window);
        virtual void Update ();
        virtual int getPositionX();
        virtual int getPositionY();


        virtual void SetPosition(float, float);
        virtual sf::Vector2f GetPosition() const;
        virtual bool IsLoaded() const;

        //robots sraff (κοινά στοιχειά 3ων ρομπότ)
        virtual void GenerateAccessAbility();
        virtual bool isDamaged(int, int);
        virtual bool outofBase (int,int); //returns true if the coordinates are out of base area
        virtual float getDamageChance(int, int);
        virtual float getAccessAbility() const;

        virtual void setDamagedFalse();

        int poreia;

        //οι παρακάτω έγιναν virtual για να χρησιμοποιηθούν στην εμφάνιση των πληροφοριών των ρομπότ.
        virtual int getPalladioLoad() const;
        virtual int getIridioLoad() const;
        virtual int getLeukoxrisosLoad() const;

        virtual int GetDamages(); //for explorers and analyzers
        virtual int GetTotalFlags(); //for explorers
        virtual int GetTotalSaves(); //for rescuers

        std::string Type;
        virtual int GetSteps();

    protected:
        sf::Sprite& GetSprite();

        float accessAbility;


    private:

        bool Repair;
        bool PrRepair; //previous repair

        sf::Sprite sprite;
        sf::Texture image;
        std::string filename;
        bool isLoaded;

};

#endif // VISIBLEGAMEOBJECT_H
