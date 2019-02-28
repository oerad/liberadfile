#include "liberadfile/liberadfile.h"
#include <iostream>

using namespace std;
using namespace liberad;


int main(){

  //open file for read
  LiberadFile file;
  const char* filename = "003.erad";
  liberad_open_file(&file, filename, LiberadFile::LIBERAD_READ);

  //check if file is compatible
  if (!liberad_check_file(&file)){
    cout << "file not compatible" << endl;
    return 1;
  }

  // give new segy file a name
  const char* segy_dest = "export_003.sgy";
  //export file to segy
  liberad_export_to_segy(&file, segy_dest);

  // close file
  liberad_close_file(&file);

}
