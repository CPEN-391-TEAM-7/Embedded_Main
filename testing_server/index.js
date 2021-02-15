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


client.bind(56789);

var size = domains.length


var random = domains[Math.floor(Math.random()*size)];
const message = Buffer.from(random);
client.send(message, 41234, '192.168.1.74');
console.log("sending...");


client.on("message",(msg, remote) => {

    console.log("recieved: " + msg);

} );

