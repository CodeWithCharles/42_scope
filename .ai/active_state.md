# Etat Actuel

## Ce qui marche

- creation de fenetre OpenGL
- boucle d'application propre
- chargement de shaders depuis `assets/shaders/`
- couche maths maison (`Vec2`, `Vec3`, `Mat4`)
- rendu en perspective
- rotation independante du framerate
- translation utilisateur sur X / Y / Z
- recentrage et normalisation des meshes / modeles
- rotation autour du vrai centre
- chargement de nombreux `.obj` reels
- parsing `.mtl` en V1

## Etat reel du chantier

La pile `Material` / `Model` / `MtlLoader` / `ObjLoader` a bien avance.

Le gros point de transition encore visible :

- `include/App.hpp` est passe a `Model*`
- `src/App.cpp` utilise encore `m_mesh`, `m_mesh->getCenter()`, `m_mesh->draw()`

Donc le projet n'est probablement pas encore recolle entierement apres la migration `Mesh -> Model`.

## Prochain pas recommande

1. adapter `App.cpp` a `Model*`
2. utiliser les bounds globales du `Model` dans `render()`
3. remplacer `m_mesh->draw()` par `m_model->draw()`
4. verifier le build
5. tester avec :
   - `human.obj`
   - `42.obj`
   - `teapot2.obj`

## Fichiers a regarder d'abord

- `include/App.hpp`
- `src/App.cpp`
- `include/ObjLoader.hpp`
- `src/ObjLoader.cpp`
- `include/Model.hpp`
- `src/Model.cpp`
- `include/MtlLoader.hpp`
- `src/MtlLoader.cpp`

## Notes de reprise

- Si un mesh est "introuvable" visuellement, verifier d'abord qu'on ne reintroduit pas un retour vers le centre du fichier brut.
- Si un `.mtl` semble ignore, verifier le parsing de `mtllib` et la resolution du chemin relatif.
- Si les faces couleur / UV semblent bizarres, verifier la triangulation en fan et le parsing des indices negatifs / `vt`.
- `Config::MODEL_PATH` pointe actuellement sur `assets/models/human.obj`.
