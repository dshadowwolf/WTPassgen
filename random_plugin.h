#ifndef RANDOM_PLUGIN_H
#include <string>
#include <list>
#include "plugin.h"
#include <gsl/gsl_rng.h>
#include "config.h"

class random_plugin: public Plugin
{
 public:
   random_plugin();
   std::list<algo *> algorithms() { return m_algorithms; };
   std::string plugin_name() { return std::string("RadomAlgos"); };
   std::string generate( );
   unsigned int count() { return m_algorithms.size(); };
   void setConfig( ConfigData *cs ) { config = cs; };
   void dumpConfig() { config->dumpConfig(); }
	
 private:
   std::list<algo *> m_algorithms;
   std::string Generate( const gsl_rng_type *type, unsigned short len );
   std::string do_random( const char w, unsigned short len );
   std::string do_gcrypt( unsigned short len );
   void init_algorithms();
   unsigned long int seed;
   ConfigData *config;
   void init_args();
};

#define RANDOM_PLUGIN_H
#endif

	
	
	
