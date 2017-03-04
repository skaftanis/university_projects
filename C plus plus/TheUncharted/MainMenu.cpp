#include "MainMenu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <list>


MainMenu::MenuResult MainMenu::Show(sf::RenderWindow& window) {

    //load image from file via textures
    sf::Texture texture;
    sf::Sprite sprite;
    texture.loadFromFile("images/menu.png");
    sprite.setTexture(texture);


    //Setup clickable regions

   //Play menu item coordinates
    MenuItem playButton;
    playButton.rect.top= 348;
    playButton.rect.width = 412;
    playButton.rect.left = 657;
    playButton.rect.height = 936;
    playButton.action = Play;

    //Exit menu item coordinates
	MenuItem exitButton;
	exitButton.rect.left = 670;
	exitButton.rect.height = 916;
	exitButton.rect.top = 453;
	exitButton.rect.width = 540;
	exitButton.action = Exit;

    menuItems.push_back(playButton);
    menuItems.push_back(exitButton);
    //array[0] = playButton;
    //array[1] = exitButton;

    window.draw(sprite);
	window.display();

	return GetMenuResponse(window);


}

 MainMenu::MenuResult  MainMenu::GetMenuResponse(sf::RenderWindow& window)
{
	sf::Event menuEvent;

	while(true)
	{

		while(window.pollEvent(menuEvent))
		{
			if(menuEvent.type == sf::Event::MouseButtonPressed)
			{

                return HandleClick(menuEvent.mouseButton.x,menuEvent.mouseButton.y);

			}

			if(menuEvent.type == sf::Event::Closed)
			{
				return Exit;
			}
		}
	}
}

MainMenu::MenuResult MainMenu::HandleClick(int x, int y)
{
	std::list<MenuItem>::iterator it;

	for ( it = menuItems.begin(); it != menuItems.end(); it++)
	{
		sf::Rect<int> menuItemRect = (*it).rect;
		if( menuItemRect.width > y
			&& menuItemRect.top < y
			&& menuItemRect.left < x
			&& menuItemRect.height > x)
			{
				return (*it).action;
			}
	}

	return Nothing;
}
