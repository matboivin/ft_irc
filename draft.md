----------------------------------
### Architecture projet ft_irc ###
----------------------------------

I. 1 Design pattern

Quelques Design patterns qu'on sera amené à implémenter pour les besoins du projet:

> Observer pattern
> Pattern decorator
> Reactor pattern
> Strategy pattern
> Command Pattern
> Singleton Pattern

I. 2 Structures de données

*** Mises en place de différents éléments du projet sous forme de classes dont : ***

> Server
	- Un seul serveur dans le cadre de ce projet
	- Le serveur communique avec les clients via un Network
	- Singleton

> Client
	- Pattern de stratégie (Client ou Bot qui se baladent sur le serveur IRC, à chacun ses spécifités comme les droits)

> Package
	- Encapsule les commandes de l'user

> Command 
	- Pattern de comportement
	- Encapsulation d'une requête en tant qu'objet
	- Garder l'historique des commandes pour diverses opérations comme undo [un user qui est banni par un bot peut se voir toutes ses opérations annulées]

> Mediator et/ou Reactor
	- Le serveur et les clients ne communiquent que via un intermédiaire qui va s'occuper de la poll et s'assurer que les opérations soient non bloquants.
	- Le Reactor se charge de trigger les events et de les dispatch au bon endroit, redirection vers des modules de traitement [exemple: une trame arrive depuis un client et un module s'occupe d'enlever l'encapsulation pour en extraire la commande de l'user]

> Message

> State
	- Organisation des traitements de données et cheminement des events à travers des automates [exemple, la trame envoyée par le client passe par des modules de traitement comme le parsing et la lecture de l'instruction à exécuter sur le serveur]

*** Ainsi que les classes associés à IRC ***

> Channel/Direct Message
> User
> Operator
> Command

*** Pour les bonus ***

- File transfer -> Pattern decorator pour ajouter des features à la classe Command si plusieurs trames doivent être envoyés (bouts du fichier) pour ensuite être recomposer côté serveur
- A bot -> Comme un user mais avec des privilèges et doté d'une IA

I. 3 Choix technique

1 classe Server
1 classe User qui englobe Client et Bot
1 classe Client
1 classe Bot

