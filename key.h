#ifndef _CONFIG_KEY_H_
#include <string>

class ConfigKey {
 public:
   ConfigKey();
   ConfigKey( bool );
   ConfigKey( int );
   ConfigKey( std::string );
   ConfigKey( float );
   
   bool toBool();
   int toInt();
   float toFloat();
   std::string toString();
   
   enum KeyType {
      CK_TYPE_BOOL = 0,
      CK_TYPE_INT,
      CK_TYPE_STRING,
      CK_TYPE_FLOAT,
      CK_TYPE_BAD_TYPE
   };
   
   enum KeyType storedDataType();
 private:
   enum KeyType typeStored;
   std::string sdata;
   int idata;
   bool bdata;
   float fdata;
};

#define _CONFIG_KEY_H_
#endif
