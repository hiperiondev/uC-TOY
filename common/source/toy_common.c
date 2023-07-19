#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "toy_common.h"

//test variable sizes based on platform - see issue #35
#define STATIC_ASSERT(test_for_true) static_assert((test_for_true), "(" #test_for_true ") failed")

STATIC_ASSERT(sizeof(char) == 1);
STATIC_ASSERT(sizeof(short) == 2);
STATIC_ASSERT(sizeof(int) == 4);
STATIC_ASSERT(sizeof(float) == 4);
STATIC_ASSERT(sizeof(unsigned char) == 1);
STATIC_ASSERT(sizeof(unsigned short) == 2);
STATIC_ASSERT(sizeof(unsigned int) == 4);

static const char *build = __DATE__ " " __TIME__;

const char* Toy_private_version_build(void) {
    return build;
}

//declare the singleton with default values
Toy_CommandLine Toy_commandLine = {
        .error = false,
        .help = false,
        .version = false,
        .binaryfile = NULL,
        .sourcefile = NULL,
        .compilefile = NULL,
        .outfile = "out.tb",
        .source = NULL,
        .initialfile = NULL,
        .enablePrintNewline = true,
        .parseBytecodeHeader = false,
        .verbose = false
};

void Toy_initCommandLine(int argc, const char *argv[]) {
    for (int i = 1; i < argc; i++) { //start at 1 to skip the program name
        Toy_commandLine.error = true; //error state by default, set to false by successful flags

        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            Toy_commandLine.help = true;
            Toy_commandLine.error = false;
            continue;
        }

        if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            Toy_commandLine.version = true;
            Toy_commandLine.error = false;
            continue;
        }

        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
            Toy_commandLine.verbose = true;
            Toy_commandLine.error = false;
            continue;
        }

        if ((!strcmp(argv[i], "-f") || !strcmp(argv[i], "--sourcefile")) && i + 1 < argc) {
            Toy_commandLine.sourcefile = (char*) argv[i + 1];
            i++;
            Toy_commandLine.error = false;
            continue;
        }

        if ((!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input")) && i + 1 < argc) {
            Toy_commandLine.source = (char*) argv[i + 1];
            i++;
            Toy_commandLine.error = false;
            continue;
        }

        if ((!strcmp(argv[i], "-c") || !strcmp(argv[i], "--compile")) && i + 1 < argc) {
            Toy_commandLine.compilefile = (char*) argv[i + 1];
            i++;
            Toy_commandLine.error = false;
            continue;
        }

        if ((!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) && i + 1 < argc) {
            Toy_commandLine.outfile = (char*) argv[i + 1];
            i++;
            Toy_commandLine.error = false;
            continue;
        }

        if ((!strcmp(argv[i], "-t") || !strcmp(argv[i], "--initial")) && i + 1 < argc) {
            Toy_commandLine.initialfile = (char*) argv[i + 1];
            i++;
            Toy_commandLine.error = false;
            continue;
        }

        if (!strcmp(argv[i], "-p")) {
            Toy_commandLine.parseBytecodeHeader = true;

            if (Toy_commandLine.binaryfile) {
                Toy_commandLine.error = false;
            }

            continue;
        }

        if (!strcmp(argv[i], "-n")) {
            Toy_commandLine.enablePrintNewline = false;
            Toy_commandLine.error = false;
            continue;
        }

        //option without a flag + ending in .tb = binary input
        if (i < argc) {
            if (strncmp(&(argv[i][strlen(argv[i]) - 3]), ".tb", 3) == 0) {
                Toy_commandLine.binaryfile = (char*) argv[i];
                Toy_commandLine.error = false;
                continue;
            }
        }

        //don't keep reading in an error state
        return;
    }
}

void Toy_usageCommandLine(int argc, const char *argv[]) {
    printf("Usage: %s [ file.tb | -h | -v | -d | -f file.toy | -i source | -c file.toy -o out.tb | -t file.toy ]\n\n", argv[0]);
}

void Toy_helpCommandLine(int argc, const char *argv[]) {
    Toy_usageCommandLine(argc, argv);

    printf("  -h, --help\t\t\tShow this help then exit.\n");
    printf("  -v, --version\t\t\tShow version and copyright information then exit.\n");
    printf("  -d, --debug\t\t\tBe verbose when operating.\n");
    printf("  -f, --file filename\t\tParse, compile and execute the source file.\n");
    printf("  -i, --input source\t\tParse, compile and execute this given string of source code.\n");
    printf("  -c, --compile filename\tParse and compile the specified source file into an output file.\n");
    printf("  -o, --output outfile\t\tName of the output file built with --compile (default: out.tb).\n");
    printf("  -t, --initial filename\tStart the repl as normal, after first running the given file.\n");
    printf("  -p\t\t\t\tParse the given bytecode's header, then exit (requires file.tb).\n");
    printf("  -n\t\t\t\tDisable the newline character at the end of the print statement.\n");
}

void Toy_copyrightCommandLine(int argc, const char *argv[]) {
    printf("Toy Programming Language Interpreter Version %d.%d.%d (built on %s)\n\n", TOY_VERSION_MAJOR, TOY_VERSION_MINOR, TOY_VERSION_PATCH,
            TOY_VERSION_BUILD);
    printf("Copyright (c) 2020-2023 Kayne Ruse, KR Game Studios\n\n");
    printf(
            "This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.\n\n");
    printf(
            "Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:\n\n");
    printf(
            "1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.\n\n");
    printf("2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.\n\n");
    printf("3. This notice may not be removed or altered from any source distribution.\n\n");
}
