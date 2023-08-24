/* Copyright (C) 2023
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "snowflake.h"

#define VERSION "0.1.0"
#define printError(format, ...) printf("\033[1;31mError: " format "\033[0m\n", __VA_ARGS__)

/*
 * This program is a CLI tool to generate snowflake id.
 * Usage:
 *
 * snowflake [global flags] [subcommand] [flags] [args]
 *
 * global flags:
 *  -h, --help    show this help message
 *  -v, --version show version
 *
 * subcommands:
 *  gen    generate snowflake id
 *  parse  to parse snowflake id to timestamp, worker id and count
 *
 * gen flags:
 *  -w, --worker-id int worker id (default 1)
 *  -n, --number int    number of ids to generate (default 1)
 */

typedef struct genCmd {
  uint64_t worker_id;
  uint64_t number;
} genCmd;

typedef struct parseCmd {
  snowflake_ID id;
} parseCmd;

void parseGenCmd(genCmd *cmd, int argc, char *argv[]);
void parseParseCmd(parseCmd *cmd, int argc, char *argv[]);
void parse(parseCmd *cmd);
void gen(genCmd *cmd);

const char *usage =
    "Usage:\n"
    "    snowflake [global flags] [subcommand] [flags] [args]\n\n"
    "global flags: \n"
    "    -h, --help    show this help message\n"
    "    -v, --version show version\n\n"
    "subcommands:\n"
    "    gen    generate snowflake id\n"
    "    parse  to parse snowflake id to timestamp, worker id and count\n\n"
    "gen flags:\n"
    "    -w, --worker-id int worker id (default 1)\n"
    "    -n, --number int    number of ids to generate (default 1)\n";

void printUsage(void) {
  printf("%s", usage);
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printUsage();
    return 1;
  }

  int i = 1;

  // handle global flags
  if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
    printUsage();
  } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
    printf("snowflake version %s\n", VERSION);
    return 0;
  }

  // handle subcommands
  if (strcmp(argv[i], "gen") == 0) {
    genCmd cmd = {
        .worker_id = 1,
        .number = 1,
    };
    parseGenCmd(&cmd, argc, argv);
    gen(&cmd);
  } else if (strcmp(argv[i], "parse") == 0) {
    parseCmd cmd = {
        .id = 0,
    };
    parseParseCmd(&cmd, argc, argv);
    parse(&cmd);
  } else {
    // printf with red color
    printError("unknown subcommand %s", argv[i]);
    printUsage();
  }

  return 0;
}

const char *timestampToString(uint64_t timestamp) {
  time_t t = (long) timestamp / 1000;
  struct tm *tm = localtime(&t);
  static char buf[64];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
  return buf;
}

void parseParseCmd(parseCmd *cmd, int argc, char *argv[]) {
  if (argc < 3) {
    return;
  }

  cmd->id = strtol(argv[2], NULL, 10);
}

void parse(parseCmd *cmd) {
  if (cmd->id == 0) {
    printError("parse command need an snowflake id and it should be a number%s", "");
    exit(1);
  }

  const snowflake_IDState *state = snowflake_ParseId((snowflake_ID) cmd->id);
  printf("timestamp: %llu (%s)\n", state->timestamp, timestampToString(state->timestamp));
  printf("worker_id: %llu\n", state->worker_id);
  printf("    count: %llu\n", state->count);
}

void parseGenCmd(genCmd *cmd, int argc, char *argv[]) {
  if (argc < 3) {
    return;
  }

  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--worker-id") == 0) {
      if (i + 1 < argc) {
        cmd->worker_id = strtol(argv[i + 1], NULL, 10);
      }
    } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0) {
      if (i + 1 < argc) {
        cmd->number = strtol(argv[i + 1], NULL, 10);
      }
    }
  }
}

void gen(genCmd *cmd) {
  snowflake_Worker *worker = snowflake_NewWorker(cmd->worker_id);
  if (NULL == worker) {
    printError("initialize snowflake worker failed: %d", errno);
    exit(1);
  }

  snowflake_ID *ids = malloc(sizeof(snowflake_ID) * cmd->number);
  if (snowflake_NextIds(worker, cmd->number, ids, true) <= 0) {
    printError("generate %lld snowflakeIds failed", cmd->number);
    goto quit;
  }

  for (uint64_t i = 0; i < cmd->number; i++) {
    printf("%lld\n", ids[i]);
  }

quit:
  free(ids);
  snowflake_FreeWorker(worker);
}
