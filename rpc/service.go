package rpc

import (
	context "context"
	"errors"

	"github.com/yeqown/snowflake"
)

var (
	errEmptyWorker = errors.New("empty worker")
)

// NewWorkerService ...
func NewWorkerService(machineID int64) (*WorkerService, error) {
	worker, err := snowflake.New(machineID)
	if err != nil {
		return nil, err
	}
	services := &WorkerService{worker: worker}

	return services, nil
}

// WorkerService ...
type WorkerService struct {
	worker *snowflake.Worker
}

// Generate ...
func (s *WorkerService) Generate(ctx context.Context, req *Request) (*Reply, error) {
	if s.worker == nil {
		return nil, errEmptyWorker
	}

	reply := new(Reply)
	reply.ID = make([]int64, req.Count)

	for i := int32(0); i < req.Count; i++ {
		reply.ID[i] = s.worker.Generate().Int64()
	}
	return reply, nil
}
