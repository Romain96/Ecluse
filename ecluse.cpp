/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/
#include "ecluse.h"
#include "ui_ecluse.h"

Ecluse::Ecluse(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ecluse)
{
    ui->setupUi(this);
    porteAmont = new Porte(this);
    porteAval = new Porte(this);
    vanneAmont = new Vanne(this);
    vanneAval = new Vanne(this);
    estAuthentifie = false;
    modeManuel = false;
    sensAvalant = true; // peu importe ici
    etape = TERMINE;   // le cycle n'a pas encore débuté
    etatSas = FERME;

    // initialisation des variables
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QTimer::singleShot(10000, this, SLOT(genererPanne()));  // générer (ou non) une panne sur une ou plusieurs portes/vannes toutes les 10s

    /*
     * -----------------------------------------------------------------
     *                          CONNECTIONS
     * -----------------------------------------------------------------
     */

    // diffusion de l'arrêt d'urgence
    QObject::connect(ui->boutonArretUrgence, SIGNAL(clicked()), this->porteAmont, SLOT(arretUrgenceRecu()));
    QObject::connect(ui->boutonArretUrgence, SIGNAL(clicked()), this->porteAval, SLOT(arretUrgenceRecu()));
    QObject::connect(ui->boutonArretUrgence, SIGNAL(clicked()), this->vanneAmont, SLOT(arretUrgenceRecu()));
    QObject::connect(ui->boutonArretUrgence, SIGNAL(clicked()), this->vanneAval, SLOT(arretUrgenceRecu()));

    // réception du signal de déblocage
    QObject::connect(ui->boutonDebloquer, SIGNAL(clicked()), this, SLOT(deblocageRecu()));
    // diffusion du signal de déblocage
    QObject::connect(this, SIGNAL(diffuserDeblocage()), this->porteAmont, SLOT(deblocageRecu()));
    QObject::connect(this, SIGNAL(diffuserDeblocage()), this->porteAval, SLOT(deblocageRecu()));
    QObject::connect(this, SIGNAL(diffuserDeblocage()), this->vanneAmont, SLOT(deblocageRecu()));
    QObject::connect(this, SIGNAL(diffuserDeblocage()), this->vanneAval, SLOT(deblocageRecu()));

    // traitement de la saisie du code d'authentification
    QObject::connect(ui->boutonAuthentification, SIGNAL(clicked()), this, SLOT(codeAuthentificationRecu()));

    // choix du mode manuel ou automatique
    QObject::connect(ui->verticalSliderModeManuelAuto, SIGNAL(valueChanged(int)), this, SLOT(modeDeFonctionnnement(int)));

    // choix du sens montant ou avalant
    QObject::connect(ui->boutonSensDeParcoursAvalant, SIGNAL(clicked()), this, SLOT(sensDeParcoursAvalantRecu()));
    QObject::connect(ui->boutonSensDeParcoursMontant, SIGNAL(clicked()), this, SLOT(sensDeParcoursMontantRecu()));

    // entrer ou sortir du sas
    QObject::connect(ui->boutonCommadesDuSasDemarrer, SIGNAL(clicked()), this, SLOT(diffuserDemarrerCycle()));
    QObject::connect(ui->boutonCommandesDuSasSuivant, SIGNAL(clicked()), this, SLOT(diffuserEtapeSuivante()));

    // traitement des actions manuelles pour les vannes de l'UI à Ecluse
    QObject::connect(ui->boutonCommandesVannesAmontOuvrir, SIGNAL(clicked()), this, SLOT(diffuserOuvrirVanneAmont()));
    QObject::connect(ui->boutonCommandesVannesAmontFermer, SIGNAL(clicked()), this, SLOT(diffuserFermerVanneAmont()));

    QObject::connect(ui->boutonCommandesVannesAvalOuvrir, SIGNAL(clicked()), this, SLOT(diffuserOuvrirVanneAval()));
    QObject::connect(ui->boutonCommandesVannesAvalFermer, SIGNAL(clicked()), this, SLOT(diffuserFermerVanneAval()));

    // traitement des actions manuelles pour les portes de l'UI à Ecluse
    QObject::connect(ui->boutonCommandesPortesAmontOuvrir, SIGNAL(clicked()), this, SLOT(diffuserOuvrirPorteAmont()));
    QObject::connect(ui->boutonCommandesPortesAmontFermer, SIGNAL(clicked()), this, SLOT(diffuserFermerPorteAmont()));
    QObject::connect(ui->boutonCommandesPortesAmontStop, SIGNAL(clicked()), this, SLOT(diffuserStopperPorteAmont()));

    QObject::connect(ui->boutonCommandesPortesAvalOuvrir, SIGNAL(clicked()), this, SLOT(diffuserOuvrirPorteAval()));
    QObject::connect(ui->boutonCommandesPortesAvalFermer, SIGNAL(clicked()), this, SLOT(diffuserFermerPorteAval()));
    QObject::connect(ui->boutonCommandesPortesAvalStop, SIGNAL(clicked()), this, SLOT(diffuserStopperPorteAval()));

    // réception des états des portes
    QObject::connect(this->porteAmont, SIGNAL(signalerEtatPorte(ETAT)), this, SLOT(etatPorteAmontRecu(ETAT)));
    QObject::connect(this->porteAval, SIGNAL(signalerEtatPorte(ETAT)), this, SLOT(etatPorteAvalRecu(ETAT)));

    // réception des états des vannes
    QObject::connect(this->vanneAmont, SIGNAL(signalerEtatVanne(ETAT)), this, SLOT(etatVanneAmontRecu(ETAT)));
    QObject::connect(this->vanneAval, SIGNAL(signalerEtatVanne(ETAT)), this, SLOT(etatVanneAvalRecu(ETAT)));

    // réception des positions des portes
    QObject::connect(this->porteAmont, SIGNAL(signalerPositionPorte(POSITION)), this, SLOT(positionPorteAmontRecue(POSITION)));
    QObject::connect(this->porteAval, SIGNAL(signalerPositionPorte(POSITION)), this, SLOT(positionPorteAvalRecue(POSITION)));

    // réception des positions des vannes
    QObject::connect(this->vanneAmont, SIGNAL(signalerPositionVanne(POSITION)), this, SLOT(positionVanneAmontRecue(POSITION)));
    QObject::connect(this->vanneAval, SIGNAL(signalerPositionVanne(POSITION)), this, SLOT(positionVanneAvalRecue(POSITION)));

    // envoi des ordres aux portes
    QObject::connect(this, SIGNAL(signalerActionPorteAmont(ACTION)), this->porteAmont, SLOT(actionPorteRecue(ACTION)));
    QObject::connect(this, SIGNAL(signalerActionPorteAval(ACTION)), this->porteAval, SLOT(actionPorteRecue(ACTION)));

    // envoi des ordres aux vannes
    QObject::connect(this, SIGNAL(signalerActionVanneAmont(ACTION)), this->vanneAmont, SLOT(actionVanneRecue(ACTION)));
    QObject::connect(this, SIGNAL(signalerActionVanneAval(ACTION)), this->vanneAval, SLOT(actionVanneRecue(ACTION)));

    // envoi des états (pannes aléatoires) aux portes/vannes
    QObject::connect(this, SIGNAL(signalerEtatPorteAmont(ETAT)), this->porteAmont, SLOT(changerEtat(ETAT)));
    QObject::connect(this, SIGNAL(signalerEtatPorteAval(ETAT)), this->porteAval, SLOT(changerEtat(ETAT)));
    QObject::connect(this, SIGNAL(signalerEtatVanneAmont(ETAT)), this->vanneAmont, SLOT(changerEtat(ETAT)));
    QObject::connect(this, SIGNAL(signalerEtatVanneAval(ETAT)), this->vanneAval, SLOT(changerEtat(ETAT)));

    // réception du changement d'état du sas
    QObject::connect(this, SIGNAL(signalerChangementEtatSas()), this, SLOT(etatSasRecu()));

    /*
     * -----------------------------------------------------------------
     *                       affichages divers
     * -----------------------------------------------------------------
     */
    ui->labelModeManuel->setStyleSheet("QLabel{color : red;}");
    ui->labelModeAutomatique->setStyleSheet("QLabel{color : green;}");
    ui->boutonArretUrgence->setStyleSheet("QPushButton{background-color : red; color : white;}");
    ui->labelEtatDuSas->setStyleSheet("QLabel{color : red;}");
    ui->labelSensDeParcoursChoisi->setStyleSheet("QLabel{color : green;}");
    ui->labelEtatPortesAmont->setStyleSheet("QLabel{color : red;}");
    ui->labelEtatPortesAval->setStyleSheet("QLabel{color : red;}");
    ui->labelEtatVannesAmont->setStyleSheet("QLabel{color : red;}");
    ui->labelEtatVannesAval->setStyleSheet("QLabel{color : red;}");
    ui->labelEtatSantePorteAmont->setStyleSheet("QLabel{color : green;}");
    ui->labelEtatSantePorteAval->setStyleSheet("QLabel{color : green;}");
    ui->labelEtatSanteVanneAmont->setStyleSheet("QLabel{color : green;}");
    ui->labelEtatSanteVanneAval->setStyleSheet("QLabel{color : green;}");
    // désactiver les commandes manuelles et activer les commandes automatiques par défaut
    cacherPanneauManuel();
    montrerPanneauAutomatique();

}

