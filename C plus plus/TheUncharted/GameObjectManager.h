#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "Map.h"
//#include "VisibleGameObject.h"


class VisibleGameObject;

class GameObjectManager
{
    public:
        GameObjectManager();
        ~GameObjectManager();

        void Add(std::string name, VisibleGameObject* gameObject );
        void Remove (std::string name);
        int GetObjectCount() const;
        VisibleGameObject* Get(std::string name ) const;

        void DrawAll (sf::RenderWindow& renderwindow);
        void UpdateAll ();

    private:
        std::map<std::string, VisibleGameObject*> gameObjects;

       // struct GameObjectDeallocator {

        //    void operator()(const std::pair<std::string, VisibleGameObject*> & p ) const {
        //        delete p.second;
        //    }

      //  };
};

#endif // GAMEOBJECTMANAGER_H
