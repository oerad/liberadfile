#ifndef LIBERADFILE_H
#define LIBERADFILE_H

#include <string>
#include <iostream>
#include "erad.h"
#include "segy.h"

/*
* Template function for shifting endianness of any data type
*/
template<typename T>
T shift_endianness(T source){
  union {
    T t;
    uint8_t b[sizeof(source)];
  }src, result;
  src.t = source;
  for (unsigned long i = 0; i< sizeof(source); i++){
    result.b[i] = src.b[sizeof(source) - i - 1];
  }
  return result.t;
}

/* ----------------------------------------------------------------------------------------------------------------- */

/* Opens an instance of LiberadFile at location file in mode
* @param LiberadFile* efile - pointer to instance of .erad file
* @param const char* file_loc - path of file for read/write
* @param LiberadFile::Mode mode - READ or WRITE
* @return -1 on ERROR, 0 on SUCCESS
*/
int liberad_open_file(LiberadFile* efile, const char* file_loc, LiberadFile::Mode mode);

/* Opens an instance of LiberadFile with initialized file location and access mode (Read or Write)
* @param LiberadFile* efile - pointer to instance of .erad file
* @return -1 on ERROR, 0 on SUCCESS
*/
int liberad_open_file(LiberadFile* efile);

/* Checks if file opened in Read mode is a valid Erad log file
* @param LiberadFile* efile - pointer to .erad file instance
* @return TRUE if log file is valid; FALSE if file is not compatible.
*/
bool liberad_check_file(LiberadFile* efile);

/* ----------------------------------------------------------------------------------------------------------------- */

/* Extracts basic information about .erad file - filesize, trace_count and reads the file header into EradFileHeader f_header struct instance
* @param LiberadFile* efile - pointer to opened and valid .erad file instance
* @param EradFileHeader* f_header - pointer to file header struct to populate
*/
void liberad_get_file_info(LiberadFile* efile, EradFileHeader* f_header);

/* Reads trace header and trace data with trace_index from efile
* @param LiberadFile* efile - pointer to opened and valid .erad file instance
* @param int64_t trace_index - trace index within file
* @param EradTraceHeader* t_header - pointer to trace header struct to populate
* @param uint8_t* data - pointer to uint8_t buffer - must be at least f_header->sample_size big assuming a uniform trace count.
*/
void liberad_get_trace_at(LiberadFile* efile, int64_t trace_index, EradTraceHeader* t_header, uint8_t* data);

/* Reads trace header data only of trace with trace_index within file in t_header.
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
* @param int64_t trace_index - trace index within file
* @param  EradTraceHeader* t_header - pointer to trace header struct to populate
*/
void liberad_get_trace_header_at(LiberadFile* efile, int64_t trace_index, EradTraceHeader* t_header);

/* Reads raw trace data from efile at trace_index and stores it in data.
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
* @param int64_t trace_index - trace index within file
* @param uint8_t* data - pointer to uint8_t buffer - must be at least f_header->sample_size big assuming a uniform trace count.
*/
void liberad_get_trace_data_at(LiberadFile* efile, int64_t trace_index, uint8_t* data);

//TODO
/*
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
*
*/
void liberad_get_trace_data(LiberadFile* efile, int64_t index_start, int64_t index_end, uint8_t* data);

//TODO
/*
*
*/
void liberad_get_trace_data(LiberadFile* efile, int64_t index_start, int64_t index_end, std::ostream* out_stream);

//TODO
//void liberad_get_trace_data(LiberadFile* efile, int index_start, int index_end, callbackfunction* functor);

/* ----------------------------------------------------------------------------------------------------------------- */

/* Reads the total trace count of this instance of .erad file and stores it in efile->trace_count
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
*/
void liberad_get_trace_count(LiberadFile* efile);

/* Gets the file size of efile and stores it in efile->file_size
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
*/
void liberad_get_file_size(LiberadFile* efile);

/* Helper function for reading
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
* @param long int index_file - byte index of trace header within file
* @param EradTraceHeader* t_header - pointer to EradTraceHeader struct to populate
*/
void liberad_read_trace_header(LiberadFile* efile, long int index_file, EradTraceHeader* t_header);

/* Helper function for reading a trace header from an open FILE stream into t_header
* @param  FILE* stream - pointer to opened and valid LiberadFile FILE stream instance
* @param long int index_f - byte index of trace header within file
* @param EradTraceHeader* t_header - pointer to EradTraceHeader struct to populate
* @param liberad::EndiannessMarker marker - byte endianness of file (stored in LiberadFile::endianness)
* @param int8_t file_ver - version of file (stored in LiberadFile::file_ver)
*/
void liberad_read_trace_header(FILE* stream, long int index_f, EradTraceHeader* t_header, liberad::EndiannessMarker endianness, int8_t file_ver);

/* Helper function for reading file header from FILE stream into f_header
* @param FILE* stream - pointer to opened and valid LiberadFile FILE stream instance
* @param EradFileHeader *f_header - pointer to EradFileHeader struct to populate with data from file
*/
// liberad::EndiannessMarker liberad_read_file_header(FILE* stream, EradFileHeader* f_header);
liberad::EndiannessMarker liberad_read_file_header(FILE* stream, EradFileHeader* f_header, int8_t file_ver);

/* Returns a string with the radar name based on file code
* @param int16_t radar - code stored in EradFileHeader of every .erad file
* @return string - human readable Oerad hardware name
*/
std::string liberad_get_radar_string(int16_t radar);


/* ----------------------------------------------------------------------------------------------------------------- */

