#include "SplashScreen.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

void SplashScreen::Show(sf::RenderWindow & window) {

    //creates the texture
    sf::Texture texture;
    if (!texture.loadFromFile("images/splash.png") )
        return;
    //creates the sprite
    sf::Sprite sprite;
    sprite.setTexture(texture);

     window.draw(sprite);
     window.display();

    sf::Event event;

    //main splashscreen loop
    while (true) {
        while ( window.pollEvent(event)) {

            if (event.type == sf::Event::KeyPressed
                || event.type == sf::Event::MouseButtonPressed
                    || event.type == sf::Event::Closed )
                        return;

        }
    }
}
