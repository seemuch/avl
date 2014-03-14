#include <sys/stat.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "config.h"

const char *USAGE = "Usage: " PACKAGE_STRING " [-h|-o|-t] file\n"
	"Options:\n"
	"  -h --help                Display this information\n"
	"  -o --output=<file>       Place the output into <file>\n"
	"  -t --translate           Translate the source files into c++\n"
	"                           xxx.avl -> xxx.cpp\n"
	"Use only one option at a time.\n"
	"\n"
	"For bug reporting instructions, please see:\n"
	"<" PACKAGE_URL ">.\n";

void usage()
{
	fprintf(stderr, "%s", USAGE);
}

void die(const char *msg)
{
	fprintf(stderr, PACKAGE ": %s\n", msg);
	exit(EXIT_FAILURE);
}

#define MAX_FILENAME 256
const char *DEFAULT_OUTPUT = "a.out";
const char *DEFAULT_EXT = ".avl";
const char *TRANSLATE_EXT = ".cpp";
const char *DEFAULT_TEMP = "/tmp/avl_temp.cpp";

char input_file[MAX_FILENAME];
char output_file[MAX_FILENAME];
char translate_file[MAX_FILENAME];
char temp_file[MAX_FILENAME];
int translate_flag = 0;
int output_flag = 0;

const struct option long_options[] = {
	{"help",      no_argument,       NULL, 'h'},
	{"output",    required_argument, NULL, 'o'},
	{"translate", no_argument,       NULL, 't'},
	{0, 0, 0, 0}
};

void parse_command_line(int argc, char *argv[])
{
	memset(input_file, 0, MAX_FILENAME);
	memset(output_file, 0, MAX_FILENAME);
	memset(translate_file, 0, MAX_FILENAME);
	strcpy(output_file, DEFAULT_OUTPUT);
	strcpy(temp_file, DEFAULT_TEMP);

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
				if (strlen(optarg) <= 0)
					die("no output filename.");
				else if (strlen(optarg) >= MAX_FILENAME)
					die("output filename too long.");
				strcpy(output_file, optarg);
				output_flag = 1;
				break;
			case 't':
				translate_flag = 1;
				break;
			default:
				usage();
				exit(EXIT_FAILURE);
		}
	}

	if (output_flag && translate_flag)
		die("Please specify only one option at a time.");

	if (optind != argc - 1) {
		usage();
		exit(EXIT_FAILURE);
	}
	if (strlen(argv[optind]) <= strlen(DEFAULT_EXT)
			|| strlen(argv[optind]) >= MAX_FILENAME)
		die("invalid input filename.");
	strcpy(input_file, argv[optind]);
	if (strcmp(input_file + strlen(input_file) - strlen(DEFAULT_EXT),
				DEFAULT_EXT) != 0)
		die("not a valid avl source file.");

	struct stat buf;
	if (stat(input_file, &buf) != 0)
		die("input file does not exist.");

	if (translate_flag) {
		strcpy(translate_file, input_file);
		strcpy(translate_file + strlen(translate_file) - strlen(DEFAULT_EXT),
				TRANSLATE_EXT);
	}
}

extern FILE *yyin;
extern FILE *yyout;

int main(int argc, char *argv[])
{
	parse_command_line(argc, argv);

	yyin = fopen(input_file, "r");
	if (!yyin)
		die("can not open input file.");

	if (translate_flag) {
		yyout = fopen(translate_file, "w");
		if (!yyout)
			die("can not open translate file.");
	}
	else {
		yyout = fopen(temp_file, "w");
		if (!yyout)
			die("can not open temporary file.");
	}

	yyparse();

	fclose(yyin);
	fclose(yyout);

	return 0;
}
