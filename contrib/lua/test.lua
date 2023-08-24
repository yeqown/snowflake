require "snowflake"

snowflake.new()

print("creating a worker\n\n")
worker = snowflake.new(1)

print("generating one id")
id = worker.next_id()
print(id.. "\n\n")

print("generating 100 ids")
ids = worker.next_ids(100)
for i, id in ipairs(ids) do
    print(id)
end