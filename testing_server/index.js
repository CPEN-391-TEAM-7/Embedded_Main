const dgram = require('dgram');
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


client.bind(56789);

var size = domains.length

setInterval( () => {
    var random = domains[Math.floor(Math.random()*size)];
    const message = Buffer.from(random);
    client.send(message, 41234, '192.168.1.123');
    console.log("sending...");
}, 1000);


client.on("message",(msg, remote) => {

    console.log("recieved: " + msg);

} );

