#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fstream>
#include <sys/types.h>
#include <iostream>
#include <vector>

#include <sstream>

#include "./aes.h"
using namespace std;

#define POSEIDON_HEADER "POSEIDON_DB\x32\x64\x39"
#define IP "127.0.0.1"
#define PORT 3322
string CRYPT_KEY;

vector<string> split1(string str,string del){
	vector<string>dele;
	ssize_t beg,pos=0;
	while((beg=str.find_first_not_of(del,pos)) != string::npos){ // loop until you find everything that isn't a delimiter , and always set that to be beginning
		pos = str.find_first_of(del,pos+1);//position is always the next case of del
	
		dele.push_back(str.substr(beg,pos-beg)); // and push 
	}
	return dele;
}
int uclength(unsigned char*a){
  int l =0;
  while(1){
    if(a[l] == '\0'){
      break;
    }
    l++;
  }
  return l;
}
unsigned char* encrypt(string cmd,string key){
  
  for(int i=0 ;i < cmd.length()+16;i++){
    cmd+="\x00";
  }

  
  stringstream ss;
  unsigned char* res= (unsigned char*)malloc(2048);// for key-value store , this is enough.
  vector<string>lmao;
  vector<unsigned char*> bb;
  if(cmd.length()>16){
            int k= 0; 
            
            
            while(true){
              if(k > cmd.length()){
                break;
              }
              ss.clear();
                string chunk = cmd.substr(k,16);
                //cout<<chunk.length()<<endl;
                if(chunk.length() <=6){
                  for(int i =0;i<7-chunk.length() ;i++){
                    chunk+=" ";
                  }
                }
                unsigned char* msg =  AES_E((unsigned char*)chunk.c_str());
                ss.clear();
                ss<<msg;
                unsigned char*comb =(unsigned char*)malloc(2048);
                comb[0] = 'B';
                comb[1] = 'L';
                comb[2] = 'K';
                int z =0; 
                for(int i =3;i<19;i++){
                    comb[i] = msg[z];
                    z++;
                }
                
                bb.push_back(comb);
                
                k+=16;
            }
          int mama =0;
          for(int i =0;i<bb.size();i++){
                for(int n =0;n<uclength(bb[i]);n++)
                {
                    res[mama] = bb[i][n];
                    mama++;

                }
          }
          
           
    }
    return res;
}

vector<unsigned char*>c_style_split(unsigned char*a ,unsigned char*del){
  vector<unsigned char*>res;

  for(int i =0;i<uclength(a);i++){
    if(a[i] == del[0] && a[i+1] == del[1] && a[i+2] == del[2]){
      unsigned char*mam = (unsigned char*)malloc(128);
      int kok =0;

      
      for(int j = i+3;j<16+i+3;j++){
          mam[kok] = a[j];
          printf("\n%x",a[j]);
          kok++;
      }
      exit(1);
      res.push_back(mam);
    }
  }
  //exit(1);
  return res;
}
string decrypt(unsigned char*  res,string key){
    string rm ;
    stringstream ss;
    if(uclength(res) > 16){
    
      vector<unsigned char*> blobs = c_style_split(res,(unsigned char*)"BLK");
      cout<<"\n"<<blobs.size()<<endl;
      for(int i =0;i<blobs.size();i++){
        unsigned char*blob = blobs[i];

        if(uclength(blob) >0 ){
          unsigned char * msg = AES_D(blob);
          string coci="";
          for(int j =0;j<uclength(msg);j++){
              coci+=msg[j];
          }
          rm+=coci;

        }
      }
      
    }
    return rm;    
}

void send_cmd(unsigned  char *buffer){
    
    int sock = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);
    connect(sock,(struct sockaddr*)&addr,sizeof(addr));
    send(sock,buffer,strlen((const char*)buffer),0);
    unsigned char *b = (unsigned char*)malloc(512);
    memset(b,0,512);
    recv(sock,b,512,0);

    cout<<b<<endl;
    //TODO: add AES decrypt received buffer.
    free(b);
    close(sock);


}
extern "C"{
  int  main(){
    
    ifstream strkey("key.key");
    strkey>>CRYPT_KEY;
  
    string header = POSEIDON_HEADER;
    stringstream ss;
    AES_init((unsigned char*)CRYPT_KEY.c_str());
    cout<<"\n";
    string cmd = "INSERT 'lmao':'test' / poseidon";
    unsigned char*encr = encrypt(cmd,CRYPT_KEY);
    unsigned char *test = (unsigned char*)"POSEIDON_DB\x32\x64\x39";
    unsigned char*mama =(unsigned char*)malloc(uclength(encr)+uclength(test));
    //nsigned char *test = (unsigned char*)"POSEIDON_DB\x32\x64\x39";
    int i =0;
    for(i =0;i<uclength(test);i++){
        mama[i] = test[i];
    }
    cout<<i<<endl;
    for(int k = 0;k<uclength(encr);k++){
      mama[i] =encr[k];
      i++;
    }
    for(int i =0;i<uclength(encr);i++){
        printf("\n%x ", encr[i]);
    }
    send_cmd(mama);





  }
}
