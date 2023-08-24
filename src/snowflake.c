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

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>

#include "snowflake.h"

static uint64_t getTimestamp(void);

snowflake_Worker *snowflake_NewWorker(uint64_t worker_id) {
  if (worker_id > MAX_WORKER_ID) {
    return NULL;
  }

  snowflake_Worker *worker = malloc(sizeof(snowflake_Worker));
  if (NULL == worker) {
    return NULL;
  }

  worker->epoch = getTimestamp();
  worker->worker_id = worker_id;
  worker->count_per_mill = 0;
  atomic_init(&(worker->lock), 0);

  return worker;
}

void snowflake_FreeWorker(snowflake_Worker *worker) {
  if (NULL != worker) {
    free(worker);
  }
}

static uint64_t getTimestamp(void) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  return (uint64_t) now.tv_sec * 1000 + (uint64_t) now.tv_nsec / 1000000;
}

snowflake_ID nextId(snowflake_Worker *worker) {
  // get millisecond timestamp
  uint64_t now = getTimestamp();
  if (worker->epoch == now) {
    worker->count_per_mill = (worker->count_per_mill + 1) & COUNT_MASK;
    if (worker->count_per_mill == 0) {
      // overflow, wait next millisecond
      while (now <= worker->epoch) {
        now = getTimestamp();
      }
    }
  } else {
    worker->count_per_mill = 0;
  }

  worker->epoch = now;
  uint64_t worker_id = worker->worker_id;
  uint64_t t = now - EPOCH;
  uint64_t count = worker->count_per_mill;

  return (snowflake_ID)((t << TIMESTAMP_SHIFT) |
      (worker_id << WORKER_ID_SHIFT) | count);
}

#define cas(ptr, old, new) atomic_compare_exchange_strong(ptr, old, new)

bool acquireLock(snowflake_Worker *worker, bool wait) {
  if (NULL == worker) {
    return false;
  }

  int expected = 0;
  if (!wait) return cas(&worker->lock, &expected, 0);
  while (!cas(&worker->lock, &expected, 0)) { /* spin */ }

  return true;
}

void releaseLock(snowflake_Worker *worker) {
  int expected = 1;
  cas(&worker->lock, &expected, 1);
}

snowflake_ID snowflake_NextId(snowflake_Worker *worker, bool wait) {
  if (NULL == worker) {
    return ERR_NULL_WORKER;
  }

  bool got = acquireLock(worker, wait);
  if (!got) {
    return ERR_LOCKED;
  }

  snowflake_ID id = nextId(worker);
  releaseLock(worker);

  return id;
}

int snowflake_NextIds(snowflake_Worker *worker, uint64_t n, snowflake_ID *ids,
                      bool wait) {
  if (NULL == worker) {
    return ERR_NULL_WORKER;
  }
  if (NULL == ids) {
    return ERR_NULL_IDS;
  }

  if (n == 0) {
    return 0;
  }

  for (uint64_t i = 0; i < n; i++) {
    ids[i] = snowflake_NextId(worker, wait);
  }

  return (int)n;
}

const char *snowflake_ToString(snowflake_ID id) {
  static char str[20];
  sprintf(str, "%llu", id);
  return str;
}

const snowflake_IDState *snowflake_ParseId(snowflake_ID id) {
  struct snowflake_IDState *state = malloc(sizeof(snowflake_IDState));

  state->timestamp = (id >> TIMESTAMP_SHIFT) + EPOCH;
  state->worker_id = (id & MACHINE_ID_MASK) >> WORKER_ID_SHIFT;
  state->count = id & COUNT_MASK;

  return state;
}
