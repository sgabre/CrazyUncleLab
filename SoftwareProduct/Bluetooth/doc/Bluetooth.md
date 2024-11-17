**Under Developement**

Commandes CLI Bluetooth à Implémenter

## 1. Gestion des appareils Bluetooth

	scan on : Active le mode de recherche pour découvrir des appareils.

	bluetooth> scan on

Sous Linux, cette commande peut utiliser hcitool scan ou bluetoothctl scan on.
Vous devrez écouter les événements des appareils détectés (e.g., via hci_le_scan() pour BLE).

	scan off : Désactive le mode de recherche.

	bluetooth> scan off

	devices : Liste les appareils détectés ou appairés.

	bluetooth> devices

Retourne une liste avec des informations comme les adresses MAC, noms et RSSI (force du signal).

## 2. Connexion et appairage

pair <adresse> : Appaire l'appareil avec l'adresse MAC spécifiée.

	bluetooth> pair AA:BB:CC:DD:EE:FF

Sous Linux, cela correspond à bluetoothctl pair.

	connect <adresse> : Connecte un appareil appairé.

	bluetooth> connect AA:BB:CC:DD:EE:FF

	disconnect <adresse> : Déconnecte un appareil connecté.

	bluetooth> disconnect AA:BB:CC:DD:EE:FF

## 3. Gestion des paramètres

	status : Affiche l’état actuel du Bluetooth (activé/désactivé, appareils connectés, etc.).

	bluetooth> status

	set discoverable <on|off> : Rend le périphérique visible ou non.

	bluetooth> set discoverable on

	set alias <nom> : Change le nom visible du périphérique Bluetooth local.

	bluetooth> set alias MonBluetooth

## 4. Services et profils

	services <adresse> : Liste les services disponibles sur un appareil.

	bluetooth> services AA:BB:CC:DD:EE:FF

Pour BLE, cela peut inclure la liste des UUID GATT.
	
 	gatt read <handle> : Lit une caractéristique GATT spécifique (pour BLE).

	bluetooth> gatt read 0x0025

	gatt write <handle> <valeur> : Écrit une valeur dans une caractéristique GATT.

	bluetooth> gatt write 0x0025 0x01

## 5. Suppression et réinitialisation

	unpair <adresse> : Supprime l’appairage avec un appareil.
	
 	bluetooth> unpair AA:BB:CC:DD:EE:FF

reset : Réinitialise le contrôleur Bluetooth local.
	
 	bluetooth> reset

Cela pourrait désactiver et réactiver Bluetooth pour une remise à zéro complète.
Fonctionnalités Avancées

	rssi <adresse> : Affiche la force du signal d’un appareil (RSSI).
	
 	bluetooth> rssi AA:BB:CC:DD:EE:FF
	
 	info <adresse> : Montre les informations détaillées d’un appareil (nom, classe, services, etc.).
	
 	bluetooth> info AA:BB:CC:DD:EE:FF
	
 	start advertising : Démarre la diffusion d’annonces BLE.
	
 	bluetooth> start advertising
	
 	stop advertising : Arrête la diffusion d’annonces BLE.
	
 	bluetooth> stop advertising


## 6. Commandes pour le profil FTP (File Transfer Profile)
	
Les commandes suivantes permettent de gérer les opérations FTP (obtenir, envoyer, lister des fichiers, etc.) en utilisant OBEX sous Linux ou avec des outils comme obexftp.
	
Lister les services FTP disponibles :
	
	obexftp --list
	
 Cette commande permet de lister les services FTP disponibles sur un appareil Bluetooth.
	
Connexion à un appareil FTP :
	
	obexftp --bluetooth <adresse_Bluetooth> --channel <numéro_du_channel>

Cette commande établit une connexion FTP avec un appareil à l'adresse Bluetooth spécifiée et sur le canal approprié.
		
Lister les fichiers d'un répertoire FTP :
		
	obexftp --bluetooth <adresse_Bluetooth> --list

Liste les fichiers dans le répertoire racine du serveur FTP.
			
Télécharger un fichier via FTP :
			
	obexftp --bluetooth <adresse_Bluetooth> --get <chemin_du_fichier_sur_serveur> --put <chemin_local>

Télécharge un fichier depuis le serveur FTP vers le système local.
				
Envoyer un fichier via FTP :
				
	obexftp --bluetooth <adresse_Bluetooth> --put <chemin_local> --get <chemin_sur_serveur>

Transfert un fichier local vers le serveur FTP.
					
Mettre à jour le répertoire FTP :
					
	obexftp --bluetooth <adresse_Bluetooth> --setpath <chemin_du_répertoire>

Change de répertoire sur le serveur FTP.
						
## 7. Commandes pour le profil OPP (Object Push Profile)

