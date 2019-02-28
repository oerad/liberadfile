#include "../include/liberadfile.h"
#include <cstring>
#include <math.h>

using namespace std;
using namespace liberad;

EndiannessMarker system_endianness;

EndiannessMarker liberad_get_file_endianness(int8_t* header_endianness);

EndiannessMarker liberad_get_system_endianness();

std::string liberad_get_stream_mode(LiberadFile::Mode mode);

/* Shift fields in f_header from little to big endianness and vice versa.
* @param EradFileHeader* f_header - pointer to file header for endianness conversion
*/
void liberad_shift_file_header_endianness(EradFileHeader* f_header);

void liberad_shift_trace_header_ver1_endianness(EradTraceHeader_VER_1* t_header);

void liberad_shift_trace_header_ver2_endianness(EradTraceHeader* t_header);


LiberadFile::LiberadFile():
            f_header(nullptr),
            mode(LIBERAD_APPEND),
            stream(nullptr),
            filename(nullptr),
            file_size(0),
            trace_count(0),
            is_open(false),
            is_valid(false){}

LiberadFile::LiberadFile(Mode mode_):
            f_header(nullptr),
            mode(mode_),
            stream(nullptr),
            filename(nullptr),
            file_size(0),
            trace_count(0),
            is_open(false),
            is_valid(false){}

LiberadFile::LiberadFile(Mode mode_, const char* filename_):
            f_header(nullptr),
            mode(mode_),
            stream(nullptr),
            filename(filename_),
            file_size(0),
            trace_count(0),
            is_open(false),
            is_valid(false){}

LiberadFile::~LiberadFile(){

}
/* ----------------------------------------------------------------------------------------------------------------- */


//done
int liberad_open_file(LiberadFile* efile, const char* filename, LiberadFile::Mode mode){
  if (filename == NULL || filename[0] == '\0'){
    cout << "filename is empty string " << endl;
    return ERROR;
  }

  efile->mode = mode;
  efile->filename = filename;

  return liberad_open_file(efile);
}

//done
int liberad_open_file(LiberadFile* efile){
  if (efile->filename == nullptr || efile->filename[0] == '\0'){
    cout << "no file location associated with this LiberadFile" << endl;
    return ERROR;
  }

  // get system endianness on first call - maybe put in init call
  system_endianness = liberad_get_system_endianness();

  string stream_type = liberad_get_stream_mode(efile->mode);
  efile->stream = fopen(efile->filename, stream_type.c_str());

  if (efile->stream == NULL){
    // std::cout << "File errors: " << ferror(efile->stream) << std::endl;
    cout << "could not open file " << endl;
    return ERROR;
  }
  efile->is_open = true;
  return SUCCESS;
}
/* ----------------------------------------------------------------------------------------------------------------- */

//done
bool liberad_check_file(LiberadFile* efile){
  if (!efile->is_open){
    cout << "File not opened" << endl;
    return false;
  }

  int8_t magic[8] = {0x00, 0x45, 0x41, 0x53, 0x59, 0x52, 0x41, 0x44};
  int8_t buffer[8];
  fseek(efile->stream, 0, SEEK_SET);
  fread(&buffer, sizeof(int8_t), 8, efile->stream);

  int result = memcmp(buffer, magic, 8);
  if (result != 0){
    cout << "Header bytes do not match pattern" << endl;
    liberad_close_file(efile);
    return false;
  }
  fread(&efile->file_ver, sizeof(int8_t), 1, efile->stream);
  efile->is_valid = true;

  return true;
}
/* ----------------------------------------------------------------------------------------------------------------- */

//done
void liberad_get_file_info(LiberadFile* efile, EradFileHeader* f_header){
  if (!(efile->is_open && efile->is_valid)){
    cout << "File not opened or valid"<< endl;
    return;
  }

  efile->endianness = liberad_read_file_header(efile->stream, f_header, efile->file_ver);
  efile->f_header = f_header;

  liberad_get_trace_count(efile);
  liberad_get_file_size(efile);
}


/* ----------------------------------------------------------------------------------------------------------------- */

//done
void liberad_get_trace_at(LiberadFile* efile, int64_t trace_index, EradTraceHeader* t_header, uint8_t* data){
  if (!efile->is_valid){
    cout << "File not compatible"<< endl;
    return;
  }

  long int index = liberad_get_trace_header_index_at(trace_index, efile->f_header->sample_size, efile->file_ver);

  liberad_read_trace_header(efile, index, t_header);

  fread(data, efile->f_header->sample_size, 1, efile->stream);

}

