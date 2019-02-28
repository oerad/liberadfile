#ifndef LIBERAD_ERAD
#define LIBERAD_ERAD

#include <string>

#define FH_SIZE 212

#define TH_SIZE_VER_1 55
#define TH_SIZE_VER_2 66


namespace liberad{

  enum LiberadExitCodes{ERROR = -1, SUCCESS = 0};
  enum RadarType{SCUDO = 0x09, DIPOLO = 0x0B, CONCRETTO = 0x0A, CUSTOM = 0x08};
  enum FileVersion{VER_2018 = 0x03, VER_2019 = 0x04};
  enum HardwareVersion{PRE2017 = 0x01, POST2017 = 0x05};
  enum Dimension{SINGLE_SLICE_TEMPORAL = 0x00, SINGLE_SLICE_SPATIAL = 0x01, VERTICAL_3D = 0x02, HORIZONTAL_3D = 0x03, VERTICAL_HORIZONTAL = 0x04,
                VERTICAL_SLICES_TEMPORAL= 0x05, HORIZONTAL_SLICES_TEMPORAL = 0x06, VER_HOR_TEMPORAL = 0x07};
  enum CoordinateSystem{GLOBAL = 0x01, LOCAL = 0x02, GLOBAL_LOCAL = 0x03};
  enum FoldOrientation{VERTICAL = 0x00, HORIZONTAL = 0x01};

  enum EndiannessMarker{BIG_END = 0x00, LITTLE_END = 0x02};

}



struct EradFileHeader{

  int8_t magic_num[8];
  int8_t file_version;
  int8_t endianness_marker[2];
  int8_t hardware_version;
  int16_t radar_type;
  int16_t year;
  int16_t month;
  int16_t day;
  int16_t dimension; //record type
  int16_t data_offset;
  float time_window;
  float total_x;
  float total_y;
  int16_t sample_size;
  uint8_t steps_per_meter;
  int8_t coordinate_system; //if (file_version==FILE_OLD) coordinate_system = LOCAL;
  float dielectric_coeff;
  float interval_x;
  float interval_y;
  char scan_operator[56]; // + 2
  char location[100]; // + 2

};

struct EradTraceHeader{

  int64_t trace_index = 0;
  int16_t sample_size = 0;
  int16_t steps_per_trace = 0;
  int8_t hour = 0;
  int8_t minute = 0;
  int8_t second = 0;
  int16_t millisecond = 0;
  int32_t fold_index = 0;
  int8_t fold_orientation = 0;
  int32_t trace_index_in_fold = 0;
  double x_local = 0;
  double y_local = 0;
  double z_local = 0;
  double longitude = 0;
  double latitude = 0;

};


struct EradFileHeader_VER_1{

    int8_t magin_num[8];
    int8_t file_version;
    int8_t endianness_marker[2];
    int8_t hardware_version;
    int16_t radar_type;
    int16_t year;
    int16_t month;
    int16_t day;
    int16_t dimension; //record type
    int16_t data_offset;
    float time_window;
    float total_x;
    float total_y;
    int16_t sample_size;
    int16_t steps_per_meter;
    float dielectric_coeff;
    float interval_x;
    float interval_y;
    char scan_operator[56];
    char location[100];

};

struct EradTraceHeader_VER_1{

    int64_t trace_index = 0;
    int16_t sample_size = 0;
    int16_t steps_per_trace = 0;
    int16_t hour = 0;
    int16_t minute = 0;
    int16_t second = 0;
    int32_t millisecond = 0;
    int32_t fold_index = 0;
    int8_t fold_orientation = 0;
    int32_t trace_index_in_fold = 0;
    double x_local = 0;
    double y_local = 0;
    double z_local = 0;

};


struct LiberadFile{

  enum Mode{LIBERAD_WRITE, LIBERAD_READ, LIBERAD_APPEND};

  LiberadFile();
  LiberadFile(Mode mode);
  LiberadFile(Mode mode, const char* filename);
  ~LiberadFile();

  EradFileHeader* f_header = nullptr;
  Mode mode = LIBERAD_READ;
  liberad::EndiannessMarker endianness = liberad::LITTLE_END;
  FILE* stream = nullptr;

  const char* filename = nullptr;
  int8_t file_ver = liberad::VER_2019;
  int64_t file_size = 0;
  int64_t trace_count = 0;
  bool is_open = false;
  bool is_valid = false;

  //TODO fold data array with fold trace counts

};


#endif
