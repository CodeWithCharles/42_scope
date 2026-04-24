# Contexte Projet

## But

Construire un viewer 3D `scop` en C++ / OpenGL pour afficher des fichiers `OBJ`, gerer les materiaux `MTL`, appliquer des transformations maison, et enrichir progressivement le rendu sans dependances externes hors fenetre / events.

## Contraintes sujet

- bibliotheques externes autorisees uniquement pour la fenetre et les events
- aucune bibliotheque externe pour charger les `OBJ`
- aucune bibliotheque externe pour les matrices
- aucune bibliotheque externe pour charger les shaders

## Architecture actuelle

### Bas niveau

- `Window` : encapsulation GLFW / contexte OpenGL
- `Shader` : chargement des shaders depuis fichiers + uniforms
- `Mesh` : VAO / VBO / EBO + vertices / indices + bounds locales
- `math/Vec2`, `math/Vec3`, `math/Mat4` : couche maths maison

### Chargement / donnees

- `Vertex` : `position`, `color`, `uv`
- `Material` : `Ka`, `Kd`, `Ks`, `Ns`, `d`, `illum`, `diffuseTexturePath`
- `Model` : ensemble de `ModelPart` (`Mesh*` + nom + materiau) + bounds globales
- `MtlLoader` : parse `newmtl`, `Ka`, `Kd`, `Ks`, `Ns`, `d`, `illum`, `map_Kd`
- `ObjLoader` : charge `OBJ` et retourne un `Model*`

### Rendu / orchestration

- `Camera` : calcule `view` et `projection`
- `InputController` : porte les controles runtime et les anti-rebonds clavier
- `TextureLibrary` : porte la fallback texture, le cache texture et la selection `FallbackTexture` / `MaterialTexture`
- `ModelTransformBuilder` : porte le calcul de la matrice `model` et du `mvp`
- `App` : orchestre init, boucle principale, update, rendu global et cycle de vie de la scene

## Pipeline de rendu actuel

1. chargement du modele depuis la CLI : `./scop <model.obj> [fallback_texture.ppm]`
2. chargement optionnel de la texture fallback `PPM`
3. recentrage du modele a l'origine
4. normalisation via la plus grande dimension
5. rotation automatique independante du framerate
6. translation utilisateur
7. projection perspective
8. rendu par `ModelPart`

## Modes runtime deja en place

- source texture a 3 etats :
  - `PolygonColor`
  - `FallbackTexture`
  - `MaterialTexture`
- blend texture manuel via `F` / `G`
- rasterisation via `R` :
  - `Filled`
  - `Wireframe`
  - `Point`
- translation via `A` / `D`, `W` / `S`, `Q` / `E`

## Conventions de rendu

- convention interne unique pour l'application
- correction d'orientation par rotation d'import si necessaire
- les objets sont affiches centres a l'origine par defaut
- on ne remet pas l'objet a sa position brute du fichier dans le viewer
- `PolygonColor` est un vrai mode visuel : le rendu force `uTextureBlend = 0.0`

## Etat du parser OBJ

Support present :

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

## Etat texture

- support actuel limite a `PPM` (`P3` / `P6`)
- `map_Kd` est resolu relativement au dossier du `.mtl`
- `TextureLibrary` peut precharger les textures de materiaux du `Model`
- si une texture materiau n'est pas exploitable en `PPM`, le rendu retombe sur la fallback texture

## Ressources de test utiles

- `assets/models/test.obj`
- `assets/models/test2.obj`
- `assets/models/test_uv.obj`
- `assets/models/diamond.obj`
- `assets/models/test_neg_face.obj`
- `assets/models/human.obj`
- `assets/models/42.obj`
- `assets/models/42.mtl`
- `assets/models/teapot.obj`
- `assets/models/teapot2.obj`
- `assets/models/teapot2.mtl`
- `assets/models/Watchtower/wooden_watch_tower2.obj`

## Chantiers suivants

- `N` / `M` pour l'opacite
- `LEFT` / `RIGHT` pour `Grey` / `Color`
- `DOWN` / `UP` pour `Flat` / `Smooth`
- `I` / `O` / `P` pour la rotation par axe
- menu d'infos a l'ecran
- file picker integre

## Points de vigilance

- le pipeline texture reste `PPM only`
- `grey/color` et `flat/smooth` demanderont un vrai chantier shader + normales GPU
- beaucoup d'assets de test n'ont pas de `vt`, donc sont peu utiles pour valider le pipeline texture
- le file picker integre devra rester coherent avec les contraintes du projet
