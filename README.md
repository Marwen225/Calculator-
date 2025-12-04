# Calculatrice d'Expressions Mathématiques

## Description

Programme de calculatrice avancée qui :
- Convertit des expressions infixes en notation postfixe
- Construit un arbre d'expression
- Simplifie l'arbre (constant folding, identités mathématiques)
- Évalue l'expression avec des variables

## Compilation

```bash
gcc calculator_single_file.c -o calculator_single
```

## Utilisation en ligne de commande

Le programme accepte n'importe quelle expression :

```bash
./calculator_single "expression" valeur_de_x
```

### Exemples testés :

1. **Simplification d'identité** :
   ```bash
   ./calculator_single "x * 1 + 0" 5
   ```
   - Résultat : `x` simplifié → `5.00`

2. **Pliage de constantes** :
   ```bash
   ./calculator_single "(3 + 4) * (2 + 1)" 0
   ```
   - Résultat : `21.0` (tout simplifié) → `21.00`

3. **Propriété du zéro** :
   ```bash
   ./calculator_single "x * 0 + 5" 100
   ```
   - Résultat : `5.0` (x*0 éliminé) → `5.00`

4. **Expression par défaut** :
   ```bash
   ./calculator_single
   ```
   - Utilise `(x + 0) * (5 - 3)` avec x = 10

## Utilisation avec l'interface web

### Installation :

1. Installez Flask :
   ```bash
   pip3 install flask flask-cors
   ```

2. Compilez le programme (si pas déjà fait) :
   ```bash
   gcc calculator_single_file.c -o calculator_single
   ```

3. Démarrez le serveur :
   ```bash
   python3 server.py
   ```

4. Ouvrez votre navigateur à : `http://localhost:5001`

### Fonctionnalités :

- ✅ Entrez n'importe quelle expression
- ✅ Changez la valeur de x
- ✅ Voir la conversion postfixe
- ✅ Voir l'arbre original et simplifié
- ✅ Obtenir le résultat final
- ✅ Exemples rapides pré-définis

## Structure du projet

```
calculator_single_file.c    # Programme principal (fichier unique)
calculator_single           # Exécutable compilé
server.py                   # Serveur Flask backend
interface.html              # Interface web
README.md                   # Ce fichier
README_SINGLE_FILE.md       # Documentation détaillée du code
```

## Expressions supportées :

- Variables : `x`
- Opérateurs : `+`, `-`, `*`, `/`
- Parenthèses : `(`, `)`
- Nombres : entiers et décimaux

## Simplifications appliquées :

1. **Pliage de constantes** : `3 + 4` → `7`
2. **Identité addition** : `x + 0` → `x` et `0 + x` → `x`
3. **Identité multiplication** : `x * 1` → `x` et `1 * x` → `x`
4. **Propriété du zéro** : `x * 0` → `0` et `0 * x` → `0`

## Technologies utilisées

- **C** : Logique de calcul et traitement
- **Python Flask** : Serveur backend pour l'interface web
- **HTML/CSS/JavaScript** : Interface utilisateur moderne et responsive

## Auteur

Projet de calculatrice d'expressions mathématiques avec simplification automatique
