Node subscriber client
=====================

Get the metrics via MQTT protocol and save them in MongoDB database.
Can be display through web page in realtime

Installation
--------------
`npm install`

Download [socket.io](http://socket.io/download/) client version 1.0.3 and put it in this
directory and renames it 'socket.io.js'

Download [ChartJS](https://github.com/nnnick/Chart.js) version 0.2 and put it in this
directory and renames it 'Chart.min.js'

Usage
--------
**/!\ Be careful, the program use 1337 port**

*You have to start an mqtt broker and a mongo server before launch the program*

`node subscriber.js mqtt_host mqtt_port mongo_host`
