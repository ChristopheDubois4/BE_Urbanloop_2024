# JETSON

Les fichiers dans ce dossier sont à mettre sur la JETSON 

## Principe

De manière initial, le fichier *docker-compose.slam.urban.yml* permet de lancer plusiers conteneurs docker permettant de faire fonctionner le plateau de capteur. 

Pour ajouter des fichiers sans compremettre le fonctionnemnt du plateau, un nouveau conteneur docker "urban" est créer pour y ajouter nos fichiers.

## Copie du dossier

Il est possible de téélcharger ces fichiers directement depuis [github](https://github.com/ChristopheDubois4/BE_Urbanloop_2024/tree/main/JETSON)

Sinon, via ssh (cf [prise_en_main.md](https://github.com/ChristopheDubois4/BE_Urbanloop_2024/blob/main/Documentation/prise_en_main.md)), il est possible de copiées les différents éléments : 


- copier le dossier urban

``` bash
scp -r "chemin_du_dossier"/urban shuttle@100.64.212.88:/home/shuttle/
```

- copier le fichier yml

``` bash
scp "chemin_du_dossier"/docker-compose.slam.urban.yml shuttle@100.64.212.88:/home/shuttle/
```

## Setup

La première fois que les fichiers sont mis sur la JETSON ou à chaque fois qu'ils sont modifiés, il faut recréer l'image docker *urban*

``` bash
cd /urban
```
``` bash
docker build -t urban .
```

*Pour voir la liste des images :*

``` bash
docker images
```

## Lancement

Démarrage de tous les conteneurs docker :

``` bash
docker compose -f docker-compose.slam.urban.yml up -d
```

Arrêt :

``` bash
docker compose -f docker-compose.slam.urban.yml down
```

### Lancement de nos fichiers dans le conteneur urban

- Accès au conteneur

``` bash
docker exec -it shuttle-urban-1 bash
```

*Pour voir la liste des conteneurs actifs :*

``` bash
docker ps
```

- Lancements des scripts

``` bash
cd src/urban
```
``` bash
bash urban.launch.sh
```



