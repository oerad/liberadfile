#include "liberadfile/liberadfile.h"
#include "liberad/liberad.h"
#include <iostream>
#include <thread>
#include <math.h>


using namespace std;
using namespace liberad;

#define IN_BUFFER_SIZE 1024*8
#define OUT_BUFFER_SIZE 8*8
static uint8_t in_buffer[IN_BUFFER_SIZE];
static uint8_t out_buffer[OUT_BUFFER_SIZE];


Oeradar* active_gpr = nullptr;
SegyFile* file = nullptr;
SegyTraceHeader* t_h = nullptr;

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
    // set necessary fields for segy trace header
    t_h->traceSequenceNumInLine = trace_count;
    t_h->traceSequenceNumInFile = trace_count;
    t_h->originalRecordNum = trace_count;
    t_h->ensembleNum = trace_count;
    t_h->numSamples = received;

    // record time
    // t_h->hour = hour;
    // t_h->minute = minute;
    // t_h->second = second;

    //write trace data
    liberad_write_segy_trace(file, t_h, buffer, received);

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
  const char* filename = "generated_001.sgy";
  file = new SegyFile();
  liberad_open_segy_file_w(file, filename);

  //Segy Textual header section. This example uses an erad header and then the helper function
  //liberad_produce_segy_txt_header() which takes an erad header and translates it to a txt header
  char* segy_txt_header = new char[SEGY_TXT_HEADER_SIZE];
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

  liberad_produce_segy_txt_header(&f_header, segy_txt_header, SEGY_TXT_HEADER_SIZE);
  //write txt header to file
  liberad_write_segy_txt_h(file, segy_txt_header);
  //free up resources
  delete segy_txt_header;

  // segy binary header section
  SegyBinaryHeader bin_header;

  // set necessary fields - here for convenience the erad file header fields are used
  bin_header.jobID = 1;
  bin_header.sampleIntervalUs = static_cast<int16_t>(round(f_header.time_window / 0.585)); //somehow this works
  bin_header.samplesPerDataTrace = f_header.sample_size;
  // this is the data format code. BYTE_1_UNS was added in the latest revision of the segy standard.
  // We've noted that not all packages are compatible with the latest standard. If another format is used then
  // incoming data in the data_in(..) callback should be cast to a suitable format. 
  bin_header.formatCode = SegyBinaryHeader::BYTE_1_UNS;

  //write binary header to file
  liberad_write_segy_bin_h(file, &bin_header);

  //init trace header struct for reuse on incoming data from gpr
  t_h = new SegyTraceHeader();

  // init logging params
  trace_count = 0;
  logging = true;
}

//stop logging gpr data to file
void stop_logging(){
  logging = false;

  // close file
  liberad_close_segy_file_w(file);

  //clear resources
  delete file;
  delete t_h;
  file = nullptr;
  t_h = nullptr;
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
