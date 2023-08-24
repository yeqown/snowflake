require "snowflake"

print("   NAME: " .. snowflake._NAME)
print("VERSION: " .. snowflake._VERSION)
print(" AUTHOR: " .. snowflake._AUTHOR)

local worker = snowflake.new(1)

local id = worker.next_id()
print("generating one id: " .. id .. "\n")

print("parsing snowflake id: " .. id)
local state = snowflake.parse(id);
print("timestamp: " .. state.timestamp)
print("worker_id: " .. state.worker_id)
print("    count: " .. state.count .. "\n")

print("generating 10 ids in a batch: ")
local ids = worker.next_ids(10)
for i, id in ipairs(ids) do
    print(id)
end