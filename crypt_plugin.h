#ifndef CRYPT_PLUGIN_H
#include <string>
#include <list>
#include "plugin.h"
#include "config.h"

class crypt_plugin: public Plugin
{
 public:
   crypt_plugin( );
   std::list<algo *> algorithms() { return m_algorithms; };
   std::string plugin_name() { return std::string("CryptPlug"); };
   std::string generate( );
   unsigned int count() { return m_algorithms.size(); };
   void setConfig( ConfigData *cs ) { config = cs; };
   void dumpConfig() { config->dumpConfig(); }
   
 private:
   std::list<algo *> m_algorithms;
   std::string do_crypto( unsigned short len );
   std::string do_sha( unsigned short which, unsigned short len );
   std::string do_leet( unsigned short len );
   std::list<std::string> dictionary;
   void init_algorithms();
   ConfigData *config;
   void init_args();
};

#define CRYPT_PLUGIN_H
#endif

	
	
	
