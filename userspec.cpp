#include "userspec.h"
#include <fstream>
#include <iostream>
#include "config.h"
#include <cstdlib>
#include <list>

userspec::userspec( ) 
{
   unsigned int seed = time(NULL);
   srandom( seed );
   init_args();
   init_algorithms();
}

void userspec::init_args() {
   c_l_arg t[] = { 
	{ "validchars", 'v', true, "userspec.validchars", "Characters to use to generate password", TYPE_STRING },
	{ "randsource", 'r', true, "userspec.randomsource", "Source of randomness (U/u/urandom == /dev/urandom, R/r/rand == libc rand() function", TYPE_STRING }
      };
   m_args.push_front( t[0] );
   m_args.push_front( t[1] );
}

void userspec::setConfig( ConfigData *cs ) 
{
   config = cs;
   for( std::list<c_l_arg>::iterator x = m_args.begin(); x != m_args.end(); x++ )
     cs->addOption( (*x) );
}

void userspec::init_algorithms() {
   algo *t = new algo;
   t->name = "UserSpec";
   t->tip = "User Specified Valid Characters";
   t->len = 65535;
   m_algorithms.push_back( t );
}

std::string userspec::generate( ) {
   unsigned short len = config->getInt("core.length");
   std::string valid;
   std::string source;
   if( config->hasKey("userspec.validchars") )
     valid = config->getString("userspec.validchars");
   else {
      valid = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()-_=+`~'\";:/?.>,<[]{}\\|";
      config->addKey( "userspec.validchars", valid );
   }
   
   if( config->hasKey("userspec.randomsource") )
     source = config->getString("userspec.randomsource");
   else {
      source = "rand";
      config->addKey( "userspec.randomsource", source );
   }

   std::string rv;
   switch( source[0] ) {
    case 'U':
    case 'u':
      rv = do_urandom( valid, len );
      break;
    case 'R':
    case 'r':
      rv = do_random( valid, len );
      break;
    default:
      rv =std::string("Algo is fscked!");
   }
   return rv;
}

std::string userspec::do_random( std::string valid, unsigned short len ) {
   std::string out("");
   
   for( unsigned int x = 0; x < len; x++ ) {
      int oc;
      int l = valid.length();
      oc = 1 + (int)((float)l * ((float)((char)rand())/255));
      
      if( oc < 0 )
	oc *= -1;
      out += valid[oc];
   }

   return out;
}

std::string userspec::do_urandom( std::string valid, unsigned short len ) {
   std::string out("");
   std::ifstream ur( "/dev/urandom", std::ios::in | std::ios::binary );
   
   for( unsigned int x = 0; x < len; x++ ) {
      char cc;
      int oc;
      int l = valid.length();
      ur.read( &cc, 1 );
      oc = 1 + (int)((float)l * ((float)(cc)/255));
      
      if( oc < 0 )
	oc *= -1;
      out += valid[oc];
   }
   ur.close();
   return out;
}

extern "C" Plugin * init_plugin() {
   return dynamic_cast<Plugin *>(new userspec());
}   
