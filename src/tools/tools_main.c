#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obj_loader.h"

typedef enum {
  OBJ_LOADER,
  LIBS_NUM // Keep last - count of libraries supported
} SelectedLib;

// Options Errors
#define OPTIONS_OK 0
#define ARGUMENTS_ERROR 1

#define FILE_OK 0
#define FILE_NOT_OK 1
#define FILE_NOT_OBJ 2

typedef struct {
  SelectedLib selected_lib;
  char* input_file_name;
} options_t;

typedef struct {
  char* mtllib_file;
  char* object_name; // Assuming one object name per file - triangulated mesh export from bforartists
  int num_verts;
  int num_faces;
} fileinfo_t;

void print_help_message() {
  printf("Usage: render_tools [ARGUMENTS]\n");
  printf("\n");
  printf("Description:\n");
  printf("render_tools are tools used for work required to prepare data to be rendered. For now loads OBJ files as a standalone tool for tests and verification and as a libraries used in the project.\n");
  printf("\n");
  printf("Arguments:\n");
  printf("ARG1\tInput file name.\n");
}

static options_t* main_options;
static FILE* obj_file;

int string_split(char* string, char* delimeter, char* splitted_elements[]) {
  // Function returns number of splitted elements (how much is after splitting)
  // 0 means no splitting has occured
  // So the function returns 0 or 2 or more
  (void) splitted_elements;

  size_t string_len = strlen(string);
  size_t delimeter_len = strlen(delimeter);
  for (size_t x = 0; x < string_len - delimeter_len; ++x) {
    printf("x: %zd, string_len: %zd, delimeter_len: %zd\n", x, string_len, delimeter_len);
  }

  return 0;
}

int parse_file(char* filename) {
  // Checking file opening
  errno_t obj_file_err = fopen_s(&obj_file, filename, "r");
  printf("obj_file_err: %d, obj_file: %p\n", obj_file_err, obj_file);

  if (!(obj_file_err == 0)) return FILE_NOT_OK;

  // TEST - print file line by line
  char buffer[1024];

  while (fgets(buffer, sizeof(buffer), obj_file) != NULL) {
    printf("%s", buffer);
    // printf("position: %ld\n", ftell(obj_file)); // Peeking into FILE structure
  }

  // In this function, we assume bforartists format that has "o" and "mtllib" section
  // and some number of "v" (vertices) and "f" (faces) sections

  // Test string split
  char** split_buffer = NULL;
  int splitted_num = string_split("Mama ma kota", " ", split_buffer);
  printf("splitted_num: %d\n", splitted_num);

  // Check for mtllib value
  // Check for o (object) value

  // All done - closing file
  fclose(obj_file);
  return FILE_OK;
}

int parse_options(int argc, char** argv, options_t* main_opt) {
  printf("Parsing options\n");

  // Setting up selected lib as a OBJ loader - for now
  main_opt->selected_lib = OBJ_LOADER;

  // Parse stuff
  if (argc < 2) {
    print_help_message();
    return ARGUMENTS_ERROR;
  } else {
    // Only option for now is a filename with OBJ data
    // printf(argv[argc - 1]); // argc is out of bounds for an array
    main_opt->input_file_name = argv[argc - 1]; // First argument is filename.
  }

  return OPTIONS_OK;
}

int main(int argc, char* argv[])
{
  // Welcome message
  printf("Tools binary - 3D Software Rendering libs.\n");

  // Initial allocation
  main_options = malloc(sizeof(options_t));

  // Parsing CLI options
  int parse_ret_code = parse_options(argc, argv, main_options);
  if (parse_ret_code != OPTIONS_OK) return parse_ret_code;

  // Everything is correct here - we can proceed
  // Check if filename is something that exists
  printf("main_options->input_file_name: %s\n", main_options->input_file_name);
  int parse_file_ret = parse_file(main_options->input_file_name);
  (void) parse_file_ret;

  // Cleanup
  free(main_options);

  return 0;
}
