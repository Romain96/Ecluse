/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/
#ifndef ECLUSE_H
#define ECLUSE_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QString>
#include "porte.h"
#include "vanne.h"

namespace Ui {
class Ecluse;
}

class Ecluse : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ecluse(QWidget *parent = 0);
    ~Ecluse();
    int randInt(int min, int max);
    void cacherPanneauManuel();
    void cacherPanneauAutomatique();
    void montrerPanneauManuel();
    void montrerPanneauAutomatique();

signals:
    // signaler les actions sur les portes
    void signalerActionPorteAmont(ACTION action);
    void signalerActionPorteAval(ACTION action);

    // signaler les actions sur les vannes
    void signalerActionVanneAmont(ACTION action);
    void signalerActionVanneAval(ACTION action);

    // vérifier un changement de l'état du sas
    void signalerChangementEtatSas();
    // génaration alétoire de pannes
    void signalerEtatPorteAmont(ETAT etat);
    void signalerEtatPorteAval(ETAT etat);
    void signalerEtatVanneAmont(ETAT etat);
    void signalerEtatVanneAval(ETAT etat);

    //  diffuser le déblocage
    void diffuserDeblocage();

public slots:
    // générer des pannes aléatoires
    void genererPanne();

    // réceptionner le signal de déblocage pour le rediriger si l'utilisateur a le droit
    void deblocageRecu();

    // réception du sens de parcours montant ou avalant
    void sensDeParcoursMontantRecu();
    void sensDeParcoursAvalantRecu();

    // réception de l'ouverture/fermeture du sas
    void diffuserDemarrerCycle();
    void diffuserEtapeSuivante();

    // réception d'un changement de position d'une porte
    void positionPorteAmontRecue(POSITION position);
    void positionPorteAvalRecue(POSITION position);

    // réception d'un changement de position d'une vanne
    void positionVanneAmontRecue(POSITION position);
    void positionVanneAvalRecue(POSITION position);

    // réception d'un changement d'état d'une porte
    void etatPorteAmontRecu(ETAT etat);
    void etatPorteAvalRecu(ETAT etat);

    // réception d'un changement d'état d'une vanne
    void etatVanneAmontRecu(ETAT etat);
    void etatVanneAvalRecu(ETAT etat);

    // réception de l'état du sas
    void etatSasRecu();

    // traitement du code d'authentification
    void codeAuthentificationRecu();

    // choix du mode de commandes
    void modeDeFonctionnnement(int val);

    // fonction de diffusion des ordres aux portes
    void diffuserOuvrirPorteAmont();
    void diffuserFermerPorteAmont();
    void diffuserStopperPorteAmont();
    void diffuserOuvrirPorteAval();
    void diffuserFermerPorteAval();
    void diffuserStopperPorteAval();

    // fonction de diffusion des ordres aux vannes
    void diffuserOuvrirVanneAmont();
    void diffuserFermerVanneAmont();
    void diffuserOuvrirVanneAval();
    void diffuserFermerVanneAval();

private:
    Ui::Ecluse *ui;
    QTime time;
    Porte *porteAmont;
    Porte *porteAval;
    Vanne *vanneAmont;
    Vanne *vanneAval;
    bool estAuthentifie;
    bool modeManuel;
    bool sensAvalant;   // faux = sens montant
    CYCLE etape;        // étape courante dans le cycle automatique
    POSITION etatSas;
};

#endif // ECLUSE_H
