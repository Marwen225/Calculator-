# Guide d'utilisation de la calculatrice

## Utilisation en ligne de commande

Le programme accepte maintenant n'importe quelle expression :

```bash
./calculator "expression" valeur_de_x
```

### Exemples testés :

1. **Simplification d'identité** :
   ```bash
   ./calculator "x * 1 + 0" 5
   ```
   - Résultat : `x` simplifié → `5.00`

2. **Pliage de constantes** :
   ```bash
   ./calculator "(3 + 4) * (2 + 1)" 0
   ```
   - Résultat : `21.0` (tout simplifié) → `21.00`

3. **Propriété du zéro** :
   ```bash
   ./calculator "x * 0 + 5" 100
   ```
   - Résultat : `5.0` (x*0 éliminé) → `5.00`

## Utilisation avec l'interface web

### Installation :

1. Installez Flask :
   ```bash
   pip3 install flask flask-cors
   ```

2. Démarrez le serveur :
   ```bash
   python3 server.py
   ```

3. Ouvrez `interface.html` dans votre navigateur

### Fonctionnalités :

- ✅ Entrez n'importe quelle expression
- ✅ Changez la valeur de x
- ✅ Voir la conversion postfixe
- ✅ Voir l'arbre original et simplifié
- ✅ Obtenir le résultat final
- ✅ Exemples rapides pré-définis

## Expressions supportées :

- Variables : `x`
- Opérateurs : `+`, `-`, `*`, `/`
- Parenthèses : `(`, `)`
- Nombres : entiers et décimaux

## Simplifications appliquées :

1. **Pliage de constantes** : `3 + 4` → `7`
2. **Identité addition** : `x + 0` → `x`
3. **Identité multiplication** : `x * 1` → `x`
4. **Propriété du zéro** : `x * 0` → `0`
