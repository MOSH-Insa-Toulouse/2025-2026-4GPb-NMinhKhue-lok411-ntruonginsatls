# Projet: Capteur Graphite à Crayon
MOSH projet crée par les étudiant.e.s en 4eme année de Génie Physique: 
**NGUYEN Minh Khue, VO Phuc Loc, TRUONG Nguyen Gia Khanh**

# Tableau de matière
- [Introduction]_(#introduction)
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

### Code Arduino

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



