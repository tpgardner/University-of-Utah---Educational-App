#include "graphicsobject.h"

#include <QStyle>
#include <QDesktopWidget>

#include <SFML/Window/WindowStyle.hpp>
#include <QApplication>

//A QWidget for rending SFML sprites in QT
GraphicsObject::GraphicsObject(QWidget *Parent) :
    //  QWidget(Parent), goinitialized(false) {
    QWidget(Parent), sf::RenderWindow(), goinitialized(false) {    // Setup some states to allow direct rendering into the widget
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);




    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);
}

void GraphicsObject::showEvent(QShowEvent *) {
    if (!goinitialized) {
        //Under X11, we need to flush the commands sent to the server to ensure that
        //SFML will get an updated view of the windows
            #ifdef Q_WS_X11
        XFlush(QX11Info::display());
        #endif
        #ifdef __APPLE__
        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));
        #elif __linux__
        sf::RenderWindow::create((sf::WindowHandle)winId());
        #elif __unix__ // all unices not caught above
        sf::RenderWindow::create((sf::WindowHandle)winId());
        #elif _WIN32
        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));
        #endif
        #ifdef __APPLE__
        sf::Vector2u dimensions(1600, 1200);
//            sf::Vector2u dimensions(1600,1200);
//            sf::Vector2u dimensions(800,600);
        sf::RenderWindow::setSize(dimensions);
        #elif __linux__
        sf::Vector2u dimensions(800, 600);
        sf::RenderWindow::setSize(dimensions);
        #elif __unix__ // all unices not caught above
        sf::Vector2u dimensions(800, 600);
        sf::RenderWindow::setSize(dimensions);
        #elif _WIN32
        sf::Vector2u dimensions(800, 600);
        sf::RenderWindow::setSize(dimensions);
        //set up size?
        #endif

        sf::View view1(sf::FloatRect(0, 0, 800, 600));
        this->setView(view1);

        sf::RenderWindow::display();
        //   QWidget::setFixedSize(800, 600);
        QWidget::showNormal();

        OnInit();
        connect(&gotimer, SIGNAL(timeout()), this, SLOT(repaint()));
        gotimer.start();
        goinitialized = true;
    }
}

//Don't know what this does
QPaintEngine * GraphicsObject::paintEngine() const {
    return 0;
}

//Updates the window or something
void GraphicsObject::paintEvent(QPaintEvent *) {
    RenderWindow::clear();
    OnUpdate();
    RenderWindow::display();
}

//Will be written by derived class
void GraphicsObject::OnInit() {
    //to override
}

//Will be written by derived class
void GraphicsObject::OnUpdate() {
    //to override
}
