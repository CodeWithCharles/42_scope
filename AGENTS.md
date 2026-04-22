# AGENTS

## Langue et style

- Repondre en francais si l'utilisateur parle francais.
- Rester direct, concret, modulaire.

## Profil de collaboration par défaut

Par défaut sur ce repo, travailler en mode `collab-pairing-mentor`.
Ne basculer vers un mode plus autonome que si l'utilisateur le demande explicitement.

## Point d'entree du repo

Avant tout travail significatif, lire dans cet ordre :

1. `./.ai/bootstrap.md`
2. `./.ai/active_state.md`
3. `./.ai/project_context.md`
4. `./.ai/decisions.md`

## Regles de reprise

- Ne pas supposer que l'historique de conversation est disponible.
- Utiliser les fichiers `.ai/` comme source de continuite principale.
- Proposer d'abord le prochain pas concret avant un gros changement.

## Regles projet `scop`

- Bibliotheques externes autorisees uniquement pour la fenetre et les events.
- Aucune bibliotheque externe pour charger les `.obj`.
- Aucune bibliotheque externe pour les matrices.
- Aucune bibliotheque externe pour charger les shaders.
- Garder une architecture legere et incrementalement validee.

## Workflow prefere

- Pour une nouvelle abstraction : header d'abord, puis implementation, puis review.
- Pour une review : findings d'abord, resume ensuite.
- Pour les evolutions importantes : petits pas valides avec vrais fichiers de test.

## Fin de session

Si la session fait avancer le projet, mettre a jour :

- `./.ai/active_state.md`
- `./.ai/decisions.md` si une decision durable a ete prise
