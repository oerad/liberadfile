#include "liberadfile/liberadfile.h"
#include "liberad/liberad.h"
#include <iostream>
#include <thread>


using namespace std;
using namespace liberad;

#define IN_BUFFER_SIZE 1024*8
#define OUT_BUFFER_SIZE 8*8
static uint8_t in_buffer[IN_BUFFER_SIZE];
static uint8_t out_buffer[OUT_BUFFER_SIZE];


Oeradar* active_gpr = nullptr;
LiberadFile* file = nullptr;;
EradTraceHeader* t_h = nullptr;

atomic<bool> running;
atomic<bool> logging;
int trace_count;

void user_input();
void data_in(unsigned char* buffer, int received, signed char steps);
void data_out(unsigned char* buffer, int sent);
void start_logging();
void stop_logging();
void print_fh_info(EradFileHeader* f_h);
void print_th_info(EradTraceHeader* t_h);

int main(){
  //init connection to gpr
  liberad_init(LIBERAD_DEBUG);
  std::vector<Oeradar*> valid_devices;
  int count = liberad_get_valid_devices(&valid_devices);
  if (count <= 0){
    liberad_exit();
    return 1;
  }
  active_gpr = valid_devices.at(0);
  liberad_connect_to_device(active_gpr);

  //set gpr connection params
  liberad_init_device(active_gpr);
  liberad_start_io_async(active_gpr, SHORT, LEVEL1, data_in, data_out, in_buffer, IN_BUFFER_SIZE, out_buffer, OUT_BUFFER_SIZE);
  running = true;


  //init worker threads
  thread user_input_thread(user_input);
  thread gpr_connection_thread(liberad_handle_io_async, active_gpr);
  gpr_connection_thread.join();
  user_input_thread.join();

  return 0;

}



//callback on new data from GPR
void data_in(unsigned char* buffer, int received, signed char steps){
  if (logging){
    t_h->trace_index = trace_count;
    t_h->sample_size = received;
    t_h->steps_per_trace = steps;
    t_h->trace_index_in_fold = trace_count;
    liberad_write_trace(file, t_h, buffer);
    trace_count++;
  }
}

//callback on outbound data to GPR
void data_out(unsigned char* buffer, int sent){

}

//start logging gpr data to file
void start_logging(){
  cout << "start recording " << endl;
  //open file for write
  const char* filename = "generated_001.erad";
  file = new LiberadFile();
  liberad_open_file(file, filename, LiberadFile::LIBERAD_WRITE);


  //set header params
  EradFileHeader f_header;
  f_header.hardware_version = static_cast<int8_t>(liberad::POST2017);
  f_header.radar_type = static_cast<int16_t>(liberad::CONCRETTO);
  f_header.year = static_cast<int16_t>(2019);
  f_header.month = static_cast<int16_t>(2);
  f_header.day = static_cast<int16_t>(18);
  f_header.dimension = static_cast<int16_t>(SINGLE_SLICE_TEMPORAL);
  f_header.data_offset = static_cast<int16_t>(FH_SIZE);
  f_header.time_window = static_cast<float>(7.5);
  f_header.total_x = 0;
  f_header.total_y = 0;
  f_header.sample_size = 585;
  f_header.coordinate_system = static_cast<int16_t>(liberad::LOCAL);
  f_header.dielectric_coeff = static_cast<float>(1);
  f_header.interval_x = 0;
  f_header.interval_y = 0;

  //write header to file
  liberad_write_file_header(file, &f_header);

  //init trace header struct
  t_h = new EradTraceHeader();

  trace_count = 0;
  logging = true;
}

//stop logging gpr data to file
void stop_logging(){
  logging = false;
  // finish file write (writes total trace count to file)
  liberad_finish_write(file);

  // close file
  liberad_close_file(file);

  //clear resources
  delete file;
  delete t_h;
  file = nullptr;
  t_h = nullptr;
}

void check_file(){
  if (file != nullptr){
    cout << "file not null" << endl;
    return;
  }
  const char* fn = "generated_001.erad";
  file = new LiberadFile();
  liberad_open_file(file, fn, LiberadFile::LIBERAD_READ);
  if (!liberad_check_file(file)){
    cout << "file not compatible" << endl;
    delete file;
    file = nullptr;
    return;
  }

  EradFileHeader f_header;
  liberad_get_file_info(file, &f_header);

  printf ("Location: %s \n", file->filename);
  printf ("Size: %ld \n", file->file_size);
  printf ("version: %hhd \n", file->file_ver);
  printf ("total trace count: %ld \n", file->trace_count);

  print_fh_info(&f_header);

// cleanup resources
  liberad_close_file(file);
  delete file;
  file = nullptr;
}

void print_fh_info(EradFileHeader* f_h){

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

// Get user input from terminal
void user_input(){
  while(running){
    string temp;
    while(getline(cin, temp, '\n')){
      if (temp == "r"){
        //start logging
        start_logging();
      }
      if (temp == "x"){
        //stop logging
        stop_logging();
      }
      if (temp == "s"){
        //short time window
        liberad_set_time_window_async(active_gpr, SHORT);

      }
      if (temp == "l"){
        //long time window
        liberad_set_time_window_async(active_gpr, LONG);

      }
      if (temp == "1"){
        //gain level 1
        liberad_set_gain_async(active_gpr, LEVEL1);

      }
      if (temp == "2"){
        //gain level 2
        liberad_set_gain_async(active_gpr, LEVEL2);

      }
      if (temp == "3"){
        //gain level 3
        liberad_set_gain_async(active_gpr, LEVEL3);

      }
      if (temp == "4"){
        //gain level 4
        liberad_set_gain_async(active_gpr, LEVEL4);

      }
      if (temp == "5"){
        //gain level 5
        liberad_set_gain_async(active_gpr, LEVEL5);

      }
      if (temp == "c"){
        //check file we just wrote
        check_file();
      }
      if (temp == "q"){
        //quit
        running = false;
        if (logging){
          stop_logging();
        }

        liberad_stop_io(active_gpr);
        liberad_disconnect_device(active_gpr);
        liberad_exit();

        delete active_gpr;

        break;
      }
    }
  }
}
