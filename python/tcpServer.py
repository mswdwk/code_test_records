from socket import *
from time import ctime

HOST = '127.0.0.1'
# convert to network port
#PORT = htons(7654)
PORT = 7654
#PORT = 20123
BUFSIZ = 256
ADDR = (HOST, PORT)

tcpServer = socket(AF_INET, SOCK_STREAM)
# socket address reuse
tcpServer.setsockopt(SOL_SOCKET,SO_REUSEADDR,1)
tcpServer.bind(ADDR)
tcpServer.listen(5)

while True:
    print 'waiting for message...'
    conn,addr = tcpServer.accept()
    data=conn.recv(BUFSIZ)

    #tcpServer.sendto('[%s] %s' % (ctime(), data), addr)
    print 'recv:', addr,",",data
    conn.close()

tcpServer.close()
