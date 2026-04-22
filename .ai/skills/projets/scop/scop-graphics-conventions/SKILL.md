---
name: scop-graphics-conventions
description: Utiliser ce skill pour toute discussion ou implementation concernant le rendu, les transformations, le chargement OBJ/MTL et les conventions internes de scop.
---

# Scop Graphics Conventions

## Conventions internes

- une convention interne stable pour l'application
- rotation d'import si un modele arrive dans un repere inattendu
- les objets sont affiches centres a l'origine par defaut
- on normalise via la plus grande dimension du modele

## Regles de transformation

- recentrage par `toOrigin`
- pas de retour a la position brute du fichier pour le viewer par defaut
- rotation autour du vrai centre global du modele
- translation utilisateur appliquee apres recentrage / scale / rotation

## Parsing OBJ/MTL

- parser maison
- support progressif mais propre
- accepter `v`, `vt`, `vn`
- accepter indices positifs et negatifs
- triangulation en fan pour faces a N sommets
- `Material`, `Model`, `MtlLoader` et `ObjLoader` ont des responsabilites separees
