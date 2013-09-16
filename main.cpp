#include <dlfcn.h>
#include <iostream>
#include <iomanip>
#include <list>
#include <cstdio>
#include <cstdlib>
#include "loader.h"
#include "plugin.h"
#include "config.h"

using namespace std;
list<c_l_arg> clargs;
c_l_arg m_args[] = {
  { "algo", 'a', true, "core.algo", "select which algorithm to use to generate the password", TYPE_STRING },
  { "length", 'l', true, "core.length", "set the length of the password to generate", TYPE_INT },
  { "list", 't', false, "LIST", "list the algorithms currently known by the loaded plugins" },
  { "help", 'h', false, "HELP", "give command-line help (ie: this screen)" },
  { "dump", 'd', false, "DUMP", "dump the config database and quit" }
};

int main( int argc, char *argv[]) {
  list<algo *> algos, t;
  list<void *> ps;
  Loader k;
  ConfigData l;
   
  k.loadDir( "/usr/lib/passgen" );
  l.setCommandLine( argc, argv );
  l.addKey("core.algo",string("Random"));
  l.addKey("core.length",8);
  l.load( string( getenv("HOME") ) + string("/.passgenrc") );
  ps = k.plugins();

  for( int i = 0; i < 5; i++ )
    clargs.push_back( m_args[i] );

  for( list<void *>::iterator x = ps.begin(); x != ps.end(); x++ ) {
    Plugin *cb = (Plugin *)(*x);
    t = cb->algorithms();
    for( list<algo *>::iterator y = t.begin(); y != t.end(); y++ ) {
      algos.push_back((algo *)(*y));
    }
  }

   for( list<void *>::iterator x = ps.begin(); x != ps.end(); x++ ) {
      Plugin *cb = (Plugin *)(*x);
      cb->setConfig( &l );
   }
 
   for( list<c_l_arg>::iterator x = clargs.begin(); x != clargs.end(); x++ )
     l.addOption( (*x) );
  l.parseCommandLine();
  
  if( l.hasKey( "HELP" ) ) {
    // -h or --help were specified
    // or an unkown option was found on the command-line
    cout << argv[0] << "\tUsage:" << endl << "\t" << argv[0] << " <args>" << endl;
    cout << "Arguments with the following plugins loaded" << endl;
    ps = k.plugins();
    for( list<void *>::iterator x = ps.begin(); x != ps.end(); x++ ) {
      Plugin *cb = (Plugin *)(*x);
      cout << "\t" << cb->plugin_name() << endl;
    }
     list<c_l_arg> asdf = l.getOptions();
    for( list<c_l_arg>::iterator x = asdf.begin(); x != asdf.end(); x++ )
      cout << "--" << (*x).long_opt << " (-" << (*x).short_opt << ")\t" << (*x).option_help << endl;
    return 0;
  }

  if( l.hasKey( "LIST" ) ) {
    cout << internal << setw(10) << "Name" << setw(5) << " ";
    cout << internal << setw(15) << "Maximum Length" << setw(5) << " ";
    cout << internal << setw(45) << "Description" << endl;
    cout << internal << setw(10) << setfill('-') << "-";
    cout << internal << setw(5) << setfill(' ') << " " << setw(15) << setfill('-') << "-";
    cout << internal << setw(5) << setfill(' ') << " " << setw(45) << setfill('-') << "-" << endl;
    for( list<algo *>::iterator x = algos.begin(); x!= algos.end(); x++ ) {
      cout << internal << setfill(' ') << setw(10) << (*x)->name << setw(5) << " ";
      cout << internal << setfill(' ') << setw(15) << (*x)->len << setw(5) << " ";
      cout << internal << setfill(' ') << setw(45) << (*x)->tip << endl;
    }
    return 0;
  }

  if( l.hasKey("DUMP") ) {
     l.dumpConfig();
     return 0;
  }
   
  bool found = false;
  for( list<void *>::iterator x = ps.begin(); x != ps.end(); x++ ) {
    Plugin *cb = (Plugin *)(*x);
    t = cb->algorithms();
    for( list<algo *>::iterator y = t.begin(); y != t.end(); y++ ) {
      if( (*y)->name == l.getString("core.algo") && !found ) {
        found = true;
        cout << "Password (" << l.getString("core.algo") << "): " << cb->generate( ) << endl;
      }
    }
  }

  if( !found )
    cerr << "Invalid algorithm " << l.getString("core.algo") << endl;

  return 0;
}
