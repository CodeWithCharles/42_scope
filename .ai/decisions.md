# Decisions Durables

## 2026-04-22

- Namespace du projet : `Scop`, pour coller au nom du sujet.
- Pas de GLM : matrices maison a cause des contraintes du sujet.
- Les shaders sont charges depuis des fichiers externes, pas laisses inline dans `App.cpp`.
- `Vertex` utilise une couche maths unique (`Math::Vec2`, `Math::Vec3`) pour eviter deux systemes de vecteurs concurrents.
- `Mesh` possede ses buffers GPU et calcule ses propres bounds.
- Les couleurs de faces sont obtenues en dupliquant les sommets par face au chargement, afin d'eviter les artefacts d'interpolation sur des sommets partages.
- Le parser OBJ supporte les indices negatifs, car c'est une vraie regle du format.
- La triangulation des faces a N sommets se fait en fan, suffisante pour triangles / quads / polygones convexes.
- Les objets sont recentres a l'origine pour le rendu par defaut ; on n'applique pas le retour a la position brute du fichier dans le viewer.
- L'orientation d'import doit rester un correctif par modele, pas une identite globale de l'application.
- L'apparition de `mtllib`, `usemtl` et `o` a justifie l'introduction de `Material`, `Model`, `MtlLoader` et la migration de `ObjLoader` vers un retour `Model*`.
- La premiere abstraction de texture du projet passe par une classe `Texture` dediee et un loader PPM maison (`P3`/`P6`) pour rester dans les contraintes sans dependance externe.
- Le shader principal melange couleur vertex et texture via un uniforme `uTextureBlend` plutot qu'un simple bool, afin de couvrir directement la transition douce demandee par le sujet.

## 2026-04-23

- La logique de recherche de texture diffuse reste dans le domaine modele/import, pas dans `App` : `Model` expose les helpers de consultation, `App` se contente d'orchestrer.
- Les chemins `map_Kd` sont resolves au chargement relativement au dossier du fichier `.mtl`, pas au cwd ni directement au dossier du `.obj`.
- Tant que le projet ne supporte officiellement que les textures `PPM`, `App` n'auto-charge la texture diffuse d'un modele que si le chemin resolu pointe vers un `.ppm`; sinon il retombe sur `Config::TEXTURE_PATH`.
- La convention d'import UV retenue est un flip global de la coordonnee `V`, applique uniquement aux vertices qui ont effectivement un `vt`, afin de rester compatible avec les modeles sans UV.

## Regle d'entretien

Ajouter ici uniquement les decisions qui :

- changent durablement l'architecture
- influencent les choix futurs
- meritent d'etre connues sur une autre machine
