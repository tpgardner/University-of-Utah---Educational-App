#include "treepuzzle.h"
#include "cmath"
#include <iostream>
#include <math.h>
#include <puzzle.h>
#include <random>
#include <sstream>
#include "client.h"

TreePuzzle::TreePuzzle(QSize size) : Puzzle(size) {
    QObject::connect(&treetime, SIGNAL(timeout()), this, SLOT(updateContact()));

    treetime.start(100);
    this->establishFloor();
    this->establishGravity(100);
    this->establishSides();
    int sides = 4;
    int l = 70;
    int h = 0;

    int boxsize = 35;

    //platforms (i.e. tree)
    this->addComponent("plat_3", sides, l, h, 190, 50, b2_staticBody);
    this->addComponent("plat_1", sides, l, h, 90, 100, b2_staticBody);
    this->addComponent("plat_5", sides, l, h, 290, 100, b2_staticBody);
    this->addComponent("plat_0", sides, l, h, 40, 150, b2_staticBody);
    this->addComponent("plat_2", sides, l, h, 140, 150, b2_staticBody);
    this->addComponent("plat_4", sides, l, h, 240, 150, b2_staticBody);
    this->addComponent("plat_6", sides, l, h, 340, 150, b2_staticBody);


    //create random values for boxes
    std::vector<int> values;
    int r = std::rand() % 50 + 1;
    values.push_back(r);

    while (values.size() < 7) {
        r = std::rand() % 50 + 1;

        for (unsigned int i = 0; i < values.size(); i++) {
            if (values.at(i) == r) {
                break;
            }

            if (i == values.size() - 1) {
                values.push_back(r);
            }
        }
    }
    std::sort(values.begin(), values.end());

    //create random array to shuffle boxes
    int randArray[] = { 0, 1, 2, 3, 4, 5, 6 };

    for (int i = 0; i < 7; i++) {
        int r = std::rand() % 7;
        int x = randArray[r];
        randArray[r] = randArray[i];
        randArray[i] = x;
    }

    //boxes (i.e. data to go into the tree)
    std::string name;

    for (int i = 0; i < 7; i++) {
        SpriteDefinition stack_p;
        stack_p.setShape(4, boxsize, boxsize);
        stack_p.setDensity(3);
        stack_p.setPosition(100 + randArray[i] * 100, 390);
        stack_p.setText(std::to_string(values[i]));
        stack_p.setType(b2_dynamicBody);
        name = "box_" + std::to_string(i);
        sprite2dObject *s = new sprite2dObject(thisWorld, stack_p);
        s->setName(name);
        addComponent(s);
    }

    for (auto it = components.begin(); it < components.end(); it++) {
        sprite2dObject *obj = *it;

        if (obj->getName().find("box") != -1) {
            boxes.push_back(obj->getName());
            sf::Color color = sf::Color::Magenta;
            obj->changeColor(color);
        } else if (obj->getName().find("plat") != -1) {
            plats.push_back(obj->getName());
            sf::Color color = sf::Color::Yellow;
            obj->changeColor(color);
        }
    }

    std::stringstream ss;
    ss << "To solve this puzzle" << std::endl <<
        "Click and drag the boxes above" << std::endl
       << "to construct a balanced binary tree." << std::endl;


    std::string text = ss.str();

    SpriteDefinition box(150, 250, b2_staticBody, "text");
    box.setShape(4, 300, 100);
    box.setBorderColor(sf::Color::White);
    sf::Color grey(84, 84, 84);
    box.setColor(grey);
    sprite2dObject *obj = new sprite2dObject(thisWorld, box);
    obj->setText(text,  sf::Color(10, 10, 10));
    obj->ignoreObject(); // nothing can interact with this
    addComponent(obj, true);
}

TreePuzzle::~TreePuzzle() {
}

void TreePuzzle::mousePressedSlot(QPointF qpoint) {
//    std::cout<<"X box_0 " << getComponent("box_0")->getBody()->GetPosition().x;
//   std::cout<<"Y box_0 " << getComponent("box_0")->getBody()->GetPosition().y << std::endl;
    int scale = 1;

    int x = (qpoint.x()) / scale;
//    std::cout << "x" << x << std::endl;

    int y = (qpoint.y()) / scale;
//    std::cout << "y" << y << std::endl;

//    this->addComponent("name", 4 ,15,15,x, y, b2_dynamicBody);

    sprite2dObject *b = getComponentAt(x, y); //reverse and accommodate offset ie +90?

//    std::cout << "BUG?" << std::endl;

    if (b != nullptr && b->getName().find("box") != -1) {
        curr = b;
        curr->bindToMouse();
    }
}

