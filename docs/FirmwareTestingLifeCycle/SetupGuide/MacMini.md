# Préparation d'une clé USB Ubuntu Server headless pour Mac mini (Macmini3,1)

## Objectif
Créer une clé USB bootable avec **Ubuntu Server 22.04.4** et activer **SSH dès le premier boot**, pour installer Ubuntu sur un Mac mini 2009 sans clavier ni écran (headless).

---

## Matériel requis
- MacBook Pro (ou autre macOS) pour préparer la clé
- Clé USB de 8 GB minimum
- ISO Ubuntu Server 22.04.4
- Câble Ethernet pour la connexion réseau du Mac mini

---

## Étape 0 : Préparer la clé USB (tout sera effacé !)

1. Identifier la clé USB :

```bash
diskutil list

diskutil eraseDisk MS-DOS UBUNTU MBR /dev/diskX
```

## Étape 1 : Copier l'ISO Ubuntu Server sur la clé

1. Démonter toutes les partitions de la clé pour éviter l’erreur Resource busy :


```bash
diskutil unmountDisk /dev/diskX
```

2. Copier l’ISO sur la clé :
```bash
sudo dd if=/chemin/vers/ubuntu-22.04.4-live-server-amd64.iso of=/dev/rdiskX bs=4m status=progress
sync
```

/dev/rdiskX = accès raw rapide à la clé USB.

## Étape 2 : Créer la partition FAT32 pour cloud-init

⚠️ Cette étape crée un petit volume FAT32 pour cloud-init et SSH headless (~50 MB).

```bash
diskutil partitionDisk /dev/diskX 2 MBR FAT32 UBUNTU 50M
diskutil mount /dev/diskXs2
ls /Volumes
mkdir /Volumes/UBUNTU/cidata
```

Remplace UBUNTU par le nom réel de la partition FAT32 monté.

## Étape 3 : Ajouter les fichiers cloud-init pour SSH

user-data (/Volumes/UBUNTU/cidata/user-data)

```yaml
#cloud-config
users:
  - name: ubuntu
    sudo: ALL=(ALL) NOPASSWD:ALL
    shell: /bin/bash
    lock_passwd: false
    passwd: "$6$rounds=4096$ABCDEFG$abcdef123456..."  # mot de passe chiffré
ssh_pwauth: True
chpasswd:
  list: |
     ubuntu:ubuntu
  expire: False
```

meta-data (/Volumes/UBUNTU/cidata/meta-data)

```yaml
instance-id: ubuntu-macmini
local-hostname: macmini
```

## Étape 4 : Démarrer le Mac mini sur la clé USB

```bash
sudo bless --device /dev/diskXs1 --setBoot --nextonly
sudo reboot
```

## Étape 5 : Connexion SSH

* 1. Branche un câble Ethernet sur le Mac mini.
* 2. Scanner le réseau pour trouver l’adresse IP :
  
```bash
nmap -p 22 192.168.1.0/24
```

* 3. Connexion SSH :

```bash
ssh ubuntu@IP_DU_MACMINI
# mot de passe : ubuntu
passwd  # changer le mot de passe immédiatement
```

## Étape 6 : Vérifier le matériel sous Linux

```bash
lspci       # GPU, Ethernet
lsusb       # USB devices
ip link show # réseau
aplay -l    # audio
sudo apt install firmware-b43-installer  # Wi-Fi Broadcom si nécessaire
```

## Notes

La partition FAT32 avec cidata est obligatoire pour SSH headless.
Laisser un câble Ethernet branché pour la première connexion est recommandé.
Après la première connexion, tu peux installer Ubuntu et configurer le système normalement.








