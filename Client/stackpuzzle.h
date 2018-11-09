#ifndef STACKPUZZLE_H
#define STACKPUZZLE_H
#include <QGraphicsScene>
#include <QWidget>
#include <QObject>
#include <QSize>
#include <QTimer>
#include <atomic>
#include <stdlib.h>
#include <sstream>
#include <puzzle.h>
#include <sprite2dobject.h>
#include <Box2D/Box2D.h>
#include <time.h>

/**
 * @brief The StackPuzzle class inherits from the puzzle class. It is part of a suite of puzzles
 * for the  Stackd education game API. StackPuzzle simulates the functionality of a stack to
 * teach students about stack data structures.
 */
class StackPuzzle : public Puzzle
{

public:
    StackPuzzle();
    StackPuzzle(QSize size);
    ~StackPuzzle();
    void virtual runAction(Qt::Key action) override;
    void virtual step(float time) override;

private:

    void popAndSend(sprite2dObject *);
    void popAction();
    void dropOperator();
    void pushAction();

    bool pitFull();
    void emptyPit();
    bool addToPit(sprite2dObject*);
    void updatePit(); //update logic

    //world generation
    void buildPuzzle();
    void createStackContainer(int);
    void createBoundary(int,bool);
    void createOperatorInterface();
    void createInstructions(b2Vec2, int);
    void generateQuestionValue(int,int);
    void generateStackSetWithAnswer();
    void generateStackPiece(int,int, int value = -1);
    void generateNewQuestion();
    void setActiveOperator(unsigned int);
    void explodeAtPoint(int,int,sf::Color c = sf::Color::White);

    const unsigned int itemlimit = 15;
    const unsigned int stacklocation = 100;

    std::atomic_bool waiting;
    std::atomic_bool produced;

    QTimer gen_timer;
    std::atomic_bool can_generate;

    unsigned int operatorindex=0;
    int currentanswer = 0;
    unsigned int timessolved = 0;

    sprite2dObject * createNode(int,int, b2BodyType);
    sprite2dObject * left;
    sprite2dObject * right;
    sprite2dObject * middle;
    sprite2dObject * question;
    sprite2dObject * leftDisplay;
    sprite2dObject * rightDisplay;
    sprite2dObject * operatorDisplay;
    std::vector<sprite2dObject*> operators;
    b2Vec2 ssize;


public slots:
    void onTick();


};

#endif // STACKPUZZLE_H
