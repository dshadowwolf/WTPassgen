#ifndef USERSPEC_PLUGIN_H
#include <string>
#include <list>
#include <fstream>
#include "plugin.h"
#include "config.h"

class userspec: public Plugin
{
 public:
   userspec();
   std::list<algo *> algorithms() { return m_algorithms; };
   std::string plugin_name() { return std::string("UserSpec"); };
   std::string generate();
   unsigned int count() { return m_algorithms.size(); };
   void setConfig( ConfigData *cs );
   void dumpConfig() { config->dumpConfig(); }
	
 private:
   std::string do_urandom( std::string valid, unsigned short len );
   std::string do_random( std::string valid, unsigned short len );
   std::list<algo *> m_algorithms;
   void init_algorithms();
   ConfigData *config;
   void init_args();
   std::list<c_l_arg> m_args;
};

#define USERSPEC_PLUGIN_H
#endif

	
	
	
