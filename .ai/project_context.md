# Contexte Projet

## But

Construire un viewer 3D `scop` en C++/OpenGL pour afficher des fichiers OBJ en perspective, avec rotation, translation, couleurs par face, et a terme exploitation correcte des materiaux / textures.

## Contraintes sujet

- bibliotheques externes autorisees uniquement pour la fenetre et les events
- aucune bibliotheque externe pour charger les OBJ
- aucune bibliotheque externe pour les matrices
- aucune bibliotheque externe pour charger les shaders

## Architecture actuelle

### Bas niveau

- `Window` : encapsulation GLFW / contexte OpenGL
- `Shader` : chargement des shaders depuis fichiers + uniforms de matrices
- `Mesh` : VAO / VBO / EBO + vertices/indices + bounds locales
- `math/Vec2`, `math/Vec3`, `math/Mat4` : couche maths maison

### Chargement / donnees

- `Vertex` : `position`, `color`, `uv`
- `Material` : `Ka`, `Kd`, `Ks`, `Ns`, `d`, `illum`, `diffuseTexturePath`
- `Model` : contient plusieurs `ModelPart` (`Mesh*` + nom + nom de materiau) et des bounds globales
- `MtlLoader` : parse les fichiers `.mtl`
- `ObjLoader` : en cours de migration / deja migre vers un retour `Model*`

### Orchestration

- `App` : boucle principale, input, calcul des matrices, rendu

## Pipeline de rendu vise

1. Chargement du modele
2. Recentrage du modele a l'origine
3. Normalisation via la plus grande dimension
4. Rotation automatique independante du framerate
5. Translation utilisateur
6. Projection perspective

## Conventions de rendu

- convention interne unique pour l'application
- correction d'orientation par rotation d'import si necessaire
- les objets sont affiches centres a l'origine par defaut
- on ne remet pas l'objet a sa position brute du fichier dans le viewer

## Etat du parser OBJ

Support deja present ou quasi present :

- `v`
- `vt`
- `vn`
- indices positifs
- indices negatifs
- faces `v`
- faces `v/vt`
- faces `v//vn`
- faces `v/vt/vn`
- triangulation en fan pour les faces a plus de 3 sommets
- couleurs par face via duplication des sommets GPU
- `o`
- `usemtl`
- `mtllib`

## Etat du parser MTL

Supporte deja :

- `newmtl`
- `Ka`
- `Kd`
- `Ks`
- `Ns`
- `d`
- `illum`
- `map_Kd`

## Ressources de test utiles

- `assets/models/test.obj`
- `assets/models/test2.obj`
- `assets/models/diamond.obj`
- `assets/models/test_neg_face.obj`
- `assets/models/human.obj`
- `assets/models/42.obj`
- `assets/models/42.mtl`
- `assets/models/teapot.obj`
- `assets/models/teapot2.obj`
- `assets/models/teapot2.mtl`

## Point de vigilance actuel

Le repo est dans un etat de transition :

- `include/App.hpp` utilise deja `Model*`
- `src/App.cpp` semble encore reference l'ancien `m_mesh`

La prochaine etape doit donc recoller l'orchestration `App` avec le nouveau pipeline `Model`.
