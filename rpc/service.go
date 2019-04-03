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

	id := s.worker.Generate()
	return &Reply{ID: id.Int64()}, nil
}
