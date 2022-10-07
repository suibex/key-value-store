from socket import * 
import os,io,sys
import crypto_aes
aes = crypto_aes.AES()
aes.lib.AES_init("t7w!z%C*F-JaNdRf".encode("utf-8"))
sock = socket(AF_INET,SOCK_STREAM,0)
sock.connect(("127.0.0.1",3322))
print("**** connected to server. ****")
while True:
    msg =input("db>")
    
    if(msg == "exit"):
        break

    encr = aes.encrypt(msg)
    encr = b"POSEIDON_DB\x32\x64\x39"+encr
    sock.send(encr)
    data =sock.recv(2048)
    #print(data)
    data = aes.decrypt(data)
    print("SERVER RESPOSEN:",data)

sock.close()