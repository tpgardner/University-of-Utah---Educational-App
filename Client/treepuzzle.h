#ifndef BINARYTREEPUZZLE_H
#define BINARYTREEPUZZLE_H
#include <QGraphicsScene>
#include <QWidget>
#include <QObject>
#include <puzzle.h>
#include <QSize>
#include <sprite2dobject.h>
#include <QTimer>

/**
 * @brief The TreePuzzle class inherits from the puzzle class. It is part of a suite of puzzles
 * for the  Stackd education game API. TreePuzzle simulates the functionality of a tree to
 * teach students about tree data structures.
 */
class TreePuzzle : public Puzzle

{
    Q_OBJECT

public:
    TreePuzzle();
    TreePuzzle(QSize size);
    void loadColors();
    ~TreePuzzle();
//    void updateContact();
    void resetColors();
    bool allGreen();

private:
    std::vector<std::string> boxes;
    std::vector<std::string> plats;
    sprite2dObject* curr;
    QTimer treetime;
    void endGameExplosion();

public slots:
    void mousePressedSlot(QPointF qpoint);
    void mouseMovedSlot(QPointF qpoint);
    void mouseReleasedSlot(QPointF qpoint);
    void updateContact();  
    void chaos();
};


#endif // BINARYTREEPUZZLE_H
