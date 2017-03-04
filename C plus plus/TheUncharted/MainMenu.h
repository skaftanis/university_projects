#ifndef MAINMENU_H
#define MAINMENU_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <list>


class MainMenu
{
    public:
        enum MenuResult { Nothing, Exit, Play };

        struct MenuItem {
            public:
                sf::Rect<int> rect;
                MenuResult action;
        };

        MenuResult Show(sf::RenderWindow& window);
    protected:
    private:
        MenuResult GetMenuResponse(sf::RenderWindow& window);
        MenuResult HandleClick(int x, int y);
        std::list<MenuItem> menuItems;
        //MenuResult array[2];
};

#endif // MAINMENU_H
