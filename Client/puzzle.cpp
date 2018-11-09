#include "puzzle.h"
#include <iostream>

Puzzle::Puzzle() {
    this->size = size;
    b2Vec2 g(0, 9.8);
    thisWorld = new b2World(g);
}

Puzzle::Puzzle(QObject *parent) : Puzzle(QSize(800, 600), parent) {
}

Puzzle::Puzzle(QSize size, QObject *parent) :  QObject(parent) {
    this->size = size;
    b2Vec2 g(0, 9.8);
    thisWorld = new b2World(g);
}

Puzzle::~Puzzle() {
    delete thisWorld;

    for (auto it = inactive_components.begin(); it < inactive_components.end(); it++) {
        sprite2dObject *obj = *it;
        delete obj;
    }

    for (auto it = components.begin(); it < components.end(); it++) {
        sprite2dObject *obj = *it;
        delete obj;
    }
}

void Puzzle::step(float time) {
    thisWorld->Step(time, 10, 10);
    //apparently there are performance issues when the last two numbers are < 10
}


sprite2dObject * Puzzle::getComponentAt(int x, int y) {
    //x and y need to be adjusted. Switch x and y, add 90 to x on Tyler's machine
    foreach(sprite2dObject * sp, components) {
        int spx = sp->getBody()->GetPosition().x;
//        std::cout << "spx "<<spx<<std::endl;
//        std::cout << "x "<<x<<std::endl;
        int spy = sp->getBody()->GetPosition().y;
//        std::cout << "spy "<<spy<<std::endl;
//        std::cout << "y "<<y<<std::endl;

        int diffx = abs(spx - x);
        int diffy = abs(spy - y);

//        std::cout << "diffx " << diffx << std::endl;
//        std::cout << "diffy " << diffy << std::endl;
        if (diffx < 25 && diffy < 25) {
//            std::cout << "object found " << std::endl;
            return sp;
        }
    } return nullptr;
}

void Puzzle::addComponent(std::string name, int points, int width, int height, int x, int y, b2BodyType type, bool ignored, bool pushFront, std::string text) {
    SpriteDefinition tempdef(x, y, type, name);

    tempdef.setShape(points, width, height); // set shape is (verticeCount, width, height ) -- if 0 the height/width will be 1.
    tempdef.setPosition(x, y);
    tempdef.setDensity(1);

    if (text.compare("") != 0) {
        tempdef.setText(text);
    }

    if (!ignored) {
        if (pushFront) {
            components.insert(components.begin(), new sprite2dObject(thisWorld, tempdef));
        } else {
            components.push_back(new sprite2dObject(thisWorld, tempdef));
        }
    } else {
        inactive_components.push_back(new sprite2dObject(thisWorld, tempdef));
    }
}

bool Puzzle::solved() {
    return isSolved;
}

void Puzzle::createInstructions(b2Vec2 position) {
    SpriteDefinition box(position.x, position.y, b2_staticBody, "instructions_box");

    box.setShape(4, 0, 0);
    box.setBorderColor(sf::Color::Transparent);
    box.setColor(sf::Color::Transparent);
    sprite2dObject *obj = new sprite2dObject(thisWorld, box);
    obj->setText(instructionstream.str(),  sf::Color::White);
    obj->ignoreObject(); // nothing can interact with this
    addComponent(obj, true);
}

//For creating a "resettable" instruction box
std::stringstream * Puzzle::createInstructions(b2Vec2 position, std::string boxname) {
    std::stringstream *stream = new std::stringstream();
    SpriteDefinition box(position.x, position.y, b2_staticBody, boxname);
    box.setShape(4, 0, 0);
    box.setBorderColor(sf::Color::Transparent);
    box.setColor(sf::Color::Transparent);
    sprite2dObject *obj = new sprite2dObject(thisWorld, box);
    obj->setText(stream->str(),  sf::Color::White);
    obj->ignoreObject(); // nothing can interact with this
    addComponent(obj, true);
    return stream;
}

std::stringstream * Puzzle::resetInstructions(std::string box2reset, b2Vec2 newposition) {
    std::stringstream *stream = new std::stringstream();
    SpriteDefinition box(newposition.x, newposition.y, b2_staticBody, box2reset); //box2reset name is reused
    box.setShape(4, 0, 0);
    box.setBorderColor(sf::Color::Transparent);
    box.setColor(sf::Color::Transparent);
    sprite2dObject *obj = new sprite2dObject(thisWorld, box);
    obj->setText(stream->str(),  sf::Color::White);
    obj->ignoreObject(); // nothing can interact with this

    for (unsigned int i = 0; i < inactive_components.size(); i++) {
        if ((inactive_components[i])->getName() == box2reset) {
            inactive_components[i] = obj;
        }
    }

    return stream;
}

std::string Puzzle::getInstructions() {
    return instructionstream.str();
}

