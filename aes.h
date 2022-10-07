
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#ifndef AES_H
#define AES_H

void add_round_key( unsigned char *msg);
void  sub_bytes(unsigned char*msg);
void  rsub_bytes(unsigned char*msg);
void  shift_rows(unsigned char* state);
void  rshift_rows(unsigned char*a);
void mix_cols(unsigned char* state);
void rmix_cols(unsigned char* state);
void add_expanded_key(unsigned char* msg,unsigned char*exp);
void AES_encrypt(unsigned char* msg);
void AES_decrypt(unsigned char * c);
void KeyExpansionCore(unsigned char* in, unsigned char i);
void expand_keys();
unsigned char* AES_E(unsigned char* msg);
unsigned char* AES_D(unsigned char* msg);
void AES_init(unsigned char*key);

#endif 