/* Writes f_header to file
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
* @param EradFileHeader* f_header - pointer to header to write to file
*/
void liberad_write_file_header(LiberadFile* efile, EradFileHeader* f_header);

/* Writes theader to file
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
* @param EradTraceHeader* theader - pointer to header to write to file
* @param uint8_t* data - trace data
*/
void liberad_write_trace(LiberadFile* efile, EradTraceHeader* theader, uint8_t* data);

/* Finishes writing .erad log file
* @param  LiberadFile* efile - pointer to opened and valid .erad file instance
*/
void liberad_finish_write(LiberadFile* efile);

/* ----------------------------------------------------------------------------------------------------------------- */

/* Open a .sgy file for writing
* @param SegyFile* sfile - pointer to SegyFile .sgy instance
* @param const char* filename - location of file
*/
int liberad_open_segy_file_w(SegyFile* sfile, const char* filename);

/* Open a segy file for writing. Assumes filename field has been set
* @param SegyFile* sfile - pointer to SegyFile .sgy instance
*/
int liberad_open_segy_file_w(SegyFile* sfile);

/* Writes a 3200-byte segy txt_header to file
* @param SegyFile* sfile - pointer to SegyFile .sgy instance
* char* txt_header - pointer to char array holding the segy txt header
*/
void liberad_write_segy_txt_h(SegyFile* sfile, char* txt_header);

/* Writes a segy binary header to file
* @param SegyFile* sfile - pointer to SegyFile .sgy instance
* SegyBinaryHeader* b_header - pointer to segy binary header to write to file
*/
void liberad_write_segy_bin_h(SegyFile* sfile, SegyBinaryHeader* b_header);

/* Writes segy binary trace header and raw trace data to file
* @param SegyFile* sfile - pointer to SegyFile .sgy instance
* @param SegyTraceHeader* t_header - pointer to segy trace header to write to file
* @param void* data - raw trace data to write to file
* @param int data_size - number of samples in current trace
*/
void liberad_write_segy_trace(SegyFile* sfile, SegyTraceHeader* t_header, void* data, int data_size);


/* Close the SegyFile stream instance
* @param SegyFile* sfile - pointer to SegyFile .sgy instance
*/
void liberad_close_segy_file_w(SegyFile* sfile);

/* ----------------------------------------------------------------------------------------------------------------- */

/* Calculates the byte index of an .erad trace header within a file
* @param int64_t trace_index - trace index within file
* @param int sample_size - number of samples in a single trace. This function assumes uniform traces
* @param int8_t file_version - version of recorded file
*/
long int liberad_get_trace_header_index_at(int64_t trace_index, int sample_size, int8_t file_version);

/* Calculates the byte index of an .erad raw trace data within a file
* @param int64_t trace_index - trace index within file
* @param int sample_size - number of samples in a single trace. This function assumes uniform traces
* @param int8_t file_version - version of recorded file
*/
long int liberad_get_trace_data_index_at(int64_t trace_index, int sample_size, int8_t file_version);

/* Ports a pre VER_2019 file version trace header to a 2019 file version
* @param EradTraceHeader_VER_1* source - pointer to a VER_1 (pre 2019) erad trace header
* @param EradTraceHeader* destination - pointer to >= VER_2019 trace header struct instance
*/
void liberad_port_trace_header_data(EradTraceHeader_VER_1* source, EradTraceHeader* destination);

/* ----------------------------------------------------------------------------------------------------------------- */


/* Exports an erad file to a segy file
* @param  LiberadFile* efile - pointer to .erad file instance for export
* @param const char* destination - file location of new segy file
*/
void liberad_export_to_segy(LiberadFile* source, const char* destination);

/* Produces a SEG-Y textual header as per the standard definition from fields of an .erad file header
* @param EradFileHeader* f_header - pointer to .erad file header
* @param char* txt_header - char buffer to hold f_header fields
* @param txt_h_size - size of char buffer. As per the SEG-Y file definition this should be 3200 bytes
*/
void liberad_produce_segy_txt_header(EradFileHeader* f_header, char* txt_header, int txt_h_size);

/* Ports fields from an .erad trace header to a SEG-Y binary trace header as per the SEG-Y standard definition.
* @param EradTraceHeader* t_header - pointer to source .erad trace header
* @param SegyTraceHeader* segy_t_header - pointer to SegyTraceHeader struct to be populated
*/
void liberad_port_erad_segy_bin_trace_header(EradTraceHeader* t_header, SegyTraceHeader* segy_t_header);

/* Ports fields from an .erad file header to a SEG-Y binary file header as per the SEG-Y standard definition.
* @param EradFileHeader* f_header - pointer to source .erad file header
* @param SegyBinaryHeader* segy_bin_header - pointer to SegyBinaryHeader struct to be populated
*/
void liberad_port_erad_segy_bin_file_header(EradFileHeader* f_header, SegyBinaryHeader* segy_bin_header);

/* Ports trace data acquired from Oerad hardware and logged to an .erad file (unsigned byte) to a signed short.
* Technically the latest SEG-Y revision allows for uint8_t data but not every SEG-Y viewing package
* has implemented the latest version of the standard.
* @param uint8_t* data_source - pointer to raw data source buffer
* @param int16_t* data_dest - pointer to segy raw data destination buffer
* @param int data_length - number of samples in trace.
*/
void liberad_port_data_segy(uint8_t* data_source, int16_t* data_dest, int data_length);


/* ----------------------------------------------------------------------------------------------------------------- */

/* Closes this efile instance's FILE stream
* @param  LiberadFile* efile - pointer to opened .erad file instance
*/
void liberad_close_file(LiberadFile* efile);


#endif
