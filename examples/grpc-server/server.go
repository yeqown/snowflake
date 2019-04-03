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

	rpcServer := grpc.NewServer()

	workerService, err := rpc.NewWorkerService(0)
	if err != nil {
		log.Fatal(err)
	}
	rpcService := rpc.WorkerServer(workerService)
	rpc.RegisterWorkerServer(rpcServer, rpcService)

	println("done init server")
	if err := rpcServer.Serve(l); err != nil {
		log.Fatal(err)
	}
}
