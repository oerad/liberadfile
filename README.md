# liberadfile
Logging library for georadar data 

1.  [Introduction](#introduction)
2.  [Installation](#installation)
3.  [File Structure](#file-structure)
4.  [Glossary](#glossary)
5.  [Workflow](#workflow)
6.  [Examples](#examples)

### Introduction
LiberadFile is an open source C++ library for reading and logging trace data in the SEG-Y binary file format standard as well as in .erad file format. Only writing support is provided for the SEG-Y standard because of its complexity and variety - there are numerous both free and paid software packages for opening, viewing and manipulating data in the SEG-Y format. Erad on the other hand was created by Oerad Tech Ltd with simplicity in mind - it follows the SEG-Y paradigm of File Header + n*(Trace Header + data) but has stripped all unnecessary fields, simplified naming conventions and thus greatly reduced file size and readability. 

As of February 2019 .erad files are processed only by software developed by Oerad but this open source library is an invitation for incorporating this simple GPR data format in other tools and platforms.

This library is intended for software developers, researchers and students who wish to log and manipulate GPR data.


### Installation


### File Structure

#### Erad
The erad file type is composed of three main modules: A file header, a trace header and trace data. The file ends with a total trace count field. In the graphic below you can see the file structure. The file format is endianness-agnostic.
![Erad file format structure](https://i.imgur.com/KTs4cgE.png)

##### File Header

|Index in file  | Data Type  | Description                        | Value
|    :---:      |     :---:  |:-----                              | :-------- 
|     0         |`int8_t[8]` | Magic number                       | `{0x00, 0x45, 0x41, 0x53, 0x59, 0x52, 0x41, 0x44}`
|     8         |`int8_t`    | File version                       | `{VER_2018 = 0x03, VER_2019 = 0x04}`
|     9         |`int8_t[2]` | Endianness marker                  | Big endian:`{0xFE, 0xFF}` Little endian: `{0xFF, 0xFE}`
|     11        |`int8_t`    | Hardware version                   | `{PRE2017 = 0x01, POST2017 = 0x05}`
|     12        |`int8_t`    | Radar type                         |`{SCUDO = 0x09, DIPOLO = 0x0B, CONCRETTO = 0x0A, CUSTOM = 0x08}`
|     14        |`int16_t`   | Year                               |
|     16        |`int16_t`   | Month                              |
|     18        |`int16_t`   | Day                                |
|     20        |`int16_t`   | Dimension                          |`{SINGLE_SLICE_TEMPORAL = 0x00, SINGLE_SLICE_SPATIAL = 0x01, VERTICAL_3D = 0x02, HORIZONTAL_3D = 0x03, VERTICAL_HORIZONTAL = 0x04, VERTICAL_SLICES_TEMPORAL= 0x05, HORIZONTAL_SLICES_TEMPORAL = 0x06, VER_HOR_TEMPORAL = 0x07}`
|     22        |`int16_t`   | Data offset to first trace         | `212`
|     24        |`float`     | Time window                        | This value depends on your GPR
|     28        |`float`     | Total X                            | 0 if Dimension < `VERTICAL_3D`
|     32        |`float`     | Total Y                            | 0 if Dimension < `VERTICAL_3D`
|     36        |`int16_t`   | Sample size                        | Usually 585 for Hardware version >= `POST2017`
|     38        |`uint8_t`   | Steps per meter                    | Value of recorded signals on a stepped wheel encoder over a 1 meter distance
|     39        |`int8_t`    | Coordinate system                  |`{GLOBAL = 0x01, LOCAL = 0x02, GLOBAL_LOCAL = 0x03}`
|     40        |`float`     | Dielectric of medium               | 
|     44        |`float`     | Interval along X                   | 0 if Dimension < `VERTICAL_3D` || Dimension == `HORIZONTAL_3D`
|     48        |`float`     | Interval along Y                   | 0 if Dimension < `HORIZONTAL_3D`
|     52        |`char[58]`  | Operator                           |
|     110       |`char[102]` | Location                           |


##### Trace Header
Each trace header field has an byte index `FH_SIZE + trace_index_in_file*(TH_SIZE + sample_size) + N` where FH_SIZE is the size of the file header and TH_SIZE is the size of the trace header.

| N             | Data Type  | Description                        | Value
|    :---:      |     :---:  |:-----                              | :-------- 
|     0         |`int64_t`   | Trace index in file                | Start at 0 
|     8         |`int16_t`   | Sample size                        | Usually 585 for modern Oerad hardware. Number of sample points per trace 
|     10        |`int16_t`   | Steps per trace                    | Recorded step signals from stepped wheel encoder per sample
|     12        |`int8_t`    | Hour                               |
|     13        |`int8_t`    | Minute                             |
|     14        |`int8_t`    | Second                             |
|     15        |`int16_t`   | Millisecond                        | 
|     17        |`int32_t`   | Fold index                         | Start at 0
|     21        |`int8_t`    | Fold orientation                   | `{VERTICAL = 0x00, HORIZONTAL = 0x01}`
|     22        |`int32_t`   | Trace index in fold                |
|     26        |`double`    | X local                            | In meters from survey start point
|     34        |`double`    | Y local                            | In meters from survey start point
|     42        |`double`    | Z local                            | In meters from survey start point
|     50        |`double`    | Longitude                          | In degrees as fixed point value. Negative for S and W, positive for N and E
|     58        |`double`    | Latitude                           | In degrees as fixed point value. Negative for S and W, positive for N and E

##### Data
Data from Oerad radar systems is encoded in 585-byte traces that are fed to the acquisition system every 55ms. Each sample point is an unsigned integer (uint8_t). In Oerad's hardware each data sample is derived from 32 separate measurements for Scudo and Dipolo, and 384 measurements per sample for Concretto. 

#### Segy
Liberadfile takes on a simplistic approach with regards to the SEG-Y standard. For a detailed account on the file structure please refer to [the official standard definition](https://seg.org/Portals/0/SEG/News%20and%20Resources/Technical%20Standards/seg_y_rev2_0-mar2017.pdf). The general structure of the segy is
    `Segy Textual Header + Segy Binary Header + N*(Segy Trace Header + Trace Data)`
In practice there may be extended textual headers of both the whole file and the traces. Liberadfile omits these.


### Glossary

![liberad gpr glossary](https://i.imgur.com/ujKHVlO.jpg)



### Workflow
![liberadfile workflow diagram](https://i.imgur.com/YWUhj0G.png)


### Examples 