Le profil OPP est plus simple que FTP et est principalement utilisé pour envoyer des objets simples comme des vCards ou des images. Vous pouvez utiliser des outils comme obexpushd ou obexftp pour interagir avec OPP.

Envoyer un objet (ex. vCard ou image) :

obexftp --bluetooth <adresse_Bluetooth> --put <fichier_objet>
Envoie un fichier, tel qu'une vCard ou une image, à un appareil via OPP.

Recevoir un objet :

obexftp --bluetooth <adresse_Bluetooth> --get <fichier_objet>
Télécharge un objet depuis un appareil via OPP.

## 8. Commandes pour le profil SPP (Serial Port Profile)

Le SPP permet de simuler une connexion série entre deux appareils Bluetooth. Vous pouvez l'utiliser pour envoyer des données sous forme de texte ou de fichiers via un terminal série.

Lister les périphériques Bluetooth et leurs ports série disponibles :

sdptool browse local
Cette commande liste les services Bluetooth disponibles sur votre appareil, y compris les ports série.

Se connecter à un appareil via un port série SPP :

rfcomm connect /dev/rfcomm0 <adresse_Bluetooth> <port_SPP>
Établit une connexion série Bluetooth à un appareil sur un port spécifique.

Écrire des données sur un port série SPP :

echo "data" > /dev/rfcomm0
Envoie des données sur un périphérique Bluetooth via SPP.

Lire des données depuis un port série SPP :

cat /dev/rfcomm0
Lit les données envoyées sur le port série Bluetooth.

## 9. Commandes pour le profil A2DP (Advanced Audio Distribution Profile)

Le A2DP est principalement utilisé pour le streaming audio, mais vous pouvez l'utiliser pour envoyer des fichiers audio via Bluetooth.

Connecter un périphérique A2DP :

bluetoothctl connect <adresse_Bluetooth>
Cette commande connecte votre appareil à un périphérique A2DP, permettant ainsi de transmettre des flux audio.

Lancer le streaming audio :

pactl load-module module-bluez5-device address=<adresse_Bluetooth>
Envoie un flux audio vers un périphérique Bluetooth A2DP.

Contrôler le volume audio Bluetooth :

pactl set-sink-volume <sink_name> 50%
Régler le volume du périphérique de sortie Bluetooth.

## 10. Commandes Générales pour le Bluetooth (avec bluetoothctl)

bluetoothctl est un outil très pratique sous Linux pour gérer les connexions Bluetooth. Voici quelques commandes génériques que vous pouvez utiliser pour interagir avec différents profils Bluetooth.

Lancer l'outil bluetoothctl :

bluetoothctl
Lance l'interface de commande interactive pour gérer les appareils Bluetooth.

Activer le Bluetooth :

power on
Allume le Bluetooth sur votre appareil.

Lister les appareils Bluetooth disponibles :

devices
Affiche tous les appareils Bluetooth appairés ou détectés à proximité.

Appairer avec un périphérique Bluetooth :

pair <adresse_Bluetooth>
Effectue l'appariement avec un appareil Bluetooth.

Connecter un appareil Bluetooth :

connect <adresse_Bluetooth>
Établit une connexion avec un appareil Bluetooth spécifié.

Se déconnecter d'un périphérique Bluetooth :

disconnect <adresse_Bluetooth>
Déconnecte un appareil Bluetooth.

## 11. Commandes pour OBEX via obexftp ou obexpushd
Démarrer un serveur OBEX (pour recevoir des fichiers) :

obexpushd -B
Lance un serveur OBEX en arrière-plan pour recevoir des fichiers via Bluetooth.

Recevoir un fichier via OBEX :

obexpushd --get <fichier>
Permet de recevoir un fichier depuis un appareil Bluetooth via OBEX.

