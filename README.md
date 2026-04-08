# Projet: Capteur Graphite à Crayon
MOSH projet crée par les étudiant.e.s en 4eme année de Génie Physique: 
**NGUYEN Minh Khue, VO Phuc Loc, TRUONG Nguyen Gia Khanh**

# Tableau de matière
- [Introduction](#introduction)
## Introduction

### Objectifs
Dans le cadre de l’UF « Du capteur au banc de test en open source hardware » en 4ᵉ année de Génie Physique à l’INSA Toulouse, ce projet a pour objectif de concevoir un dispositif permettant de mesurer une déformation mécanique à l’aide d’un capteur de contrainte low-tech réalisé avec du graphite déposé au crayon sur du papier.

Ce projet est basé sur le rapport scientifique "Pencil Drawn Strain Gauges and Chemiresistors on Paper", publié en 2014 par Cheng-Wei Lin*, Zhibo Zhao*, Jaemyung Kim & Jiaxing Huang. Le principe repose sur le fait que la résistance électrique du graphite varie lorsque le papier est déformé. En effet, la déformation modifie la distance entre les grains de graphite, ce qui entraîne une variation de la conductance du capteur. En mesurant cette variation de résistance, il est possible d’estimer la contrainte appliquée.

L’objectif est donc de concevoir un système de mesure basé sur ce capteur simple, puis d’évaluer ses performances en les comparant à celles d’une jauge de contrainte commerciale.

### Livrables
Plusieurs livrables sont contenus:
- **Un shield PCB connecté à une carte Arduino UNO** avec differents composants.
- Fiches techniques des composants (HC-05, Keyes KY-040, SSD 1306, LLC 1070, LTC1050, MCP41100)
- **Un code Arduino** qui gère les différents composants cités précédemments (mesures de résistance d'une jauge de contrainte faite en graphite, la communication avec un module Bluetooth, l'affichage sur l'écran OLED d'actions possibles et la gestion du potentiomètre digital permettant la calibration de notre capteur).
- **Une application Android** (grâce aux Tutoriels MIT App Inventor) permet de connecter le PCB et le code Arduino.
- La datasheet du capteur de contrainte en graphite.
- Les outils de simulation LTSpice et KiCad.

## Réalisation du projet

### Conception
(Insérer le schéma de conception ici)

### Matériels nécessaires 
Dans ce projet, nous utilisons des composants électroniques et des modules dans la salle d'instrumentation avec une carte Arduino UNO. Tous les composants sont listés ci-dessous:
- 1 Carte Arduino UNO
- 1 Module Bluetooth (HC-05)
- 1 Encoder rotatif (Keyes KY-040)
- 1 Écran OLED
- 1 Capteur en graphite fait à l'aide de la couche d'un crayon de papier
- 1 Capteur de flexion commercial (LLC 1070)
- 1 Potentiomètre numérique (MCP41100)
- 1 Amplificateur de transimpédance (LTC1050)
- 2 Supports IC 
- 1 Résistance 1kΩ, 2 résistances 10kΩ, 2 résistances 100kΩ
- 3 condensateurs 100nF, 1 condensateur 1µF

### Simulation LTSpice

### Réalisation du PCB
Afin de concevoir le circuit électronique, le logiciel en libre accès KICAD a été utilisé. L'ensemble des fichiers KiCad est disponible dans le dossier .[Shield_TP_Arduino](./Shield_TP_Arduino).

- Réalisation des symboles et des empreints 
Pour commencer notre PCB (Printed Circuit Board), il est nécessaire de créer les symboles et les empreintes des composants qui ne sont pas disponibles dans la bibliothèque de KiCad. Nous avons réalisé les symboles du module Bluetooth, de l'encodeur rotatif, de l'écran OLED, du capteur de flexion et du capteur en graphite, afin de les ajouter au schéma de connexion. Ensuite, ces symboles ont été associés à une empreinte trouvée sur des sites de libre téléchargement.
![Schéma électronique réalisé sur KiCad](/Images/SchemaPCB.png)

- Réalisation du schéma électronique
Ensuite, le schéma électronique est à construire. Dans cette partie, les différentes connexions entre composants sont définies et les pins de la carte Arduino sont attribués à chacun des composants.

- Réalisation du PCB
L'objectif ici était d'allouer un branchement sur les entrées d'une carte Arduino UNO à chaque module. Nous avons aussi pour but de faire le moins possible de via (pont traversant permettant de chavaucher un routage). Le circuit imprimé a été dessiné avec une attention particulière portée à la disposition des pistes pour minimiser les couplages parasites et faciliter le routage manuel.

### Code Arduino

Le code Arduino permet de lire les valeurs du capteur en graphite, de gérer l'affichage d'un Menu sur l'écran OLED, la communication par Bluetooth avec l'application Android et le contrôle via l'encodeur rotatif.

Le code Arduino est disponible dans le dossier .[programme_arduino](./programme_arduino).

## Résultats

## Contacts
Pour toutes les informations complémentaires: 
### Étudiants réalisés
- NGUYEN Minh Khue : mngu@insa-toulouse.fr
- VO Phuc Loc : pvo@insa-toulouse.fr
- TRUONG Nguyen Gia Khanh : ntruong@insa-toulouse.fr

### Enseignants
- Jérémie Grisolia : jeremie.grisolia@insa-toulouse.fr
- Arnauld Biganzoli : arnauld.biganzoli@insa-toulouse.fr
- Cathy Crouzet : crouzet@insa-toulouse.fr
- Benjamin Mestre : benjamin.mestre@scalian.com



