#include "stackpuzzle.h"


StackPuzzle::StackPuzzle(QSize size) : Puzzle(size) {
    QObject::connect(&gen_timer, &QTimer::timeout, this, &StackPuzzle::onTick);
    srand(time(nullptr));
    b2Vec2 graf(0, 0.98); // normalish gravity
    thisWorld->SetGravity(graf);

    left = nullptr;
    right = nullptr;
    middle = nullptr;
    can_generate = true;

    ssize.x = 60;
    ssize.y = 20;

    buildPuzzle();
}

StackPuzzle::~StackPuzzle() {
}

void StackPuzzle::onTick() {
    if (can_generate.is_lock_free() && !can_generate) {
        can_generate = true;
        gen_timer.stop();
    }
}

void StackPuzzle::generateStackSetWithAnswer() {
    int pos = rand() % 5;
    int pos2 = pos + rand() % 5;

    std::vector<int> facts;

    for (int i = 1; i <= currentanswer; i++) {
        if (currentanswer % i == 0) {
            facts.push_back(i);
        }
    }

    for (int i = 0; i < 5; i++) {
        if (facts.size() > 1) {
            if (i == pos) {// there are at least two factors for the number, :) as long as the numbers aren't huge we will be ok
                generateStackPiece(stacklocation, 0 - i * 20, facts.back());
                facts.pop_back();
            } else if (i == pos2) {
                generateStackPiece(stacklocation, 0 - i * 20, facts.back());
                facts.pop_back();
            } else {
                generateStackPiece(stacklocation, 0 - i * 20);
            }
        } else {
            generateStackPiece(stacklocation, 0 - i * 20);
        }
    }
}

void StackPuzzle::generateNewQuestion() {
    std::stringstream ss;
    int rande = 100 * rand();
    currentanswer = std::abs(rande % 100);
    ss << currentanswer;
    question->setText(ss.str());
}

void StackPuzzle::generateQuestionValue(int x, int y) {
    SpriteDefinition question;

    question.setShape(4, 30, 20);
    question.setType(b2_staticBody);
    question.setPosition(x, y);
    question.setColor(sf::Color::Transparent);
    question.setBorderColor(sf::Color::White);
    std::stringstream ss;
    int rande = 100 * rand();
    currentanswer = std::abs(rande % 100);
    ss << currentanswer;
    question.setText(ss.str());
    sprite2dObject *obj = new sprite2dObject(thisWorld, question);
    this->question =  obj;
    obj->ignoreObject();
    obj->getText()->setColor(sf::Color::Red);
    addComponent(obj, true);
}

void StackPuzzle::popAndSend(sprite2dObject *obj) {
    if (left != nullptr && right != nullptr) return;

    if (can_generate.is_lock_free() && can_generate) {
        if (components.size() < 1) return;

        SpriteDefinition value;
        value.name = "value:" + obj->getText()->getString();
        value.setShape(4, 0, 0);
        value.setText(obj->getText()->getString());
        value.setDensity(10);
        value.setRestitution(.6);
        value.setColor(sf::Color::Transparent);
        value.setType(b2_dynamicBody);
        value.setBorderColor(sf::Color::Transparent);
        sf::ConvexShape sh(obj->getShape());
        sf::FloatRect rect(sh.getGlobalBounds());
        value.setPosition(rect.left + 120, rect.top);
        b2Vec2 v(25, 0);
        value.setInitialVelocity(0, v);
        sprite2dObject *newObj = new sprite2dObject(thisWorld, value);

        if (addToPit(newObj)) {
            obj->mark();
            obj->ignoreObject();
            addComponent(newObj, true);
        }

        can_generate = false;
        gen_timer.start(500);
    }
}

void StackPuzzle::dropOperator() {
    if (middle != nullptr) return;

    SpriteDefinition op;
    sprite2dObject *obj = operators[operatorindex];
    sf::ConvexShape sh(obj->getShape());
    sf::FloatRect rect(sh.getGlobalBounds());
    b2Vec2 v(10, 10);
    op.name = "operator:" + obj->getText()->getString();
    op.setShape(4, 0, 0);
    op.setInitialVelocity(0, v);
    op.setText(obj->getText()->getString());
    op.setColor(sf::Color::Transparent);
    op.setBorderColor(sf::Color::Transparent);
    op.setType(b2_dynamicBody);
    op.setPosition(rect.left, rect.top);
    sprite2dObject *newObj = new sprite2dObject(thisWorld, op);

    if (addToPit(newObj)) {
        addComponent(newObj, true);
    }
}

