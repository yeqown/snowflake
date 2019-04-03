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
* [x] provide command line interface

### Generation process

Each time you generate an ID, it works, like this.

* A `timestamp` with millisecond precision is stored using 41 bits of the ID.
* Then the `machineID` is added in subsequent bits.
* Then the `counter` is added, starting at 0 and incrementing for each ID generated in the same millisecond. If you generate enough IDs in the same millisecond that the sequence would roll over or overfill then the generate function will pause until the next millisecond.

### Be imported as a package

```go
package main

import (
   "github.com/yeqown/snowflake"
)

func main() {
   worker, err := snowflake.New(1)
   if err != nil {
      panic(err)
   }

   id := worker.Generate()
   println(id) // id like: 1113311935803162633
}
```

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

### RPC call

[client.go](examples/grpc-client/client.go) CODE:

```go
package main

import (
	"context"
	"log"

	"github.com/yeqown/snowflake/rpc"
	"google.golang.org/grpc"
)

func main() {
	var (
		err  error
		conn *grpc.ClientConn
	)

	if conn, err = grpc.Dial("localhost:50051", grpc.WithInsecure()); err != nil {
		log.Fatal(err)
	}
	defer conn.Close()

	reply, err := rpc.NewWorkerClient(conn).Generate(context.Background(), &rpc.Request{Count: 10})
	if err != nil {
		log.Fatal(err)
	}

	for _, v := range reply.GetID() {
		println(v)
	}
}

```

[server.go](examples/grpc-server/server.go) CODE:

```go
package main

import (
	"log"
	"net"

	"github.com/yeqown/snowflake/rpc"
	"google.golang.org/grpc"
)

func main() {
	l, err := net.Listen("tcp", ":50051")
	if err != nil {
		log.Fatal(err)
	}
	// new a grpc server
	rpcServer := grpc.NewServer()

	// create a worker service and register into server
	workerService, err := rpc.NewWorkerService(0)
	if err != nil {
		log.Fatal(err)
	}
	rpcService := rpc.WorkerServer(workerService)
	rpc.RegisterWorkerServer(rpcServer, rpcService)

	// server serve request
	println("server initialize done! and listen on: 50051")
	if err := rpcServer.Serve(l); err != nil {
		log.Fatal(err)
	}
}

```

#### output

```sh
# one terminal to:
go run server.go

# another terminal to:
go run client.go

# output:
1113437949933912064
1113437949933912065
1113437949933912066
1113437949933912067
1113437949933912068
1113437949933912069
1113437949933912070
1113437949933912071
1113437949933912072
1113437949933912073
```

### References

* [twitter-snowflake](https://github.com/twitter-archive/snowflake)
* thanks to [bwmarrin/snowflake](https://github.com/bwmarrin/snowflake)