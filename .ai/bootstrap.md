# Bootstrap Scop

## A lire d'abord

1. `./.ai/active_state.md`
2. `./.ai/project_context.md`
3. `./.ai/decisions.md`

## Profil par défaut

Profil recommandé pour ce projet : `collab-pairing-mentor`.

## Projet

`scop` est un viewer 3D OpenGL en C++ pour afficher des fichiers OBJ, gerer des materiaux MTL, appliquer des transformations maison, et avancer sans dependances externes hors fenetre/events.

## Priorite actuelle

Finir proprement la transition vers :

- `Material`
- `Model`
- `MtlLoader`
- `ObjLoader` retournant un `Model*`

et rebrancher `App` sur `Model` au lieu de l'ancien `Mesh`.

## Regles de reprise

- rester en francais
- rester concret et modulaire
- preferer petits pas valides
- verifier avec de vrais fichiers `.obj` / `.mtl`
- garder une architecture legere

## Fin de session

Mettre a jour :

- `./.ai/active_state.md`
- `./.ai/decisions.md` si une decision durable a ete prise
