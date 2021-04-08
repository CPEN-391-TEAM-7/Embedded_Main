import socket
import time

test_cases = open("binary_test_1000.txt").readlines()

de1_address = "192.168.1.123"
de1_port    = 41234

receiver = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
receiver.bind(("0.0.0.0",8082));

sender   = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 

ones  = 0
zeros = 0
other = 0

correct   = 0
incorrect = 0

for raw_line in test_cases:
    line = raw_line.strip()
    test = line.split(",")
    domain = test[0]
    actual = test[1]

    sender.sendto((domain+"\n").encode(),(de1_address,de1_port))

    data, addr = receiver.recvfrom(1024)
    response = data.decode()

    if(response[-1] == actual):
        correct +=1
    else:
        print(response)
        incorrect +=1

    if(response[-1] == '1'):
        ones += 1
    elif (response[-1] == '0'):
        zeros += 1
    else:
        other += 1

print("CORRECT   :",correct)
print("INCORRECT :",incorrect)
print("ONES      :",ones)
print("ZEROS     :",zeros)
print("OTHER     :",other)

     

