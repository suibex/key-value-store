/*
 *
 *	very crappy AES algorithm
 *		by nitrodegen
 *		September 2022.
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include "./tables.h"
#include "./aes.h"
unsigned char *round_key;
unsigned char expanded[176];

bool done = false;

void add_round_key( unsigned char *msg){
  for(int i =0;i<16;i++){
			msg[i] = msg[i] ^ round_key[i];
	}
}
void  sub_bytes(unsigned char*msg){
    for(int i =0;i<16;i++){
          msg[i] = s_box[msg[i]];
       
    }     
}
void  rsub_bytes(unsigned char* msg){

      for(int i =0 ;i<16;i++){
        msg[i] = inv_s_box[msg[i]];

      } 
}


void  shift_rows(unsigned char* state){

   unsigned char tmp[16];
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];
  for(int i =0;i<16;i++){
    state[i] =tmp[i];
  }

}

void  rshift_rows(unsigned char*a){
  
    unsigned char tmp[16];
    memset(tmp,0,16);


    tmp[0]  =a[0];
    tmp[1] = a[13];
    tmp[2] = a[10];
    tmp[3] = a[7];
    tmp[4] = a[4];
    tmp[5] = a[1];  
    tmp[6] = a[14];
    tmp[7] = a[11];
    tmp[8] = a[8];
    
    tmp[9] = a[5];
    tmp[10] = a[2];
    tmp[11] = a[15];

    tmp[12] = a[12];
    tmp[13] = a[9];
    tmp[14] = a[6];
    tmp[15] = a[3];
  for(int i =0 ;i<16;i++){
    a[i] = tmp[i];
  }

}
void mix_cols(unsigned char* state){
    

unsigned char tmp[16];

	tmp[0] = (unsigned char)(mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3]);	//This line is represented in above explanation
	tmp[1] = (unsigned char)(state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3]);
	tmp[2] = (unsigned char)(state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]]);
	tmp[3] = (unsigned char)(mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]]);

	tmp[4] = (unsigned char)(mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7]);
	tmp[5] = (unsigned char)(state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7]);
	tmp[6] = (unsigned char)(state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]]);
	tmp[7] = (unsigned char)(mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]]);

	tmp[8] = (unsigned char)(mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11]);
	tmp[9] = (unsigned char)(state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11]);
	tmp[10] = (unsigned char)(state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]]);
	tmp[11] = (unsigned char)(mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]]);

	tmp[12] = (unsigned char)(mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15]);
	tmp[13] = (unsigned char)(state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15]);
	tmp[14] = (unsigned char)(state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]]);
	tmp[15] = (unsigned char)(mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]]);

  for(int i =0;i<16;i++){
      state[i] = tmp[i];
  }
}

void rmix_cols(unsigned char* state){
    

unsigned char temp[16];

  temp[0] = (unsigned char)(mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]]);
	temp[1] = (unsigned char)(mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]]);
	temp[2] = (unsigned char)(mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]]);
	temp[3] = (unsigned char)(mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]]);

	temp[4] = (unsigned char)(mul14[state[4]] ^ mul11[state[5]] ^ mul13[state[6]] ^ mul9[state[7]]);
	temp[5] = (unsigned char)(mul9[state[4]] ^ mul14[state[5]] ^ mul11[state[6]] ^ mul13[state[7]]);
	temp[6] = (unsigned char)(mul13[state[4]] ^ mul9[state[5]] ^ mul14[state[6]] ^ mul11[state[7]]);
	temp[7] = (unsigned char)(mul11[state[4]] ^ mul13[state[5]] ^ mul9[state[6]] ^ mul14[state[7]]);

	temp[8] = (unsigned char)(mul14[state[8]] ^ mul11[state[9]] ^ mul13[state[10]] ^ mul9[state[11]]);
	temp[9] = (unsigned char)(mul9[state[8]] ^ mul14[state[9]] ^ mul11[state[10]] ^ mul13[state[11]]);
	temp[10] = (unsigned char)(mul13[state[8]] ^ mul9[state[9]] ^ mul14[state[10]] ^ mul11[state[11]]);
	temp[11] = (unsigned char)(mul11[state[8]] ^ mul13[state[9]] ^ mul9[state[10]] ^ mul14[state[11]]);

	temp[12] = (unsigned char)(mul14[state[12]] ^ mul11[state[13]] ^ mul13[state[14]] ^ mul9[state[15]]);
	temp[13] = (unsigned char)(mul9[state[12]] ^ mul14[state[13]] ^ mul11[state[14]] ^ mul13[state[15]]);
	temp[14] = (unsigned char)(mul13[state[12]] ^ mul9[state[13]] ^ mul14[state[14]] ^ mul11[state[15]]);
	temp[15] = (unsigned char)(mul11[state[12]] ^ mul13[state[13]] ^ mul9[state[14]] ^ mul14[state[15]]);

  for(int i =0;i<16;i++){
      state[i] = temp[i];
  }
}

void add_expanded_key(unsigned char* msg,unsigned char*exp){
    for(int i =0;i<16;i++){
      msg[i] ^= exp[i];
    }
}
void AES_encrypt(unsigned char* msg){
  unsigned char *sample = (unsigned char*)malloc(16);
  int i =0 ; 
  memset(sample,0,16);
	for(i = 0 ;i <strlen((const char*)msg);i++){
			sample[i] = msg[i];
	}
  add_round_key(sample);
	
  //do an actual AES encryption
  for(int i =0 ;i <9;i++){   
    sub_bytes(sample);
    shift_rows(sample);
    mix_cols(sample);
    add_expanded_key(sample,expanded+(16*(i+1)));
  }
 
  sub_bytes(sample);
  shift_rows(sample);
  add_expanded_key(sample,expanded+160);
 

  for(int i =0 ;i<16;i++){
    msg[i] = sample[i];
  }



}
void AES_decrypt(unsigned char * c){


  unsigned char *crypt= (unsigned char*)malloc(16);
  for(int i =0;i<16;i++){
    crypt[i] = c[i];
  }
   add_expanded_key(crypt,expanded+160);

    rshift_rows(crypt);
    rsub_bytes(crypt);
    for(int i = 0;i<9;i++){
      add_expanded_key(crypt,expanded+(160-((i+1)*16)));
      rmix_cols(crypt);
      rshift_rows(crypt);
      rsub_bytes(crypt);
    }
    add_round_key(crypt);

//    exit(1);  

  for(int i =0;i<16;i++){
    c[i] = crypt[i];
  }

}
void KeyExpansionCore(unsigned char* in, unsigned char i)
{
	//rotate left
	//| 0 | 1 | 2 | 3 | => | 1 | 2 | 3 | 0 |
	unsigned char t = in[0];
	in[0] = in[1];
	in[1] = in[2];
	in[2] = in[3];
	in[3] = t;

	//S-Box Four Bytes
	//Substitue each byte for corresponding S-Box value
	in[0] = s_box[in[0]];
	in[1] = s_box[in[1]];
	in[2] = s_box[in[2]];
	in[3] = s_box[in[3]];

	//RCon
	//First byte is XORd with corresponding RCon value ( i = rconCounter which is incremented for each key expansion core call ie every 16 bytes of expanded key)
	in[0] ^= rcon[i];

}

void expand_keys(){
  for(int i =0 ;i<16;i++){
    expanded[i] = round_key[i];
  }
  unsigned char t[4];
  int r =1;
  int bytes =16;
  while(bytes< 176)
  {
      for(int i = 0;i<4;i++){
        t[i] = expanded[i+bytes-4]; 
      }
      if(bytes % 16 ==0){
        KeyExpansionCore(t,r);
        r++;
      }
      for(unsigned char a =0 ;a<4;a++){
        expanded[bytes]= expanded[bytes-16] ^ t[a];
        bytes++;        
      }

  } 
}
unsigned char* AES_E(unsigned char* msg){

    AES_encrypt(msg);
  
    return msg;   
}

unsigned char* AES_D(unsigned char* msg){
    AES_decrypt(msg);
    return msg;   
}

void AES_init(unsigned char*key){
      round_key=(unsigned char*)malloc(16);
      for(int i=0;i<16;i++){
        round_key[i] = key[i];
      }   
      //printf("\n**** AES key initialized :%s",key);
  
 //   if(done == false){
      expand_keys();
}

