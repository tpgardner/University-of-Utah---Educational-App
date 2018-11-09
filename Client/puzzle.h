#ifndef PUZZLE_H
#define PUZZLE_H
#include <QObject>
#include <vector>
#include <QSize>
#include <QPoint>
#include <QImage>
#include <Box2D/Box2D.h>
#include <sprite2dobject.h>
#include <iterator>
#include <string>

/**
 * @brief The Puzzle class is the parent class to Stackd game API puzzles. All puzzles
 * to be used in the Stackd game API must inherit from the Puzzle class.
 */
class Puzzle : public QObject
{
    Q_OBJECT
public:

    Puzzle();
    Puzzle(QSize size, QObject *parent = 0);
    Puzzle(QObject *parent = 0);
    virtual ~Puzzle();
    //managing components
    void addComponent(std::string name, int points, int width, int height, int x, int y, b2BodyType type, bool ignored = false, bool pushFront = false, std::string text = "");
    void replaceComponent(std::string name, int points, int width, int height, int x, int y, b2BodyType type, int ind, bool ignored = false);
    void changeComponentImage(unsigned int, sf::Image);
    void createTextBox(int,int,std::string,std::string);
    void addComponent(SpriteDefinition, bool ignored = false, bool pushFront = false);
    void addComponent(sprite2dObject*, bool ignored = false, bool pushFront = false);
    sprite2dObject* getComponent(std::string name, bool active = true);
    std::vector<sprite2dObject*> getAllComponents();
    void collectGarbage();
    std::string getInstructions();
    void createInstructions(b2Vec2 pos);
    std::stringstream* createInstructions(b2Vec2 position, std::string boxname);
    std::stringstream* resetInstructions(std::string boxnametoreset, b2Vec2 newposition);

    //phy engine step
    virtual void step(float time);
    //polymorphic stuff

    virtual void runAction(Qt::Key);
    virtual bool solved();


protected:
    std::stringstream instructionstream;

    std::vector<sprite2dObject*> components;
    std::vector<sprite2dObject*> inactive_components;
    b2World *thisWorld;
    void establishGravity();
    void establishGravity(int gravityfactor);
    void establishFloor();
    void establishSides();
    sprite2dObject * getComponentAt(int,int);
    QSize size;
    bool isSolved = false;

private:
    void garbageCollection(std::vector<sprite2dObject*>&);

public slots:
    virtual void mousePressedSlot(QPointF qpoint);
    virtual void mouseMovedSlot(QPointF qpoint);
    virtual void mouseReleasedSlot(QPointF qpoint);

};

#endif // PUZZLE_H
