/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/
#include "porte.h"
#include <QThread>
#include "ecluse.h"

Porte::Porte(QObject *parent) :
    QThread(parent)
{
    // initialisation des valeurs par défaut
    timer = new QTimer();
    position = FERME;
    etat = EN_SERVICE;
    estBloquee = false;
}

Porte::~Porte()
{
    delete timer;
}

POSITION Porte::getPosition()
{
    return this->position;
}

/*
 * -----------------------------------------------------------------
 *                              SLOTS
 * -----------------------------------------------------------------
 */
void Porte::arretUrgenceRecu()
{
    if(timer->isActive())
    {
        timer->stop();
        position = STOPPE;
        emit diffuserPosition();
    }
    estBloquee = true;
}

void Porte::deblocageRecu()
{
    estBloquee = false;
}

// action a effectuer sur la porte
void Porte::actionPorteRecue(ACTION action)
{
    switch(action)
    {
    case OUVRIR:
        // si la porte n'est pas ouverte on ordonne son ouverture
        if(!estBloquee && etat == EN_SERVICE)
        {
            if(position != OUVERT)
            {
                QObject::connect(timer, SIGNAL(timeout()), this, SLOT(porteOuverte()));
                this->position = EN_OUVERTURE;
                emit diffuserPosition();
                timer->start(10000);    // 10s
            }
        }
        break;
    case FERMER:
        // si la porte n'est pas fermée on ordonne sa fermeture
        if(!estBloquee && etat == EN_SERVICE)
        {
            if(position != FERME)
            {
                QObject::connect(timer, SIGNAL(timeout()), this, SLOT(porteFermee()));
                this->position = EN_FERMETURE;
                emit diffuserPosition();
                timer->start(10000);    // 10s
            }
        }
        break;
    case STOPPER:
        // si la porte était en action, on stoppe l'action
        if(!estBloquee && etat == EN_SERVICE)
        {
            if(timer->isActive())
            {
                timer->stop();
                this->position = STOPPE;
                emit diffuserPosition();
            }
        }
        break;
    default:
           qDebug("on de devrait pas être ici...");
        break;
    }
}

// changement d'état de la porte
void Porte::changerEtat(ETAT etat)
{
    if(etat == EN_SERVICE)
    {
        this->etat = EN_SERVICE;
        emit signalerEtatPorte(this->etat);
    }
    else if(etat == EN_PANNE)
    {
        // interruption de l'action en cours si nécessaire
        if(timer->isActive())
        {
            timer->stop();
            this->etat = EN_PANNE;
            emit diffuserPosition();
            emit signalerEtatPorte(this->etat);
        }
    }
    else
    {
        // erreur
        qDebug("la porte n'est ni en service ni en panne :(");
    }
}

// diffuse sa position à Ecluse
void Porte::diffuserPosition()
{
    emit signalerPositionPorte(this->position);
}

// positionne la variable position
void Porte::porteOuverte()
{
    position = OUVERT;
    emit diffuserPosition();
}

// positionne la variable position
void Porte::porteFermee()
{
    position = FERME;
    emit diffuserPosition();
}
