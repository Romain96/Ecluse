/*
* Auteurs :
* - BALZAN Juan (juan-andres.balzan-wong@etu.unistra.fr)
* - PERRIN Romain (romain.perrin@etu.unistra.fr)
*/
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// représente les différentes positions d'une porte et d'une vanne
enum POSITION { OUVERT, FERME, EN_OUVERTURE, EN_FERMETURE, STOPPE };

// représente les différents états d'une porte et d'une vanne
enum ETAT { EN_SERVICE, EN_PANNE };

// représente les actions pouvant être réalisées avec un porte et une vanne
enum ACTION { OUVRIR, FERMER, STOPPER };

// représente les différentes étapes du cycle automatique
// TERMINE = cycle pas encore commencé ou bien terminé
// ENTRER = sas en cours d'ouverture pour sortie
// SORTIR = sas en cours d'ouverture pour sortie
// ATTENDRE = bateau au milieu du sas fermé en attente de l'ordre de sortie (+ de niveau constant)
enum CYCLE { TERMINE, ENTRER, ATTENDRE, SORTIR };

#endif // DEFINITIONS_H
