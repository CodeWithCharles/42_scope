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
- `App` charge une texture de test et l'affiche correctement sur `test_uv.obj`

## Etat reel du chantier

Le chemin critique a bien avance jusqu'a une vraie `V1 texture`.

Le rendu texture fonctionne sur un asset de test maitrise :

- `Config::MODEL_PATH` pointe actuellement sur `assets/models/test_uv.obj`
- `Config::TEXTURE_PATH` pointe actuellement sur `assets/textures/test.ppm`
- `render()` envoie pour l'instant `uTextureBlend = 1.0f`

Donc la texture est visible, mais le comportement mandatory du sujet n'est pas encore termine :

- pas encore de touche dediee pour alterner couleurs / texture
- pas encore de transition douce pilotee dans `App`
- pas encore de validation finale sur un vrai asset texture pour la defense

## Ce qu'il reste a faire

1. Ajouter dans `App` un petit etat pour la texture :
   - blend courant
   - cible couleur / texture
   - anti-rebond de touche
2. Ajouter une touche dediee dans `processInput()` pour toggler entre mode couleur et mode texture.
3. Faire evoluer `m_textureBlend` dans `update()` avec `deltaTime` au lieu d'envoyer `1.0f` en dur.
4. Envoyer la valeur courante de blend au shader dans `render()`.
5. Verifier visuellement :
   - retour aux couleurs vertex
   - passage progressif vers la texture
   - retour progressif vers les couleurs
6. Rebrancher ensuite un modele / une texture plus proches du rendu de defense.

## Blocages / points de vigilance

- Beaucoup de `.obj` importes pour test (`shuttle`, `skyscraper`, `cessna`) n'ont pas de `vt`, donc ils ne sont pas de bons candidats pour valider le pipeline texture.
- Les tokens OBJ `g` et `s` apparaissent dans certains assets reels, mais ce n'est pas bloquant pour finir le chemin critique mandatory.
- Le rendu texture a ete valide sur un asset de test synthetique ; il manque encore une validation finale sur un cas plus proche de la defense.

## Prochain pas recommande

Faire le toggle texture dans `App` :

1. ajouter `m_textureBlend`
2. ajouter un bool cible couleur / texture
3. ajouter un bool anti-rebond pour la touche
4. utiliser cette cible dans `update()` pour animer progressivement le blend
5. remplacer le `1.0f` en dur dans `render()` par la valeur courante

## Fichiers a regarder d'abord

- `include/App.hpp`
- `src/App.cpp`
- `include/Shader.hpp`
- `src/Shader.cpp`
- `include/Texture.hpp`
- `src/Texture.cpp`
- `assets/shaders/basic.vert`
- `assets/shaders/basic.frag`
- `include/Config.hpp`

## Notes de reprise

- La `V1 texture` fonctionne deja : ne pas rouvrir le chantier parser image tant que le toggle + blend mandatory ne sont pas termines.
- Si le rendu parait flou sur `test.ppm`, c'est normal avec une texture minuscule et les filtres lineaires.
- Pour debugger la texture plus lisiblement, on peut temporairement passer en `GL_NEAREST`, mais ce n'est pas le chemin critique.
