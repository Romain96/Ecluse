/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/

#include "vanne.h"

Vanne::Vanne(QObject *parent) :
    QThread(parent)
{
    // initialisation des valeurs par défaut
    position = FERME;
    etat = EN_SERVICE;
    estBloquee = false;
}

Vanne::~Vanne()
{
    // rien
}

/*
 * -----------------------------------------------------------------
 *                              SLOTS
 * -----------------------------------------------------------------
 */
void Vanne::arretUrgenceRecu()
{
    estBloquee = true;
}

void Vanne::deblocageRecu()
{
    estBloquee = false;
}

// action a effectuer sur la vanne
void Vanne::actionVanneRecue(ACTION action)
{
    switch(action)
    {
    case OUVRIR:
        if(!estBloquee && etat == EN_SERVICE)
        {
            this->position = OUVERT;
        }
        emit signalerPositionVanne(this->position);
        break;
    case FERMER:
        if(!estBloquee && etat == EN_SERVICE)
        {
            this->position = FERME;
        }
        emit signalerPositionVanne(this->position);
        break;
    default:
           qDebug("on de devrait pas être ici...");
        break;
    }
}

// changement d'état de la vanne
void Vanne::changerEtat(ETAT etat)
{
    if(etat == EN_SERVICE)
    {
        this->etat = EN_SERVICE;
        emit signalerEtatVanne(this->etat);
    }
    else if(etat == EN_PANNE)
    {
        this->etat = EN_PANNE;
        emit signalerEtatVanne(this->etat);
    }
    else
    {
        // erreur
        qDebug("la vanne n'est ni en service ni en panne :(");
    }
}
