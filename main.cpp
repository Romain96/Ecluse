/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/
#include "ecluse.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ecluse w;
    w.show();
    w.setFixedSize(w.size());
    return a.exec();
}
