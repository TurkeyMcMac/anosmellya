#include "Options.hpp"
#include ".version.hpp"
#include "Random.hpp"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace anosmellya;

static char* get_arg(char* argv[], int& i)
{
    ++i;
    if (!argv[i]) {
        fprintf(stderr, "%s: Missing argument to option %s\n", argv[0],
            argv[i - 1]);
        exit(EXIT_FAILURE);
    }
    return argv[i];
}

static long get_num_arg(char* argv[], int& i, long lower, unsigned long upper)
{
    char* arg_string = get_arg(argv, i);
    char* end;
    long arg_long = strtol(arg_string, &end, 10);
    if (*end != '\0') {
        fprintf(stderr, "%s: Invalid numeric argument %s to option %s\n",
            argv[0], arg_string, argv[i - 1]);
        exit(EXIT_FAILURE);
    }
    if (arg_long < (long)lower || arg_long > (long)upper) {
        fprintf(stderr,
            "%s: Numeric argument %s to option %s out of range %ld to %ld\n",
            argv[0], arg_string, argv[i - 1], lower, upper);
        exit(EXIT_FAILURE);
    }
    return arg_long;
}

static void print_usage(char* progname, FILE* to)
{
    fprintf(to, "Usage: %s [options]\n", progname);
}

static void print_help(char* progname)
{
    print_usage(progname, stdout);
    puts("\
Options:\n\
 -conf <path>            Load the simulation configuration at <path>. The\n\
                         format is not described here.\n\
 -conf-str, -cs <conf>   Load configuration from the given string <conf>.\n\
 -world-width <width>    Make the world <width> tiles wide.\n\
 -world-height <height>  Make the world <height> tiles tall.\n\
 -seed <seed>            Set the initial random seed. The default is random.\n\
 -draw                   Open a window and draw the world every frame.\n\
                         This is the default.\n\
 -no-draw                Do not open a window or draw the world.\n\
 -print-stats            Print JSON statistics every tick.\n\
 -no-print-stats         Do not print statistics. This is the default.\n\
 -frame-delay <delay>    Delay for <delay> milliseconds per frame.\n\
 -pixel-size <size>      Set the simulation pixel size in screen pixels.\n\
 -help                   Print this help information.\n\
 -version                Print version information.");
}

static void print_version(char* progname)
{
    printf("%s version " VERSION "\n", progname);
}

Options::Options(int argc, char* argv[])
    : conf()
    , world_width(300)
    , world_height(210)
    , seed((uint32_t)time(NULL))
    , draw(true)
    , print_stats(false)
    , frame_delay(60)
    , pixel_size(3)
{
    char* progname = argv[0];
    for (int i = 1; i < argc; ++i) {
        char* opt = argv[i];
        if (opt[0] == '-' && opt[1] == '-') {
            ++opt;
        }
        if (!strcmp(opt, "-conf")) {
            char* file = get_arg(argv, i);
            if (conf.parse(file) < 0) {
                fprintf(stderr, "%s: Unable to parse configuration from '%s'\n",
                    progname, file);
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(opt, "-conf-str") || !strcmp(opt, "-cs")) {
            char* str = get_arg(argv, i);
            if (conf.parse_cstr(str) < 0) {
                fprintf(stderr,
                    "%s: Unable to parse configuration string '%s'\n", progname,
                    str);
                exit(EXIT_FAILURE);
            }
        } else if (!strcmp(opt, "-world-width")) {
            world_width = get_num_arg(argv, i, 1, 1000000);
        } else if (!strcmp(opt, "-world-height")) {
            world_height = get_num_arg(argv, i, 1, 1000000);
        } else if (!strcmp(opt, "-seed")) {
            seed = get_num_arg(argv, i, 0, Random::MAX_INT);
        } else if (!strcmp(opt, "-draw")) {
            draw = true;
        } else if (!strcmp(opt, "-no-draw")) {
            draw = false;
        } else if (!strcmp(opt, "-print-stats")) {
            print_stats = true;
        } else if (!strcmp(opt, "-no-print-stats")) {
            print_stats = false;
        } else if (!strcmp(opt, "-frame-delay")) {
            frame_delay = (unsigned)get_num_arg(argv, i, 0, 2000000000);
        } else if (!strcmp(opt, "-pixel-size")) {
            pixel_size = get_num_arg(argv, i, 1, 1000000);
        } else if (!strcmp(opt, "-help") || !strcmp(opt, "-h")) {
            print_help(progname);
            exit(EXIT_SUCCESS);
        } else if (!strcmp(opt, "-version") || !strcmp(opt, "-v")) {
            print_version(progname);
            exit(EXIT_SUCCESS);
        } else {
            fprintf(stderr, "%s: Invalid option: %s\n", progname, argv[i]);
            print_usage(progname, stderr);
            fprintf(stderr, "Type %s -help for more information\n", progname);
            exit(EXIT_FAILURE);
        }
    }
}
