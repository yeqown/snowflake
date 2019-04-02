package snowflake

import (
	"errors"
	"fmt"
	"sync"
	"time"
)

const (
	// data bits
	uselessBits uint8 = 1
	timeBits    uint8 = 41
	machineBits uint8 = 10
	counterBits uint8 = 12

	// left shift
	timeShift    uint8 = machineBits + counterBits
	machineShift uint8 = counterBits
	counterShift uint8 = 0

	workerMax   int64 = -1 ^ (-1 << machineBits)
	counterMax  int64 = -1 ^ (-1 << counterBits)
	machineMask int64 = workerMax << counterBits
	counterMask int64 = -1 ^ (-1 << counterBits)

	// Epoch is set to the twitter snowflake epoch of Nov 04 2010 01:42:54 UTC
	// You may customize this to set a different epoch for your application.
	Epoch int64 = 1288834974657
)

// Config for test and check config is ok or not
func Config() {
	fmt.Printf(
		`
		uselessBits uint8 = %d
		timeBits    uint8 = %d
		machineBits uint8 = %d
		counterBits uint8 = %d
		timeShift    uint8 = %d
		machineShift uint8 = %d
		counterShift uint8 = %d
		workerMax   int64 = %d
		counterMax  int64 = %d
		machineMask int64 = %d
		counterMask int64 = %d
		`,
		uselessBits,
		timeBits,
		machineBits,
		counterBits,
		timeShift,
		machineShift,
		counterShift,
		workerMax,
		counterMax,
		machineMask,
		counterMask,
	)
}

var (
	errOverLimitMachineMax = errors.New("invalid machine id")
)

// Worker ... unique id generator
type Worker struct {
	time            int64       // worker time
	machineIdentity int64       // machineIdentity
	counterPerMill  int64       // generated id count per millsecond
	mu              *sync.Mutex // lock for counterPerMill and time
}

// New Worker with options
func New(machineID int64) (*Worker, error) {
	if machineID < 0 || machineID > workerMax {
		return nil, errOverLimitMachineMax
	}

	return &Worker{
		time:            0,
		machineIdentity: machineID,
		counterPerMill:  0,
		mu:              &sync.Mutex{},
	}, nil
}

// Generate ... generate an unique ID
func (w *Worker) Generate() ID {

	w.mu.Lock()
	now := time.Now().UnixNano() / 1000000
	if w.time == now {
		w.counterPerMill = (w.counterPerMill + 1) & counterMask
		// over counterMax pause the function
		if w.counterPerMill == 0 {
			for now <= w.time {
				now = time.Now().UnixNano() / 1000000
			}
		}
	} else {
		w.counterPerMill = 0
	}

	w.time = now
	machineID := w.machineIdentity
	d := now - Epoch
	counter := w.counterPerMill
	w.mu.Unlock()

	return ID(d<<timeShift | machineID<<machineShift | counter)
}
