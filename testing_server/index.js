const dgram = require('dgram');
const { exit } = require('process');
const client = dgram.createSocket('udp4');

domains = [
    "facebook.com",
    "fonts.googleapis.com",
    "youtube.com",
    "instagram.com",
    "bit.ly",
    "gmpg.org",
    "s.w.org",
    "quest.com",
    "mongodb.net",
    "reddit.com",
    "ubc.com",
    "ubyssey.net",
    "viruz.hack",
    "sfu.ca",
    "pigeons.x",
]


client.bind(8082);

var size = domains.length

setInterval( () => {
    var random = domains[Math.floor(Math.random()*size)];
    const message = Buffer.from(random+"\n");
    client.send(message, 41234, '192.168.1.123');
    console.log("sending:  " + random);
}, 60);


client.on("message",(msg, remote) => {

    console.log("recieved: " + msg);
} );