Résumé des commandes par profil :
Profil	Commandes Principales
FTP	obexftp --list, obexftp --get, obexftp --put
OPP	obexftp --put, obexftp --get
SPP	rfcomm connect, echo "data" > /dev/rfcomm0, cat /dev/rfcomm0
A2DP	bluetoothctl connect, pactl load-module module-bluez5-device
Générales Bluetooth	bluetoothctl pair, bluetoothctl connect, bluetoothctl devices





	
	Les profils Bluetooth sont des ensembles de spécifications définissant des fonctionnalités et des services spécifiques pour l'échange de données entre deux appareils Bluetooth. Ils permettent une interaction standardisée entre les périphériques, garantissant une compatibilité mutuelle pour des applications spécifiques. Voici une liste des principaux profils Bluetooth et leurs usages :
	
	1. Audio et Média
	A2DP (Advanced Audio Distribution Profile) : Permet le streaming audio de haute qualité entre des appareils (par exemple, envoyer de la musique d'un téléphone vers un casque sans fil ou des haut-parleurs Bluetooth).
	HFP (Hands-Free Profile) : Utilisé pour les systèmes mains libres dans les voitures et les casques Bluetooth. Il permet de prendre des appels téléphoniques via un périphérique Bluetooth tout en gardant les mains libres.
	HSP (Headset Profile) : Similaire à HFP, mais principalement utilisé pour les casques audio, permettant des appels et des contrôles audio basiques.
	AUDIO GATEWAY : Permet de rediriger les flux audio entre différents appareils, tels que des systèmes audio et des périphériques Bluetooth.
	PBAP (Phone Book Access Profile) : Permet d'accéder aux contacts et informations du carnet d'adresses d'un téléphone Bluetooth via un périphérique compatible.
	2. Transfert de Fichiers et Communication
	FTP (File Transfer Profile) : Permet le transfert de fichiers entre deux périphériques Bluetooth. Utile pour envoyer des fichiers comme des images, des vidéos, des documents, etc.
	OPP (Object Push Profile) : Permet de transférer des fichiers ou objets, comme des contacts vCard, des photos, ou des calendriers entre deux appareils.
	BIP (Basic Imaging Profile) : Utilisé pour le transfert d'images entre des périphériques, par exemple pour envoyer des photos d'un appareil photo numérique vers un imprimante Bluetooth.
	SPP (Serial Port Profile) : Crée un port série virtuel entre deux appareils, souvent utilisé pour des communications en série et des applications de communication de données brutes.
	MAP (Message Access Profile) : Permet l'accès aux messages, comme les SMS et les notifications de messagerie, sur un appareil Bluetooth à partir d'un autre appareil (exemple : afficher les messages SMS sur une montre connectée).
	3. Connexions de Données
	PAN (Personal Area Networking Profile) : Permet la mise en réseau d'appareils Bluetooth, pour partager des connexions réseau ou des ressources entre plusieurs périphériques.
	HID (Human Interface Device Profile) : Utilisé pour les périphériques d'entrée, tels que les claviers, souris, manettes de jeu, etc. Ce profil permet à ces dispositifs de se connecter sans fil via Bluetooth.
	LE (Low Energy) : Utilisé pour les appareils à faible consommation d'énergie, typiquement pour les dispositifs comme les capteurs, trackers de fitness, etc.
	4. Gestion de la Connexion et de la Sécurité
	GAP (Generic Access Profile) : Gère la manière dont les périphériques Bluetooth se découvrent, se connectent et interagissent avec d'autres appareils Bluetooth.
	GATT (Generic Attribute Profile) : Définit la structure et les services pour les appareils Bluetooth Low Energy (BLE). Il est souvent utilisé pour les applications de santé, de fitness, et de domotique.
	L2CAP (Logical Link Control and Adaptation Protocol) : Permet la gestion des connexions logiques et les communications entre périphériques Bluetooth via des canaux de données.
	5. Synchronisation et Réseau
	SYNC (Synchronization Profile) : Utilisé pour synchroniser les informations (contacts, calendrier, etc.) entre des appareils Bluetooth.
	BPP (Basic Printing Profile) : Utilisé pour l'impression de documents sur des imprimantes compatibles Bluetooth.
	6. Autres Profils Spécifiques
	MCP (Message Capabilities Profile) : Permet à un périphérique de vérifier la capacité d'un autre périphérique à recevoir et envoyer des messages dans le cadre d'une communication Bluetooth.
	CIP (Cordless Telephony Profile) : Utilisé pour les téléphones sans fil Bluetooth.
	Profils Spécifiques à Bluetooth Low Energy (BLE)
	FMP (Find Me Profile) : Utilisé pour localiser un périphérique Bluetooth, généralement un "balise" ou un "appareil émetteur".
	HTP (Health Thermometer Profile) : Utilisé pour des appareils de mesure de la température corporelle.
	BAS (Battery Service Profile) : Permet à un périphérique de surveiller l'état de la batterie d'un autre appareil.
	DIS (Device Information Service Profile) : Fournit des informations détaillées sur le périphérique Bluetooth, comme le fabricant, le modèle, la version du firmware, etc.
	Résumé des Profils Bluetooth par Catégorie
	Catégorie	Profils Bluetooth
	Audio et Média	A2DP, HFP, HSP, PBAP, AUDIO GATEWAY
	Transfert de Fichiers	FTP, OPP, BIP, SPP
	Connexion de Données	PAN, HID, LE
	Gestion de Connexion et Sécurité	GAP, GATT, L2CAP
	Synchronisation	SYNC, BPP
	Profils Spécifiques BLE	FMP, HTP, BAS, DIS
	Autres	MCP, CIP
