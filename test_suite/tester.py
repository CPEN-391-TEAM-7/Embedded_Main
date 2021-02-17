import socket
import time

def fail(msg):
    print("TEST FAILED: ",msg)
    exit()

de1_address = "192.168.1.74"
de1_port    = 41234

receiver = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
receiver.bind(("0.0.0.0",56789));

sender   = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 

####################################################################
# TEST 1, sending and receing a good single domain
####################################################################
print("TEST 1, sending and receing a good single domain")
sender.sendto(b"facebook.com",(de1_address,de1_port))

data, addr = receiver.recvfrom(1024) # buffer size is 1024 bytes
response = data.decode()
print("received message: %s" % response)
if(response == "facebook.com0"):
    print("passed")
else:
    fail(response)

time.sleep(5)

####################################################################
# TEST 2, sending and receing a bad single domain
####################################################################
print("TEST 2, sending and receing a bad single domain")
sender.sendto(b"badnet.hack",(de1_address,de1_port))

data, addr = receiver.recvfrom(1024) # buffer size is 1024 bytes
response = data.decode()
print("received message: %s" % response)
if(response == "badnet.hack1"):
    print("passed")
else:
    fail(response)

time.sleep(5)

####################################################################
# TEST 3, sending 2 good domains at once
####################################################################

print("TEST 3, sending 2 good domains at once")
sender.sendto(b"face.com",(de1_address,de1_port))
sender.sendto(b"book.com",(de1_address,de1_port))
d1 = True
d2 = True
while d1 or d2:
    data, addr = receiver.recvfrom(1024) # buffer size is 1024 bytes
    response = data.decode()
    print("received message: %s" % response)
    if d1 and response == "face.com0":
        d1 = False

    if d2 and response == "book.com0":
        d2 = False

    if (response not in ["book.com0","face.com0"]):
        fail("Unexpected response: " + response)


time.sleep(5)

####################################################################
# TEST 4, sending good domain and bad domain at once
####################################################################

print("TEST 4, sending good domain and bad domain at once")
sender.sendto(b"face.com",(de1_address,de1_port))
sender.sendto(b"book.hack",(de1_address,de1_port))
d1 = True
d2 = True
while d1 or d2:
    data, addr = receiver.recvfrom(1024) # buffer size is 1024 bytes
    response = data.decode()
    print("received message: %s" % response)
    if d1 and response == "face.com0":
        d1 = False

    if d2 and response == "book.hack1":
        d2 = False

    if (response not in ["book.hack1","face.com0"]):
        fail("Unexpected response: " + response)


time.sleep(5)

####################################################################
# TEST 5, sending 10 domains at once
####################################################################

print("TEST 5, sending 10 domains at once")

domains = ["1.com","2.hack","3.com","4.hack","5.com","6.hack","7.com","8.hack","9.com","10.hack"]

sender.sendto(b"1.com",(de1_address,de1_port))
sender.sendto(b"2.hack",(de1_address,de1_port))
sender.sendto(b"3.com",(de1_address,de1_port))
sender.sendto(b"4.hack",(de1_address,de1_port))
sender.sendto(b"5.com",(de1_address,de1_port))
sender.sendto(b"6.hack",(de1_address,de1_port))
sender.sendto(b"7.com",(de1_address,de1_port))
sender.sendto(b"8.hack",(de1_address,de1_port))
sender.sendto(b"9.com",(de1_address,de1_port))
sender.sendto(b"10.hack",(de1_address,de1_port))

count = 0 

while count < 10:
    data, addr = receiver.recvfrom(1024) # buffer size is 1024 bytes
    response = data.decode()
    print("received message: %s" % response)

    if (response[:-1] in domains):
        count += 1
    else:
        fail("Unexpected: " + response)



time.sleep(5)

print("ALL TESTS PASSED")