//done
void liberad_get_trace_header_at(LiberadFile* efile, int64_t trace_index, EradTraceHeader* t_header){
  if (!efile->is_valid){
    cout << "File not compatible"<< endl;
    return;
  }

  long int index = liberad_get_trace_header_index_at(trace_index, efile->f_header->sample_size, efile->file_ver);

  liberad_read_trace_header(efile, index, t_header);

}

//done
void liberad_get_trace_data_at(LiberadFile* efile, int64_t trace_index, uint8_t* data){
  if (!efile->is_valid){
    cout << "File not compatible"<< endl;
    return;
  }

  long int index = liberad_get_trace_data_index_at(trace_index, efile->f_header->sample_size, efile->file_ver);
  fseek(efile->stream, index, SEEK_SET);
  fread(data, efile->f_header->sample_size, 1, efile->stream);

}

//data buffer must be at least (index_end-index_start)*sample_size big
//done
void liberad_get_trace_data(LiberadFile* efile, int64_t index_start, int64_t index_end, uint8_t* data){
  if (!efile->is_valid){
    cout << "File not compatible"<< endl;
    return;
  }
  long int index;
  int sample_size = efile->f_header->sample_size;

  for (int i = index_start, j=0; i <= index_end; i++, j++){
    index = liberad_get_trace_data_index_at(index_start, sample_size, efile->file_ver);
    fseek(efile->stream, index, SEEK_SET);
    fread(&data[j*sample_size], sample_size, 1, efile->stream);
  }

}


// TODO
void liberad_get_trace_data(LiberadFile* efile, int64_t index_start, int64_t index_end, ostream* out_stream){
  if (!efile->is_valid){
    cout << "File not compatible"<< endl;
    return;
  }

  long int index;
  int sample_size = efile->f_header->sample_size;
  uint8_t* data_buffer = new uint8_t[sample_size];


  for (int i = index_start; i <= index_end; i++){
    index = liberad_get_trace_data_index_at(i, sample_size, efile->file_ver);
    fseek(efile->stream, index, SEEK_SET);
    fread(data_buffer, sample_size, 1, efile->stream);
    // out_stream->write(data_buffer, sample_size);
  }

  delete[] data_buffer;
}



/* ----------------------------------------------------------------------------------------------------------------- */

//
void liberad_write_file_header(LiberadFile* efile, EradFileHeader* f_header){
  if (!efile->is_open){
    cout << "File not opened"<< endl;
    return;
  }
  int8_t magic[8] = {0x00, 0x45, 0x41, 0x53, 0x59, 0x52, 0x41, 0x44};

  memcpy(f_header->magic_num, magic, 8);
  f_header->file_version = VER_2019;
  if (system_endianness == BIG_END){
    f_header->endianness_marker[0] = 0xFE;
    f_header->endianness_marker[1] = 0xFF;

  } else {
    f_header->endianness_marker[0] = 0xFF;
    f_header->endianness_marker[1] = 0xFE;
  }

  fwrite(f_header, FH_SIZE, 1, efile->stream);
  fflush(efile->stream);
}

//
void liberad_write_trace(LiberadFile* efile, EradTraceHeader* t_header, uint8_t* data){
  if (!efile->is_open){
    cout << "File not opened"<< endl;
    return;
  }
  fseek(efile->stream, liberad_get_trace_header_index_at(t_header->trace_index, t_header->sample_size, efile->file_ver), SEEK_SET);
  fwrite(t_header, TH_SIZE_VER_2, 1, efile->stream);
  fwrite(data, t_header->sample_size, 1, efile->stream);
  efile->trace_count++;
  fflush(efile->stream);

}

void liberad_finish_write(LiberadFile* efile){
  if (efile->trace_count == 0){
    cout << "nothing written to file" << endl;
    return;
  }
  fwrite(&efile->trace_count, sizeof(efile->trace_count), 1, efile->stream);
}

/* ----------------------------------------------------------------------------------------------------------------- */


