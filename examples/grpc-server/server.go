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
