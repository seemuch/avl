#include <unistd.h>
#include <sys/stat.h>
#include <getopt.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "config.h"

const char *USAGE = "Usage: " PACKAGE_STRING " [options] file...\n"
	"Options:\n"
	"  -h --help                Display this information\n"
	"  -o --output=<file>       Place the output into <file>\n"
	"  --translate[=file]       Translate the source files into c++ [file]\n"
	"                           If [file] is not specified, xxx.avl -> xxx.cpp\n"
	"\n"
	"For bug reporting instructions, please see:\n"
	"<" PACKAGE_URL ">.\n";

const struct option long_options[] = {
	{"help", no_argument, NULL, 'h'},
	{"output", required_argument, NULL, 'o'},
	{"translate", optional_argument, NULL, 't'},
	{0, 0, 0, 0}
};

void usage()
{
	fprintf(stderr, "%s", USAGE);
}

void die(const char *msg)
{
	fprintf(stderr, PACKAGE ": %s\n", msg);
	exit(EXIT_FAILURE);
}

const size_t MAX_FILENAME = 128;
const char *DEFAULT_OUTPUT = "a.out";
const char *DEFAULT_EXT = ".avl";
const char *TRANSLATE_EXT = ".cpp";

int main(int argc, char *argv[])
{
	char input_file[MAX_FILENAME];
	char output_file[MAX_FILENAME];
	char translate_file[MAX_FILENAME];
	int translate_flag = 0;

	memset(input_file, 0, MAX_FILENAME);
	memset(output_file, 0, MAX_FILENAME);
	memset(translate_file, 0, MAX_FILENAME);
	strcpy(output_file, DEFAULT_OUTPUT);

	while (1) {
		int option_index = 0;
		char c = getopt_long(argc, argv, "ho:t", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			case 'o':
				if (!optarg) {
					usage();
					exit(EXIT_FAILURE);
				}
				if (strlen(optarg) <= 0 || strlen(optarg) >= MAX_FILENAME)
					die("Incorrect output filename.");
				strcpy(output_file, optarg);
				break;
			case 't':
				if (optarg) {
					if (strlen(optarg) <= 0 || strlen(optarg) >= MAX_FILENAME)
						die("Incorrect translate filename.");
					strcpy(translate_file, optarg);
				}
				translate_flag = 1;
				break;
			default:
				usage();
				exit(EXIT_FAILURE);
		}
	}

	if (optind != argc - 1) {
		usage();
		exit(EXIT_FAILURE);
	}
	if (strlen(argv[optind]) <= strlen(DEFAULT_EXT)
			|| strlen(argv[optind]) >= MAX_FILENAME)
		die("Incorrect input filename.");
	strcpy(input_file, argv[optind]);
	if (strcmp(input_file + strlen(input_file) - strlen(DEFAULT_EXT),
				DEFAULT_EXT) != 0)
		die("Not a valid avl source file.");

	struct stat buf;
	if (stat(input_file, &buf) != 0)
		die("Input filename does not exist.");

	if (translate_flag) {
		if (strlen(translate_file) == 0) {
			strcpy(translate_file, input_file);
			strcpy(translate_file + strlen(translate_file) - strlen(DEFAULT_EXT),
					TRANSLATE_EXT);
		}
	}

	printf("Input: %s\n", input_file);
	printf("Output: %s\n", output_file);
	printf("Translate: %s\n", translate_file);

	return 0;
}
