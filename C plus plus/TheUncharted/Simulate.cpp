#include "Simulate.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Map.h"
#include <SFML/System.hpp>
#include "SplashScreen.h"
#include "MainMenu.h"
#include "VisibleGameObject.h"
#include "GameObjectManager.h"
#include <iostream>
#include "Analyzer.h"
#include "InfoWindow.h"
#include <string>
#include <sstream>
#include "Explorer.h"
#include <cstdlib>
#include <sstream>
#include "Rescue.h"
#include <fstream>


static InfoWindow info;

void Simulate::Start(void) {
    Analyzers2Base=0;


    mainwindow.create(sf::VideoMode(1024,768), "TheUncharted Project");
    mainwindow.setPosition(sf::Vector2i(400,90));
    if (!font.loadFromFile("font/Comfortaa-Regular.ttf")) {
         return;
    }


    //create pause text
    text.setFont(font);
    text.setCharacterSize(30);
    text.setColor(sf::Color::White);
    text.setPosition(0,0);
    text.setString("0.Pause");


    lapLabel.setString("Lap:");
    lapLabel.setFont(font);
    lapLabel.setColor(sf::Color::White);
    lapLabel.setPosition(1024-64,0);
    lapLabel.setCharacterSize(15);

    std::string tempStr = info.toString(lap);
    LapNo.setString(tempStr);
    LapNo.setFont(font);
    LapNo.setColor(sf::Color::White);
    LapNo.setCharacterSize(15);
    LapNo.setPosition(1024-32,0);

    ShowSplashScreen();
    ShowMenu();

}
void Simulate::GameLoop() {


while (mainwindow.isOpen()) {
        sf::Event CurrentEvent;
        while (mainwindow.pollEvent(CurrentEvent)) {
            if (CurrentEvent.type == sf::Event::KeyPressed) {
                if (CurrentEvent.key.code == sf::Keyboard::Escape) {
                    ShowMenu();
                    }
                if (CurrentEvent.key.code == sf::Keyboard::Num0) {
                    ShowPaused();
                    }

            }


             if ( CurrentEvent.type == sf::Event::Closed) {
                    exit(0);
                }

        }

                lap++;
                std::string str = info.toString(lap);
                LapNo.setString(str);


                mainwindow.clear();


                map.DrawMap(mainwindow);
                mainwindow.draw(text);
                mainwindow.draw(lapLabel);
                mainwindow.draw(LapNo);


                gameobjectmanager.DrawAll(mainwindow);


                gameobjectmanager.UpdateAll();


                mainwindow.display();

                system ("sleep 0.5");

                if (Analyzers2Base == 18) { //επιτυχημένη ολοκλήρωση εφαρμογής! 18 ρομπότ στη βάση.

                    sf::Sprite finalSprite;
                    sf::Texture finalTexture;
                     if (!finalTexture.loadFromFile("images/final.png") ) {
                        std::cout << "Error! Can't find a photo show.png" << std::endl;
                    }
                    finalSprite.setTexture(finalTexture);
                    mainwindow.clear();
                    mainwindow.draw(finalSprite);
                    mainwindow.display();
                    system ("sleep 10");
                    exit(0);

                }
    }
}


