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
