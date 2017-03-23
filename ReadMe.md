# Automatisation d'une écluse (projet IHM)

## decription
Projet universitaire réalisé dans le cadre de l'UE "Interfaces homme-machine".
Ce projet comporte plusieurs objectif :
* prise en main de l'environnement Qt Creator
* prise en main du langage C++
* réalisation d'une interface graphique

L'objectif est de réaliser l'IHM d'une écluse dans le cadre de son automatisation (fictive).
L'IHM doit contenir deux modes distincts : un mode automatique (par cycle) et un mode manuel qui requiert une authentification.
Les commandes automatiques sont simples :
* un choix du sens de parcours montant ou descendant (avalant)
* un bouton de démarage de cycle
* un bouton de passage à l'étape suivante du cycle

Les commandes automatiques sont plus précises :
* une commande d'ouverture et de fermeture pour chaque vanne (deux vannes au total)
* une commade d'ouverture, de fermeture et d'arrêt pour chaque porte (deux portes au total)
* des indicateurs sur l'état de santé de chaque vanne et porte (en service ou en panne)
* des indicateurs de position de chaque vanne (ouverte ou fermée)
* des indicateurs de position de chaque porte (ouverte, fermée, en ouverture, en fermeture, stoppée)

Pour tous les modes de fonctionnements :
* un bouton d'arrêt d'urgence pour stopper tout le système
* un bouton de déblocage (seulement si l'utilisateur est authentifié)
* un sélecteur de mode manuel/automatique

## Auteurs
* Juan BALZAN
* Romain PERRIN