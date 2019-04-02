package snowflake

import (
	"strconv"
)

// ID ... new type for easily use
type ID int64

func (d ID) String() string {
	return strconv.FormatInt(int64(d), 10)
}

// Int64 ...
func (d ID) Int64() int64 {
	return int64(d)
}

// Bytes returns a byte slice of the snowflake ID
func (d ID) Bytes() []byte {
	return []byte(d.String())
}

// Time returns an int64 unix timestamp of the snowflake ID time
func (d ID) Time() int64 {
	return (int64(d) >> timeShift) + Epoch
}

// MachineID returns an int64 of the snowflake ID node number
func (d ID) MachineID() int64 {
	return (int64(d) & machineMask) >> machineShift
}

// Counter returns an int64 of the snowflake step (or sequence) number
func (d ID) Counter() int64 {
	return int64(d) & counterMask
}
