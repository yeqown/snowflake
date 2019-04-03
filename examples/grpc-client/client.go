package main

import (
	"context"
	"fmt"
	"log"

	"github.com/yeqown/snowflake/rpc"
	"google.golang.org/grpc"
)

func main() {
	conn, err := grpc.Dial("localhost:50051", grpc.WithInsecure())
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	client := rpc.NewWorkerClient(conn)
	reply, err := client.Generate(context.Background(), &rpc.Request{Count: 1})
	if err != nil {
		log.Fatal(err)
	}
	fmt.Printf("relpy ids: %v", reply.ID)
}
