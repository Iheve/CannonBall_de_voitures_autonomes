#!/usr/bin/env node

var path = require('path');
var mqtt = require('mqtt');
var mongoose = require('mongoose');
var app = require('http').createServer(handler)
var io = require('socket.io')(app);
var fs = require('fs');

/* Main program */
app.listen(1337);
var argv = process.argv;
var Accelerometer = mongoose.model('Accelerometer', { x: Number, y: Number, z: Number });
var Steering = mongoose.model('Steering', { time: Number, value: Number });
var SteeringCounter = 0;
var Throttle = mongoose.model('Throttle', { time: Number, value: Number });
var ThrottleCounter = 0;

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
client.subscribe('metrics/steering');
client.subscribe('metrics/throttle');
client.on('message', function(topic, message) {
    console.log(topic + " " + message);
    if (topic === 'metrics/accelerometer') {
        var split = message.split(':');
        var insert = new Accelerometer({ x: split[0],
                                         y: split[1],
                                         z: split[2] });
        insert.save(function (err) {
            if (err)
                console.log(err);
        });
        io.emit('Accelerometer', { x: parseInt(split[0]),
                                       y: parseInt(split[1]),
                                       z: parseInt(split[2]) });

    } else if (topic === 'metrics/steering') {
        var insert = new Steering({ time: SteeringCounter, value: message });
        insert.save(function (err) {
            if (err)
                console.log(err);
        });
        io.emit('Steering', { time: SteeringCounter, value: parseInt(message) });
        SteeringCounter++;
    } else if (topic === 'metrics/throttle') {
        var insert = new Throttle({ time: ThrottleCounter, value: message });
        insert.save(function (err) {
            if (err)
                console.log(err);
        });
        io.emit('Throttle', { time: ThrottleCounter, value: parseInt(message) });
        ThrottleCounter++;
    }
});
client.options.reconnectPeriod = 60;

/* Functions */
function usage() {
    console.log('Usage : ' + argv[0] + ' ' + path.basename(argv[1])
            + ' mqtt_host mqtt_port mongo_host');
}

function handler (req, res) {
    fs.readFile(__dirname + '/view.html',
            function (err, data) {
                if (err) {
                    res.writeHead(500);
                    return res.end('Error loading view.html');
                }

                res.writeHead(200);
                res.end(data);
            });
}
