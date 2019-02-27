#ifndef OERAD_SEGY_H
#define OERAD_SEGY_H

#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

#define SEGY_TXT_HEADER_SIZE 3200
#define SEGY_BIN_HEADER_SIZE 400
#define SEGY_TRACE_HEADER_SIZE 240

/*
 * BIG ENDIAN
 * NO FLOATS IN HEADERS
 * TWO'S COMPLEMENT INTS  */

struct SegyBinaryHeader{

  enum Undefined{OTHER = -1, UNKNOWN = 0};

  enum DataSampleFormat{BYTE_4_IBM = 1, BYTE_4_TWOS = 2, BYTE_2_TWOS = 3, BYTE_4_FIXEDP = 4, BYTE_4_IEEE = 5, BYTE_8_IEEE = 6, BYTE_3_TWOS = 7, BYTE_1_TWOS = 8, BYTE_8_TWOS = 9, BYTE_4_UNS = 10, BYTE_2_UNS = 11, BYTE_8_UNS = 12, BYTE_3_UNS = 15, BYTE_1_UNS = 16 };

  enum TraceSortingCode{ AS_RECORDED = 1, CDP_ENSEMBLE = 2, SINGLE_FOLD_CONT = 3, HORIZONTAL_STACK = 4, COMMON_SOURCE = 5, COMMON_RECEIVER = 6, COMMON_OFFSET = 7, COMMON_MID_P = 8, COMMON_CONVERSION_P = 9};

  enum SweepTypeCode{LINEAR = 1, PARABOLIC = 2, EXPONENTIAL = 3, STC_OTHER = 4};

  enum TaperType{TT_LINEAR = 1, COSINE_SQ = 2, TT_OTHER = 3};

  enum CorrelatedData{CD_YES = 2, CD_NO = 1};

  enum BinaryGainRecovered{BGR_YES = 1, BGR_NO = 2};

  enum AmplitudeRecoverMethod{NONE = 1, SPHERICAL_DIV = 2, AGC = 3, ARM_OTHER = 4};

  enum MeasurementSystem{METRIC = 1, IMPERIAL = 2};

  enum ImpulseSignalPolarity{NEGATIVE = 1, POSITIVE = 3};

  enum VibratoryPolarityCode{VPC_337_22 = 1, VPC_22_67 = 2, VPC_67_112 = 3, VPC_112_157 = 4, VPC_157_202 = 5, VPC_202_247 = 6, VPC_247_292 = 7, VPC_292_337 = 8};

  enum SegyVersion{STANDARD_1975 = 0X00, STANDARD_2017 = 0X02};

  enum FixedTraceLenghtFlag{FIXED = 1, VARIABLE = 0};

  enum TimeBasisCode{LOCAL = 1, GMT = 2, TBC_OTHER = 3, UTC = 4, GPS = 5};

    int32_t jobID = 0;
    int32_t lineNum = 1;
    int32_t reelNum = 4;
    int16_t dataTracesPerEnsemble = 1;
    int16_t auxiliaryTracesPerEnsemble = 0;
    int16_t sampleIntervalUs = 0;
    int16_t sampleIntervalUsOriginal = 0 ;
    int16_t samplesPerDataTrace = 0 ;
    int16_t samplesPerDataTraceOriginal = 0 ;
    int16_t formatCode = BYTE_2_TWOS;
    int16_t ensembleFold = 1;
    int16_t traceSortingCode = 0 ;
    int16_t verticalSum = 0 ;
    int16_t sweepFreqStart = 0 ;
    int16_t sweepFreqEnd = 0 ;
    int16_t sweepLength = 0 ;
    int16_t sweepTypeCode = 0 ;
    int16_t traceNumSweepChannel = 0 ;
    int16_t sweepTaperLengthStart = 0 ;
    int16_t sweepTaperLengthEnd = 0 ;
    int16_t taperType = 0 ;
    int16_t correlatedData = 0 ;
    int16_t binaryGain = 0 ;
    int16_t amplitudeRecoverMethod = 0 ;
    int16_t measurementSys = METRIC;
    int16_t impulseSignalPolarity = 0 ;
    int16_t vibratoryPolarityCode = 0 ;
    int32_t extendedNumDataTraces = 0 ;
    int32_t extendedNumAuxTraces = 0 ;
    int32_t extendedNumSamplesPerEnsemble = 0 ;
    int64_t extendedSampleInterval = 0 ;
    int64_t extendedSampleIntervalOriginal = 0 ;
    int32_t extendedNumSamplesPerTrace = 0 ;
    int32_t extendedEnsembleFold = 0 ;
    int32_t integerConstant = 0x01020304; //0x01020304
    int8_t unassigned_one[200];
    int8_t majorRevNum = STANDARD_2017;
    int8_t minorRevNum = 0;
    int16_t fixedLengthTrace = FIXED;
    int16_t numExtendedTextHeaders = 0 ;
    int32_t maxNumAdditionalTraceHeaders = 0 ;
    int16_t timeBasisCode = 0 ;
    int64_t numOfTracesInFile = 0 ;
    int64_t offsetFirstTrace  = 0; //relative to start of file
    int32_t numTrailerStanzaRecs = 0 ;
    int8_t unassigned_two[68];

};





