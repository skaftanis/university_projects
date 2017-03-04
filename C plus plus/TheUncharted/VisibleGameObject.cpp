#include "VisibleGameObject.h"
#include "Map.h"
#include "GameObjectManager.h"


VisibleGameObject::VisibleGameObject(){
    isLoaded=false;
    Repair=false;
}

VisibleGameObject::~VisibleGameObject(){

}

void VisibleGameObject::Load(std::string name) {

    if (!image.loadFromFile(name) ) {
        filename="";
        isLoaded=false;
    }
    else {
        filename=name;
        sprite.setTexture(image);
        isLoaded=true;
    }
}

void VisibleGameObject::Draw (sf::RenderWindow& renderWindow) {
    if (isLoaded)
        renderWindow.draw(sprite);

}

void VisibleGameObject::SetPosition(float x, float y) {

    if (isLoaded) {
        sprite.setPosition(x,y);
    }
}

void VisibleGameObject::Update() {
}

int VisibleGameObject::getPositionX() {
}

int VisibleGameObject::getPositionY(){
}


sf::Vector2f VisibleGameObject::GetPosition() const {
    if (isLoaded)
        return sprite.getPosition();
}

sf::Sprite& VisibleGameObject::GetSprite() {
    return sprite;
}

bool VisibleGameObject::IsLoaded() const {
    return isLoaded;
}

//robots staff
void VisibleGameObject::GenerateAccessAbility() { //(0,1-1)

    srand (static_cast <unsigned> (time(0)));
    int x;
    x= 0.1 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1-0.1)));
    accessAbility=x;
}

bool VisibleGameObject::isDamaged(int x, int y) {
    if ( Repair == true) {
         Repair=false;
         PrRepair=true;
         return false;
    }
    if ( Simulate::map.flagPos[x/32][y/32] == 1 ) { //αν έχει σημαία δεν κάνει εξόρυξη, άρα δεν κινδυνεύει από κάτι
        PrRepair=false;
        return false;
    }
    if ( getDamageChance(x,y) >= 0.84 && PrRepair==false ) { // 0<=P(damage)<=0.90
        return true;
    }
    return false;
}

bool VisibleGameObject::outofBase (int x, int y) {

    if ( (x==14*32 && (y==11*32 || y==12*32)) || (x==15*32 && (y==11*32||y==12*32)) || (x==16*32 && (y==11*32||y==12*32))  )
        return false;
    return true;
}

float VisibleGameObject::getDamageChance(int x, int y) {
    return Simulate::map.GetDangerLevel(x/32,y/32)*(1-getAccessAbility());
}

float VisibleGameObject::getAccessAbility() const {
    return accessAbility;
}

void VisibleGameObject::setDamagedFalse() {
    Repair=true;
}

int VisibleGameObject::getPalladioLoad() const{
}

int VisibleGameObject::getIridioLoad() const {
}

int VisibleGameObject::getLeukoxrisosLoad() const {
}

int VisibleGameObject::GetSteps(){
}

int VisibleGameObject::GetDamages(){
}

int VisibleGameObject::GetTotalFlags(){
}

int VisibleGameObject::GetTotalSaves(){
}

//int VisibleGameObject::totalFlags=0;


