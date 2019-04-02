package snowflake_test

import (
	"testing"

	"github.com/yeqown/snowflake"
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

func Test_GenerateIDandParse(t *testing.T) {
	var ts, m, cnt int64 = 265349750720, 1, 12

	id := snowflake.ID(ts<<timeShift | m<<machineShift | cnt)
	// 											 0b1100110 011001100110
	// 0b11110111001000000100111101111111000000 0000000001 000000001100
	t.Logf("id: %s, %d", id, id)
	t.Logf("time: %d, machine: %d, counter: %d", id.Time(), id.MachineID(), id.Counter())
}
