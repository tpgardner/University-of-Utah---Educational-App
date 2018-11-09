#include <spritedefinition.h>


SpriteDefinition::~SpriteDefinition() {
}

SpriteDefinition::SpriteDefinition(int x, int y, b2BodyType type, std::string s) : SpriteDefinition() {
    body->type = type;
    body->position.Set(x, y);
    name = s;
    color = sf::Color::White;
    bordercolor = sf::Color::White;
}

SpriteDefinition::SpriteDefinition() {
    body = new b2BodyDef;
    fixture = new b2FixtureDef;
    body->allowSleep = true;
    body->active = true;
}

void SpriteDefinition::setRestitution(double value) {
    fixture->restitution = value;
}

void SpriteDefinition::setBorderColor(sf::Color c) {
    bordercolor = c;
}

void SpriteDefinition::setShape(int count, int width, int height) {
    b2PolygonShape *shape = new b2PolygonShape;

    fixture->density = 1;

    //SETUP
    if (width < 9) width = 10; // filter bad input

    if (height < 9) height = 10;

    if (count < 3) count = 3;
    else if (count > 8) count = 8;

    double alpha = 0;

    if (count == 4) { //the way the alg works it makes it crooked.
        alpha = M_PI / 4; //so we are just straightening things out
    } else if (count == 3) {
        alpha = 3 * M_PI / 2;
    }

    //GATHER POINTS
    std::vector<b2Vec2> points;

    for (int i = 0; i < count; i++) {
        int tempx = 0;
        int tempy = 0;
        tempx += (width) * std::cos(alpha + (2 * M_PI * i / count));
        tempx += body->position.x;
        tempy += (height) * std::sin(alpha + (2 * M_PI * i / count));
        tempy += body->position.y;
        b2Vec2 vec(tempx, tempy);
        points.push_back(vec);
    }

    //CREATE SHAPE and return
    shape->Set(&*points.begin(), count);
    fixture->shape = shape;
}

void SpriteDefinition::setInitialVelocity(double angular, b2Vec2 linear) {
    body->angularVelocity = angular;
    body->linearVelocity = linear;
}

void SpriteDefinition::setPosition(int x, int y) {
    b2Vec2 v(x, y);

    body->position.Set(x, y);
}

void SpriteDefinition::setText(std::string s) {
    text = s;
}

void SpriteDefinition::setType(b2BodyType type) {
    body->type = type;
}

void SpriteDefinition::setDensity(double set) {
    fixture->density = set;
}

void SpriteDefinition::dampen(double amount) {
    body->linearDamping += amount;
}

void SpriteDefinition::setFriction(double set) {
    fixture->friction = set;
}

void SpriteDefinition::setColor(sf::Color c) {
    color = c;
}
