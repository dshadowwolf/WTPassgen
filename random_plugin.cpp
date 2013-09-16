#include "random_plugin.h"
#include <gsl/gsl_rng.h>
#include <gcrypt.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "config.h"

random_plugin::random_plugin( ) 
{
   seed = time(NULL);
   srandom( seed );
   if( !gcry_check_version(GCRYPT_VERSION) ) {
      std::cerr << "LIBGCRYPT version " << GCRYPT_VERSION << " required, " << gcry_check_version(NULL) << " found" << std::endl;
   }
   init_algorithms();
}
	
void random_plugin::init_algorithms() {
   for( int i = 0; i < 6; i++ ) {
      algo *t = new algo;
      switch(i) {
       case 0:
	 t->name = "Mersenne";
	 t->tip = "Mersenne Twister RNG";
	 t->len = 65535;
	 break;
       case 1:
	 t->name = "L'Ecuyer";
	 t->tip = "L'Ecuyer's CMRG RNG";
	 t->len = 65535;
	 break;
       case 2:
	 t->name = "Tausworthe";
	 t->tip = "Tausworthe's RNG";
	 t->len = 65535;
	 break;
       case 3:
	 t->name = "GCrypt";
	 t->tip  = "libgcrypt's \"secure\" RNG";
	 t->len  = 65535;
	 break;
       case 4:
	 t->name = "Random";
	 t->tip = "libc's RNG";
	 t->len = 65535;
	 break;
       case 5:
	 t->name = "URandom";
	 t->tip = "/dev/urandom Entropy/Randomness";
	 t->len = 65535;
	 break;
      }
      m_algorithms.push_back( t );
   }
}

std::string random_plugin::generate( ) {
   std::string name = config->getString("core.algo");
   unsigned short len = config->getInt("core.length");
   if( name == "Mersenne" )
     return Generate( gsl_rng_mt19937, len );
   else if( name == "L'Ecuyer" )
     return Generate( gsl_rng_cmrg, len );
   else if( name == "Tausworthe" )
     return Generate( gsl_rng_taus2, len );
   else if( name == "GCrypt" )
     return do_gcrypt( len );
   else if( name == "Random" )
     return do_random( 'R', len );
   else if( name == "URandom" )
     return do_random( 'U', len );
   return std::string("Algo is fucked");
}

std::string random_plugin::Generate( const gsl_rng_type *type, unsigned short len ) {
   char *buff;
   buff = new char[len+1];
   std::string out;
   gsl_rng *r;
   memset( buff, 0, len+1 );
   gsl_rng_env_setup();
   r = gsl_rng_alloc( type );
   seed = time(NULL);
   gsl_rng_set( r, seed );
   for( unsigned int x = 0; x < len; x++ ) {
      int oc = 1 + (int)(92.0 * gsl_rng_uniform(r));
      if( oc < 0 )
	oc *= -1;
      buff[x] = oc+34;
   }
   gsl_rng_free(r);
   out = buff;
   delete buff;
   return out;
}

std::string random_plugin::do_gcrypt( unsigned short len ) {
   char *work_a, *work_b;
   std::string out;
   work_a = new char[len+1];
   work_b = new char[len+1];
   
   memset( work_a, 0, len+1 );
   memset( work_b, 0, len+1 );
   gcry_randomize( work_a, len, GCRY_STRONG_RANDOM );
   for( unsigned int x = 0; x < len; x++ ) {
      int k = 1 + (int) (92.0 * ((float)work_a[x] / 256.0));
      if( k < 0 )
	k *= -1;
      work_b[x] = k+34;
   }
   
   out = work_b; 
   delete work_a;
   delete work_b;
   return out;
}

std::string random_plugin::do_random( const char w, unsigned short len ) {
   std::ifstream f("/dev/urandom", std::ios::in | std::ios::binary);
   char *b;
   std::string out;
   b = new char[len+1];
   
   if( w == 'U' )
      if( !f.is_open() )
	 return "Unable to open /dev/urandom";
   
   memset( b, 0, len+1 );
   for( unsigned int x = 0; x < len; x++ ) {
      char cc;
      int oc;
      if( w == 'U' ) {
	 f.read(&cc,1);
	 oc = 1 + (int)(92.0 * ((float)cc/255));
      } else {
	 oc = 1 + (int)(92.0 * ((float)((char)rand())/255));
      }
      
      if( oc < 0 )
	oc *= -1;
      b[x] = oc+34;
   }

   if( w == 'U' )
     f.close();
   out = b;
   delete b;
   return out;
}

extern "C" Plugin * init_plugin() {
   return dynamic_cast<Plugin *>(new random_plugin());
}   
