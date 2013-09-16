#include "crypt_plugin.h"
#include <gcrypt.h>
#include <string>
#include <list>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "config.h"

crypt_plugin::crypt_plugin( )
{
   if( !gcry_check_version(GCRYPT_VERSION) ) {
      std::cerr << "LIBGCRYPT version " << GCRYPT_VERSION << " required, " << gcry_check_version(NULL) << " found" << std::endl;
   }
   
   init_algorithms();
};

void crypt_plugin::init_algorithms() 
{
   for( int i = 0; i < 3; i++ ) {
      algo *t = new algo;
      switch(i) {
       case 0:
	 t->name = "Blowfish";
	 t->tip  = "Blowfish - random data, random key";
	 t->len  = 128;
	 break;
       case 1:
	 t->name = "Sha-512";
	 t->tip  = "Sha-512 of random data";
	 t->len  = 64;
	 break;
       case 2:
	 t->name = "Sha-256";
	 t->tip  = "Sha-256 of random data";
	 t->len  = 32;
	 break;
      }
      m_algorithms.push_back( t );
   }
}

std::string crypt_plugin::generate( ) 
{
   std::string name = config->getString("core.algo");
   unsigned short len = config->getInt("core.length");
   if( name == "Blowfish")  {
     return do_crypto(len);
   } else if( name == "Sha-256")  {
     return do_sha(256,len);
   } else if( name == "Sha-512")  {
     return do_sha(512,len);
   }
   return "System Error - unknown algo ";
}

std::string crypt_plugin::do_crypto( unsigned short len )
{
   char *b, *r, *buff;
   std::string out;
   gcry_cipher_hd_t hand;

   gcry_cipher_open( &hand, GCRY_CIPHER_BLOWFISH, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS );
   gcry_cipher_setiv( hand, NULL, 0 );
   
   b = new char[16];
   gcry_randomize( b, 16, GCRY_STRONG_RANDOM );
   gcry_cipher_setkey( hand, b, 16 );
   
   buff = new char[len];
   r = new char[len+1];
   gcry_randomize( r, len, GCRY_STRONG_RANDOM );
   gcry_cipher_encrypt( hand, buff, len, r, len );
   
   delete b;
   memset( r, 0, len+1 );
   for( unsigned int i = 0; i < len; i++ ) {
      int k = 1 + (int) (92.0 * ((float)buff[i] / 256.0));
      if( k < 0 )
	k *= -1;
      r[i] = k+34;
   }
   gcry_cipher_close(hand);
   out = r;
   delete buff;
   delete r;
   return out;
}

std::string crypt_plugin::do_sha( unsigned short which, unsigned short len ) {
   char *work_a, *work_b, *work_c;
   std::string out;
   int alg;
   work_a = new char[which];
   work_b = new char[which];
   work_c = new char[len];
   alg = 0;
   gcry_randomize( work_a, which, GCRY_STRONG_RANDOM );
   switch( which ) {
    case 256:
      alg = GCRY_MD_SHA256;
      break;
    case 512:
      alg = GCRY_MD_SHA512;
      break;
   }
   
   gcry_md_hash_buffer( alg, work_b, work_a, which );
   delete work_a;
   for( unsigned int x = 0; x < len; x++ ) {
      int k = 1 + (int) (92.0 * ((float)work_b[x] / 256.0));
      if( k < 0 )
	k *= -1;
      work_c[x] = k+34;
   }
	
   delete work_b;
   out = work_c;
   delete work_c;
   return out;
}

extern "C" Plugin * init_plugin() {
   return dynamic_cast<Plugin *>(new crypt_plugin());
}   
