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
* [x] gRPC to support RPC call
* [ ] provide command line interface

### Generation process

Each time you generate an ID, it works, like this.

* A `timestamp` with millisecond precision is stored using 41 bits of the ID.
* Then the `machineID` is added in subsequent bits.
* Then the `counter` is added, starting at 0 and incrementing for each ID generated in the same millisecond. If you generate enough IDs in the same millisecond that the sequence would roll over or overfill then the generate function will pause until the next millisecond.

### CLI

#### `cli` help docs:

```sh
➜  snowflake git:(master) ✗ ./package/osx/snowflake gen -h
NAME:
   snowflake gen - 

USAGE:
   snowflake gen [command options] [arguments...]

OPTIONS:
   --machineID value, -m value  specify the machine id (default: 0)
   --count value, -c value      count of ids to generate (default: 1)
   --output value, -o value     output stream, default output to stdout (default: "stdout")
```

#### usage sample:

```sh
➜  snowflake git:(master) ✗ ./package/osx/snowflake gen -m 0 -c 20            
1113312324124409856
1113312324124409857
1113312324124409858
1113312324124409859
1113312324124409860
1113312324128604160
1113312324128604161
1113312324128604162
1113312324128604163
1113312324128604164
1113312324128604165
1113312324128604166
1113312324128604167
1113312324128604168
1113312324128604169
1113312324128604170
1113312324128604171
1113312324128604172
1113312324128604173
1113312324128604174
➜  snowflake git:(master) ✗ ./package/osx/snowflake gen -m 0 -c 20 -o test.txt
➜  snowflake git:(master) ✗
```

### References

* [twitter-snowflake](https://github.com/twitter-archive/snowflake)
* [bwmarrin/snowflake](https://github.com/bwmarrin/snowflake)