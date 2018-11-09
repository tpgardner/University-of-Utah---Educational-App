#include "arraypuzzle.h"
#include "client.h"
ArrayPuzzle::ArrayPuzzle(QSize size) : Puzzle(size) {
    createEnvironment();

    for (int i = 0; i < 5; i++) {
        this->addComponent("array_" + i, 4, 50, 25, 20 + (i * 45), -25, b2_dynamicBody);
        randomNum = rand() % 8 + 1;
        components[i]->setText(std::to_string(randomNum), sf::Color::Black);
    }

    activeIndex = 0;

    components[0]->changeColor(mint);

    setupInstructions();
    setupQuestion();

    equationCount = 0;
    outOfBoundsCount = 0;
}

ArrayPuzzle::~ArrayPuzzle() {
}

void ArrayPuzzle::runAction(Qt::Key key) {
    if (key == Qt::Key_Left) {
        if (activeIndex != 0) {
            components[activeIndex]->changeColor(sf::Color::White);
            activeIndex--;
            components[activeIndex]->changeColor(mint);
        }
    }

    if (key == Qt::Key_Right) {
        if (activeIndex < components.size() - 1) {
            components[activeIndex]->changeColor(sf::Color::White);
            activeIndex++;
            components[activeIndex]->changeColor(mint);
        }
    }

    if (key == Qt::Key_E) {
        replaceAtIndexAction();
    }

    if (key == Qt::Key_R) {
        setupEquation(getNumAtActive());
        equationCount++;
    }

    if (key == Qt::Key_Plus) {
        setupEquation("+");
        equationCount++;
    }

    if (key == Qt::Key_Minus) {
        setupEquation("-");
        equationCount++;
    }

    if (key == Qt::Key_Asterisk) {
        setupEquation("*");
        equationCount++;
    }

    if (key == Qt::Key_Slash) {
        setupEquation("/");
        equationCount++;
    }

    if (key == Qt::Key_ParenLeft) {
        setupEquation("(");
        equationCount++;
    }

    if (key == Qt::Key_ParenRight) {
        setupEquation(")");
        equationCount++;
    }

    if (key == Qt::Key_Return) {
        Calculator c;
        //std::cout << c.calculate ("(20+10)*3/2-3") << std::endl;
        std::string equation = getComponent("equation_box", false)->getText()->getString();
        QString qEquation = QString::fromStdString(equation);
        QRegExp equationRegEx("([-+]?[0-9]*\.?[0-9]+[\/\+\-\*])+([-+]?[0-9]*\.?[0-9]+)");

        if (!equationRegEx.exactMatch(qEquation)) {
            getComponent("equation_box", false)->setText("Incorrect Format");
            equationCount = getComponent("equation_box", false)->getText()->getString().getSize();
        } else {
            //std::cout << c.calculate(equation)<< std::endl;
            if (!question1Done && (equation.length() > 0)) {
                if (firstAnswer == c.calculate(equation)) {
                    question1Done = true;
                    getComponent("question_1", false)->setTextColor(sf::Color::Green);
                    clearEntireEquation();
                    setupQuestion();
                } else {
                    getComponent("question_1", false)->setTextColor(sf::Color::Red);
                    getComponent("equation_box", false)->setText("Wrong, try again!", sf::Color::Red);
                    equationCount = getComponent("equation_box", false)->getText()->getString().getSize();
                    int x = getComponent("equation_box", false)->getBody()->GetPosition().x;
                    int y = getComponent("equation_box", false)->getBody()->GetPosition().y;
                    explodeAtPoint(x, y, sf::Color::Red);
                }
            } else if (!question2Done && (equation.length() > 0)) {
                if (secondAnswer == c.calculate(equation)) {
                    question2Done = true;
                    getComponent("question_2", false)->setTextColor(sf::Color::Green);
                    getComponent("equation_box", false)->setText("Great Job!", sf::Color::Green);
                    equationCount = getComponent("equation_box", false)->getText()->getString().getSize();
                    int x = getComponent("equation_box", false)->getBody()->GetPosition().x;
                    int y = getComponent("equation_box", false)->getBody()->GetPosition().y;
                    explodeAtPoint(x, y, hulkGreen);
                    this->isSolved = true;
                } else {
                    getComponent("question_2", false)->setTextColor(sf::Color::Red);
                    getComponent("equation_box", false)->setText("Wrong, try again!", sf::Color::Red);
                    equationCount = getComponent("equation_box", false)->getText()->getString().getSize();
                    int x = getComponent("equation_box", false)->getBody()->GetPosition().x;
                    int y = getComponent("equation_box", false)->getBody()->GetPosition().y;
                    explodeAtPoint(x, y, sf::Color::Red);
                }
            }
        }
    }

    if (key == Qt::Key_Backspace) {
        //std::cout<<"backspace" <<std::endl;
        if (equationCount != 0) {
            clearEquation();
            equationCount--;
        }
    }
    if ( key == Qt::Key_U){
           this->isSolved = true;
           this->explodeAtPoint(50,50,sf::Color::Green);
           this->explodeAtPoint(150,150,sf::Color::Green);
           this->explodeAtPoint(200,200,sf::Color::Green);
    }
}

