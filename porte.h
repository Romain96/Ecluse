/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/
#ifndef PORTE_H
#define PORTE_H

#include <QThread>
#include <QTimer>
#include "definitions.h"

class Porte : public QThread
{
    Q_OBJECT
public:
    Porte(QObject *parent = 0);
    ~Porte();
    POSITION getPosition();

signals:
    // signal envoyé à Ecluse pour signaler la position de la porte
    void signalerPositionPorte(POSITION position);
    // signal envoyé à Ecluse pour signaler l'état de la porte
    void signalerEtatPorte(ETAT etat);

public slots:
    // signaux réceptionnés pour recevoir les actions à faire sur la porte
    void arretUrgenceRecu();
    void deblocageRecu();
    void actionPorteRecue(ACTION action);
    void changerEtat(ETAT etat);
    // pour envoyer la position de la porte à Ecluse
    void diffuserPosition();
    void porteOuverte();
    void porteFermee();

private:
    QTimer *timer;              // timer pour représenter le temps d'ouverture/fermeture de la porte
    bool estBloquee;            // stoppée après appui sur l'arrêt d'urgence
    POSITION position;          // position courante de la porte
    ETAT etat;                  // état courant de la porte
};

#endif // PORTE_H