void liberad_export_to_segy(LiberadFile* source, const char* destination){


  if (!(source->is_open && source->is_valid) ){
    cout << "source file not open or valid" << endl;
    return;
  }

  if (source->file_size == 0){
    EradFileHeader f_header;
    liberad_get_file_info(source, &f_header);
  }

  FILE* dest = fopen(destination, "wb");
  if (dest == NULL){
    cout << "error opening segy destination location" << endl;
    return;
  }


  fseek(dest, 0, SEEK_SET);

  char* segy_txt_header = new char[SEGY_TXT_HEADER_SIZE];

  SegyBinaryHeader bin_header;
  SegyTraceHeader segy_trace_header;

  liberad_produce_segy_txt_header(source->f_header, segy_txt_header, SEGY_TXT_HEADER_SIZE);
  liberad_port_erad_segy_bin_file_header(source->f_header, &bin_header);

  segy_trace_header.year = source->f_header->year;
  segy_trace_header.day = source->f_header->day;
  segy_trace_header.sampleInterval = static_cast<int16_t>(round(source->f_header->time_window / 0.585)) ;


  fwrite(segy_txt_header, sizeof(char), SEGY_TXT_HEADER_SIZE, dest);
  fwrite(&bin_header, SEGY_BIN_HEADER_SIZE, 1, dest);

  EradTraceHeader t_header;
  int sample_size = source->f_header->sample_size;
  uint8_t* data = new uint8_t[sample_size];
  int16_t* segy_data = new int16_t[sample_size];

  for (int i = 0; i < source->trace_count; i++){
    liberad_get_trace_at(source, i, &t_header, data);
    liberad_port_erad_segy_bin_trace_header(&t_header, &segy_trace_header);
    liberad_port_data_segy(data, segy_data, sample_size);

    fwrite(&segy_trace_header, SEGY_TRACE_HEADER_SIZE, 1, dest);
    fwrite(segy_data, sizeof(int16_t), sample_size, dest );
  }

  delete[] segy_txt_header;
  delete[] data;
  delete[] segy_data;
  fclose(dest);
}

//DONE assumes sizeof(txt_header) = 3200;
void liberad_produce_segy_txt_header(EradFileHeader* f_header, char* txt_header, int txt_h_size){
  string radar = liberad_get_radar_string(f_header->radar_type);

  string endOfHeader = "C39 SEG Y REV2.0";
  endOfHeader.resize(80, ' ');
  endOfHeader += "C40 END TEXTUAL HEADER";
  endOfHeader.resize(160, ' ');

  sprintf(txt_header, "File generated by Oerad Tech Ltd \n Recording device: %s \n Time Window: %f \n Samples per Trace: %hd \n Dielectric of surveyed medium: %f \n Recorded on %hd / %hd / %hd \n Location: %s \n Operator: %s \n Offset to raw trace data: 3840 \n Data sample format: 16bit \n Offset to first trace header data: 3600 \n No extended textual headers \n ", radar.c_str(), f_header->time_window, f_header->sample_size, f_header->dielectric_coeff, f_header->day, f_header->month, f_header->year, f_header->location, f_header->scan_operator);

  int size_so_far = strlen(txt_header);
  int size_end_header = endOfHeader.size();
  // int finalsize = sizeof(txt_header);

  memset(txt_header + size_so_far, ' ', txt_h_size - size_so_far - size_end_header);
  size_so_far = strlen(txt_header);

  strcpy(txt_header + size_so_far, endOfHeader.c_str());

}

//done
void liberad_port_erad_segy_bin_file_header(EradFileHeader* f_header, SegyBinaryHeader* segy_bin_header){

  segy_bin_header->jobID = 1;
  segy_bin_header->sampleIntervalUs = static_cast<int16_t>(round(f_header->time_window / 0.585)); //somehow this works
  segy_bin_header->samplesPerDataTrace = f_header->sample_size;

}

//done
void liberad_port_erad_segy_bin_trace_header(EradTraceHeader* t_header, SegyTraceHeader* segy_t_header){

  segy_t_header->traceSequenceNumInLine = static_cast<int32_t> (t_header->trace_index);
  segy_t_header->traceSequenceNumInFile = static_cast<int32_t>(t_header->trace_index) ;
  segy_t_header->originalRecordNum = static_cast<int32_t>(t_header->trace_index);
  segy_t_header->ensembleNum = static_cast<int32_t>(t_header->trace_index) ;
  segy_t_header->numSamples = static_cast<int32_t> (t_header->sample_size);
  segy_t_header->hour = t_header->hour;
  segy_t_header->minute = t_header->minute;
  segy_t_header->second = t_header->second;

}