//if you want to change by index
void Puzzle::replaceComponent(std::string name, int points, int width, int height, int x, int y, b2BodyType type, int ind, bool ignored) {
    SpriteDefinition tempdef(x, y, type, name);

    tempdef.setShape(points, width, height); // set shape is (verticeCount, width, height ) -- if 0 the height/width will be 1.

    if (!ignored) {
        components[ind] = new sprite2dObject(thisWorld, tempdef);
    } else {
        inactive_components.push_back(new sprite2dObject(thisWorld, tempdef));
    }
}

void Puzzle::addComponent(SpriteDefinition def, bool ignored, bool pushFront) {
    if (!ignored) {
        if (pushFront) {
            components.insert(components.begin(), new sprite2dObject(thisWorld, def));
        } else {
            components.push_back(new sprite2dObject(thisWorld, def));
        }
    } else {
        inactive_components.push_back(new sprite2dObject(thisWorld, def));
    }
}

void Puzzle::addComponent(sprite2dObject *obj, bool ignored, bool pushFront) {
    if (!ignored) {
        if (pushFront) {
            components.insert(components.begin(), obj);
        } else {
            components.push_back(obj);
        }
    } else {
        inactive_components.push_back(obj);
    }
}

void Puzzle::createTextBox(int x, int y, std::string text, std::string name) {
    SpriteDefinition def;

    def.setShape(4, 0, 0);
    def.setText(text);
    def.setColor(sf::Color::Transparent);
    def.setColor(sf::Color::Transparent);
    def.name = name;
    def.setPosition(x, y);
    sprite2dObject *obj = new sprite2dObject(thisWorld, def);
    obj->ignoreObject();
    addComponent(obj, true);
}

sprite2dObject * Puzzle::getComponent(std::string name, bool active) {
    if (active) {
        for (auto it = components.begin(); it < components.end(); it++) {
            sprite2dObject *obj = *it;

            if (obj->getName() == name) {
                return obj;
            }
        }
    } else {
        for (auto it = inactive_components.begin(); it < inactive_components.end(); it++) {
            sprite2dObject *obj = *it;

            if (obj->getName() == name) {
                return obj;
            }
        }
    }

    return nullptr;
}

//polymorphic stuff
void Puzzle::runAction(Qt::Key) {
}

//returns vector of all of the sprites2dobjects/sprites
//(i.e. so they can be pulled and drawn by PuzzleWindow)
std::vector<sprite2dObject *> Puzzle::getAllComponents() {
    std::vector<sprite2dObject *> vec;
    vec.insert(vec.end(), components.begin(), components.end());
    vec.insert(vec.end(), inactive_components.begin(), inactive_components.end());
    return vec;
}

void Puzzle::establishGravity() {
    b2Vec2 graf(0, 0.1); // gravity is set low here

    thisWorld->SetGravity(graf);
}

void Puzzle::establishGravity(int gravityfactor) {
    b2Vec2 graf(0, 0.1 * gravityfactor); // gravity is set low here

    thisWorld->SetGravity(graf);
}

void Puzzle::establishFloor() {
    SpriteDefinition floordef(100, 200, b2_staticBody, "floor");

    floordef.setShape(4, 1000, 0); // set shape is (verticeCount, width, height ) -- if 0 the height/width will be 1.
    sprite2dObject *floor = new sprite2dObject(thisWorld, floordef);
    inactive_components.push_back(floor);
}

void Puzzle::establishSides() {
    SpriteDefinition lsidedef(0, 0, b2_staticBody, "lside");
    SpriteDefinition rsidedef(390, 0, b2_staticBody, "rside");

    lsidedef.setShape(4, 0, 650); // set shape is (verticeCount, width, height ) -- if 0 the height/width will be 1.
    rsidedef.setShape(4, 0, 650);
    sprite2dObject *lside = new sprite2dObject(thisWorld, lsidedef);
    sprite2dObject *rside = new sprite2dObject(thisWorld, rsidedef);
    inactive_components.push_back(lside);
    inactive_components.push_back(rside);
}

void Puzzle::collectGarbage() {
    garbageCollection(components);
    garbageCollection(inactive_components);
}

void Puzzle::mousePressedSlot(QPointF) {
    //to be implemented in child class
}

void Puzzle::mouseMovedSlot(QPointF) {
    //to be implemented in child class
}

void Puzzle::mouseReleasedSlot(QPointF) {
    //to be implemented in child class
}

//here be dragons.
void Puzzle::garbageCollection(std::vector<sprite2dObject *>& objs) {
    int i = 0;

    for (auto it = objs.begin(); it < objs.end(); it++) {
        sprite2dObject *obj = *it;

        if (obj == nullptr || obj->getBody() == nullptr) {
            delete obj;
            objs.erase(objs.begin() + i); // erase if we need to
            break;
        } else if (obj->isIgnored()) {
            b2Vec2 vec(obj->getBody()->GetPosition());

            if (vec.x < -10000 || vec.x > 10000 || vec.y > 10000) { // magic numbers here << we should probably tie in some sort of size
                delete obj;
                objs.erase(objs.begin() + i); // erase if we need to
                break;
            }
        }

        if (obj->marked()) {
            delete obj;
            objs.erase(objs.begin() + i); // erase if we need to
            break;
        }

        i++;
    }
}
