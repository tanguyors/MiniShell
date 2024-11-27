Pour Tanguy : Parsing et gestion des commandes
Parsing des commandes :

Découper les commandes et arguments : gérer les espaces, guillemets, et caractères spéciaux.
Identifier les redirections (>, <, >>) et les pipes (|).
Livrable : Une structure claire (par exemple, une liste chaînée ou un tableau structuré) contenant chaque commande et ses métadonnées.
Built-ins de gestion de l’environnement :

Implémenter les commandes internes liées aux variables d’environnement :
export : Ajouter/modifier des variables d’environnement.
unset : Supprimer des variables.
env : Afficher les variables d’environnement.
Livrable : Fonctions fonctionnelles et bien testées.
Conformité des fonctions utilisées dans le parsing :

Remplacer les fonctions interdites, si nécessaires, par des alternatives maison ou de la libft.
Pour Lilian : Exécution et gestion des processus
Exécution des commandes :

Gérer la création des processus avec fork, execve, et waitpid.
Gérer les redirections (>, <, >>) et les pipes (|).
Exécuter les commandes externes avec recherche dans le PATH.
Implémenter les built-ins restants comme cd, pwd, echo, et exit.
Livrable : Une fonction capable d'exécuter tout type de commande.
Gestion des signaux et boucle interactive :

Gérer les interruptions comme Ctrl+C, Ctrl+Z, et Ctrl+D.
Intégrer une boucle propre avec readline pour permettre une interaction fluide avec l’utilisateur.
Livrable : Une boucle robuste qui s’adapte bien aux signaux et erreurs.
Optimisation des performances et tests d’intégration :

Tester les différentes commandes et redirections dans des scénarios réalistes.
Collaborer avec Lilian pour intégrer le parsing et l’exécution de manière fluide.
