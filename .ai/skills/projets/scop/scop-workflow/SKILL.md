---
name: scop-workflow
description: Utiliser ce skill pour implementer ou faire evoluer scop avec la methode de travail deja utilisee sur ce projet : petits pas valides, headers d'abord, architecture legere et priorite au parsing/rendu robuste.
---

# Scop Workflow

## Methode preferee

- nouveaux concepts : header d'abord, puis cpp
- commencer par une V1 simple et validee
- valider avec de vrais fichiers `.obj` / `.mtl`
- preferer la robustesse du pipeline avant le confort utilisateur

## Priorites types

1. parser propre
2. structures de donnees saines
3. rendu stable
4. UX / controles / confort ensuite

## Ce qu'il faut eviter

- sur-architecture precoce
- classes trop ambitieuses trop tot
- abstractions qui ne servent pas encore le sujet
