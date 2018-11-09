#include "sprite2dobject.h"
#include <iostream>

// A class to integrate the SFML sprite class with the Box2D objects.
sprite2dObject::sprite2dObject() {
    body = NULL;
    sprite = NULL;
    color = sf::Color::White;
    bordercolor = sf::Color::White;
    text = new sf::Text;
}

sprite2dObject::~sprite2dObject() {
    if (body != nullptr) {
        body = NULL;
    }

    text->setString("");
}

sprite2dObject::sprite2dObject(std::string description, b2World *world, b2BodyDef *def) : sprite2dObject() { // call the super constructor
    body = world->CreateBody(def);
    name = description;
    //There may be better way to get directory
    Q_INIT_RESOURCE(sprites);
    QFile f(":/datacontrol.ttf");
    f.copy(QString("datacontrol.ttf"));

    if (!font.loadFromFile("datacontrol.ttf")) {
        // throw new std::exception;
    }

    text->setFont(font);
    text->setOrigin(0, 0);
    text->setCharacterSize(20);
}

sprite2dObject::sprite2dObject(b2World *world, SpriteDefinition def) : sprite2dObject(def.name, world, def.body) { // call the super constructor
    bordercolor = def.bordercolor;
    body->CreateFixture(def.fixture);
    setText(def.text);
    text->setString(def.text);
    color = def.color;
}

/*
 * only really used in tree
 */

//------------------------------------------------------|
void sprite2dObject::bindToMouse() {
    //ignore gravity
    body->SetType(b2_staticBody);
}

void sprite2dObject::unbind() {
    //restore gravity
    body->SetType(b2_dynamicBody);
    //apply some velocity to prevent sticking
    body->SetLinearVelocity(b2Vec2(0, 10));
}

void sprite2dObject::applyAngularForce(Direction d, double magnitude) {
    if (d == right || d == down) { // not implemented
        body->ApplyAngularImpulse(magnitude, true);
    } else if (d == left || d == up) return;
}

//-------------------------------------------------------|




/*
 * Physics Engine Manipulations
 */

void sprite2dObject::moveBody(Direction d, int magnitude) {
    int scale = body->GetMass() * magnitude;

    body->SetAwake(true);

    switch (d) {
        case left: {
            b2Vec2 vec(-1 * scale, 0);
            body->ApplyForce(vec, body->GetPosition(), true);
            break;
        }

        case right: {
            b2Vec2 vec(1 * scale, 0);
            body->ApplyForce(vec, body->GetPosition(), true);
            break;
        }

        case up: {
            b2Vec2 vec(0, -1 * scale);
            body->ApplyLinearImpulse(vec, body->GetPosition(), true);
            break;
        }

        case down: {
            b2Vec2 vec(0, -1 * scale);
            body->ApplyLinearImpulse(vec, body->GetPosition(), true);
            break;
        }

        default: {
            break;
        }
    }
}

void sprite2dObject::moveToPoint(int x, int y) {
    b2Vec2 v(x, y);

    body->SetTransform(v, body->GetAngle());
}

bool sprite2dObject::inContact(sprite2dObject *s) {
    b2Body *b = s->getBody();
    b2ContactEdge *ce = body->GetContactList();

    for (ce = body->GetContactList(); ce != NULL; ce = ce->next) {
        if (ce->other == b && ce->contact->IsTouching()) {
            return true;
        }
    }

    return false;
}

void sprite2dObject::connectRope(sprite2dObject *other, int length) {
    b2RopeJointDef jd;

    if (length < 0) {
        b2Vec2 vec(other->getBody()->GetPosition() - getBody()->GetPosition());
        b2Vec2 tmp(vec.x, vec.y);
        jd.maxLength = tmp.Normalize();
    } else {
        jd.maxLength = length;
    }

    jd.bodyA = getBody();
    jd.bodyB = other->getBody();
    jd.localAnchorA = getBody()->GetLocalVector(getBody()->GetPosition());
    jd.localAnchorB = other->getBody()->GetLocalVector(other->getBody()->GetPosition());
    jd.collideConnected = false;
    b2Joint *joint = getBody()->GetWorld()->CreateJoint(&jd);
    joints.push_back(joint);
}

void sprite2dObject::setFontSize(int size) {
    this->text->setCharacterSize(size);
}

