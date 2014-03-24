#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"

int yyparse();

const char *USAGE = "Usage: " PACKAGE " [-h|-o|-t] file\n"
	"Options:\n"
	"  -h --help                Display this information\n"
	"  -o --output=<file>       Compile and place the executable into <file>\n"
	"  -t --translate           Translate the source files into c++\n"
	"                           xxx.avl -> xxx.cpp\n"
	"Use at most one option at a time.\n"
	"\n"
	"For bug reporting instructions, please see:\n"
	"<" PACKAGE_URL ">.\n";

#define MAX_FILENAME 256
char input_file[MAX_FILENAME];
char output_file[MAX_FILENAME];
char translate_file[MAX_FILENAME];
char temp_file[MAX_FILENAME];
int translate_flag = 0;
int output_flag = 0;

extern FILE *yyin;
extern FILE *yyout;

const char *const DEFAULT_OUTPUT = "a.out";
const char *const DEFAULT_EXT = ".avl";
const char *const TRANSLATE_EXT = ".cpp";
const char *const DEFAULT_TEMP = "/tmp/avl_temp.XXXXXX";
char *const CXX_OPTIONS[] = {
	"g++",
	"-x",
	"c++",
	"-std=c++11",
	"-o",
	output_file,
	"-I/opt/local/include",
	"-L/opt/local/lib",
	"-lavl",
	"-lGL",
	"-lglut",
	temp_file,
	NULL
};

const struct option long_options[] = {
	{"help",      no_argument,       NULL, 'h'},
	{"output",    required_argument, NULL, 'o'},
	{"translate", no_argument,       NULL, 't'},
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

void die_err(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void parse_command_line(int argc, char *argv[])
{
	memset(input_file, 0, MAX_FILENAME);
	memset(output_file, 0, MAX_FILENAME);
	memset(translate_file, 0, MAX_FILENAME);
	memset(temp_file, 0, MAX_FILENAME);
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
		die("Please specify at most one option at a time.");

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
		die_err("invalid input file");

	if (translate_flag) {
		strcpy(translate_file, input_file);
		strcpy(translate_file + strlen(translate_file) - strlen(DEFAULT_EXT),
				TRANSLATE_EXT);
	}
}

int main(int argc, char *argv[])
{
	parse_command_line(argc, argv);

	yyin = fopen(input_file, "r");
	if (!yyin)
		die_err("can not open input file");

	if (translate_flag) {
		yyout = fopen(translate_file, "w");
		if (!yyout)
			die_err("can not open translate file");
	}
	else {
		int fd = mkstemp(temp_file);
		yyout = fdopen(fd, "w");
		if (!yyout)
			die_err("can not open temporary file");
	}

	if (yyparse() != 0)
		die("Parsing failed.");

	fclose(yyin);
	fclose(yyout);

	if (!translate_flag) {
		pid_t pid = fork();

		if (pid < 0)
			die_err("fork() failed");
		else if (pid == 0) {
			char *const *s = CXX_OPTIONS;
			while (*s)
				printf("%s ", *s++);
			printf("\n");
			if (execvp(CXX_OPTIONS[0], CXX_OPTIONS) < 0)
				die_err("execlp() failed");
		}

		int status;
		if (waitpid(pid, &status, 0) != pid)
			die_err("waitpid() failed");
		if (!status) {
			die("failed to compile the temporary file.");
		}
	}

	return 0;
}