void StackPuzzle::createInstructions(b2Vec2 position, int fontsize) {
    SpriteDefinition box(position.x, position.y, b2_staticBody, "instructions_box");

    box.setShape(4, 0, 0);
    box.setBorderColor(sf::Color::Transparent);
    box.setColor(sf::Color::Transparent);
    sprite2dObject *obj = new sprite2dObject(thisWorld, box);
    obj->setText(instructionstream.str(),  sf::Color::White);
    obj->getText()->setCharacterSize(fontsize);
    obj->ignoreObject(); // nothing can interact with this
    addComponent(obj, true);
}

bool StackPuzzle::pitFull() {
    return (left != nullptr && right != nullptr && middle != nullptr);
}

void StackPuzzle::emptyPit() {
    if (middle != nullptr) {
        operatorDisplay->setText("");
        middle->ignoreObject();
        middle = nullptr;
    }

    if (left != nullptr) {
        leftDisplay->setText("");
        left->ignoreObject();
        left = nullptr;
    }

    if (right != nullptr) {
        rightDisplay->setText("");
        right->ignoreObject();
        right = nullptr;
    }
}

bool StackPuzzle::addToPit(sprite2dObject *obj) {
    std::string txt = obj->getText()->getString().toAnsiString();

    if (obj->getName().find("operator:") != std::string::npos && middle == nullptr) {
        operatorDisplay->setText(txt);
        middle = obj;
        return true;
    } else if (obj->getName().find("value:") != std::string::npos) {
        if (left == nullptr) {
            leftDisplay->setText(txt);
            left = obj;
            return true;
        } else if (right == nullptr) {
            rightDisplay->setText(txt);
            right = obj;
            return true;
        }
    }
    return false;
}

void StackPuzzle::updatePit() { // this is the animation logic
    // first check the assumptions about left/right/middle
    if (pitFull() && !waiting) {
        middle->getBody()->SetType(b2_staticBody);
        middle->setDensity(100000);
        waiting = true;
        produced = false;
    } else if (pitFull() && waiting) {
        if ((left->inContact(middle) && right->inContact(middle)) || left->inContact(right)) {
            // then explode the suckers
            int lefti = std::stoi(left->getText()->getString().toAnsiString());
            int righti = std::stoi(right->getText()->getString().toAnsiString());
            int results = 0;
            std::string compare = middle->getText()->getString().toAnsiString();

            if (compare == "*") {
                results = lefti * righti;
                //add stack value
            } else if (compare == "/") {
                if (righti != 0) results = lefti / righti;
                else { //boom
                }
            } else if (compare == "+") {
                results = lefti + righti;
            } else if (compare == "-") {
                results = lefti - righti;
            }

            sf::Color color;
            b2Vec2 pos(middle->getBody()->GetPosition());

            if (results == currentanswer) {
                this->timessolved++;

                if (timessolved == 3) {
                    isSolved = true;
                    // launch animation for completing the puzzle
                }

                middle->getBody()->SetType(b2_dynamicBody);
                emptyPit();
                waiting = false;
                generateNewQuestion();
                generateStackSetWithAnswer();
                color = (sf::Color::Green);
                explodeAtPoint(pos.x, pos.y, color);
            } else if (produced.is_lock_free() && !produced) {
                produced = true;
                waiting = false;
                middle->getBody()->SetType(b2_dynamicBody);
                emptyPit();
                //push new value onto the stack
                generateStackPiece(stacklocation, 0, results);
                color = (sf::Color::Red);
                explodeAtPoint(pos.x, pos.y, color);
            }

            waiting = false;
        } else {
            b2Vec2 leftv(left->getBody()->GetPosition());
            b2Vec2 rightv(right->getBody()->GetPosition());
            b2Vec2 middlev(middle->getBody()->GetPosition());
            left->getBody()->SetLinearVelocity(middlev - leftv);
            right->getBody()->SetLinearVelocity(middlev - rightv);
        }
    }
}

