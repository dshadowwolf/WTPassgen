#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include "section.h"
#include "key.h"

using namespace std;

ConfigSection::ConfigSection() 
{
};

std::string ConfigSection::getString(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) ) {
	   return m_sections[b].getString(c);
	 } else {
	   return string("");
	 }
      } else {
	 if( !!m_keys.count(name) ) {
	    return m_keys[name].toString();
	 } else {
	    return string("");
	 }
      }
   } else {
      return string("");
   }
   return string("There is an error");
}
	
float ConfigSection::getFloat(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) )
	   return m_sections[b].getFloat(c);
	 else
	   return 0.0f;
      } else {
	 if( !!m_keys.count(name) )
	   return m_keys[name].toFloat();
	 else
	   return -0.0f;
      }
   } else {
      return 0.0f;
   }
   return -0.0f;
}

bool ConfigSection::getBool(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) )
	   return m_sections[b].getBool(c);
	 else
	   return false;
      } else {
	 if( !!m_keys.count(name) )
	   return m_keys[name].toBool();
	 else
	   return false;
      }
   } else {
      return false;
   }
   return false;
}

int ConfigSection::getInt(std::string name) {
   if( hasKey(name) ) {
      boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
      boost::smatch results;
      if( boost::regex_match( name, results, bitmatch ) ) {
	 string b(results[1].first,results[1].second);
	 string c(results[2].first,results[2].second);
	 if( !!m_sections.count(b) )
	   return m_sections[b].getInt(c);
	 else
	   return 0;
      } else {
	 if( !!m_keys.count(name) )
	   return m_keys[name].toInt();
	 else
	   return -1;
      }
   } else {
      return 0;
   } 
   return -1;
}

// propogate down until nextIsSection() returns false, then report
// whether we actually have the key or not
bool ConfigSection::hasKey( string name ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( m_sections.count(b) )
	return m_sections[b].hasKey( c );
      else
	return false;
   } else
     return !!m_keys.count(name);
}

// create a new string key with the given name and value
void ConfigSection::addKey( string name, string val ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, val );
      else {
	 ConfigSection d;
	 d.addKey( c, val );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(val);
}

// boolean key with defined value
void ConfigSection::addKey( string name, bool val ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, val );
      else {
	 ConfigSection d;
	 d.addKey( c, val );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(val);
}


// integer key with the defined value
void ConfigSection::addKey( string name, int val ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, val );
      else {
	 ConfigSection d;
	 d.addKey( c, val );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(val);
}


// float key with the defined value
void ConfigSection::addKey( string name, float val ) {
   boost::regex bitmatch( "^(\\w+)\\.(.*)$" );
   boost::smatch results;
   if( boost::regex_match( name, results, bitmatch ) ) {
      string b(results[1].first,results[1].second);
      string c(results[2].first,results[2].second);
      if( !!m_sections.count(b) )
	m_sections[b].addKey( c, val );
      else {
	 ConfigSection d;
	 d.addKey( c, val );
	 m_sections[b] = d;
      }
   } else
      m_keys[name] = ConfigKey(val);
}

void ConfigSection::save( fstream &f ) {   
   for( map<string,ConfigKey>::iterator x = m_keys.begin(); x != m_keys.end(); x++ ) {
      f << (*x).first << " " << (*x).second.toString() << endl;
   }
   
   for( map<string,ConfigSection>::iterator x = m_sections.begin(); x != m_sections.end(); x++ ) {
      f << "section " << (*x).first << endl;
      (*x).second.save( f );
      f << "endsection" << endl;
   }
   f << endl;
}

void ConfigSection::dump() {
   for( map<string,ConfigSection>::iterator x = m_sections.begin(); x != m_sections.end(); x++ ) {
      cout << "section " << (*x).first << endl;
      (*x).second.dump();
      cout << "endsection" << endl << endl;
   }
   cout << endl;
   for( map<string,ConfigKey>::iterator x = m_keys.begin(); x != m_keys.end(); x++ ) {
      cout << (*x).first << " == " << (*x).second.toString() << endl;
   }
   cout << endl;
}
