#include "client.h"
#include <QApplication>
#include <qfile.h>
#include <QTextStream>
#include <SFML/Audio.hpp>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //Initializing style sheets
    QFile f(":qdarkstyle/style.qss");

    if (!f.exists()) {
        printf("Unable to set stylesheet, file not found\n");
    } else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    f.close();

    //There may be better way to get directory
    Q_INIT_RESOURCE(sprites);

    Client *w = new Client;
    w->show();

    return a.exec();
}
