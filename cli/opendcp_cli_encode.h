/*
    OpenDCP: Builds Digital Cinema Packages
    Copyright (c) 2010-2014 Terrence Meiczinger, All Rights Reserved

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _OPENDCP_CLI_ENCODE_H_
#define _OPENDCP_CLI_ENCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FOREACH(THING, DO) THING(DO)

#define TO_INT(x) atoi(x)
#define TO_BOOL(x) x != NULL ? 1:0
#define STRING_TO_BOOL(x) !strcmp(x, "true") ? 0:1

#define COMMAND_ENUM(cmd, desc, count, args...) cmd,
#define COMMAND_SET(cmd, desc, count, ...) SET_VALUE(command, args, cmd)
#define COMMAND_INITIALIZE(name, desc, count, args...) {#name, desc, name, #args, count, 0},

#define ARGUMENT_SET(name, ...) SET_VALUE(argument, args, name)
#define ARGUMENT_INITIALIZE(name, desc) {#name, desc, NULL}, 

#define FLAG_SET(name, ...) SET_FLAG_VALUE(option, args, name)
#define OPTION_SET(name, ...) SET_VALUE(option, args, name)
#define OPTION_INITIALIZE(name, req, value, desc) {#name, desc, req, #value, #value},

#define SET_VALUE(src, dst, var) if (!strcmp(src->name, #var)) { dst->var = src->value; }
#define SET_FLAG_VALUE(src, dst, var) if (!strcmp(src->name, #var)) { dst->var = value; }

#define GENERATE_COMMAND(cmd, desc, count, args...) int cmd;
#define GENERATE_CHAR(name, ...) char* name;
#define GENERATE_OPTIONS(name, ...) char* name;
#define GENERATE_INT(name, ...) int name;

#define SET_args(arg, value) args->#arg = value;

/* (command, desc, arg count, args) */
#define COMMANDS(GENERATOR) \
     GENERATOR(j2k,    "Encode JPEG2000",              2, input,output)  \
     GENERATOR(j2k_3d, "Encode JPEG2000 Sterepscopic", 4, input_left,input_right,output_left,output_right)  \
     GENERATOR(mxf,    "Encode MXF",                   2, input,output)  \
     GENERATOR(mxf_3d, "Encode MXF Stereoscopic",      3, input_left,input_right,output)

/* (argument, desc) */
#define ARGUMENTS(GENERATOR) \
    GENERATOR(input,        "Input file or directory") \
    GENERATOR(input_left,   "Left input file or directory in 3D mode") \
    GENERATOR(input_right,  "Right input file or directory in 3D mode") \
    GENERATOR(output,       "Output file or directory") \
    GENERATOR(output_left,  "Left output file or directory in 3D mode") \
    GENERATOR(output_right, "Right output file or directory in 3D mode")

/* (option, value_required, default, desc) */
#define FLAGS(GENERATOR) \
    GENERATOR(help,           0, NULL,     "Show help") \
    GENERATOR(version,        0, NULL,     "Show version") \
    GENERATOR(stereoscopic,   0, NULL,     "Stereoscopic mode")
    
/* (option, short name, value_required, default, desc) */
#define OPTIONS(GENERATOR) \
    GENERATOR(bw,             1, 125,      "The bandwidth in MB/s for codestream") \
    GENERATOR(overwrite,      1, true,     "Overwrite existing files") \
    GENERATOR(xyz,            1, true,     "Perform XYZ<->RGB conversion") \
    GENERATOR(resize,         1, false,    "Resize image to closest DCI compliant resolution") \
    GENERATOR(colorspace,     1, rec709,   "Source colorspace (srgb|rec709|p3|srgb_complex|rec709_complex)") \
    GENERATOR(encoder,        1, openjpeg, "JPEG2000 j2k (openjpeg|kakadu)") \
    GENERATOR(profile,        1, 2k,       "The cinema profile (2k | 4k)") \
    GENERATOR(frame_rate,     1, 24,       "Frame rate of source") \
    GENERATOR(type,           1, smpte,    "Generate SMPTE or MXF Interop labels (smpte|interop)") \
    GENERATOR(start,          1, 1,        "The start frame") \
    GENERATOR(end,            1, 0,        "The end frame") \
    GENERATOR(slideshow,      1, 0,        "Create a slideshow with each image having duration specified in seconds") \
    GENERATOR(log_level,      1, 2,        "Sets the log level 0-4. Higher means more logging") \
    GENERATOR(key,            1, NULL,     "Set encryption key and enable encryption (not recommended)") \
    GENERATOR(key_id,         1, NULL,     "Set encryption key id (leaving blank generates a random uuid)") \
    GENERATOR(threads,        1, 0,        "The number of threads to use") \
    GENERATOR(tmp_path,       1, NULL,     "Temporary directory for intermediate files")

enum OPENDCP_CLI_COMMAND_ENUM {
    FOREACH(COMMANDS, COMMAND_ENUM)
};

typedef struct {
    /* main commands */
    FOREACH(COMMANDS, GENERATE_COMMAND)
    /* arguments */
    FOREACH(ARGUMENTS, GENERATE_CHAR)
    /* flags */
    FOREACH(FLAGS, GENERATE_INT)
    /* options */
    FOREACH(OPTIONS, GENERATE_CHAR)
} args_t;

typedef struct {
    const char *name;
    const char *description;
    int         enum_index;
    const char *args_list;
    int         args_required;
    int         value;
} command_t;

typedef struct {
    const char *name;
    const char *description;
    char       *value;
} argument_t;

typedef struct {
    const char *name;
    const char *description;
    int         value_required;
    const char *default_value;
    char       *value;
} option_t;

typedef struct {
    int           n_commands;
    int           n_arguments;
    int           n_options;
    int           n_commands_found;
    int           n_arguments_found;
    int           n_options_found;
    command_t    *commands;
    argument_t   *arguments;
    option_t     *options;
    const char         *app_name;
} cli_t;

typedef struct {
    int argc;
    char **argv;
    int i;
    char *current;
} argv_t;

int opendcp_command_j2k(opendcp_t *opendcp, args_t *args);

#ifdef __cplusplus
}
#endif

#endif // _OPENDCP_CLI_ENCODE_H_
