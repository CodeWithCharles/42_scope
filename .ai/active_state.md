# Etat Actuel

## Ce qui est termine

- fenetre OpenGL, boucle d'application, shaders externes, maths maison (`Vec2`, `Vec3`, `Mat4`)
- chargement `OBJ` / `MTL` avec `Model`, `ModelPart`, `Material`, `ObjLoader`, `MtlLoader`
- recentrage + normalisation du `Model`
- rendu en perspective avec `Camera`
- rendu texture PPM (`P3` / `P6`) operationnel
- resolution de `map_Kd` relativement au dossier du `.mtl`
- flip global de `V` a l'import uniquement quand `vt` est present
- arguments CLI branches : `./scop <model.obj> [fallback_texture.ppm]`
- source texture runtime a 3 etats via `T` :
  - `PolygonColor`
  - `FallbackTexture`
  - `MaterialTexture`
- `PolygonColor` est maintenant un vrai mode visuel : `uTextureBlend = 0.0`
- blend texture manuel via `F` / `G`
- mode de rasterisation via `R` :
  - `Filled`
  - `Wireframe`
  - `Point`
- architecture allegee :
  - `InputController` porte les controles clavier
  - `TextureLibrary` porte le cache + la fallback + le choix de texture
  - `ModelTransformBuilder` porte le calcul `model` / `mvp`

## Etat reel du chantier

Le chantier texture / rendu interactif a bien avance.

`App` est beaucoup plus proche d'un role d'orchestration :

- init de la scene
- boucle principale
- appel input
- appel update
- rendu global

Les responsabilites secondaires ont commence a sortir de `App` :

- input -> `InputController`
- textures -> `TextureLibrary`
- transforms -> `ModelTransformBuilder`

Le viewer permet maintenant de lancer un modele et une texture fallback depuis la ligne de commande, puis de tester plusieurs presentations sans recompiler.

## Controles actuellement en place

- `A` / `D` : translation X
- `W` / `S` : translation Y
- `Q` / `E` : translation Z
- `T` : cycle source texture
- `F` / `G` : blend texture
- `R` : cycle `Filled` / `Wireframe` / `Point`
- `ESC` : quitter

## Ce qu'il reste a implementer

1. `N` / `M` pour l'opacite
2. `LEFT` / `RIGHT` pour `Grey` / `Color`
3. `DOWN` / `UP` pour `Flat` / `Smooth`
4. `I` / `O` / `P` pour la rotation par axe
5. menu d'infos a l'ecran :
   - aide commandes
   - mode courant
   - infos modele utiles
6. file picker integre

## Points de vigilance

- le pipeline texture reste `PPM only`
- `grey/color` et `flat/smooth` demanderont un vrai chantier shader + normales GPU
- le menu d'infos demandera une solution texte / HUD
- le file picker integre demandera un choix d'implementation :
  - navigateur interne minimal
  - ou integration systeme si acceptable
- beaucoup d'assets de test n'ont pas de `vt`, donc ils sont peu utiles pour valider les modes texture

## Prochain pas immediat recommande

Commencer par `N` / `M` pour l'opacite.

Pourquoi :

- meme famille de controle que `F` / `G`
- faible impact architectural
- prepare directement le futur shader enrichi

Apres ca, enchainer sur `grey/color` puis `flat/smooth`, car ces deux blocs imposeront ensemble la vraie evolution du pipeline shader.

## Fichiers a regarder d'abord

- `include/app/App.hpp`
- `src/app/App.cpp`
- `include/app/InputController.hpp`
- `src/app/InputController.cpp`
- `include/render/TextureLibrary.hpp`
- `src/render/TextureLibrary.cpp`
- `include/render/ModelTransformBuilder.hpp`
- `src/render/ModelTransformBuilder.cpp`
- `include/render/RenderModes.hpp`
- `include/render/RenderState.hpp`
- `include/core/Config.hpp`

## Notes de reprise

- `Config::MODEL_PATH` et `Config::TEXTURE_PATH` restent des fallbacks, mais le chemin normal passe maintenant par les arguments CLI
- le blend texture visible est controle par `RenderState.textureBlend`
- `PolygonColor` ignore visuellement ce blend en forçant `0.0` au rendu
- `TextureLibrary` peut encore charger une texture a la volee pendant le rendu si elle n'est pas deja dans le cache