//done
void liberad_port_data_segy(uint8_t* data_source, int16_t* data_dest, int data_length){
  int i;
  int8_t original;
  int8_t mask = static_cast<int8_t>(0b10000000);
  int8_t temp;
  for (i = 0; i< data_length; i++){
    original = data_source[i] & 0xFF;
    temp = original + mask;
    data_dest[i] = static_cast<int16_t>(temp * 100);
  }
}



/* ----------------------------------------------------------------------------------------------------------------- */

//done
void liberad_close_file(LiberadFile* efile){

  if (efile->is_open){
    fclose(efile->stream);
    efile->is_open = false;
    efile->stream = nullptr;

  }
}


/* ----------------------------------------------------------------------------------------------------------------- */

//DONE
int liberad_open_segy_file_w(SegyFile* sfile, const char* filename){
  if (filename == NULL || filename[0] == '\0'){
    cout << "filename is empty string " << endl;
    return ERROR;
  }
  sfile->filename = filename;

  return liberad_open_segy_file_w(sfile);

}

//DONE
int liberad_open_segy_file_w(SegyFile* sfile){
  if (sfile->filename == nullptr || sfile->filename[0] == '\0'){
    cout << "no file location associated with this LiberadFile" << endl;
    return ERROR;
  }

  // get system endianness on first call - maybe put in init call
  system_endianness = liberad_get_system_endianness();

  sfile->stream = fopen(sfile->filename, "wb");

  if (sfile->stream == NULL){
    return ERROR;
  }
  sfile->is_open = true;
  return SUCCESS;
}

// DONE
void liberad_write_segy_txt_h(SegyFile* sfile, char* txt_header){
  if (!sfile->is_open){
    cout << "File not opened" << endl;
    return;
  }
  fseek(sfile->stream, 0, SEEK_SET);
  fwrite(txt_header, sizeof(char), SEGY_TXT_HEADER_SIZE, sfile->stream);
}

// DONE
void liberad_write_segy_bin_h(SegyFile* sfile, SegyBinaryHeader* b_header){
  if (!sfile->is_open){
    cout << "File not opened" << endl;
    return;
  }
  fseek(sfile->stream, SEGY_TXT_HEADER_SIZE - 1, SEEK_SET);
  fwrite(b_header, SEGY_BIN_HEADER_SIZE, 1, sfile->stream);

}

// Done
void liberad_write_segy_trace(SegyFile* sfile, SegyTraceHeader* t_header, const void* data, int data_size){
  if (!sfile->is_open){
    cout << "File not opened" << endl;
    return;
  }

  fseek(sfile->stream, SEGY_TXT_HEADER_SIZE + SEGY_BIN_HEADER_SIZE - 1, SEEK_SET);
  fwrite(t_header, SEGY_TRACE_HEADER_SIZE, 1, sfile->stream);
  fwrite(data, data_size, 1, sfile->stream);

}


// DONE
void liberad_close_segy_file_w(SegyFile* sfile){
  if (sfile->is_open){
    fclose(sfile->stream);
    sfile->is_open = false;
    sfile->stream = nullptr;
  }
}


/* ----------------------------------------------------------------------------------------------------------------- */

//done
void liberad_get_trace_count(LiberadFile* efile){
  if (!efile->is_open){
    cout << "File not opened"<< endl;
    return;
  }

  fseek(efile->stream, -sizeof(efile->trace_count), SEEK_END);
  fread(&efile->trace_count, sizeof(efile->trace_count), 1, efile->stream); // TODO test here pointer or address

  if (system_endianness != efile->endianness){
    efile->trace_count = shift_endianness<int64_t>(efile->trace_count);
  }
}

//done
void liberad_get_file_size(LiberadFile* efile){

  if (!efile->is_open){
    cout << "File not opened"<< endl;
    return;
  }

  fseek(efile->stream, 0, SEEK_END);
  long int count = ftell(efile->stream);
  efile->file_size = count;

}

/* ----------------------------------------------------------------------------------------------------------------- */



//done
long int liberad_get_trace_data_index_at(int64_t trace_index, int sample_size, int8_t file_version){

  if (file_version == VER_2018){
    return FH_SIZE + trace_index * (TH_SIZE_VER_1 + sample_size) + TH_SIZE_VER_1;
  } else {
    return FH_SIZE + trace_index * (TH_SIZE_VER_2 + sample_size) + TH_SIZE_VER_2;
  }

}