void TreePuzzle::mouseMovedSlot(QPointF qpoint) {
    int scale = 1;

    int x = (qpoint.x()) / scale;
//    std::cout << "x" << x << std::endl;

    int y = (qpoint.y()) / scale;

//    std::cout << "y" << y << std::endl;
    if (curr != nullptr) {
        curr->moveToPoint(x, y);
    }
}

void TreePuzzle::mouseReleasedSlot(QPointF) {
    if (curr != nullptr) {
        curr->unbind();
    }

    curr = nullptr;
}

void TreePuzzle::updateContact() {
    for (int i = 0; i < 7; i++) {
        std::string box = boxes[i];  //get box name
        //Determine if a box is touching any plats
        bool inContact = false;
        foreach(std::string s, plats) {
            if (getComponent(box)->inContact(getComponent(s))) {
                inContact = true;
            }
        }

        //If touching determine if names match
        if (inContact) {
            //In contact but names do not match -- initial state
            getComponent(box)->changeColor(sf::Color::Red);

            //If it is determined that names do match -- change color to green
            for (int j = 0; j < 7; j++) {
                std::string plat = plats[j];  //get plat name

                bool inContactWith = getComponent(box)->inContact(getComponent(plat));
                int boxl = box.length();
                int platl = plat.length();
                bool namesMatch = (box[boxl - 1] == plat[platl - 1]);

                if (namesMatch && inContactWith) {
                    //Incontact && names match
                    getComponent(box)->changeColor(sf::Color::Magenta);
//                    getComponent(plat)->changeColor(sf::Color::Yellow);
                    getComponent(box)->changeColor(sf::Color::Green);
//                    getComponent(plat)->changeColor(sf::Color::Green);
                    break;
                }
            }
        } else { //Box is not touching anything
                 //Not in contact
            getComponent(box)->changeColor(sf::Color::Magenta);
//            int boxl = box.length();
//            std::string plat = "plat_" + box[boxl-1];
//            getComponent(plat)->changeColor(sf::Color::Yellow);
        }
    }

    //Endgame check
    if (allGreen()) {
        endGameExplosion();
    }
}

bool TreePuzzle::allGreen() {
    bool isgreen = true;

    foreach(std::string s, boxes) {
        if (getComponent(s)->getColor() != sf::Color::Green) {
            isgreen = false;
        }
    }


    return isgreen;
}

void TreePuzzle::endGameExplosion() {
    isSolved = true;
    treetime.stop();
    treetime.start(420);
    QObject::disconnect(&treetime, SIGNAL(timeout()), this, SLOT(updateContact()));
    QObject::connect(&treetime, SIGNAL(timeout()), this, SLOT(chaos()));

    for (int i = 0; i < 240; i++) {
        addComponent("WIN", i % 10 + 2, 10, 10, rand() % 400, rand() % 200, b2_dynamicBody);
    }
}

void TreePuzzle::chaos() {
    int i = 0;

    foreach(sprite2dObject * sp, components) {
        sp->changeColor(sf::Color(rand() % 200 + 50, rand() % 200 + 50, rand() % 200 + 50, 255));
        sp->getBody()->SetType(b2_dynamicBody);

        if (i % 4 == 0) {
            sp->applyAngularForce(sprite2dObject::right, rand() % 500);
        } else if (i % 3 == 0) {
            sp->applyAngularForce(sprite2dObject::left, rand() % 500);
        } else if (i % 2 == 0) {
            sp->moveBody(sprite2dObject::up, rand() % 100);
        } else {
            sp->moveBody(sprite2dObject::down, rand() % 100);
        }

        i++;
    }
}

/*
 * TREEE PUZZLE SOLUTION OUTLINE:
 * 1. add 7 ints to puzzle and generate array
 * 2. sort that array.
 * 3. for solution compare the indices of that array against the platform indices
 *
 * add draggable boxes with ints
 * if box is in contact with the correct platform
 * set colour of plat and box to green
 * if incorrect set red
 * if box and platform are not in contact with each other set default colour
 *
 * when all platforms are green puzzle is solved
 */