Ecluse::~Ecluse()
{
    delete ui;
    delete porteAmont;
    delete porteAval;
}

// générer un nombre aléatoire entre min et max
int Ecluse::randInt(int min, int max)
{
    return qrand() % ((max + 1) - min) + min;
}

/*
 * toutes les 10s tire au sort un nombre aléatoire pour chaque vanne/porte
 * et envoi le signal de panne en cas de panne et de bon fonctionnement le cas échéant
 */
#define PROBA_SUR_100 10
void Ecluse::genererPanne()
{
    int probaPorteAmont = randInt(0, 100);
    int probaPorteAval = randInt(0, 100);
    int probaVanneAmont = randInt(0, 100);
    int probaVanneAval = randInt(0, 100);

    if(probaPorteAmont < PROBA_SUR_100)
        emit signalerEtatPorteAmont(EN_PANNE);
    else
        emit signalerEtatPorteAmont(EN_SERVICE);

    if(probaPorteAval < PROBA_SUR_100)
        emit signalerEtatPorteAval(EN_PANNE);
    else
        emit signalerEtatPorteAval(EN_SERVICE);

    if(probaVanneAmont < PROBA_SUR_100)
        emit signalerEtatVanneAmont(EN_PANNE);
    else
        emit signalerEtatVanneAmont(EN_SERVICE);

    if(probaVanneAval < PROBA_SUR_100)
        emit signalerEtatVanneAval(EN_PANNE);
    else
        emit signalerEtatVanneAval(EN_SERVICE);

    // rappeller la fonction dans 10s
    QTimer::singleShot(10000, this, SLOT(genererPanne()));
}