struct SegyTraceHeader{

  enum Undefined{OTHER = -1, UNKNOWN = 0};


  enum TraceIDCode{TIME_DOMAIN_SEISMIC = 1, DEAD = 2, DUMMY = 3, TIME_BREAK = 4, UPHOLE = 5, SWEEP = 6, TIMING = 7, WATERBREAK= 8, NEAR_FIELD_GUN = 9, FAR_FIELD_GUN = 10, SEISMIC_PRESSURE = 11, MSS_VERTICAL = 12, MSS_CROSSLINE = 13, MSS_INLINE = 14, RMSS_VERTICAL = 15, RMSS_TRANSVERSE = 16, RMSS_RADIAL = 17, VIB_REACTION = 18, VIB_BASEPLATE = 19, VIB_ESTIM_G_FORCE = 20, VIB_REF = 21, TIME_VELOCITY = 22, TIME_DEPTH = 23, DEPTH_VELOCITY = 24, DEPTH_DOMAIN = 25, GRAVITY_POT = 26, EF_VERTICAL = 27, EF_CROSSLINE = 28, EF_INLINE = 29, REF_VERTICAL = 30, REF_TRANSVERSE = 31, REF_RADIAL = 32, MF_VERTICAL = 33, MF_CROSSLINE = 34, MF_INLINE = 35, RMF_VERTICAL = 36, RMF_TRANSVERSE = 37, RMF_RADIAL = 38, RS_PITCH = 39, RS_ROLL = 40, RS_YAW = 41};

  enum DataUse{PRODUCTION = 1, TEST = 2};
  enum CoordinateUnits{LENGTH = 1, SECONDS_ARC = 2, DEC_DEGREES = 3, DMS = 4};
  enum GainType{FIXED = 1, BINARY = 2, FLOATING_POINT = 3};
  enum Correlated{NO = 1, YES = 2};
  enum SweepType{ST_LINEAR = 1, PARABOLIC = 2, EXPONENTIAL = 3, ST_OTHER = 4};
  enum TaperType{TT_LINEAR = 1, COS_2 = 2, TT_OTHER = 3};
  enum TimeBasisCode{LOCAL = 1, GMT = 2, TBC_OTHER = 3, UTC = 4, GPS = 5};
  enum OverTravel{DOWN_BEHIND = 1, UP_AHEAD = 2};
  enum MeasurementUnit{ PASCAL = 1, VOLTS = 2, MILLIVOLTS = 3, AMPERES = 4, METERS = 5, METERS_PER_S = 6, METERS_PER_S_SQ = 7, NEWTON = 8, WATT = 9};
  enum SourceType{VIB_VERTICAL = 1, VIB_CROSSLINE = 2, VIB_INLINE = 3, IMPULSIVE_VERTICAL = 4, IMPULSIVE_CROSSLINE = 5, IMPULSIVE_INLINE = 6, DIS_IMP_VERTICAL = 7, DIS_IMP_CROSSLINE = 8, DIS_IMP_INLINE = 9 };

  enum SourceMeasurementUnit{SMU_JOULE = 1, SMU_KILOWATT = 2, SMU_PASCAL = 3, SMU_BAR = 4, SMU_NEWTON = 5, SMU_KG = 6};


