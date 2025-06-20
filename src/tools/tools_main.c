#include <stdio.h>
#include <stdlib.h>

#include "obj_loader.h"

typedef enum {
  OBJ_LOADER,
  LIBS_NUM // Keep last - count of libraries supported
} SelectedLib;

// Options Errors
#define OPTIONS_OK 0
#define ARGUMENTS_ERROR 1

typedef struct {
  SelectedLib selected_lib;
  char* input_file_name;
} options_t;

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

  // Check if filename is something that exists
  printf("main_options->input_file_name: %s\n", main_options->input_file_name);

  return 0;
}