/*
 * -----------------------------------------------------------------
 * -----------------------------------------------------------------
 *                             METHODES
 * -----------------------------------------------------------------
 * -----------------------------------------------------------------
 */

// cache les widgets du panneau de commandes manuelles
void Ecluse::cacherPanneauManuel()
{
    for(int i = 0; i < ui->layoutCommandesManuelles->count(); ++i)
    {
        QWidget *widget = ui->layoutCommandesManuelles->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(false);
        }
    }
}

// cache les widgets du panneau de commandes automatiques
void Ecluse::cacherPanneauAutomatique()
{
    for(int i = 0; i < ui->layoutCommandesAutomatiques->count(); ++i)
    {
        QWidget *widget = ui->layoutCommandesAutomatiques->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(false);
        }
    }
}

// montre les widgets du panneau de commandes manuelles
void Ecluse::montrerPanneauManuel()
{
    for(int i = 0; i < ui->layoutCommandesManuelles->count(); ++i)
    {
        QWidget *widget = ui->layoutCommandesManuelles->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(true);
        }
    }
}

// montre les widgets du panneau de commandes automatiques
void Ecluse::montrerPanneauAutomatique()
{
    for(int i = 0; i < ui->layoutCommandesAutomatiques->count(); ++i)
    {
        QWidget *widget = ui->layoutCommandesAutomatiques->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(true);
        }
    }
}

