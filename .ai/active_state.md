# Etat Actuel

## Ce qui marche

- creation de fenetre OpenGL
- boucle d'application propre
- chargement de shaders depuis `assets/shaders/`
- couche maths maison (`Vec2`, `Vec3`, `Mat4`)
- rendu en perspective
- rotation independante du framerate
- translation utilisateur sur X / Y / Z
- recentrage et normalisation via les bounds globales du `Model`
- migration `App -> Model` recolle et build valide
- parsing `.mtl` en V1
- `ObjLoader` applique `Material.diffuse` (`Kd`) quand un `usemtl` connu est actif, avec fallback random sinon
- test multi-materiaux rouge/vert valide
- pipeline UV valide de bout en bout avec `assets/models/test_uv.obj`
- shaders prepares pour melanger couleur vertex et texture via `uTextureBlend`
- nouvelle abstraction `Texture`
- chargement de textures PPM `P6` et `P3`
- upload OpenGL + bind texture valides
- toggle texture sur `T` avec anti-rebond
- transition douce couleur/texture pilotee par `deltaTime`
- `render()` envoie maintenant la vraie valeur de `uTextureBlend`
- `Material.diffuseTexturePath` remonte jusqu'au `Model`
- `Model` expose `findMaterialByName()` et `findFirstDiffuseTexturePath()`
- `ObjLoader` resout `map_Kd` relativement au dossier du `.mtl`
- `ObjLoader` applique maintenant un flip global de la coordonnee UV `V` a l'import, uniquement quand `vt` est present
- `Camera` porte maintenant le calcul de `view` et `projection`
- `App` rend maintenant le `Model` par `ModelPart`
- `App` garde une texture fallback et un cache de textures chargees
- rendu texture valide sur `test`, `babem` et `watchtower`

## Etat reel du chantier

Le chemin critique mandatory texture a bien avance.

Le rendu texture fonctionne sur un asset de test maitrise, et le toggle mandatory est maintenant en place :

- `Config::MODEL_PATH` pointe actuellement sur `assets/models/Watchtower/wooden_watch_tower2.obj`
- `Config::TEXTURE_PATH` pointe actuellement sur `assets/textures/test.ppm`
- `T` alterne bien entre couleurs vertex et texture avec transition douce
- le fallback sur `test.ppm` reste stable meme quand un materiau reference une texture non supportee
- le rendu passe maintenant par partie, avec choix de texture par materiau quand un `PPM` est disponible

La Watchtower a servi de validation utile :

- geometriquement le modele est bon
- les UV existent bien
- le passage texture montre bien que le pipeline est branche
- le flip `V` corrige visiblement l'orientation de texture sur cet asset
- la vraie texture diffuse du modele peut maintenant etre exploitee si on fournit un `PPM` coherent

## Ce qu'il reste a faire

1. Decider la strategie pour les textures reelles de materiaux :
   - soit convertir / fournir des textures `PPM`
   - soit ajouter un support image plus large
2. Valider un vrai couple modele + texture diffuse exploitable pour la defense.
3. Sortir le chargement de textures du chemin de rendu si on veut figer proprement le cache avant le premier frame.
4. Si besoin, enrichir encore la logique texture pour gerer plusieurs cas de materiaux plus avances.
5. Continuer le nettoyage architectural apres le refacto d'arborescence.

## Blocages / points de vigilance

- Beaucoup de `.obj` importes pour test (`shuttle`, `skyscraper`, `cessna`) n'ont pas de `vt`, donc ils ne sont pas de bons candidats pour valider le pipeline texture.
- Les tokens OBJ `g` et `s` apparaissent dans certains assets reels, mais ce n'est pas bloquant pour finir le chemin critique mandatory.
- Le loader texture actuel reste limite au `PPM` (`P3` / `P6`).
- La Watchtower reference `map_Kd textures\\t_C_3.png`, mais le repo contient surtout `textures/Wood_Tower_Col.jpg` : il y a probablement un decalage entre le `.mtl` et les assets reels.
- Le rendu par `ModelPart` est en place, mais le cache de textures vit encore dans `App`.
- Le flip `V` ne doit jamais etre applique hors du chemin ou `textureIndex >= 0`, sinon les modeles sans `vt` comme `cessna.obj` plantent.

## Prochain pas recommande

Choisir et valider un vrai chemin de texture de defense :

1. trouver un modele avec `vt` et une texture diffuse exploitable en `PPM`
2. ou convertir une texture de test vers `PPM` si on veut rester strictement dans la voie actuelle
3. verifier ensuite visuellement le rendu sans fallback artificiel
4. seulement apres, decider s'il faut vraiment ajouter le support `jpg/png`

## Fichiers a regarder d'abord

- `include/app/App.hpp`
- `src/app/App.cpp`
- `include/render/Camera.hpp`
- `src/render/Camera.cpp`
- `include/scene/Model.hpp`
- `src/scene/Model.cpp`
- `include/io/ObjLoader.hpp`
- `src/io/ObjLoader.cpp`
- `include/render/Texture.hpp`
- `src/render/Texture.cpp`
- `include/core/Config.hpp`

## Notes de reprise

- Le toggle + blend mandatory sont termines et valides visuellement.
- La convention UV actuellement validee est : flip global de `V` a l'import, uniquement pour les sommets qui ont des coordonnees de texture.
- L'arborescence a ete reorganisee en sous-dossiers (`app`, `core`, `io`, `render`, `scene`, `system`, `math`).
- Si le rendu parait flou sur `test.ppm`, c'est normal avec une texture minuscule et les filtres lineaires.
- Pour debugger la texture plus lisiblement, on peut temporairement passer en `GL_NEAREST`, mais ce n'est pas le chemin critique.
- Tant que le projet reste en `PPM only`, `App` garde une texture fallback simple sur `Config::TEXTURE_PATH` pour ne pas casser les assets qui referencent du `jpg/png`.
