#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H


#include <QTimer>
#include <QWidget>
#include <QObject>
#include <QPoint>
#include <QSize>
#include <SFML/Graphics.hpp>


#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif


//Sourced: http://www.sfml-dev.org/tutorials/1.6/graphics-qt.php
//Also: https://www.youtube.com/watch?v=D9XK1KHI0Xk
//Also: http://becomingindiedev.blogspot.com/2013/10/qt-5-and-sfml-20-integration.html
/**
 * @brief The GraphicsObject class is a wrapper class for QWidget and SFML's RenderWindow. It is
 * used in the Stackd game API for generating a game window.
 */
class GraphicsObject : public QWidget, public sf::RenderWindow
{
    Q_OBJECT

public:

    explicit GraphicsObject(QWidget* Parent = 0);

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);
    virtual void OnInit();
    virtual void OnUpdate();
   // virtual void onResize();


private:

    QTimer gotimer;
    bool   goinitialized;
};

#endif //GRAPHICSOBJECT_H
