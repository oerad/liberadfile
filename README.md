# liberadfile
Logging library for georadar data 

1.  [Introduction](#introduction)
2.  [Installation](#installation)
3.  [File Structure](#file-structure)
4.  [Glossary](#glossary)
5.  [Library Structure](#library-structure)
6.  [Workflow](#workflow)
7.  [Examples](#examples)

### Introduction
LiberadFile is an open source C++ library for reading and logging trace data in the SEG-Y binary file format standard as well as in .erad file format. Only writing support is provided for the SEG-Y standard because of its complexity and variety - there are numerous both free and paid software packages for opening, viewing and manipulating data in the SEG-Y format. Erad on the other hand was created by Oerad Tech Ltd with simplicity in mind - it follows the SEG-Y paradigm of File Header + n*(Trace Header + data) but has stripped all unnecessary fields, simplified naming conventions and thus greatly reduced file size and readability. 

As of February 2019 .erad files are processed only by software developed by Oerad but this open source library is an invitation for incorporating this simple GPR data format in other tools and platforms.

This library is intended for software developers, researchers and students who wish to log and manipulate GPR data.


### Installation


### File Structure

#### Erad
The erad file type is composed of three main modules: A file header, a trace header and trace data. The file ends with a total trace count field. In the graphic below you can see the file structure. The file format is endianness-agnostic.

##### File Header

|Index in file  | Data Type  | Description                        | Value
|    :---:      |     :---:  |:-----                              | :-------- 
|     0         |`int8_t[8]` | Magic number                       | `{0x00, 0x45, 0x41, 0x53, 0x59, 0x52, 0x41, 0x44}`
|     8         |`int8_t`    | File version                       |
|     9         |`int8_t[2]` | Endianness marker                  |
|     11        |`int8_t`    | Radar type                         |
|     12        |`int16_t`   | Year                               |
|     14        |`int16_t`   | Month                              |
|     16        |`int16_t`   | Day                                |
|     18        |`int16_t`   | Dimension                          |
|     20        |`int16_t`   | Data offset                        |
|     22        |`float`     | Time window                        |
|     24        |`float`     | Total X                            |
|     28        |`float`     | Total Y                            |
|     32        |`int16_t`   | Sample size                        |
|     36        |`uint8_t`   | Steps per meter                    |
|     38        |`int8_t`    | Coordinate system                  |
|     40        |`float`     | Dielectric of medium               |
|     44        |`float`     | Interval along X                   |
|     48        |`float`     | Interval along Y                   |
|     52        |`char[58]`  | Operator                           |
|     110       |`char[102]` | Location                           |


##### Trace Header

Each trace header field has an byte index `FH_SIZE + trace_index_in_file*(TH_SIZE + sample_size) + N` where FH_SIZE is the size of the file header and TH_SIZE is the size of the trace header.

| N             | Data Type  | Description                        | Value
|    :---:      |     :---:  |:-----                              | :-------- 
|     0         |`int64_t`   | Trace index in file                | Start at 0 
|     8         |`int16_t`   | Sample size                        |
|     10        |`int16_t`   | Steps per trace                    |
|     12        |`int8_t`    | Hour                               |
|     13        |`int8_t`    | Minute                             |
|     14        |`int8_t`    | Second                             |
|     15        |`int16_t`   | Millisecond                        |
|     17        |`int32_t`   | Fold index                         |
|     21        |`int8_t`    | Fold orientation                   |
|     22        |`int32_t`   | Trace index in fold                |
|     26        |`double`    | X local                            |
|     34        |`double`    | Y local                            |
|     42        |`double`    | Z local                            |
|     50        |`double`    | Longitude                          |
|     58        |`double`    | Latitude                           |

##### Data

#### Segy




### Glossary


### Library Structure


### Workflow


### Examples 
