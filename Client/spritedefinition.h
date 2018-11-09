#ifndef SPRITEDEFINITION_H
#define SPRITEDEFINITION_H
#include <Box2D/Box2D.h>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>

/**
 * @brief The SpriteDefinition class acts as a template ( following the body definition approach )
 * for Sprite2dobject in the Stackd game API.
 */
class SpriteDefinition
{
public:

    SpriteDefinition(int, int, b2BodyType, std::string);
    SpriteDefinition();
    ~SpriteDefinition();

    void setText(std::string);
    void setRestitution(double value);
    void setDensity(double);
    void dampen(double amount);
    void setFriction(double);
    void setPosition(int,int);
    void setInitialVelocity(double,b2Vec2);
    void setColor(sf::Color);
    void setShape(int,int,int);
    void setType(b2BodyType);
    void setBorderColor(sf::Color);

    b2BodyDef * body;
    b2FixtureDef * fixture;
    std::string name;
    sf::Color color;
    std::string text;
    sf::Color bordercolor;

};

#endif // SPRITEDEFINITION_H