void StackPuzzle::explodeAtPoint(int x, int y, sf::Color color) {
    unsigned int iterations = 20;

    for (unsigned int i = 0; i < iterations; i++) {
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

void StackPuzzle::step(float time) {
    Puzzle::step(time);
    //then my logic
    updatePit();
}

void StackPuzzle::buildPuzzle() {
    createBoundary(0, true);
    createBoundary(390, true);
    createBoundary(270, false);
    instructionstream << "Keys:" << std::endl;
    instructionstream << "Press Q to clear all values in the pit," << std::endl <<
        "Press W to pop into the pit" << std::endl <<
        "Press E to pop and ignore the value" << std::endl <<
        "Press R to push a random value to the stack," << std::endl <<
        "Press T to swap left/right values in pit," << std::endl <<
        "Press Space to drop an operator into the pit" << std::endl <<
        "Use the Arrow Keys to Change Operator" << std::endl;
    b2Vec2 pos(275, 50);
    createInstructions(pos, 15);
    b2Vec2 pos2(275, 100);
    instructionstream.str("");
    instructionstream << "GOAL: COMBINE THE VALUES TO MAKE " << std::endl;
    generateQuestionValue(pos2.x * 2, pos2.y * 2 + 20);
    createInstructions(pos2, 20);
    setActiveOperator(operatorindex);
//make platform independent------
    createStackContainer(stacklocation);
    generateStackSetWithAnswer();
    createOperatorInterface();
}

void StackPuzzle::createOperatorInterface() {
    SpriteDefinition mul;
    SpriteDefinition div;
    SpriteDefinition plus;
    SpriteDefinition minus;

    mul.name = "mul";
    div.name = "div";
    plus.name = "plus";
    minus.name = "minus";

    mul.setText("*");
    div.setText("/");
    plus.setText("+");
    minus.setText("-");
    {
        //try to clean up the code a bit
        std::vector<SpriteDefinition *> defs;
        defs.push_back(&mul);
        defs.push_back(&div);
        defs.push_back(&plus);
        defs.push_back(&minus);

        int startingindex = 225;

        for (auto it = defs.begin(); it < defs.end(); it++) {
            int halfs = 25;
            SpriteDefinition *def = *it;
            def->setShape(4, halfs, halfs);
            def->setType(b2_staticBody);
            def->setPosition(startingindex, 50);
            def->setColor(sf::Color::Green);
            def->setBorderColor(sf::Color::White);
            sprite2dObject *obj = new sprite2dObject(thisWorld, *def);
            obj->ignoreObject();
            addComponent(obj, true);
            operators.push_back(obj);
            startingindex += halfs * 1.5;
        }
    } //don't remove :)

    //now build the displays
    SpriteDefinition leftdisp;
    SpriteDefinition opdisp;
    SpriteDefinition rightdisp;
    std::vector<SpriteDefinition *> defs;
    defs.push_back(&leftdisp);
    defs.push_back(&opdisp);
    defs.push_back(&rightdisp);
    int startingindex = 225;

    for (auto it = defs.begin(); it < defs.end(); it++) {
        int halfs = 25;
        SpriteDefinition *def = *it;
        def->setShape(4, halfs, halfs);
        def->setType(b2_staticBody);
        def->setPosition(startingindex, 100);
        def->setColor(sf::Color::Transparent);
        def->setBorderColor(sf::Color::Red);
        startingindex += halfs * 1.5;
    }

    operatorDisplay = new sprite2dObject(thisWorld, opdisp);
    operatorDisplay->ignoreObject();
    leftDisplay = new sprite2dObject(thisWorld, leftdisp);
    leftDisplay->ignoreObject();
    rightDisplay = new sprite2dObject(thisWorld, rightdisp);
    rightDisplay->ignoreObject();
    addComponent(operatorDisplay, true);
    addComponent(leftDisplay, true);
    addComponent(rightDisplay, true);
}

void StackPuzzle::createStackContainer(int x) {
    SpriteDefinition left;
    SpriteDefinition right;

    std::vector<SpriteDefinition *> defs;
    defs.push_back(&left);
    defs.push_back(&right);

    for (auto it = defs.begin(); it < defs.end(); it++) {
        SpriteDefinition *def = *it;
        def->setShape(4, 0, 800);
        def->setShape(4, 0, 800);
        def->setColor(sf::Color::Transparent);
        def->setBorderColor(sf::Color::Blue);
        def->setType(b2_staticBody);
    }

    left.setPosition(x - ssize.x, 0);
    right.setPosition(x + ssize.x, 0);
    addComponent(left, true);
    addComponent(right, true);
}

void StackPuzzle::createBoundary(int value, bool horz) {
    SpriteDefinition def;

    def.setType(b2_staticBody);
    def.name = "boundary_value" + value;
    def.color = sf::Color::Transparent;

    if (!horz) {
        def.setPosition(0, value);
        def.setShape(4, 1600, 0);
    } else {
        def.setPosition(value, 0);
        def.setShape(4, 0, 1200);
    }

    addComponent(def, true);
}

void StackPuzzle::setActiveOperator(unsigned int toset) {
    unsigned int c = 0;

    if (toset < operators.size()) {
        operatorindex = toset;

        for (auto it = operators.begin(); it < operators.end(); it++) {
            sprite2dObject *spr = *it;

            if (c == operatorindex) {
                spr->changeColor(sf::Color::Blue);
            } else {
                spr->changeColor(sf::Color::Green);
            }

            c++;
        }
    }
}

void StackPuzzle::runAction(Qt::Key key) {
    if (key == Qt::Key_E) {
        popAction();
    } else
    if (key == Qt::Key_R) {
        pushAction();
    } else
    if (key == Qt::Key_Left) {
        if (operatorindex > 0) {
            operatorindex--;
            setActiveOperator(operatorindex);
        }
    } else
    if (key == Qt::Key_Right) {
        if (operatorindex < 3) {
            operatorindex++;
            setActiveOperator(operatorindex);
        }
    } else if (key == Qt::Key_Space) {
        dropOperator();
    } else if (key == Qt::Key_W) {
        sprite2dObject *spr = components.back();
        popAndSend(spr);
    } else if (key == Qt::Key_Q) {
        emptyPit();
    } else if (key == Qt::Key_T) {
        std::string temp = rightDisplay->getText()->getString().toAnsiString();
        rightDisplay->setText(leftDisplay->getText()->getString().toAnsiString());
        leftDisplay->setText(temp);
    } else if ( key == Qt::Key_U){
        this->isSolved = true;
        this->explodeAtPoint(50,50,sf::Color::Green);
        this->explodeAtPoint(150,150,sf::Color::Green);
        this->explodeAtPoint(200,200,sf::Color::Green);
    }
}

void StackPuzzle::generateStackPiece(int x, int y, int value) {
    if (components.size() > itemlimit) return;

    SpriteDefinition stack_p;
    stack_p.setShape(4, ssize.x, ssize.y);
    stack_p.setBorderColor(sf::Color::Green);
    stack_p.setDensity(50);
    stack_p.setColor(sf::Color::Red);
    std::stringstream ss;

    if (value < 0) {
        int range = 20;
        ss << rand() % range;
    } else {
        ss << value;
    }

    stack_p.setText(ss.str());
    stack_p.setPosition(x, y);
    stack_p.setType(b2_dynamicBody);
    addComponent(stack_p);
}

sprite2dObject * StackPuzzle::createNode(int x, int y, b2BodyType type) {
    SpriteDefinition node;

    node.setType(type);
    node.setPosition(x, y);
    node.setShape(4, 0, 0);
    node.dampen(.1);
    node.setDensity(10);
    node.setColor(sf::Color::Blue);
    return new sprite2dObject(thisWorld, node);
}

void StackPuzzle::popAction() {
    if (components.size() < 1) return;

    sprite2dObject *obj = (components.back());
    obj->moveBody(sprite2dObject::up, 7);
    obj->ignoreObject();
    inactive_components.push_back(obj);
    // inactive objects will be collected by garbage collection if they are too far out from the screens view ( besides from above )
    components.pop_back();
}

void StackPuzzle::pushAction() {
    if (can_generate.is_lock_free() && can_generate) {
        generateStackPiece(stacklocation, 0);
        can_generate = false;
        gen_timer.start(1000);
    }
}
