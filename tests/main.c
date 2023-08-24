#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "snowflake.h"

int main(void) {
  snowflake_Worker *worker = snowflake_NewWorker(1);
  if (NULL == worker) {
    printf("error: worker is null\n");
    return 1;
  }
  snowflake_ID id = snowflake_NextId(worker, true);
  printf("got snowflakeID %s\n", snowflake_ToString(id));

  const snowflake_IDState *state = snowflake_ParseId(id);
  printf("timestamp: %llu\n", state->timestamp);
  printf("worker_id: %llu\n", state->worker_id);
  printf("count: %llu\n", state->count);

  snowflake_ID *ids = malloc(sizeof(snowflake_ID) * 100);
  if (snowflake_NextIds(worker, 100, ids, false) <= 0) {
    printf("error: generate 100 snowflakeIds failed\n");
    goto quit;
  }

  for (int i = 0; i < 100; i++) {
    printf("got snowflakeID %s\n", snowflake_ToString(ids[i]));
  }

quit:
  free((void*)state);
  free(ids);
  snowflake_FreeWorker(worker);

  return 0;
}
