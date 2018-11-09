#include "listpuzzle.h"
#include "client.h"

ListPuzzle::ListPuzzle(QSize size) : Puzzle(size) {
    establishGravity();
    establishFloor();

    instructionstream << "Create the string \"ABCDDDCBAABBCC\" with the given blocks" << std::endl;
    instructionstream << "Black blocks cannot be deleted" << std::endl;
    instructionstream << "Keys:" << std::endl;
    instructionstream << "Press X/C to move left/right" << std::endl <<
        "Press W/E to push front/back" << std::endl <<
        "Press S/D to pop front/back" << std::endl <<
        "Press Space/Delete to add/delete at selection" << std::endl <<
        "Press Q/A to cycle through values" << std::endl <<
        "Press R to reset" << std::endl <<
        "Press T to submit solution" << std::endl;
    b2Vec2 pos(150, 75);
    createInstructions(pos);

    instructionstream.str("");
    instructionstream << "The next added block will have the value:" << std::endl;
    b2Vec2 pos2(105, 230);
    createInstructions(pos2);

    possibleValues.push_back("A");
    possibleValues.push_back("B");
    possibleValues.push_back("C");
    possibleValues.push_back("D");
    nextValueIndex = 0;
    nextValue = possibleValues[nextValueIndex];
    instructionstream.str("");
    instructionstream << nextValue << std::endl;
    b2Vec2 pos3(215, 225);
    createInstructions(pos3);
    updateNextValueDisplay();

    generatePuzzleSolution();
    generatePuzzleWorkingSet();
}

void ListPuzzle::generatePuzzleSolution() {
    solutionSet.clear();

    solutionSet.push_back("A");
    solutionSet.push_back("B");
    solutionSet.push_back("C");
    solutionSet.push_back("D");
    solutionSet.push_back("D");
    solutionSet.push_back("D");
    solutionSet.push_back("C");
    solutionSet.push_back("B");
    solutionSet.push_back("A");
    solutionSet.push_back("A");
    solutionSet.push_back("B");
    solutionSet.push_back("B");
    solutionSet.push_back("C");
    solutionSet.push_back("C");
}

void ListPuzzle::generatePuzzleWorkingSet() {
    workingSet.clear();

    workingSet.push_back(std::make_pair("D", false));
    workingSet.push_back(std::make_pair("A", false));
    workingSet.push_back(std::make_pair("A", true));
    workingSet.push_back(std::make_pair("C", true));
    workingSet.push_back(std::make_pair("D", false));
    workingSet.push_back(std::make_pair("B", false));
    workingSet.push_back(std::make_pair("C", true));
    workingSet.push_back(std::make_pair("A", true));
    workingSet.push_back(std::make_pair("A", false));
    workingSet.push_back(std::make_pair("B", true));
    workingSet.push_back(std::make_pair("C", false));
    workingSet.push_back(std::make_pair("D", false));

    drawBlocks(true);
}

void ListPuzzle::reset() {
    generatePuzzleWorkingSet();
}

ListPuzzle::~ListPuzzle() {
    clearBlocks();
}

void ListPuzzle::runAction(Qt::Key key) {
    if (key == Qt::Key_W) {
        pushFront();
    } else if (key == Qt::Key_E) {
        pushBack();
    } else if (key == Qt::Key_Space) {
        addAtActiveIndex();
    } else if (key == Qt::Key_S) {
        popFront();
    } else if (key == Qt::Key_D) {
        popBack();
    } else if (key == Qt::Key_X) {
        retreatActiveIndex(true);
    } else if (key == Qt::Key_C) {
        advanceActiveIndex(true);
    } else if (key == Qt::Key_R) {
        reset();
    } else if (key == Qt::Key_Delete || key == Qt::Key_Backspace) {
        deleteAtActiveIndex();
    } else if (key == Qt::Key_T) {
        bool solved = checkSolution();

        if (solved) {
            instructionstream.str("");
            instructionstream << "Good job!!!" << std::endl;
            b2Vec2 pos4(150, 150);
            createInstructions(pos4);

            isSolved = true;
          }
    } else if (key == Qt::Key_Q) {
        cycleNextValue(false);
    } else if (key == Qt::Key_A) {
        cycleNextValue(true);
    } else if ( key == Qt::Key_U){
        this->isSolved = true;

    }
}

void ListPuzzle::pushFront() {
    workingSet.push_front(std::make_pair(nextValue, false));
    advanceActiveIndex(false);
    drawBlocks(true);
}

void ListPuzzle::pushBack() {
    workingSet.push_back(std::make_pair(nextValue, false));
    drawBlocks(true);
}

void ListPuzzle::addAtActiveIndex() {
    std::list<std::pair<std::string, bool> >::iterator iter = workingSet.begin();
    std::advance(iter, activeIndex);
    workingSet.insert(iter, std::make_pair(nextValue, false));
    drawBlocks(true);
}

