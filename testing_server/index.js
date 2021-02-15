const dgram = require('dgram');
const client = dgram.createSocket('udp4');

domains = [
    "facebook.com",
    "fonts.googleapis.com",
    "youtube.com",
    "instagram.com",
    "bit.ly",
    "gmpg.org",
    "s.w.org"
]

var size = domains.length

setInterval(() => {
    var random = domains[Math.floor(Math.random()*size)];
    const message = Buffer.from(random);
    client.send(message, 41234, '192.168.1.83');
    console.log("sending...");
}, 10000);