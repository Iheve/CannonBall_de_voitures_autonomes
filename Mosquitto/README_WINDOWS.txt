Copier coller les libs de mosquitto : C:\Program Files (x86)\mosquitto\devel
dans le dossier du code /lib/

Configuration du projet :
Dans Config > Debogage > Environnement : PATH=C:\Program Files (x86)\mosquitto;%PATH% (pour les dll)
Editeur de lien, entrées : lib/mosquittopp.lib;%(AdditionalDependencies) (pour les libs)
Répertoire VC++ : path vers le dossier lib (pour les .h)