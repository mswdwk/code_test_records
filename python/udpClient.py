from socket import *
from time import ctime

HOST = ''
# convert to network port
PORT = htons(7655)
#PORT = 20123
BUFSIZ = 256
ADDR = (HOST, PORT)

udpClient = socket(AF_INET, SOCK_DGRAM)
# socket address reuse
udpClient.setsockopt(SOL_SOCKET,SO_REUSEADDR,1)
#udpClient.bind(ADDR)

while True:
    #print 'waiting for message...'
    data = raw_input('>')
    if(not data):
        break
    udpClient.sendto(data,ADDR)
    data,addr = udpClient.recvfrom(BUFSIZ)
    #udpClient.sendto('[%s] %s' % (ctime(), data), addr)
    print 'recv:', addr,",",data

udpClient.close()
