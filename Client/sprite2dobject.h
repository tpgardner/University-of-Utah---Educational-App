#ifndef SPRITE2DOBJECT_H
#define SPRITE2DOBJECT_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/Joints/b2DistanceJoint.h>
#include <Box2D/Box2D.h>
#include <exceptions.h>
#include <spritedefinition.h>
#include <cmath>
#include <math.h>
#include <vector>
#include <QFile>
#include <sstream>

/**
 * @brief The sprite2dObject class integrates SFML sprites with Box2D shapes and bodies
 * for use in the Stackd game API.
 */
class sprite2dObject{

public:
    enum Direction {//enum for directional commands
        left,right,up,down
    };

    sprite2dObject();
    ~sprite2dObject();
    sprite2dObject(std::string,b2World*, b2BodyDef*);
    sprite2dObject(b2World*,SpriteDefinition);

    //physics engine interactinons
    void bindToMouse();


    void unbind();
    void connectRope(sprite2dObject*, int length = -1);
    void connectBar(sprite2dObject*,int length = 10);
    void moveBody(Direction, int);
    void moveToPoint(int,int);
    void applyAngularForce(Direction,double);
    bool inContact(sprite2dObject* s);

    //garbage collection methods
    void ignoreObject();
    bool isIgnored();
    void mark();
    void destroy();
    bool marked();

    void setFontSize(int);

    //various getters/setters for wrapper API
    b2Body * getBody();
    sf::ConvexShape getShape();
    sf::Color getBorderColor();
    sf::Color getColor();
    void changeBorderColor(sf::Color color);
    void changeColor(sf::Color color);
    void setName(std::string);
    std::string getName();
    void setFriction(double);
    void setDensity(double);
    void setSprite(std::string pathname);
    void setText(std::string, sf::Color c = sf::Color::White);
    void setTextColor(sf::Color);
    sf::Text * getText();
    sf::Sprite * getSprite();
    b2Vec2 getSize();
    void scaleSize(int factor);

private:
    sf::Text * text;
    sf::Font font;
    sf::Color color;
    b2Body * textBody;
    sf::Color bordercolor;
    sf::Sprite * sprite = nullptr;
    std::vector<b2Joint*> joints;
    std::string name = "";
    b2Body * body = nullptr;
    bool ignore = false;
    bool remove = false;
    int width = 0;
    int height = 0;
    double SCALE=1;
};

#endif // SPRITE2DOBJECT_H
