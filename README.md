# Nuage

Nuage est une application basée sur Qt pour visualiser un rendu volumique de nuage en temps réel à l'aide de OpenGL/Compute Shader et Fragment Shader.

## Fonctionnalités

-Visualisation du nuage / d'une scène. 
-Paramétrable (taille de la texture des bruits, fréquence des bruits, intensitée du bruit, couleurs, nombre d'échantillons, forme du nuage (SDF), lumière ...).

## Prérequis
- **Qt** (version 5) installé.
- Un compilateur compatible C++ 

## Installation et utilisation

1. Clonez le dépôt :
   git clone https://github.com/votre-utilisateur/nuage.git
   cd nuage

2. Soit utiliser QT Creator pour run le projet ou alors :
   qmake
   make -j
   ./nuage
