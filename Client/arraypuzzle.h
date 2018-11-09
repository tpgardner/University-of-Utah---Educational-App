#ifndef ARRAYPUZZLE_H
#define ARRAYPUZZLE_H

#include <QString>
#include <QGraphicsScene>
#include <QWidget>
#include <QObject>
#include <puzzle.h>
#include <QSize>
#include <sprite2dobject.h>
#include <array>
#include <algorithm>
#include <sstream>
#include <iostream>
#include "calculator.cpp"
#include <QRegExp>

/**
 * @brief The ArrayPuzzle class inherits from the puzzle class. It is part of a suite of puzzles
 * for the  Stackd education game API. ArrayPuzzle simulates the functionality of an array to
 * teach students about array datastructures.
 */
class ArrayPuzzle : public Puzzle
{
public:
    ArrayPuzzle();
    ArrayPuzzle(QSize size);
    ~ArrayPuzzle();
    void virtual runAction(Qt::Key) override;
private:
    unsigned int activeIndex;
    //for the sprite text
    int randomNum;
    int equationCount;
    bool question1Done;
    bool question2Done;
    void replaceAtIndexAction();
    void addAtIndexAction();
    void createEnvironment();
    std::string getNumAtActive();
    void setupInstructions();
    void setupQuestion();
    void setupEquation(std::string);
    void clearEquation();
    void clearEntireEquation();
    void explodeAtPoint(int,int,sf::Color c = sf::Color::White);
    const static int firstAnswer = 13;
    const static int secondAnswer = 55;
    sf::Color mint = sf::Color(105,240,174);
    sf::Color purpleHaze = sf::Color(156, 39, 176);
    sf::Color hulkGreen = sf::Color(0, 200, 83);
    sf::Color ashGreen = sf::Color(0, 191, 165);
    sf::Color blueberry = sf::Color(0, 176, 255);
    int outOfBoundsCount;
};

#endif // ARRAYPUZZLE_H