//done
long int liberad_get_trace_header_index_at(int64_t trace_index, int sample_size, int8_t file_version){

  if (file_version == VER_2018){
    return FH_SIZE + trace_index * (TH_SIZE_VER_1 + sample_size);
  } else {
    return FH_SIZE + trace_index * (TH_SIZE_VER_2 + sample_size);
  }

}


/* ----------------------------------------------------------------------------------------------------------------- */

//done
void liberad_port_trace_header_data(EradTraceHeader_VER_1* source, EradTraceHeader* destination){

  destination->trace_index = source->trace_index;
  destination->sample_size = source->sample_size;
  destination->steps_per_trace = source->steps_per_trace;
  destination->hour = static_cast<int8_t>(source->hour);
  destination->minute = static_cast<int8_t>(source->minute);
  destination->second = static_cast<int8_t>(source->second);
  destination->millisecond = static_cast<int8_t>(source->millisecond);
  destination->fold_index = source->fold_index;
  destination->fold_orientation = source->fold_orientation;
  destination->trace_index_in_fold = source->trace_index_in_fold;
  destination->x_local = source->x_local;
  destination->y_local = source->y_local;
  destination->z_local = source->z_local;
  destination->longitude = 0;
  destination->latitude = 0;

}


void liberad_port_file_header_data(EradFileHeader_VER_1* src, EradFileHeader* dest){
  memcpy(src, dest, FH_SIZE);
  dest->steps_per_meter = static_cast<uint8_t>(src->steps_per_meter);
  dest->coordinate_system = liberad::LOCAL;
}



/* ----------------------------------------------------------------------------------------------------------------- */

//done
std::string liberad_get_stream_mode(LiberadFile::Mode mode){
  std::string mode_str;
  if (mode == LiberadFile::LIBERAD_APPEND){
    mode_str = "ab";
  } else if (mode == LiberadFile::LIBERAD_READ){
    mode_str = "rb";
  } else {
    mode_str = "wb";
  }
  return mode_str;
}

/* ----------------------------------------------------------------------------------------------------------------- */


//done
void liberad_read_trace_header(LiberadFile* efile, long int index_file, EradTraceHeader* t_header){

  fseek(efile->stream, index_file, SEEK_SET);

  if (efile->file_ver < VER_2019){
    EradTraceHeader_VER_1 th;
    fread(&th, TH_SIZE_VER_1, 1, efile->stream);
    if (efile->endianness != system_endianness){
      liberad_shift_trace_header_ver1_endianness(&th);
    }

    liberad_port_trace_header_data(&th, t_header);

  } else {

    fread(t_header, TH_SIZE_VER_2, 1, efile->stream);
    if (efile->endianness != system_endianness){
      liberad_shift_trace_header_ver2_endianness(t_header);
    }
  }
}


void liberad_read_trace_header(FILE* stream, long int index_f, EradTraceHeader* t_header, liberad::EndiannessMarker endianness, int8_t file_ver){
  fseek(stream, index_f, SEEK_SET);

  if (file_ver < VER_2019){
    EradTraceHeader_VER_1 th;
    fread(&th, TH_SIZE_VER_1, 1, stream);
    if (endianness != system_endianness){
      liberad_shift_trace_header_ver1_endianness(&th);
    }
    liberad_port_trace_header_data(&th, t_header);

  } else {

    fread(t_header, TH_SIZE_VER_2, 1, stream);
    if (endianness != system_endianness){
      liberad_shift_trace_header_ver2_endianness(t_header);
    }
  }
}

//done
EndiannessMarker liberad_read_file_header(FILE* stream, EradFileHeader* f_header, int8_t file_ver){
  fseek(stream, 0, SEEK_SET);
  fread(f_header, FH_SIZE, 1, stream);

  if (file_ver < liberad::VER_2019){
    fseek(stream, 38, SEEK_SET);
    int16_t temp;
    fread(&temp, sizeof(int16_t), 1, stream);

    f_header->steps_per_meter = static_cast<uint8_t>(temp);
    f_header->coordinate_system = LOCAL;
  }

  EndiannessMarker endianness = liberad_get_file_endianness(f_header->endianness_marker);
  if (endianness != system_endianness){
    liberad_shift_file_header_endianness(f_header);
  }

  return endianness;
}

