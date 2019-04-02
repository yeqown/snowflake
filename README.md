# snowflake

Snowflake is a network service for generating unique ID numbers at high scale with some simple guarantees.
This `Repository` is `Golang` implementation.

<img src="resoureses/format.png" width="400px"/>

### Features:

* support `cli`
* support RPC server with `gRPC`
* concurrent security

### TODO

* [x] finish id-generator
* [ ] gRPC to support RPC call
* [ ] provide command line interface

### Generation process

Each time you generate an ID, it works, like this.

* A `timestamp` with millisecond precision is stored using 41 bits of the ID.
* Then the `machineID` is added in subsequent bits.
* Then the `counter` is added, starting at 0 and incrementing for each ID generated in the same millisecond. If you generate enough IDs in the same millisecond that the sequence would roll over or overfill then the generate function will pause until the next millisecond.

### References

* [twitter-snowflake](https://github.com/twitter-archive/snowflake)
* [bwmarrin/snowflake](https://github.com/bwmarrin/snowflake)