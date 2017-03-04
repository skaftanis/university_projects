#include "InfoWindow.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include "Analyzer.h"
#include "VisibleGameObject.h"
#include <cstdlib>
#include <sstream>


InfoWindow::InfoWindow()
{
    infowindow.create(sf::VideoMode(220,768), "info");
    font.loadFromFile("font/Comfortaa-Regular.ttf");

    if (!showTexture.loadFromFile("images/show.png") ) {
        std::cout << "Error! Can't find a photo show.png" << std::endl;
    }
}


void InfoWindow::Start() {

        title.setString("Paused Menu");
        title.setFont(font);
        title.setColor(sf::Color::White);

        add.setString("1.Add Analyzer");
        add.setFont(font);
        add.setColor(sf::Color::Red);
        add.setPosition(0,64);


        addE.setString("2.Add Explorer");
        addE.setFont(font);
        addE.setColor(sf::Color::Red);
        addE.setPosition(0,3*34);


        addR.setString("3.Add Rescue");
        addR.setFont(font);
        addR.setColor(sf::Color::Red);
        addR.setPosition(0,4*34);


        remove.setString("4.Remove (con)");
        remove.setFont(font);
        remove.setColor(sf::Color::Red);
        remove.setPosition(0,32*5);

        edit.setString("5.Edit (con) ");
        edit.setFont(font);
        edit.setColor(sf::Color::Red);
        edit.setPosition(0,32*6);

        show.setString("6.Show Info");
        show.setFont(font);
        show.setColor(sf::Color::Red);
        show.setPosition(0,32*7);

        exit.setString("7.Continue");
        exit.setFont(font);
        exit.setColor(sf::Color::Red);
        exit.setPosition(0,32*23);

        infowindow.clear();

        infowindow.draw(title);
        infowindow.draw(add);
        infowindow.draw(addE);
        infowindow.draw(addR);
        infowindow.draw(remove);
        infowindow.draw(edit);
        infowindow.draw(show);
        infowindow.draw(exit);


}

void InfoWindow::Show() {

        infowindow.display();

}


template < typename T >
std::string InfoWindow::toString(T x) {
            std::stringstream ss;
            ss << x;
            std::string str = ss.str();
            return str;

}

