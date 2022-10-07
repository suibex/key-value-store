import os,io,sys
#implement aes at the end
from socket import * 
import struct
import numpy as np 
import threading 
import crypto_aes
IP = "127.0.0.1"
PORT = 3322
POSEIDON_HEADER=b"POSEIDON_DB\x32\x64\x39"
CHECKER_HEADER = "\x64\x39"
COMMANDS=["SELECT","INSERT","DEL","CREATE"]
key = None
aes = crypto_aes.AES()
print("******* key-value pair database v1.0 ******")
def load_database(filename):
  filename = "./dbs/"+filename+".db"
  f = open(filename,"rb") 
  f = f.read()
  header = f[:len(POSEIDON_HEADER)]
  if(header != POSEIDON_HEADER):
      print("*** poseidon: invalid database.")
      return None
  return f[len(POSEIDON_HEADER):]

def insert_db(key,value,fname):
  db = load_database(fname)
  if(db == None):
    return 'err'  
  
  values = key+":"+value+";"
  values=  values.encode()
  db = POSEIDON_HEADER+db+values
  fname= "./dbs/"+fname+".db"
  f= open(fname,"wb")
  f.write(db)
  print(db)

  f.close()
  return 'ok'

def request(conn,addr):
  data = conn.recv(2048)

  if(data[:len(POSEIDON_HEADER)] == POSEIDON_HEADER):
    
    return_req = b""

    #TODO: IMPLEMENT SEND RESPONSE ENCRYPTED AND DECRYPT!
    data = data[len(POSEIDON_HEADER):]
    data = aes.decrypt(data)
    print(data)
    req = data.decode(errors='ignore').split(" ")
   
    if(req[0] not in COMMANDS):
      print("*** poseidon: invalid request received.")
      return_req = b"<E"
    print(data)
      
    if(req[0] == "CREATE"):
      os.system("touch ./dbs/"+req[1]+".db")
      fd = open("./dbs/"+req[1]+".db","wb")
      fd.write(POSEIDON_HEADER)
      print(f"*** poseidon: created database with name:{req[1]}")
      return_req = b"OK"
      fd.close()
    
    if(req[0] == "DEL"):
      os.system("rm * ./dbs/")
      return_req = b"OK"
    
    if(req[0] == "SELECT"):
      sym = req[1]
      fn = req[2]
      print(fn)
      if(sym == "*"):
        print(fn)
  #      fn = "./dbs/"+fn+".db"
        db = load_database(fn).decode(errors='ignore').replace(";","\n")
        db = "'key':'value'\n"+db
        db =db.encode()
        return_req = db
      if(sym =="where"):
        where = req[2]
        what = req[3]
        db =req[4]
        res = None
        db= load_database(db).decode(errors='ignore').split(";")
        for f in db:
            entry = f.split(":")
            if(len(entry)>0):
              if(where == 'key'):
                if(entry[0] == what):
                  res = entry[0]+":"+entry[1]
                  break
        if(res == None):
          return_req = b"OK"
        else:
          res= res.encode()
          return_req = res
      print("*** poseidon: item selected.")
          #print(entry) 1 
        #exit(1)
    if(req[0] == "INSERT"):
      print("LMAO")
      print(req)
      if(len(req) < 4):
        print("*** poseidon: invalid insert cmd.")
        print("ZSATO")
        return_req = b"<E"
      
      else:
        
        ds = req[1].split(":")
        key = ds[0]
        value = ds[1]
        print(ds)
        print(req[2])
        if(req[2] != "/"):
          print("*** poseidon: invalid insert cmd.")
          return_req = b"<E"
        else:
            filename = req[3]
            stat = insert_db(key,value,filename)      
            if(stat =='err'):
              return_req = b"<E"
            else:
              return_req = b"OK"
        print("**** insert key")

  else:
    return_req = b"<E"
 
  conn.send(aes.encrypt(return_req.decode(errors='ignore')))



fd =open("key.key","r")
key = fd.read()
fd.close()
print("*** key successfully read.")
aes.lib.AES_init(key.encode("utf-8"))
s = socket(AF_INET,SOCK_STREAM,0)
s.bind((IP,PORT))
s.listen(65536)
while True:
  conn,addr = s.accept()
  t = threading.Thread(target=request,args=[conn,addr])
  t.start()
  t.join()