/*
 * -----------------------------------------------------------------
 * -----------------------------------------------------------------
 *                              SLOTS
 * -----------------------------------------------------------------
 * -----------------------------------------------------------------
 */

// traitement du signal de déblocage et redirection si l'utilisateur est authentifié
void Ecluse::deblocageRecu()
{
    if(estAuthentifie)
    {
        emit diffuserDeblocage();
    }
}

// traitement du code d'authentification
void Ecluse::codeAuthentificationRecu()
{
    if(ui->saisieAuthentification->text() == "1234")
    {
        estAuthentifie = true;
        montrerPanneauManuel();
        ui->saisieAuthentification->setText("code correct :)");
        ui->saisieAuthentification->setStyleSheet("QLineEdit{color : green;}");
    }
    else
    {
        estAuthentifie = false;
        ui->saisieAuthentification->setText("code incorrect :(");
        ui->saisieAuthentification->setStyleSheet("QLineEdit{color : red;}");
    }
}

// traitement du mode de fonctionnnement
void Ecluse::modeDeFonctionnnement(int val)
{
    if(val == 0)
    {
        // automatique
        modeManuel = false;
        estAuthentifie = false; // logout
        ui->labelModeAutomatique->setStyleSheet("QLabel{color : green;}");
        ui->labelModeManuel->setStyleSheet("QLabel{color : red;}");
        cacherPanneauManuel();
        montrerPanneauAutomatique();
    }
    else
    {
        // manuel
        modeManuel = true;
        estAuthentifie = false;
        ui->labelModeAutomatique->setStyleSheet("QLabel{color : red;}");
        ui->labelModeManuel->setStyleSheet("QLabel{color : green;}");
        cacherPanneauAutomatique();
        cacherPanneauManuel();  // en attente du mot de passe
    }
}

/*
 * -----------------------------------------------------------------
 *        réception du sens de parcours en mode automatique
 * -----------------------------------------------------------------
 */

// remarque : le choix du sens est bloqué une fois le cycle démarré (étape != TERMINE)
void Ecluse::sensDeParcoursMontantRecu()
{
    if(etape == TERMINE)
    {
        sensAvalant = false;    // montant
        ui->labelSensDeParcoursChoisi->setText("montant (aval -> amont)");
        ui->labelSensDeParcoursChoisi->setStyleSheet("QLabel{color : green;}");
    }
}

void Ecluse::sensDeParcoursAvalantRecu()
{
    if(etape == TERMINE)
    {
        sensAvalant = true;     // avalant
        ui->labelSensDeParcoursChoisi->setText("avalant (amont -> aval)");
        ui->labelSensDeParcoursChoisi->setStyleSheet("QLabel{color : green;}");
    }
}

/*
 * -----------------------------------------------------------------
 *        réception de l'ouverture/fermeture du sas
 * -----------------------------------------------------------------
 */

// démarrage du cycle
void Ecluse::diffuserDemarrerCycle()
{
    // cette action est exécutable seulement si le cycle vient de commencer (etape == TERMINE)
    // la porte/vanne à ouvrir dépend du sens choisi
    if(!modeManuel && etape == TERMINE)
    {
        etape = ENTRER;
        if(sensAvalant)
        {
            // le sens est amont -> aval donc la porte amont est à ouvrir (le reste à fermer)
            emit diffuserOuvrirPorteAmont();
            emit diffuserFermerPorteAval();
            emit diffuserFermerVanneAmont();
            emit diffuserFermerVanneAval();
        }
        else    // montant
        {
            // le sens est aval -> amont donc la porte aval est à ouvrir (le reste à fermer)
            emit diffuserOuvrirPorteAval();
            emit diffuserFermerPorteAmont();
            emit diffuserFermerVanneAval();
            emit diffuserFermerVanneAmont();
        }
    }
}

