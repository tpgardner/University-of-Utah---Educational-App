#ifndef LISTPUZZLE_H
#define LISTPUZZLE_H

#include <puzzle.h>
#include <list>
#include <string>
#include <sprite2dobject.h>
#include <iterator>
#include <sstream>
#include <utility>

/**
 * @brief The ListPuzzle class inherits from the Puzzle class in the Stackd game API. It
 * simulates the functionality of an array to teach students about array
 * datastructures.
 */
class ListPuzzle : public Puzzle
{
public:
    ListPuzzle(QSize size);
    ~ListPuzzle();
    void virtual runAction(Qt::Key) override;

private:
    const int CubeSideLength = 10;
    const int YSpawn = 190;
    const int InitialXSpawn = 130;
    const int deltaX = 10;
    const int deltaY = 10;
    const sf::Color DefaultColor = sf::Color::Blue;
    const sf::Color ActiveColor = sf::Color::Red;
    const sf::Color LockedColor = sf::Color::Black;

    std::list<std::pair<std::string, bool>> workingSet;
    std::list<std::string> solutionSet;
    std::vector<std::string> possibleValues;
    std::string nextValue;
    int nextValueIndex;
    int activeIndex;

    void pushFront();
    void pushBack();
    void popFront();
    void popBack();
    void addAtActiveIndex();
    void deleteAtActiveIndex();
    void reset();
    bool checkSolution();
    void drawBlocks(bool updateAll);
    void highlightActiveBlock();
    void advanceActiveIndex(bool forceRedraw);
    void retreatActiveIndex(bool forceRedraw);
    void updateNextValueDisplay();
    void cycleNextValue(bool scrollDown);
    void generatePuzzleSolution();
    void generatePuzzleWorkingSet();
    bool isDeletable(int index);
    void clearBlocks();
};

#endif // LISTPUZZLE_H
