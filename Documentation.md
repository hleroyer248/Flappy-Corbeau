# Documentation du Projet : Raven Soul

**Langage/Librairie :** C++ / SFML 3.0.2

**Plateforme :** Windows (Visual Studio 2022)

---

## 1. Concept

### Pitch
Dans un monde où la Mort a perdu son emprise, les âmes sont prisonnières du Royaume d’Astryx. Le joueur incarne **Raven**, un messager créé par la Faucheuse, doté de capacités fantomatiques, dont la mission est de traverser les ruines pour libérer ces âmes égarées.

### Règles du Jeu
* **Survie :** Le joueur doit naviguer à travers des débris instables et des anomalies. Toute collision avec un obstacle entraîne la fin de la partie.
* **Capacité Spéciale :** Raven possède un mode **Fantôme** permettant d'altérer ses interactions avec l'environnement.
* **Progression :** Le jeu propose un gameplay fluide de type arcade avec des niveaux de difficulté variés.
* **Contrôles :**
    * **Saut :** Touche `ESPACE`.
    * **Mode Fantôme :** Touche `Maj / Shift`.

---

## 2. Fonctionnalités Réalisées

### Architecture & Core 🛠
* **Finite State Machine (FSM) :** Gestion modulaire des différents états du jeu (Menus, Gameplay, Boutique, Paramètres).
* **AudioManager :** Système centralisé pour la gestion des effets sonores et des musiques, crucial pour l'immersion dans le BossLevel.
* **CollisionBox :** Système de détection de collisions précis pour gérer les interactions entre le joueur et les obstacles.

### Gameplay & Systèmes ✅
* **Boutique Intégrée :** Système permettant de modifier les éléments du joueur, des ennemis et des projectiles.
* **Physique Arcade :** Implémentation d'un gameplay aérien fluide et réactif inspiré des classiques du genre.
* **Interface Utilisateur (UI) :** Menus complets incluant les paramètres et les écrans de transition.

### Multimédia
* **SFML 3 :** Utilisation intensive des modules `graphics`, `audio`, `window`, `system`, et `network`.
* **Ressources :** Les DLL nécessaires sont incluses pour faciliter l'exécution directe sur Windows.

---

## 3. Approche Pédagogique 📚

Le projet a été développé avec une volonté de **clarté** et d'**expérimentation**. L'architecture logicielle privilégie la modularité afin que chaque membre de l'équipe interdisciplinaire puisse comprendre et contribuer aux différents systèmes (audio, physique, ou interface).

---

## 4. Structure du Projet

```text
- Finite State Machine : Gestion des états (menu, jeu, boutique…)
- AudioManager : Gestion des sons et musiques
- CollisionBox : Détection des collisions
- Game : Logique principale et boucle de jeu
```

---

## 5. Organisation de l'Équipe 👥

Le projet **Raven Soul** est le fruit d'une collaboration interdisciplinaire répartie en trois pôles de compétences :

### 💻 Groupe Technique (G. Tech)
* **LEROYER Hugo**
* **PACTOLE--OTHILY Swan**
* **UWINEZA Job**
* **BOUYAT Léo**

### 🎨 Groupe Artistique (G. Art)
* **Natchimie Enzo**
* **Marie Wendy**
* **Mith Odryan**
* **Robert Arnaud**

### 📈 Groupe Business & Communication (G. Business)
* **FERRO Marco**
* **SAELEN Doryan**
