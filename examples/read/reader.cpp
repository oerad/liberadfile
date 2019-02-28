#include "liberadfile/liberadfile.h"
#include <iostream>

using namespace std;
using namespace liberad;

void print_fh_info(EradFileHeader* f_h);
void print_th_info(EradTraceHeader* t_h);

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

  //populate a file header struct and get info about file
  EradFileHeader f_header;
  liberad_get_file_info(&file, &f_header);
  print_fh_info(&f_header);

  //get trace data
  EradTraceHeader t_header;
  uint8_t* data = new uint8_t[f_header.sample_size];
  // for (int i = 0; i < f_header.trace_count; i++){  // too verbose
  for (int i = 0; i < 3; i++) { // illustrative example
    liberad_get_trace_at(&file, i, &t_header, data);
    print_th_info(&t_header);
  }

  // clear resources
  delete data;

  // close file
  liberad_close_file(&file);

}

// prints info from file header
void print_fh_info(EradFileHeader* f_h){

  cout << "FILE HEADER INFO:" << endl;
  printf ("file_version: %hhi \n", f_h->file_version);
  printf ("endianness_marker 1: %hhx \n", f_h->endianness_marker[0]);
  printf ("endianness marker 2: %hhx \n", f_h->endianness_marker[1]);
  printf ("hardware_version: %hhd \n", f_h->hardware_version);
  printf ("radar_type: %hd \n", f_h->radar_type);
  printf ("year: %hd \n", f_h->year);
  printf ("month: %hd \n", f_h->month);
  printf ("day: %hd \n", f_h->day);
  printf ("dimension: %hd \n", f_h->dimension);
  printf ("data_offset: %hd \n", f_h->data_offset);
  printf ("time_window: %f \n", f_h->time_window);
  printf ("total_x: %f \n", f_h->total_x);
  printf ("total_y: %f \n", f_h->total_y);
  printf ("sample_size: %hd \n", f_h->sample_size);
  printf ("CoordinateSystem: %hd \n", f_h->coordinate_system);
  printf ("Dielectric: %f \n", f_h->dielectric_coeff);
  printf ("interval_x: %f \n", f_h->interval_x);
  printf ("interval_y: %f \n", f_h->interval_y);
  printf ("scan_operator: %s \n", f_h->scan_operator);
  printf ("Location: %s \n", f_h->location);

  printf("------------- \n");

}

// prints info from trace header 
void print_th_info(EradTraceHeader* t_h){

  printf("trace index: %ld \n", t_h->trace_index);
  printf("sample_size: %hd \n", t_h->sample_size);
  printf("steps per t: %hd \n", t_h->steps_per_trace);
  printf("hour: %hhd \n", t_h->hour);
  printf("minute: %hhd \n", t_h->minute);
  printf("second: %hd \n", t_h->second);
  printf("millisecond: %d \n", t_h->millisecond);
  printf("fold index: %d \n", t_h->fold_index);
  printf("fold Orien: %hhd \n", t_h->fold_orientation);
  printf("trace_index_in_fold: %d \n", t_h->trace_index_in_fold);
  printf("x_local: %f \n", t_h->x_local);
  printf("y_local: %f \n", t_h->y_local);
  printf("z_local: %f \n", t_h->z_local);
  printf("longitude: %f \n", t_h->longitude);
  printf("latitude: %f \n", t_h->latitude);

  printf("------------- \n");

}