// étape suivante dans le cycle
void Ecluse::diffuserEtapeSuivante()
{
    // si le cycle est ENTRER on ferme la bonne porte
    if(etape == ENTRER)
    {
        // la porte à fermer dépend du sens choisi
        if(sensAvalant)
        {
            // le sens est amont -> aval donc la porte amont est à fermer
            emit diffuserFermerPorteAmont();
            etape = ATTENDRE;
        }
        else    // montant
        {
        // le sens est aval -> amont donc la porte aval est à fermer
            emit diffuserFermerPorteAval();
            etape = ATTENDRE;
        }
    }
    // si la cycle est ATTENDRE on ouvre la bonne porte
    // la porte à ouvrir dépend du sens choisi
    else if(etape == ATTENDRE)
    {
        if(sensAvalant)
        {
            // le sens est amont -> aval donc la porte aval est à ouvrir (le reste à fermer)
            emit diffuserOuvrirPorteAval();
            emit diffuserFermerVanneAval();
            emit diffuserFermerPorteAmont();
            emit diffuserFermerVanneAmont();
            etape = SORTIR;
        }
        else    // montant
        {
            // le sens est aval -> amont donc la porte amont est à ouvrir (le reste à fermer)
            emit diffuserOuvrirPorteAmont();
            emit diffuserFermerVanneAmont();
            emit diffuserFermerPorteAval();
            emit diffuserFermerVanneAval();
            etape = SORTIR;
        }
    }
    // si le cycle est SORTIR alors on referme toutes les portes/vannes
    else if(etape == SORTIR)
    {
        emit diffuserFermerPorteAmont();
        emit diffuserFermerPorteAval();
        emit diffuserFermerVanneAmont();
        emit diffuserFermerVanneAval();
        etape = TERMINE;    // retour au point de départ
    }
}

/*
 * -----------------------------------------------------------------
 *                    réception de l'état du sas
 * -----------------------------------------------------------------
 */

void Ecluse::etatSasRecu()
{
    if(porteAmont->getPosition() == FERME && porteAval->getPosition() == FERME)
    {
        ui->labelEtatDuSas->setText("fermé");
        ui->labelEtatDuSas->setStyleSheet("QLabel{color : red}");
    }
    else if(porteAmont->getPosition() == OUVERT || porteAval->getPosition() == OUVERT)
    {
        ui->labelEtatDuSas->setText("ouvert");
        ui->labelEtatDuSas->setStyleSheet("QLabel{color : green}");
    }
    else if(porteAmont->getPosition() == EN_OUVERTURE && (porteAval->getPosition() == FERME || porteAval->getPosition() == EN_FERMETURE))
    {
        ui->labelEtatDuSas->setText("en ouverture");
        ui->labelEtatDuSas->setStyleSheet("QLabel{color : orange}");
    }
    else if(porteAmont->getPosition() == EN_FERMETURE && (porteAval->getPosition() == FERME || porteAval->getPosition() == EN_FERMETURE))
    {
        ui->labelEtatDuSas->setText("en fermeture");
        ui->labelEtatDuSas->setStyleSheet("QLabel{color : orange}");
    }
    else if(porteAval->getPosition() == EN_OUVERTURE && (porteAmont->getPosition() == FERME || porteAmont->getPosition() == EN_FERMETURE))
    {
        ui->labelEtatDuSas->setText("en ouverture");
        ui->labelEtatDuSas->setStyleSheet("QLabel{color : orange}");
    }
    else if(porteAval->getPosition() == EN_FERMETURE && (porteAmont->getPosition() == FERME || porteAmont->getPosition() == EN_FERMETURE))
    {
        ui->labelEtatDuSas->setText("en fermeture");
        ui->labelEtatDuSas->setStyleSheet("QLabel{color : orange}");
    }
    else if(porteAval->getPosition() == STOPPE || porteAmont->getPosition() == STOPPE)
    {
        ui->labelEtatDuSas->setText("stoppé");
        ui->labelEtatDuSas->setStyleSheet("QLabel{color : red}");
    }
    else
    {
        qDebug("le sas est dans une position bien étrange...");
    }
}

/*
 * -----------------------------------------------------------------
 *        réception d'un changement de position d'une porte
 * -----------------------------------------------------------------
 */