void ListPuzzle::deleteAtActiveIndex() {
    if (activeIndex == 0) {
        popFront();
    } else if (activeIndex == ((int)workingSet.size()) - 1) {
        popBack();
    } else if (workingSet.size() > 0 && isDeletable(activeIndex)) {
        std::list<std::pair<std::string, bool> >::iterator iter = workingSet.begin();
        std::advance(iter, activeIndex);
        workingSet.erase(iter);
        drawBlocks(true);
    }
}

void ListPuzzle::popFront() {
    if (workingSet.size() > 0 && isDeletable(0)) {
        if (activeIndex > 0) {
            retreatActiveIndex(false);
        }

        workingSet.pop_front();
        drawBlocks(true);
    }
}

void ListPuzzle::popBack() {
    if (workingSet.size() > 0 && isDeletable(workingSet.size() - 1)) {
        if (activeIndex == ((int)workingSet.size()) - 1) {
            retreatActiveIndex(false);
        }

        workingSet.pop_back();
        drawBlocks(true);
    }
}

void ListPuzzle::highlightActiveBlock() {
    if (workingSet.size() > 0 && activeIndex < ((int)components.size())) {
        components[activeIndex]->changeColor(ActiveColor);
    }
}

bool ListPuzzle::isDeletable(int index) {
    if (index == 0) {
        return !(workingSet.front().second);
    } else if (index == ((int)workingSet.size() - 1)) {
        return !(workingSet.back().second);
    } else {
        std::list<std::pair<std::string, bool> >::iterator iter = workingSet.begin();
        std::advance(iter, index);
        return !(std::get<1>(*iter));
    }
}

void ListPuzzle::advanceActiveIndex(bool forceRedraw) {
    if (((int)components.size()) - 1 > activeIndex) {
        activeIndex++;

        if (forceRedraw) {
            drawBlocks(false);
        }
    }
}

void ListPuzzle::retreatActiveIndex(bool forceRedraw) {
    if (0 < activeIndex) {
        activeIndex--;

        if (forceRedraw) {
            drawBlocks(false);
        }
    }
}

bool ListPuzzle::checkSolution() {
    std::stringstream workingStream;
    std::stringstream solutionStream;

    for (auto iter = workingSet.begin(); iter != workingSet.end(); iter++) {
        workingStream << std::get<0>(*iter);
    }

    std::string workingString = workingStream.str();

    for (auto iter = solutionSet.begin(); iter != solutionSet.end(); iter++) {
        solutionStream << *iter;
    }

    std::string solutionString = solutionStream.str();
    return (solutionString.compare(workingString) == 0);
}

void ListPuzzle::updateNextValueDisplay() {
    inactive_components[3]->setText(nextValue, sf::Color::White);
}

void ListPuzzle::cycleNextValue(bool scrollDown) {
    if (scrollDown) {
        if (nextValueIndex == ((int)possibleValues.size()) - 1) {
            nextValueIndex = 0;
        } else {
            nextValueIndex++;
        }
    } else {
        if (nextValueIndex == 0) {
            nextValueIndex = possibleValues.size() - 1;
        } else {
            nextValueIndex--;
        }
    }

    nextValue = possibleValues[nextValueIndex];
    updateNextValueDisplay();
}

void ListPuzzle::drawBlocks(bool updateAll) {
    if (updateAll) {
        clearBlocks();

        int blockCount = workingSet.size();
        int stackingHeight = 1;

        if (blockCount >= 20) {
            stackingHeight = blockCount / 10;
        }

        int xSpawn = InitialXSpawn;
        int ySpawn = YSpawn;

        int index = 0;
        int currentStackHeight = 0;

        for (auto iter = workingSet.begin(); iter != workingSet.end(); iter++) {
            this->addComponent("block_" + index, 4, CubeSideLength, CubeSideLength, xSpawn, ySpawn, b2_staticBody, false, false, std::get<0>(*iter));

            if (std::get<1>(*iter)) {
                components[index]->changeColor(LockedColor);
            } else {
                components[index]->changeColor(DefaultColor);
            }

            currentStackHeight++;

            if (currentStackHeight == stackingHeight) {
                xSpawn += deltaX;
                ySpawn = YSpawn;
                currentStackHeight = 0;
            } else {
                ySpawn -= deltaY;
            }

            index++;
        }
    } else {
        int index = 0;

        for (auto iter = workingSet.begin(); iter != workingSet.end(); iter++) {
            components[index]->setText(std::get<0>(*iter), sf::Color::White);

            if (std::get<1>(*iter)) {
                components[index]->changeColor(LockedColor);
            } else {
                components[index]->changeColor(DefaultColor);
            }

            index++;
        }
    }

    highlightActiveBlock();
}

void ListPuzzle::clearBlocks() {
    for (int i = 0; i < ((int)components.size()); i++) {
        b2Body *bod;
        bod = components[i]->getBody();
        thisWorld->DestroyBody(bod);
    }

    components.clear();
}
