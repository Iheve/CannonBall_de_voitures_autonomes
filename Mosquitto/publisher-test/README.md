Publisher-test
=================
Test program to show how to send data through MQTT protocol using MQTT library

Installation
---------------
Download mosquitto :

*On linux ->*
`apt-get install mosquitto`

*On OSX (using Homebrew) ->*
`brew install mosquitto`

*On Windows ->*
Download the Native Build on this [page](http://mosquitto.org/download/)

Usage with Visual Studio
--------------------------
Copy and paste the files of the directory `C:\Program Files (x86)\mosquitto\devel`
in your project directory `/lib/`

Project configuration :

Go to Config > Debug > Environnement : `PATH=C:\Program Files (x86)\mosquitto;%PATH%` (for
the dlls)

Linker, entries : `lib/mosquittopp.lib;%(AdditionalDependencies)` (for the libs)

Directory VC++ : path to the lib directory (for the .h files)