// réception d'un changement de position de la porte amont
void Ecluse::positionPorteAmontRecue(POSITION position)
{
    switch(position)
    {
    case OUVERT:
        ui->labelEtatPortesAmont->setText("ouverte");
        ui->labelEtatPortesAmont->setStyleSheet("QLabel{color : green;}");
        emit signalerChangementEtatSas();
        break;
    case FERME:
        ui->labelEtatPortesAmont->setText("fermée");
        ui->labelEtatPortesAmont->setStyleSheet("QLabel{color : red;}");
        emit signalerChangementEtatSas();
        break;
    case EN_OUVERTURE:
        ui->labelEtatPortesAmont->setText("en ouverture");
        ui->labelEtatPortesAmont->setStyleSheet("QLabel{color : orange;}");
        emit signalerChangementEtatSas();
        break;
    case EN_FERMETURE:
        ui->labelEtatPortesAmont->setText("en fermeture");
        ui->labelEtatPortesAmont->setStyleSheet("QLabel{color : orange;}");
        emit signalerChangementEtatSas();
        break;
    case STOPPE:
        ui->labelEtatPortesAmont->setText("stoppée");
        ui->labelEtatPortesAmont->setStyleSheet("QLabel{color : red;}");
        emit signalerChangementEtatSas();
        break;
    default:
        emit signalerChangementEtatSas();
        break;
    }
}

// réception d'une position de porteAval
void Ecluse::positionPorteAvalRecue(POSITION position)
{
    switch(position)
    {
    case OUVERT:
        ui->labelEtatPortesAval->setText("ouverte");
        ui->labelEtatPortesAval->setStyleSheet("QLabel{color : green;}");
        emit signalerChangementEtatSas();
        break;
    case FERME:
        ui->labelEtatPortesAval->setText("fermée");
        ui->labelEtatPortesAval->setStyleSheet("QLabel{color : red;}");
        emit signalerChangementEtatSas();
        break;
    case EN_OUVERTURE:
        ui->labelEtatPortesAval->setText("en ouverture");
        ui->labelEtatPortesAval->setStyleSheet("QLabel{color : orange;}");
        emit signalerChangementEtatSas();
        break;
    case EN_FERMETURE:
        ui->labelEtatPortesAval->setText("en fermeture");
        ui->labelEtatPortesAval->setStyleSheet("QLabel{color : orange;}");
        emit signalerChangementEtatSas();
        break;
    case STOPPE:
        ui->labelEtatPortesAval->setText("stoppée");
        ui->labelEtatPortesAval->setStyleSheet("QLabel{color : red;}");
        emit signalerChangementEtatSas();
        break;
    default:
        emit signalerChangementEtatSas();
        break;
    }
}

/*
 * -----------------------------------------------------------------
 *        réception d'un changement de position d'une vanne
 * -----------------------------------------------------------------
 */

// réception d'une position de la vanne amont
void Ecluse::positionVanneAmontRecue(POSITION position)
{
    switch(position)
    {
    case OUVERT:
        ui->labelEtatVannesAmont->setText("ouverte");
        ui->labelEtatVannesAmont->setStyleSheet("QLabel{ color : green;}");
        break;
    case FERME:
        ui->labelEtatVannesAmont->setText("fermée");
        ui->labelEtatVannesAmont->setStyleSheet("QLabel{ color : red;}");
        break;
    default:
        qDebug("vanne amont ni ouverte ni fermée :(");
        break;
    }
}

// réception d'une position de la vanne aval
void Ecluse::positionVanneAvalRecue(POSITION position)
{
    switch(position)
    {
    case OUVERT:
        ui->labelEtatVannesAval->setText("ouverte");
        ui->labelEtatVannesAval->setStyleSheet("QLabel{ color : green;}");
        break;
    case FERME:
        ui->labelEtatVannesAval->setText("fermée");
        ui->labelEtatVannesAval->setStyleSheet("QLabel{ color : red;}");
        break;
    default:
        qDebug("vanne aval ni ouverte ni fermée :(");
        break;
    }
}

/*
 * -----------------------------------------------------------------
 *               diffusion des ordres aux portes
 * -----------------------------------------------------------------
 */

void Ecluse::diffuserOuvrirPorteAmont()
{
    emit signalerActionPorteAmont(OUVRIR);
}