void sprite2dObject::connectBar(sprite2dObject *other, int len) {
    b2DistanceJointDef jd;
    b2Vec2 vec(other->getBody()->GetPosition() - getBody()->GetPosition());
    b2Vec2 tmp(std::abs(vec.x), std::abs(vec.y));

    jd.length = len;
    jd.dampingRatio = 0;
    jd.frequencyHz = 0;
    jd.length = tmp.Normalize();
    jd.bodyA = getBody();
    jd.bodyB = other->getBody();
    jd.localAnchorA = getBody()->GetLocalVector(getBody()->GetPosition());
    jd.localAnchorB = other->getBody()->GetLocalVector(other->getBody()->GetPosition());
    jd.collideConnected = false;
    b2Joint *joint = getBody()->GetWorld()->CreateJoint(&jd);
    joints.push_back(joint);
}

void sprite2dObject::ignoreObject() {
    b2Fixture *fi = body->GetFixtureList();
    b2Filter fill;

    fill.categoryBits = 0;
    fill.maskBits = 0;
    fi->SetFilterData(fill);
    ignore = true;
}

bool sprite2dObject::isIgnored() {
    return ignore;
}

sf::ConvexShape sprite2dObject::getShape() {
    //Draw sprite from box2d information
    //http://en.sfml-dev.org/forums/index.php?topic=20216.0
    //http://www.iforce2d.net/b2dtut/fixtures
    //http://box2d.org/forum/viewtopic.php?t=7261
    sf::ConvexShape shapeToFill;

    for (b2Fixture *f = body->GetFixtureList(); f; f = f->GetNext()) {
        b2Shape::Type type = f->GetType();

        if (type == b2Shape::e_polygon) {
            b2PolygonShape *polyShape = (b2PolygonShape *)f->GetShape();
            shapeToFill.setFillColor(color);
            b2Vec2 vec(body->GetPosition());
            shapeToFill.setPosition(vec.x, vec.y);
            int vertCount = polyShape->GetVertexCount();
            shapeToFill.setPointCount(vertCount);

            for (int vert = 0; vert < vertCount; vert++) {
                b2Vec2 aVertex = polyShape->GetVertex(vert);
                sf::Vector2f sfVect;
                sfVect.x = aVertex.x * SCALE;
                sfVect.y = aVertex.y * SCALE;
                shapeToFill.setPoint(vert, sfVect);
            }

            shapeToFill.setRotation(180 / b2_pi * body->GetAngle());
            width = shapeToFill.getLocalBounds().width;
            height = shapeToFill.getLocalBounds().height;
        }
    }

    return shapeToFill;
}

/*
 * Getters/Setters
 */
#include <exceptions.h>
void sprite2dObject::setSprite(std::string pathname) {
    sf::Image tex;

    if (!tex.loadFromFile(pathname)) {
        filenotfoundexception ex(pathname);
        throw ex;
    }

    if (sprite != nullptr) {
        delete sprite;
    } else {
        sf::Texture t;
        t.loadFromImage(tex);
        sprite = new sf::Sprite(t);
    }
}

sf::Sprite * sprite2dObject::getSprite() {
    if (sprite != nullptr) {
        sf::ConvexShape sh(getShape());
        sprite->setPosition(sh.getPosition().x, sh.getPosition().y);
    }

    return sprite;
}

sf::Color sprite2dObject::getColor() {
    return color;
}

sf::Color sprite2dObject::getBorderColor() {
    return bordercolor;
}

void sprite2dObject::changeColor(sf::Color color) {
    this->color = color;
}

void sprite2dObject::changeBorderColor(sf::Color color) {
    bordercolor = color;
}

sf::Text * sprite2dObject::getText() {
    sf::ConvexShape sh(getShape());
    sf::FloatRect f(sh.getGlobalBounds());
    b2Vec2 center(f.left + f.width / 2, f.top + f.height / 2 - 8);
    int textwidth = text->getLocalBounds().width / 2;
    int textheight = text->getLocalBounds().height / 2;
    text->setPosition(center.x - textwidth, center.y - textheight);// 8 for buffer space
    return text;
}

void sprite2dObject::setText(std::string string, sf::Color color) {
    text->setCharacterSize(20);
    text->setFont(font);
    text->setColor(color);
    text->setString(string);
}

void sprite2dObject::setTextColor(sf::Color color) {
    text->setColor(color);
}

std::string sprite2dObject::getName() {
    return name;
}

void sprite2dObject::setName(std::string n) {
    name = n;
}

void sprite2dObject::setFriction(double f) {
    body->GetFixtureList()->SetFriction(f);
}

void sprite2dObject::setDensity(double f) {
    body->GetFixtureList()->SetDensity(f);
}

b2Vec2 sprite2dObject::getSize() {
    b2Vec2 vec(width, height);

    return vec;
}

void sprite2dObject::scaleSize(int factor) {
    width *= factor;
    height *= factor;
}

b2Body * sprite2dObject::getBody() {
    return body;
}

void sprite2dObject::mark() {
    remove = true;
}

bool sprite2dObject::marked() {
    return remove;
}
