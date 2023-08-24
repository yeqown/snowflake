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

#ifndef SNOWFLAKE_H
#define SNOWFLAKE_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#define EPOCH 1288834974657

// define the bits length of each part, and the total length is 64 bits
#define USELESS_BITS 1
#define TIMESTAMP_BITS 41
#define WORKER_ID_BITS 10
#define COUNT_BITS 12

// define the shift of each part, actually, the final id looks like:
// 0 | timestamp | worker_id | count
// so the shift is the length of the right of each part.
#define TIMESTAMP_SHIFT (WORKER_ID_BITS + COUNT_BITS) // 12 + 10 = 22
#define WORKER_ID_SHIFT COUNT_BITS                    // 12
#define COUNT_SHIFT 0                                 // 0

// define the max value of each part
#define MAX_WORKER_ID ((1 << WORKER_ID_BITS) - 1) // 1023
#define MAX_COUNT ((1 << COUNT_BITS) - 1)         // 4095

// define masks
#define MACHINE_ID_MASK (MAX_WORKER_ID << COUNT_BITS) // 1023 << 12 = 1047552
#define COUNT_MASK (MAX_COUNT << 0)                   // 4095 << 0 = 4095

#define ERR_INVALID_WORKER_ID -1 // invalid worker id
#define ERR_NULL_WORKER -2       // worker is null
#define ERR_NULL_IDS -3          // ids array are null
#define ERR_LOCKED -4            // worker is locked

// The snowflake worker is used to generate snowflake id which is a
// 64 bits unsigned integer.
typedef struct snowflake_Worker {
  uint64_t epoch;
  uint64_t worker_id;
  uint64_t count_per_mill;
  atomic_int lock;
} snowflake_Worker;

/* snowflake_NewWorker
 * create a new snowflake worker
 * @param worker_id: the worker id, must be less than 1024
 * @return: a new snowflake worker
 *
 * NOTE: the worker_id must be unique in your distributed system,
 *      or the snowflake id will be duplicated.
 *      the worker_id must be less than 1024, or it will be error.
 *      the worker_id could not be changed after you created the worker.
 *
 * NOTE: the worker is allocated by malloc, so you should free it after used.
 */
snowflake_Worker *snowflake_NewWorker(uint64_t worker_id);

/* snowflake_FreeWorker
 * free the snowflake worker
 * @param worker: the snowflake worker
 */
void snowflake_FreeWorker(snowflake_Worker *worker);

typedef uint64_t snowflake_ID;

/* snowflake_NextIds
 * generate new snowflake ids with the given worker and the number of ids
 * @param worker: the snowflake worker
 * @param n: the number of snowflake id to generate
 * @param snowflake_ID: the snowflake id
 * @return: the number of snowflake id generated, if the return value is
 * negative, it means some error happened.
 */
int snowflake_NextIds(snowflake_Worker *worker, uint64_t n, snowflake_ID *ids,
                      bool wait);

/* snowflake_NextId
 * generate new snowflake id with the given worker
 * @param worker: the snowflake worker
 * @return: the snowflake id, if the return value is negative,
 *         it means some error happened.
 */
snowflake_ID snowflake_NextId(snowflake_Worker *worker, bool wait);

/* snowflake_ToString
 * convert snowflake id to string
 * @param id: the snowflake id
 * @return: the string of snowflake id
 */
const char *snowflake_ToString(snowflake_ID id);

typedef struct snowflake_IDState {
  uint64_t timestamp;
  uint64_t worker_id;
  uint64_t count;
} snowflake_IDState;

/* snowflake_ParseId
 * parse snowflake id to snowflake id state
 * @param id: the snowflake id
 * @param state: the snowflake id state
 * @return: the snowflake id state
 *
 * NOTE: the state is allocated by malloc, so you should free it after used.
 */
const snowflake_IDState *snowflake_ParseId(snowflake_ID id);

#endif
