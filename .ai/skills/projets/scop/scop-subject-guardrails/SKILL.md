---
name: scop-subject-guardrails
description: Utiliser ce skill pour tout travail sur le projet 42 scop. Il rappelle les contraintes du sujet et evite les solutions invalides, notamment les dependances interdites.
---

# Scop Subject Guardrails

## Contraintes a respecter

- bibliotheques externes autorisees uniquement pour la fenetre et les events
- aucune bibliotheque externe pour charger les OBJ
- aucune bibliotheque externe pour les matrices
- aucune bibliotheque externe pour charger les shaders

## Consequences pratiques

- pas de GLM
- pas de tinyobjloader
- pas de stb_image si on veut rester strict vis-a-vis des contraintes
- preferer des parseurs maison simples et progressifs

## Regle de decision

Si une solution semble confortable mais depend d'une lib hors fenetre/events, la refuser et proposer une alternative interne modulaire.