void InfoWindow::ShowInfos(int x, int y) {  //x=y & y=x από την κλήση της

        if ( x ==11*32   &&  y == 15*32  ) { //περιοχή ελέγχου βάσης

        //base staff

        areaInfo.setString("Base Infos");
        areaInfo.setFont(font);
        areaInfo.setColor(sf::Color::Red);
        areaInfo.setPosition(0,32*10);

        BaseTitle.setString("Total in Base: ");
        BaseTitle.setFont(font);
        BaseTitle.setColor(sf::Color::White);
        BaseTitle.setPosition(0,32*11);
        BaseTitle.setCharacterSize(15);

        int temp2 = Simulate::Analyzers2Base*2000 ; //**
        std::string str2 = toString(temp2);
        TotalValue.setString(str2);
        TotalValue.setFont(font);
        TotalValue.setColor(sf::Color::White);
        TotalValue.setPosition(0,32*11.5);
        TotalValue.setCharacterSize(15);

        palladio.setString("Total Palladio: ");
        palladio.setFont(font);
        palladio.setColor(sf::Color::White);
        palladio.setPosition(0,32*12);
        palladio.setCharacterSize(15);

        int sum=0;
        for (int i=0; i<Simulate::Analyzers2Base; i++)
            sum += Simulate::map.GetPalladio(12,14+i);
        std::string stri  = toString(sum);
        palladioValue.setString(stri);
        palladioValue.setFont(font);
        palladioValue.setColor(sf::Color::White);
        palladioValue.setPosition(0,32*12.5);
        palladioValue.setCharacterSize(15);

        iridio.setString("Total Iridio: ");
        iridio.setFont(font);
        iridio.setColor(sf::Color::White);
        iridio.setPosition(0,32*13);
        iridio.setCharacterSize(15);

        sum=0;
        for (int i=0; i<Simulate::Analyzers2Base; i++)
            sum += Simulate::map.GetIridio(12,14+i);
        std::string stri2 = toString(sum);
        iridioValue.setString(stri2);
        iridioValue.setFont(font);
        iridioValue.setColor(sf::Color::White);
        iridioValue.setPosition(0,32*13.5);
        iridioValue.setCharacterSize(15);

        leukoxrisos.setString("Total Leukoxrisos: ");
        leukoxrisos.setFont(font);
        leukoxrisos.setColor(sf::Color::White);
        leukoxrisos.setPosition(0,32*14);
        leukoxrisos.setCharacterSize(15);

        sum=0;
        for (int i=0; i<Simulate::Analyzers2Base; i++)
            sum += Simulate::map.GetLeukoxrisos(12,14+i);
        std::string stri3 = toString(sum);
        leukoxrisosValue.setString(stri3);
        leukoxrisosValue.setFont(font);
        leukoxrisosValue.setColor(sf::Color::White);
        leukoxrisosValue.setPosition(0,32*14.5);
        leukoxrisosValue.setCharacterSize(15);

        TotalDamagesText.setString("Total Vehicle Damages: ");
        TotalDamagesText.setFont(font);
        TotalDamagesText.setColor(sf::Color::White);
        TotalDamagesText.setPosition(0,32*15);
        TotalDamagesText.setCharacterSize(15);
//***********
        sum=0;
        VisibleGameObject *tempRobot = new VisibleGameObject();
        for (int i=0; i<Simulate::RobotsAdded; i++) {
            tempRobot=Simulate::gameobjectmanager.Get(Simulate::names[i]);
            sum += tempRobot->GetDamages();
        }
        std::string str4 = toString(sum);
        TotalDamagesValue.setString(str4);
        TotalDamagesValue.setFont(font);
        TotalDamagesValue.setColor(sf::Color::White);
        TotalDamagesValue.setPosition(0,32*15.5);
        TotalDamagesValue.setCharacterSize(15);

        TotalFlagsText.setString("Total Flags: ");
        TotalFlagsText.setFont(font);
        TotalFlagsText.setColor(sf::Color::White);
        TotalFlagsText.setPosition(0,32*16);
        TotalFlagsText.setCharacterSize(15);

        sum=0;
        VisibleGameObject *tempRobot2 = new VisibleGameObject();
        for (int i=0; i<Simulate::RobotsAdded; i++) {
            tempRobot2=Simulate::gameobjectmanager.Get(Simulate::names[i]);
            sum += tempRobot2->GetTotalFlags();
        }
        std::string str5 = toString(sum);
        TotalFlagsValue.setString(str5);
        TotalFlagsValue.setFont(font);
        TotalFlagsValue.setColor(sf::Color::White);
        TotalFlagsValue.setPosition(0,32*16.5);
        TotalFlagsValue.setCharacterSize(15);



        infowindow.clear();

        infowindow.draw(areaInfo);
        infowindow.draw(BaseTitle);
        infowindow.draw(TotalValue);
        infowindow.draw(palladio);
        infowindow.draw(palladioValue);
        infowindow.draw(iridio);
        infowindow.draw(iridioValue);
        infowindow.draw(leukoxrisos);
        infowindow.draw(leukoxrisosValue);
        infowindow.draw(TotalDamagesText);
        infowindow.draw(TotalDamagesValue);
        infowindow.draw(TotalFlagsText);
        infowindow.draw(TotalFlagsValue);
        infowindow.draw(TotalFlagsText);
        infowindow.draw(TotalFlagsValue);

        } else { //normal area staff

        areaInfo.setString("Area Infos");
        areaInfo.setFont(font);
        areaInfo.setColor(sf::Color::Red);
        areaInfo.setPosition(0,32*10);



        palladio.setString("Palladio: ");
        palladio.setFont(font);
        palladio.setColor(sf::Color::White);
        palladio.setPosition(0,32*11);
        palladio.setCharacterSize(15);

        iridio.setString("Iridio: ");
        iridio.setFont(font);
        iridio.setColor(sf::Color::White);
        iridio.setPosition(0,32*12);
        iridio.setCharacterSize(15);

        leukoxrisos.setString("Leukoxrisos: ");
        leukoxrisos.setFont(font);
        leukoxrisos.setColor(sf::Color::White);
        leukoxrisos.setPosition(0,32*13);
        leukoxrisos.setCharacterSize(15);

        int temp = Simulate::map.GetIridio(x/32,y/32);
        std::string str = toString(temp);
        iridioValue.setString(str);
        iridioValue.setFont(font);
        iridioValue.setColor(sf::Color::White);
        iridioValue.setPosition(0,32*12.5);
        iridioValue.setCharacterSize(15);

        int temp2 = Simulate::map.GetPalladio(x/32,y/32);
        std::string str2 = toString(temp2);
        palladioValue.setString(str2);
        palladioValue.setFont(font);
        palladioValue.setColor(sf::Color::White);
        palladioValue.setPosition(0,32*11.5);
        palladioValue.setCharacterSize(15);

        int temp3 = Simulate::map.GetLeukoxrisos(x/32,y/32);
        std::string str3 = toString(temp3);
        leukoxrisosValue.setString(str3);
        leukoxrisosValue.setFont(font);
        leukoxrisosValue.setColor(sf::Color::White);
        leukoxrisosValue.setPosition(0,32*13.5);
        leukoxrisosValue.setCharacterSize(15);

        DangerLevel.setString("Danger Level: ");
        DangerLevel.setFont(font);
        DangerLevel.setColor(sf::Color::White);
        DangerLevel.setPosition(0,32*14);
        DangerLevel.setCharacterSize(15);

        float tempd = Simulate::map.GetDangerLevel(x/32,y/32);
        str3 = toString(tempd); //toString function used with template, so we can add both int and floats to it.
        DangerLevelValue.setString(str3);
        DangerLevelValue.setFont(font);
        DangerLevelValue.setColor(sf::Color::White);
        DangerLevelValue.setPosition(0,32*14.5);
        DangerLevelValue.setCharacterSize(15);


        VisibleGameObject *tem = new VisibleGameObject();
        for (int i=0; i<Simulate::RobotsAdded; i++) {

            tem=Simulate::gameobjectmanager.Get(Simulate::names[i]);
            spritePos.x = tem->getPositionX();
            spritePos.y = tem->getPositionY();

            if (spritePos.x == y && spritePos.y == x && tem->Type=="Analyzer") {
               // std::cout << " WE ARE GOOD " << std::endl;
                AnalyzerLabel.setString("Analyzer Detected: ");
                AnalyzerLabel.setFont(font);
                AnalyzerLabel.setColor(sf::Color::Red);
                AnalyzerLabel.setPosition(0,32*16);
                AnalyzerLabel.setCharacterSize(20);

                PalladioLoaded.setString("Palladio Loaded: ");
                PalladioLoaded.setFont(font);
                PalladioLoaded.setColor(sf::Color::White);
                PalladioLoaded.setPosition(0,32*16.5);
                PalladioLoaded.setCharacterSize(15);

                temp = tem->getPalladioLoad();
                str = toString(temp);
                PalladioLoadedValue.setString(str);
                PalladioLoadedValue.setFont(font);
                PalladioLoadedValue.setColor(sf::Color::White);
                PalladioLoadedValue.setPosition(0,32*17);
                PalladioLoadedValue.setCharacterSize(15);

                IridioLoaded.setString("Iridio Loaded: ");
                IridioLoaded.setFont(font);
                IridioLoaded.setColor(sf::Color::White);
                IridioLoaded.setPosition(0,32*17.5);
                IridioLoaded.setCharacterSize(15);

                temp = tem->getIridioLoad();
                str = toString(temp);
                IridioLoadedValue.setString(str);
                IridioLoadedValue.setFont(font);
                IridioLoadedValue.setColor(sf::Color::White);
                IridioLoadedValue.setPosition(0,32*18);
                IridioLoadedValue.setCharacterSize(15);

                LeukoxrisosLoaded.setString("Leukoxrisos Loaded: ");
                LeukoxrisosLoaded.setFont(font);
                LeukoxrisosLoaded.setColor(sf::Color::White);
                LeukoxrisosLoaded.setPosition(0,32*18.5);
                LeukoxrisosLoaded.setCharacterSize(15);

                temp = tem->getLeukoxrisosLoad();
                str = toString(temp);
                LeukoxrisosLoadedValue.setString(str);
                LeukoxrisosLoadedValue.setFont(font);
                LeukoxrisosLoadedValue.setColor(sf::Color::White);
                LeukoxrisosLoadedValue.setPosition(0,32*19);
                LeukoxrisosLoadedValue.setCharacterSize(15);

                TotalLoaded.setString("Total Loaded: ");
                TotalLoaded.setFont(font);
                TotalLoaded.setColor(sf::Color::White);
                TotalLoaded.setPosition(0,32*19.5);
                TotalLoaded.setCharacterSize(15);

                temp = tem->getLeukoxrisosLoad()+ tem->getIridioLoad() + tem->getPalladioLoad() ;
                str = toString(temp);
                TotalLoadedValue.setString(str);
                TotalLoadedValue.setFont(font);
                TotalLoadedValue.setColor(sf::Color::White);
                TotalLoadedValue.setPosition(0,32*20);
                TotalLoadedValue.setCharacterSize(15);

                StepsText.setString("Total Steps: ");
                StepsText.setFont(font);
                StepsText.setColor(sf::Color::White);
                StepsText.setPosition(0,32*20.5);
                StepsText.setCharacterSize(15);

                temp = tem->GetSteps();
                str = toString(temp);
                StepsValue.setString(str);
                StepsValue.setFont(font);
                StepsValue.setColor(sf::Color::White);
                StepsValue.setPosition(0,32*21);
                StepsValue.setCharacterSize(15);

                DamagesText.setString("Total Damages: ");
                DamagesText.setFont(font);
                DamagesText.setColor(sf::Color::White);
                DamagesText.setPosition(0,32*21.5);
                DamagesText.setCharacterSize(15);

                temp = tem->GetDamages();
                str = toString(temp);
                DamagesValue.setString(str);
                DamagesValue.setFont(font);
                DamagesValue.setColor(sf::Color::White);
                DamagesValue.setPosition(0,32*22);
                DamagesValue.setCharacterSize(15);


                break;
                }

            if (spritePos.x == y && spritePos.y == x && tem->Type=="Explorer") {

                ExplorerLabel.setString("Explorer Detected: ");
                ExplorerLabel.setFont(font);
                ExplorerLabel.setColor(sf::Color::Red);
                ExplorerLabel.setPosition(0,32*16);
                ExplorerLabel.setCharacterSize(20);

                StepsText.setString("Total Steps: ");
                StepsText.setFont(font);
                StepsText.setColor(sf::Color::White);
                StepsText.setPosition(0,32*17);
                StepsText.setCharacterSize(15);

                temp = tem->GetSteps();
                str = toString(temp);
                StepsValue.setString(str);
                StepsValue.setFont(font);
                StepsValue.setColor(sf::Color::White);
                StepsValue.setPosition(0,32*17.5);
                StepsValue.setCharacterSize(15);

                DamagesText.setString("Total Damages: ");
                DamagesText.setFont(font);
                DamagesText.setColor(sf::Color::White);
                DamagesText.setPosition(0,32*18);
                DamagesText.setCharacterSize(15);

                temp = tem->GetDamages();
                str = toString(temp);
                DamagesValue.setString(str);
                DamagesValue.setFont(font);
                DamagesValue.setColor(sf::Color::White);
                DamagesValue.setPosition(0,32*18.5);
                DamagesValue.setCharacterSize(15);

                ExplorerFlagsLabel.setString("Total Flags Placed: ");
                ExplorerFlagsLabel.setFont(font);
                ExplorerFlagsLabel.setColor(sf::Color::White);
                ExplorerFlagsLabel.setPosition(0,32*19);
                ExplorerFlagsLabel.setCharacterSize(15);

                temp = tem->GetTotalFlags();
                str = toString(temp);
                ExplorerFlagsValue.setString(str);
                ExplorerFlagsValue.setFont(font);
                ExplorerFlagsValue.setColor(sf::Color::White);
                ExplorerFlagsValue.setPosition(0,32*19.5);
                ExplorerFlagsValue.setCharacterSize(15);



                break;


        }

       if (spritePos.x == y && spritePos.y == x && tem->Type=="Rescue") {

                RescueLabel.setString("Rescue Robot Detected: ");
                RescueLabel.setFont(font);
                RescueLabel.setColor(sf::Color::Red);
                RescueLabel.setPosition(0,32*16);
                RescueLabel.setCharacterSize(20);

                StepsText.setString("Total Steps: ");
                StepsText.setFont(font);
                StepsText.setColor(sf::Color::White);
                StepsText.setPosition(0,32*17);
                StepsText.setCharacterSize(15);

                temp = tem->GetSteps();
                str = toString(temp);
                StepsValue.setString(str);
                StepsValue.setFont(font);
                StepsValue.setColor(sf::Color::White);
                StepsValue.setPosition(0,32*17.5);
                StepsValue.setCharacterSize(15);


                RescueSavesText.setString("Total Saves: ");
                RescueSavesText.setFont(font);
                RescueSavesText.setColor(sf::Color::White);
                RescueSavesText.setPosition(0,32*18);
                RescueSavesText.setCharacterSize(15);

                temp = tem->GetTotalSaves();
                str = toString(temp);
                RescueSavesValue.setString(str);
                RescueSavesValue.setFont(font);
                RescueSavesValue.setColor(sf::Color::White);
                RescueSavesValue.setPosition(0,32*18.5);
                RescueSavesValue.setCharacterSize(15);

                break;
       }

        }
        infowindow.clear();

        infowindow.draw(areaInfo);
        infowindow.draw(palladio);
        infowindow.draw(palladioValue);
        infowindow.draw(iridio);
        infowindow.draw(iridioValue);
        infowindow.draw(leukoxrisos);
        infowindow.draw(leukoxrisosValue);
        infowindow.draw(DangerLevel);
        infowindow.draw(DangerLevelValue);


        if (spritePos.x ==y && spritePos.y == x && tem->Type=="Analyzer") {
            infowindow.draw(AnalyzerLabel);
            infowindow.draw(PalladioLoaded);
            infowindow.draw(PalladioLoadedValue);
            infowindow.draw(IridioLoaded);
            infowindow.draw(IridioLoadedValue);
            infowindow.draw(LeukoxrisosLoaded);
            infowindow.draw(LeukoxrisosLoadedValue);
            infowindow.draw(TotalLoaded);
            infowindow.draw(TotalLoadedValue);
            infowindow.draw(StepsText);
            infowindow.draw(StepsValue);
            infowindow.draw(DamagesText);
            infowindow.draw(DamagesValue);
            }
        if (spritePos.x ==y && spritePos.y == x && tem->Type=="Explorer") {
            infowindow.draw(ExplorerLabel);
            infowindow.draw(StepsText);
            infowindow.draw(StepsValue);
            infowindow.draw(DamagesText);
            infowindow.draw(DamagesValue);
            infowindow.draw(ExplorerFlagsLabel);
            infowindow.draw(ExplorerFlagsValue);

        }

        if (spritePos.x ==y && spritePos.y == x && tem->Type=="Rescue") {
            infowindow.draw(RescueLabel);
            infowindow.draw(StepsText);
            infowindow.draw(StepsValue);;
            infowindow.draw(RescueSavesText);
            infowindow.draw(RescueSavesValue);
        }



    }

        //draw old staff
        infowindow.draw(title);
        infowindow.draw(add);
        infowindow.draw(addE);
        infowindow.draw(addR);
        infowindow.draw(remove);
        infowindow.draw(edit);
        infowindow.draw(show);




}


void InfoWindow::showClickedArea() {

    VisibleGameObject *tem = new VisibleGameObject();
    SpriteTablePosition=-1;
    for (int i=0; i<Simulate::RobotsAdded; i++) {
                tem=Simulate::gameobjectmanager.Get(Simulate::names[i]);
                sf::Sprite showSprite;
                SpriteTablePosition++;
                Sprites[SpriteTablePosition]=showSprite;
                Sprites[SpriteTablePosition].setPosition(tem->getPositionX(), tem->getPositionY());
                Sprites[SpriteTablePosition].setTexture(showTexture);
                Simulate::mainwindow.draw(Sprites[i]);
    }

}


sf::RenderWindow InfoWindow::infowindow;
