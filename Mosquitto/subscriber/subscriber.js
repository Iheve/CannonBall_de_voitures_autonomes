#!/usr/bin/env node

var path = require('path');
var mqtt = require('mqtt');
var mongoose = require('mongoose');

function usage() {
    console.log('Usage : ' + argv[0] + ' ' + path.basename(argv[1])
            + ' mqtt_host mqtt_port mongo_host');
}

var argv = process.argv;

for (var i = 2; i <= 4; i++) {
    if(!argv[i]) {
        usage();
        process.exit(-1);
    }
}

var mqtt_host = argv[2],
    mqtt_port = argv[3],
    mongo_host = argv[4];

var client = mqtt.createClient(mqtt_port, mqtt_host);
mongoose.connect('mongodb://' + mongo_host + '/metrics');
var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));

client.subscribe('metrics/accelerometer');
client.subscribe('metrics/direction');
client.subscribe('metrics/propulsion');
client.on('message', function(topic, message) {
    console.log(message);
});
client.options.reconnectPeriod = 60;
