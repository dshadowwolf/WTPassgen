#ifndef PLUGIN_H
#include <list>
#include <string>
#include "config.h"
typedef struct algo {
   std::string name;
   std::string tip;
   unsigned short len;
} algo;

class Plugin {
 public:
   virtual ~Plugin() {};
   virtual std::list<algo *> algorithms() = 0;
   virtual std::string plugin_name() { return std::string("This is odd - a virtual function was called."); };
   virtual std::string generate( ) = 0;
   virtual void setConfig( ConfigData * ) = 0;
   virtual unsigned int count() = 0;
   virtual void dumpConfig() = 0;
};

typedef  Plugin * init_t();

#define PLUGIN_H
#endif