    int32_t traceSequenceNumInLine = 0;
    int32_t traceSequenceNumInFile = 0;
    int32_t originalRecordNum = 0;
    int32_t traceNumInOriginalRecord = 1;
    int32_t energySourcePoint = 0;
    int32_t ensembleNum = 1;
    int32_t traceNumInEnsemble = 1;
    int16_t traceIdCode = NEAR_FIELD_GUN;
    int16_t verticallySummedTraces = 1;
    int16_t horizontallySummedTraces = 1;
    int16_t dataUse = PRODUCTION;
    int32_t distanceFromCenter = 0;
    int32_t receiverElevation = 0;
    int32_t surfaceElevation = 0;
    int32_t sourceDepth = 0;
    int32_t datumElevationReceiver = 0;
    int32_t datumElevationSource = 0;
    int32_t waterDepthSource = 0;
    int32_t waterDepthGroup = 0;
    int16_t scalar = 1;
    int16_t scalarCoordinates = -1;
    int32_t sourceCoordinateX = 0;
    int32_t sourceCoordinateY = 0;
    int32_t groupCoordinateX = 0;
    int32_t groupCoordinateY = 0;
    int16_t coordinateUnits = LENGTH;
    int16_t weatheringVelocity = 0;
    int16_t subweatheringVelocity = 0;
    int16_t upholeTimeSource = 0;
    int16_t upholeTimeGroup = 0;
    int16_t sourceStaticCorrection = 0;
    int16_t groupStaticCorrection = 0;
    int16_t totalStaticApplied = 0;
    int16_t lagTimeA = 0;
    int16_t lagTimeB = 0;
    int16_t delay = 0;
    int16_t muteTimeStart = 0;
    int16_t muteTimeEnd = 0;
    int16_t numSamples = 0;
    int16_t sampleInterval = 0;
    int16_t gainType = 0;
    int16_t instrumentGainConstant = 0;
    int16_t instrumentInitialGain = 0;
    int16_t correlated = 0;
    int16_t sweepFreqStart = 0;
    int16_t sweepFreqEnd = 0;
    int16_t sweepLength = 0;
    int16_t sweepType = 0;
    int16_t sweepTraceLenghtStart = 0;
    int16_t sweepTraceLengthEnd = 0;
    int16_t taperType = 0;
    int16_t aliasFilterFreq = 0;
    int16_t aliasFilterSlope = 0;
    int16_t notchFilterFreq = 0;
    int16_t notchFilterSlope = 0;
    int16_t lowCutFreq = 0;
    int16_t highCutFreq = 0;
    int16_t lowCutSlope = 0;
    int16_t highCutSlope = 0;
    int16_t year = 0;
    int16_t day = 0;
    int16_t hour = 0;
    int16_t minute = 0;
    int16_t second = 0;
    int16_t timeBasisCode = GMT;
    int16_t traceWeightFactor = 0;
    int16_t geophoneRollSwitch = 0;
    int16_t geophoneTraceFirst = 0;
    int16_t geophoneTraceLast = 0;
    int16_t gapSize = 0;
    int16_t overTravel = 0;
    int32_t ensembleX = 0;
    int32_t ensembleY = 0;
    int32_t inLineNum = 0;
    int32_t crossLineNum = 0;
    int32_t shotPoint = 0;
    int16_t scalarToShotPoint = 0;
    int16_t traceMeasurementUnit = 0;
    int32_t transductionMantissa = 0;
    int16_t transductionPower = 0;
    int16_t transductionUnits = 0;
    int16_t deviceId = 0;
    int16_t scalarTimes = 0;
    int16_t sourceOrientation = 0;
    int32_t sourceEnergyDirectioneMantissa = 0; //no fn idea
    int16_t sourceEnergyDirectionPower = 0;     //no fn idea
    int32_t sourceMeasurementMantissa = 0;
    int16_t sourceMeasurementExponent = 0;
    int16_t sourceMeasurementUnit = 0;
    uint8_t unassigned_three[8];

};

struct SegyFile {

  FILE* stream = nullptr;
  const char* filename = nullptr;
  bool is_open = false;

};



#endif //OERAD_SEGY_H