void Simulate::ShowPaused(){
             info.Start();
             info.Show();
             while (InfoWindow::infowindow.isOpen()) {
                sf::Event infoEvent;

                while (InfoWindow::infowindow.pollEvent(infoEvent)) {
                    switch (infoEvent.type) {

                        case sf::Event::Closed:
                            exit(0);
                            break;
                        case sf::Event::KeyPressed:
                                if (infoEvent.key.code == sf::Keyboard::Num7) { //continue mode
                                    //make continue button green
                                    info.exit.setColor(sf::Color::Green);
                                    info.infowindow.draw(info.exit);
                                    info.Show();
                                    //makes others back to red color
                                    info.add.setColor(sf::Color::Red);
                                    info.infowindow.draw(info.add);
                                    info.Show();
                                    info.remove.setColor(sf::Color::Red);
                                    info.infowindow.draw(info.remove);
                                    info.Show();
                                    GameLoop();
                                }
                                if (infoEvent.key.code == sf::Keyboard::Num1) { //add analyzer
                                     Analyzer *robot = new Analyzer();
                                     int x = rand();
                                     info.add.setColor(sf::Color::Green);
                                     info.infowindow.draw(info.add);
                                     info.Show();
                                     std::string str = info.toString(x);
                                     names[RobotsAdded++]=str;
                                     robot->setCode(str);
                                     gameobjectmanager.Add(str, robot);
                                     AnalyzerNumber++;
                                     robot->RobotCode=AnalyzerNumber;
                                }
                                if (infoEvent.key.code == sf::Keyboard::Num4) { //remove mode
                                    info.showClickedArea();
                                    info.remove.setColor(sf::Color::Green);
                                    info.infowindow.draw(info.remove);
                                    info.Show();


                                    VisibleGameObject *temprobot = new VisibleGameObject();
                                    for (int i=0; i<RobotsAdded; i++) {
                                      //  if (names[i] != "NULL") {
                                        std::cout << names[i] << std::endl;
                                        temprobot=gameobjectmanager.Get(names[i]);
                                        std::string str = info.toString(i+1);
                                        Number.setString(str);
                                        Number.setFont(font);
                                        Number.setColor(sf::Color::Red);
                                        Number.setPosition(temprobot->getPositionX(),temprobot->getPositionY());
                                        Number.setCharacterSize(15);
                                        mainwindow.draw(Number);
                                        mainwindow.display();
                                        }
                                        //old way (with console) in comments

                                        std::string answer;
                                        int answerInt;
                                        //std::cout << "Remove Mode" << std::endl;
                                        do{
                                            //δημιουργίου του string ορισματός της system
                                            //το remove.sh χρειαζεται σαν όρισμα το RobotsAdded ;)
                                            std::string tempRobot = info.toString(RobotsAdded);
                                            char orisma [17] ={'b','a','s','h',' ','r','e','m','o','v','e','.','s','h',' ',tempRobot[0],tempRobot[1]};
                                            std::string neo(orisma);
                                            system(neo.c_str());
                                            //std::cout << "What Robot(s) do you want to remove? (-1 for continue) " << std::endl;
                                           // do {
                                               // std::cin >> answer;
                                               // } while  (answer > RobotsAdded || answer < -1 ) ;
                                              //  if (answer != -1) {
                                                    std::ifstream re;
                                                    re.open("RE.txt");
                                                    re >> answer;
                                                    re.close();
                                                    //convert string anwer to int
                                                    answerInt = atoi (answer.c_str());
                                                    if ( answer != "Continue|Continue" ) {
                                                        gameobjectmanager.Remove(names[answerInt-1]);
                                                        //όλα πάνε μία θέση πίσω για να καλύψουν τη κενή θέση που δημιουργείται
                                                        for (int j=0; j<RobotsAdded; j++) {
                                                            if (j>answerInt-1) {
                                                                names[answerInt-1]=names[j];
                                                            }
                                                        }
                                                        RobotsAdded--;
                                                    //names[answer-1]="NULL";
                                                    }
                                                    else {
                                                        GameLoop();
                                                    }
                                        //} while (answer != -1 );
                                        //system("clear");
                                        //GameLoop();
                                      // }
                                    } while (answer != "Continue|Continue");
                                }

                                    if (infoEvent.key.code == sf::Keyboard::Num6) { //Show Infos Mode
                                    info.showClickedArea();
                                    info.show.setColor(sf::Color::Green);
                                    info.infowindow.draw(info.show);
                                    info.Show();
                                    //start new event (mouse)
                                    while (mainwindow.isOpen()) {
                                    sf::Event MouseEvent;
                                    while (mainwindow.pollEvent(MouseEvent)) {
                                        switch (MouseEvent.type) {
                                            case sf::Event::MouseButtonPressed:

                                                if (MouseEvent.mouseButton.button == sf::Mouse::Left) {
                                                    //στρογγυλοποίηση των συνεταγμένων ως προς το προηγούμενο πολλαπλάσιο του 30
                                                    int ButtonX;
                                                    int ButtonY;
                                                    for (int i=0; i<=1024; i=i+32) {
                                                        if (i > MouseEvent.mouseButton.x) {
                                                            ButtonX=i-32;
                                                            break;
                                                        }
                                                    }
                                                    for (int i=0; i<=768; i=i+32) {
                                                        if ( i > MouseEvent.mouseButton.y) {
                                                            ButtonY=i-32;
                                                            break;
                                                        }

                                                    }
                                                    //std::cout << "x = " << ButtonX << std::endl;

                                                    playTexture.loadFromFile("images/button-play.png");
                                                    play.setTexture(playTexture);
                                                    play.setPosition(16*32,13*32);
                                                    mainwindow.draw(play);
                                                    mainwindow.display();
                                                    info.ShowInfos(ButtonY,ButtonX);
                                                    info.Show();

                                                    if (ButtonY==13*32 && ButtonX==16*32)
                                                        GameLoop();



                                        }
                                    }
                                }
                            }
                        }

                                    if (infoEvent.key.code == sf::Keyboard::Num2) { //add explorer
                                        Explorer *robot0 = new Explorer();
                                        int x = rand();
                                        info.addE.setColor(sf::Color::Green);
                                        info.infowindow.draw(info.addE);
                                        info.Show();
                                        std::string str = info.toString(x);
                                        names[RobotsAdded++]=str;
                                        //robot0->setCode(str);
                                        gameobjectmanager.Add(str, robot0);
                                }
                                 if (infoEvent.key.code == sf::Keyboard::Num3) { //add rescue robot
                                        Rescue *robot3 = new Rescue();
                                        int x = rand();
                                        info.addR.setColor(sf::Color::Green);
                                        info.infowindow.draw(info.addR);
                                        info.Show();
                                        std::string str = info.toString(x);
                                        names[RobotsAdded++]=str;
                                        //robot0->setCode(str);
                                        gameobjectmanager.Add(str, robot3);
                                }

                                if (infoEvent.key.code == sf::Keyboard::Num5) { //edit the world mode
                                    info.showClickedArea();
                                    info.edit.setColor(sf::Color::Green);
                                    info.infowindow.draw(info.edit);
                                    info.Show();
                                    while (mainwindow.isOpen()) {
                                    sf::Event MouseEvent;
                                    while (mainwindow.pollEvent(MouseEvent)) {
                                        switch (MouseEvent.type) {
                                            case sf::Event::MouseButtonPressed:

                                                if (MouseEvent.mouseButton.button == sf::Mouse::Left) {
                                                     //στρογγυλοποίηση των συνεταγμένων ως προς το προηγούμενο πολλαπλάσιο του 30
                                                    int ButtonX;
                                                    int ButtonY;
                                                    for (int i=0; i<=1024; i=i+32) {
                                                        if (i > MouseEvent.mouseButton.x) {
                                                            ButtonX=i-32;
                                                            break;
                                                        }
                                                    }
                                                    for (int i=0; i<=768; i=i+32) {
                                                        if ( i > MouseEvent.mouseButton.y) {
                                                            ButtonY=i-32;
                                                            break;
                                                        }

                                                    }
                                                    std::string answer;
                                                    int NewValue;
                                                    //std::cout << "Edit Mode" << std::endl;
                                                do {
                                                    system ("bash list.sh");
                            //old way (with terminal) in comments
                                                    //std::cout << "What element would you like to change?" << std::endl;
                                                    //std::cout << "1.Leukoxrisos" << std::endl << "2.Iridio" << std::endl << "3.Palladio" << std::endl << "4.Continue " << std::endl;
                                                   // do {
                                                     //   std::cin >> answer;
                                                      //  if (answer != 1 && answer != 2 && answer != 3 && answer != 4 && answer != 5 && answer != 6)
                                                      //      std::cout << "Undefined answer! Try again " << std::endl;
                                                   // } while ( answer != 1 && answer != 2 && answer != 3 && answer != 4 && answer != 5 && answer != 6 );
                                                    std::ifstream re;
                                                    re.open("RE.txt");
                                                    re >> answer;
                                                    re.close();
                                                    std::ifstream entry;
                                                    entry.open("ENTRY.txt");
                                                    entry >> NewValue;
                                                    entry.close();

                                                    if (answer == "Leukoxrisos|Leukoxrisos" ) {
                                                        //std::cout << "Give new value for Leukoxrisos" << std::endl;
                                                        //std::cin >> NewValue;
                                                        map.SetLeukoxrisos(ButtonY/32,ButtonX/32, NewValue);
                                                        }
                                                    else if (answer == "Iridio|Iridio") {
                                                        //std::cout << "Give new value for Iridio" << std::endl;
                                                        //std::cin >> NewValue;
                                                        map.SetIridio(ButtonY/32,ButtonX/32, NewValue);
                                                        }
                                                    else if (answer == "Palladio|Palladio") {
                                                        //std::cout << "Give new value for Paladio" << std::endl;
                                                        //std::cin >> NewValue;
                                                        map.SetPalladio(ButtonY/32,ButtonX/32,NewValue);
                                                    }
                                                    else if (answer == "Continue|Continue" ) {
                                                        GameLoop();
                                                    }

                                                } while ( answer != "Continue|Continue");

                                                }
                                            }
                                    }
                                }


                                }

                                }

                    }

                }


}

void Simulate::ShowSplashScreen() {

    SplashScreen splash;
    splash.Show(mainwindow);

}

void Simulate::ShowMenu() {

MainMenu menu;
MainMenu::MenuResult result = menu.Show(mainwindow);
switch (result) {
    case MainMenu::Exit:
        exit(0);
        break;
    case MainMenu::Play:

        GameLoop();
        break;
}

}

sf::Event Simulate::GetEvent  () {
    return Event;
}

sf::RenderWindow Simulate::mainwindow;
GameObjectManager Simulate::gameobjectmanager;
Map Simulate::map;
sf::Event Simulate::Event;
sf::Font Simulate::font;
sf::Text Simulate::text;
int Simulate::lap=0;
sf::Text Simulate::BaseIridio;
sf::Text Simulate::BasePalladio;
sf::Text Simulate::BaseLeukoxrisos;
int Simulate::RobotsAdded;
int Simulate::AnalyzerNumber;
int Simulate::Analyzers2Base;
sf::Text Simulate::lapLabel;
sf::Text Simulate::LapNo;

std::string Simulate::names[100];
sf::Sprite Simulate::play;
sf::Texture Simulate::playTexture;
bool Simulate::infoMode;
sf::Text Simulate::Number;


