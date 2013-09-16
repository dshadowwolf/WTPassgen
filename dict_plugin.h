#ifndef DICT_PLUGIN_H
#include <string>
#include <list>
#include "plugin.h"
#include "config.h"

class dict_plugin: public Plugin
{
 public:
   dict_plugin( );
   std::list<algo *> algorithms() { return m_algorithms; };
   std::string plugin_name() { return std::string("DictPlug"); };
   std::string generate( );
   unsigned int count() { return m_algorithms.size(); };
   void setConfig( ConfigData *cs ) { config = cs; };
   void dumpConfig() { config->dumpConfig(); }
   
 private:
   std::list<algo *> m_algorithms;
   std::string do_dict( unsigned short words );
   std::list<std::string> dictionary;
   std::string get_word();
   void init_algorithms();
   ConfigData *config;
   void init_args();
};

#define CRYPT_PLUGIN_H
#endif
