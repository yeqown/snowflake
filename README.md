# snowflake

Snowflake is a network service for generating unique ID numbers at high scale with some simple guarantees.


<img src="assets/format.png" width="100%"/>

### Goal

- [x] developing a `c` library package.
- [x] quickly use as a CLI application.
- [x] extension to `lua`.
- [ ] extension to `go`.

### Installation

There are three ways to use `snowflake`:

- [Library](#library)
- [CLI application](#cli-application)
- [Lua extension](#lua-extension)

#### Library

In this way, you can use `snowflake` as a static library in your project.

```bash
mkdir build && cd build
cmake ..

make snowflake_lib
```

Then you can copy the `build/libsnowflake.a` to your project or install it to your system manually.

#### CLI application

In this way, you can use `snowflake` as a CLI application.

```bash
mkdir build && cd build
cmake ..

make snowflake_cli
```

Then you can copy the `build/cmd/snowflake/snowflake` to your system path.

### Lua extension

In this way, you can use `snowflake` as a lua extension.

```bash
mkdir build && cd build
cmake ..

make snowflake_lua
```

Then you can copy the `build/contrib/lua/snowflake.so` to your lua `package.cpath`.

### Generation process

Each time you generate an ID, it works, like this.

* A `timestamp` with millisecond precision is stored using 41 bits of the ID.
* Then the `machineID` is added in subsequent bits.
* Then the `counter` is added, starting at 0 and incrementing for each ID generated in the same millisecond. If you generate enough IDs in the same millisecond that the sequence would roll over or overfill then the generate function will pause until the next millisecond.


### References

* [twitter-snowflake](https://github.com/twitter-archive/snowflake)
* thanks to [bwmarrin/snowflake](https://github.com/bwmarrin/snowflake)