void Ecluse::diffuserFermerPorteAmont()
{
    emit signalerActionPorteAmont(FERMER);
}

void Ecluse::diffuserStopperPorteAmont()
{
    emit signalerActionPorteAmont(STOPPER);
}

void Ecluse::diffuserOuvrirPorteAval()
{
    emit signalerActionPorteAval(OUVRIR);
}

void Ecluse::diffuserFermerPorteAval()
{
    emit signalerActionPorteAval(FERMER);
}

void Ecluse::diffuserStopperPorteAval()
{
    emit signalerActionPorteAval(STOPPER);
}

/*
 * -----------------------------------------------------------------
 *             diffusion des ordres aux vannes
 * -----------------------------------------------------------------
 */

// ordonne à la vanne amont de s'ouvrir
void Ecluse::diffuserOuvrirVanneAmont()
{
    emit signalerActionVanneAmont(OUVRIR);
}

// ordonne à la vanne amont de se fermer
void Ecluse::diffuserFermerVanneAmont()
{
    emit signalerActionVanneAmont(FERMER);
}

// ordonne à la vanne aval de s'ouvrir
void Ecluse::diffuserOuvrirVanneAval()
{
    emit signalerActionVanneAval(OUVRIR);
}

// ordonne à la vanne aval de se fermer
void Ecluse::diffuserFermerVanneAval()
{
    emit signalerActionVanneAval(FERMER);
}

/*
 * -----------------------------------------------------------------
 *         réception des états de santé des vannes
 * -----------------------------------------------------------------
 */

// fonction de réception des états de la vanne amont
void Ecluse::etatVanneAmontRecu(ETAT etat)
{
    switch(etat)
    {
    case EN_SERVICE:
        ui->labelEtatSanteVanneAmont->setText("en service");
        ui->labelEtatSanteVanneAmont->setStyleSheet("QLabel{ color : green; }");
        break;
    case EN_PANNE:
        ui->labelEtatSanteVanneAmont->setText("en panne");
        ui->labelEtatSanteVanneAmont->setStyleSheet("QLabel{ color : red; }");
        break;
    default:
        qDebug("on ne devrait pas être ici :(");
        break;
    }
}

// fonction de réception des états de la vanne aval
void Ecluse::etatVanneAvalRecu(ETAT etat)
{
    switch(etat)
    {
    case EN_SERVICE:
        ui->labelEtatSanteVanneAval->setText("en service");
        ui->labelEtatSanteVanneAval->setStyleSheet("QLabel{ color : green; }");
        break;
    case EN_PANNE:
        ui->labelEtatSanteVanneAval->setText("en panne");
        ui->labelEtatSanteVanneAval->setStyleSheet("QLabel{ color : red; }");
        break;
    default:
        qDebug("on ne devrait pas être ici :(");
        break;
    }
}

/*
 * -----------------------------------------------------------------
 *         réception des états de santé des portes
 * -----------------------------------------------------------------
 */

// fonction de réception des états de la porte amont
void Ecluse::etatPorteAmontRecu(ETAT etat)
{
    switch(etat)
    {
    case EN_SERVICE:
            ui->labelEtatSantePorteAmont->setText("en service");
            ui->labelEtatSantePorteAmont->setStyleSheet("QLabel{ color : green; }");
        break;
    case EN_PANNE:
            ui->labelEtatSantePorteAmont->setText("en panne");
            ui->labelEtatSantePorteAmont->setStyleSheet("QLabel{ color : red; }");
    default:
        qDebug("on ne devrait pas être ici :(");
        break;
    }
}

// fonction de réception des états de la porte aval
void Ecluse::etatPorteAvalRecu(ETAT etat)
{
    switch(etat)
    {
    case EN_SERVICE:
            ui->labelEtatSantePorteAval->setText("en service");
            ui->labelEtatSantePorteAval->setStyleSheet("QLabel{ color : green; }");
        break;
    case EN_PANNE:
            ui->labelEtatSantePorteAval->setText("en panne");
            ui->labelEtatSantePorteAval->setStyleSheet("QLabel{ color : red; }");
    default:
        qDebug("on ne devrait pas être ici :(");
        break;
    }
}