void ArrayPuzzle::replaceAtIndexAction() {
    randomNum = rand() % 8 + 1;
    components[activeIndex]->setText(std::to_string(randomNum), sf::Color::Black);
}

void ArrayPuzzle::createEnvironment() {
    b2Vec2 grav(0, 0.98);

    thisWorld->SetGravity(grav);

    SpriteDefinition floordef(100, 200, b2_staticBody, "testbox");
    floordef.setShape(4, 1500, 0);
    floordef.setColor(purpleHaze);

    SpriteDefinition leftwalld(0, 20, b2_staticBody, "leftwalld");
    leftwalld.setShape(4, 0, 500);
    leftwalld.setColor(purpleHaze);
    addComponent(leftwalld, true);

    sprite2dObject *floor = new sprite2dObject(thisWorld, floordef);
    inactive_components.push_back(floor);

    //just set this up once. Before it was creating a text over the other one
    b2Vec2 pos_(300, 450);
    std::string equation = "equation_box";
    SpriteDefinition box;
    box.name = equation;
    box.setShape(4, 0, 0);
    box.setType(b2_staticBody);
    box.setBorderColor(sf::Color::Transparent);
    box.setColor(sf::Color::Transparent);
    box.setPosition(pos_.x, pos_.y);
    sprite2dObject *obj = new sprite2dObject(thisWorld, box);
    obj->setText("", sf::Color::White);
    obj->ignoreObject();
    addComponent(obj, true);
}

std::string ArrayPuzzle::getNumAtActive() {
    std::string num;
    num = components[activeIndex]->getText()->getString();
    return num;
}

void ArrayPuzzle::setupInstructions() {
    instructionstream << "Welcome to the Array Puzzle, hip hip Array!" << std::endl;
    instructionstream << "Keys: Left and Right: change selected index." << std::endl <<
        "      E: replace data with new random number. " << std::endl <<
        "      R: Add selected data to equation." << std::endl <<
        "      (,),+,-,*,/: For selected operator." << std::endl <<
        "      Backspace: Deletes last input." << std::endl <<
        "      Enter/Return: Perform arithmetic!" << std::endl <<
        "                                       " << std::endl <<
        "      Goal: Create an expression that results" << std::endl <<
        "            in the numbers below." << std::endl <<
        "                  Good luck!" << std::endl;

    b2Vec2 pos(270, 80);
    createInstructions(pos);
}

void ArrayPuzzle::setupQuestion() {
    instructionstream.str("");
    b2Vec2 pos2(100, 450);

    if (!question1Done) {
        if (getComponent("question_1", false) == nullptr) {
            createTextBox(pos2.x, pos2.y, "Get result : 13", "question_1");
        }
    } else if (!question2Done) {
        pos2.y += 30;

        if (getComponent("question_2", false) == nullptr) {
            createTextBox(pos2.x, pos2.y, "Get result: 55", "question_2");
        }
    }

    //    if(!question1Done){
    //        instructionstream<<"Get result: 13" <<std::endl;
    //        createInstructions(pos2);
    //    }
    //    else if(!question2Done){
    //        instructionstream<<"Get result: 55" <<std::endl;
    //        b2Vec2 pos2(50, 250);
    //        createInstructions(pos2);
    //    }
}

void ArrayPuzzle::setupEquation(std::string s) {
    sprite2dObject *obj = getComponent("equation_box", false);

    if (obj != nullptr) {
        std::string nextString = obj->getText()->getString().toAnsiString();
        nextString.append(s);
        obj->setText(nextString, sf::Color::White);
    }
}

void ArrayPuzzle::clearEquation() {
    sprite2dObject *obj = getComponent("equation_box", false);

    if (obj != nullptr) {
        std::string newString = obj->getText()->getString().toAnsiString();
        newString.pop_back();
        obj->setText(newString, sf::Color::White);
    }
}

void ArrayPuzzle::clearEntireEquation() {
    while (equationCount != 0) {
        clearEquation();
        equationCount--;
    }
}

void ArrayPuzzle::explodeAtPoint(int x, int y, sf::Color color) {
    unsigned int iterations = 20;

    for (int i = 0; i < iterations; i++) {
        SpriteDefinition tri;
        tri.name = "tri";
        tri.setShape(3, 0, 0);
        tri.setText("");
        tri.setColor(color);  // then rotate around with the angle
        tri.setType(b2_dynamicBody);
        tri.setPosition(x, y);
        int magnitude = 10;
        double angle = (360 / iterations) * i;
        double radians = angle * M_PI / 180.0;
        b2Vec2 v(magnitude * std::cos(radians), magnitude * std::sin(radians));
        tri.setInitialVelocity(radians, v);
        sprite2dObject *triobj = new sprite2dObject(thisWorld, tri);
        triobj->ignoreObject();
        addComponent(triobj, true);
    }
}
