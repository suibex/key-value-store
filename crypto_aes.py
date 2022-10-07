from ctypes import *
import os,io,sys
import struct
class AES(object):
    def __init__(self) -> None:
        self.lib = CDLL("./aes.so")
        self.lib.AES_E.argtypes = (c_char_p,)
        self.lib.AES_E.restype = (c_char_p)
        self.lib.AES_D.argtypes = (c_char_p,)
        self.lib.AES_D.restype = c_char_p
        self.lib.AES_init.argtypes = (c_char_p,)


    def e_word(self,wrd):
        test = self.lib.AES_E(wrd)
        return test

    def encrypt(self,wrd):
        wrd =wrd.encode("utf-8")
        res = [] 
        final_wrd=b""
        if(len(wrd) > 16):
                k = 0 
                while True:
                    if(k > len(wrd)):
                        break
                    msg= wrd[k:16+k]
                    #print(len(msg))
                    print(len(msg),":",msg)
                    if(len(msg) <=6):
                        #print(len(msg))
                        for i in range(0,7-len(msg)):
                            msg+=b" "
                    
                    res.append(msg)
                    k+=16
        else:
            res.append(wrd) 
        
        result=[]
        for i in range(len(res)):
            wrd = res[i]
            test = self.e_word(wrd)
            final_wrd+=b"BLK"+test    
        return final_wrd   
        
    def d_decr(self,wrd):
        test = self.lib.AES_D(wrd)
        return test
    def decrypt(self,wrd):
        final_wrd=b""
        if(len(wrd) > 16):
            wrd = wrd.split(b"BLK")
            print("LEN:",len(wrd))
            for wr in wrd:
                if(len(wr) >0) :
                    final_wrd+=self.d_decr(wr)
        else:
            return self.decrypt(wrd)
        return final_wrd

