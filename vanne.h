/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/
#ifndef VANNE_H
#define VANNE_H

#include <QThread>
#include "definitions.h"

class Vanne : public QThread
{
    Q_OBJECT

public:
    Vanne(QObject *parent = 0);
    ~Vanne();

signals:
    // signal envoyé à Ecluse pour signaler la position de la vanne
    void signalerPositionVanne(POSITION position);
    // signal envoyé à Ecluse pour signaler l'état de la vanne
    void signalerEtatVanne(ETAT etat);

public slots:
    // signaux réceptionnés pour recevoir les actions à faire sur la vanne
    void arretUrgenceRecu();
    void deblocageRecu();
    void actionVanneRecue(ACTION action);
    void changerEtat(ETAT etat);

private:
    POSITION position;
    ETAT etat;
    bool estBloquee;

};

#endif // VANNE_H