/* ----------------------------------------------------------------------------------------------------------------- */

//done
void liberad_shift_file_header_endianness(EradFileHeader* f_header){

  f_header->radar_type = shift_endianness<int16_t>(f_header->radar_type);
  f_header->year = shift_endianness<int16_t>(f_header->year);
  f_header->month = shift_endianness<int16_t>(f_header->month);
  f_header->day = shift_endianness<int16_t>(f_header->day);
  f_header->dimension = shift_endianness<int16_t>(f_header->dimension);
  f_header->data_offset = shift_endianness<int16_t>(f_header->data_offset);
  f_header->time_window = shift_endianness<float>(f_header->time_window);
  f_header->total_x = shift_endianness<float>(f_header->total_x);
  f_header->total_y = shift_endianness<float>(f_header->total_y);
  f_header->sample_size = shift_endianness<int16_t>(f_header->sample_size);
  f_header->coordinate_system = shift_endianness<int16_t>(f_header->coordinate_system);
  f_header->dielectric_coeff = shift_endianness<float>(f_header->dielectric_coeff);
  f_header->interval_x = shift_endianness<float>(f_header->interval_x);
  f_header->interval_y = shift_endianness<float>(f_header->interval_y);

}

//done
void liberad_shift_trace_header_ver1_endianness(EradTraceHeader_VER_1* t_header){

  t_header->trace_index = shift_endianness<int64_t>(t_header->trace_index);
  t_header->sample_size = shift_endianness<int16_t>(t_header->sample_size);
  t_header->steps_per_trace = shift_endianness<int16_t>(t_header->steps_per_trace);
  t_header->hour = shift_endianness<int16_t>(t_header->hour);
  t_header->minute = shift_endianness<int16_t>(t_header->minute);
  t_header->second = shift_endianness<int16_t>(t_header->second);
  t_header->millisecond = shift_endianness<int32_t>(t_header->millisecond);
  t_header->fold_index = shift_endianness<int32_t>(t_header->fold_index);
  t_header->trace_index_in_fold = shift_endianness<int32_t>(t_header->trace_index_in_fold);
  t_header->x_local = shift_endianness<double>(t_header->x_local);
  t_header->y_local = shift_endianness<double>(t_header->y_local);
  t_header->z_local = shift_endianness<double>(t_header->z_local);

}

//done
void liberad_shift_trace_header_ver2_endianness(EradTraceHeader* t_header){

  t_header->trace_index = shift_endianness<int64_t>(t_header->trace_index);
  t_header->sample_size = shift_endianness<int16_t>(t_header->sample_size);
  t_header->steps_per_trace = shift_endianness<int16_t>(t_header->steps_per_trace);
  t_header->millisecond = shift_endianness<int16_t>(t_header->millisecond);
  t_header->fold_index = shift_endianness<int32_t>(t_header->fold_index);
  t_header->trace_index_in_fold = shift_endianness<int32_t>(t_header->trace_index_in_fold);
  t_header->x_local = shift_endianness<double>(t_header->x_local);
  t_header->y_local = shift_endianness<double>(t_header->y_local);
  t_header->z_local = shift_endianness<double>(t_header->z_local);
  t_header->longitude = shift_endianness<double>(t_header->longitude);
  t_header->latitude = shift_endianness<double>(t_header->latitude);

}

/* ----------------------------------------------------------------------------------------------------------------- */


//done
EndiannessMarker liberad_get_system_endianness(){

  int num = 1;
  if(*(char *)&num == 1)
  {
      return LITTLE_END;
  }
  return BIG_END;

}

//done
EndiannessMarker liberad_get_file_endianness(int8_t* header_endianness){

  if (header_endianness[0] == static_cast<int8_t>(0xFE) && header_endianness[1] == static_cast<int8_t>(0xFF)){
    return BIG_END;
  }
  return LITTLE_END;


}

std::string liberad_get_radar_string(int16_t radar){
  string radar_str;
  switch (radar) {
    case SCUDO:
      radar_str = "Scudo";
      break;
    case DIPOLO:
      radar_str = "Dipolo";
      break;
    case CONCRETTO:
      radar_str = "Concretto";
      break;
    default:
      radar_str = "Custom GPR";
      break;
  }

  return radar_str;
}
