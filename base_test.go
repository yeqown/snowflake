package snowflake_test

import (
	"sync"
	"testing"

	thirdsnowflake "github.com/bwmarrin/snowflake"
	"github.com/yeqown/snowflake"
)

func TestConfig(t *testing.T) {
	snowflake.Config()
}

func Test_Worker(t *testing.T) {
	worker, err := snowflake.New(1)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}

	id := worker.Generate()
	t.Logf("int64 %d", id)
	t.Logf("string: %s", id)

	t.Logf("time: %d, machineID: %d, counter: %d", id.Time(), id.MachineID(), id.Counter())
	if mid := id.MachineID(); mid != 1 {
		t.Errorf("wrong machineID, want: 1, got: %d", mid)
	}

}

func Test_concurrent(t *testing.T) {
	worker, err := snowflake.New(1)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}

	idc := make(chan snowflake.ID, 100)
	wg := sync.WaitGroup{}

	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func(w *snowflake.Worker, idc chan<- snowflake.ID) {
			defer wg.Done()
			for j := 0; j < 10000; j++ {
				idc <- worker.Generate()
			}
		}(worker, idc)
	}

	// close channle
	go func() {
		wg.Wait()
		close(idc)
	}()

	m := make(map[snowflake.ID]bool)
	dupCnt := 0
	for id := range idc {
		if _, ok := m[id]; ok {
			t.Errorf("duplicated id: %s, time: %d, machineID: %d, counter: %d",
				id, id.Time(), id.MachineID(), id.Counter())
			dupCnt++
			continue
		}
		m[id] = true
	}

	t.Logf("dupCnt: %d", dupCnt)
}

func Test_bwmarrinSnowflake_concurrent(t *testing.T) {
	worker, err := thirdsnowflake.NewNode(1)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}

	idc := make(chan thirdsnowflake.ID, 100)
	wg := sync.WaitGroup{}

	for i := 0; i < 10; i++ {
		wg.Add(1)
		go func(w *thirdsnowflake.Node, idc chan<- thirdsnowflake.ID) {
			defer wg.Done()
			for j := 0; j < 10000; j++ {
				idc <- worker.Generate()
			}
		}(worker, idc)
	}

	// close channle
	go func() {
		wg.Wait()
		close(idc)
	}()

	m := make(map[thirdsnowflake.ID]bool)
	dupCnt := 0
	for id := range idc {
		if _, ok := m[id]; ok {
			t.Errorf("duplicated id: %s, time: %d, machineID: %d, counter: %d",
				id, id.Time(), id.Node(), id.Step())
			dupCnt++
			continue
		}
		m[id] = true
	}

	t.Logf("dupCnt: %d", dupCnt)
}

func Test_duplicated(t *testing.T) {
	worker, err := snowflake.New(1)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}
	c := make(map[snowflake.ID]bool)
	dupCount := 0
	for i := 0; i < 10000000; i++ {
		id := worker.Generate()
		if _, ok := c[id]; ok {
			t.Errorf("duplicated id: %s, time: %d, counter: %d", id, id.Time(), id.Counter())
			dupCount++
			continue
		}
		c[id] = true
	}

	t.Logf("total: %d, dulicated: %d", 10000000, dupCount)
}

func Test_bwmarrinSnowflake_duplicated(t *testing.T) {
	node, err := thirdsnowflake.NewNode(1)
	if err != nil {
		t.Error(err)
		t.FailNow()
	}
	c := make(map[thirdsnowflake.ID]bool)
	dupCount := 0
	for i := 0; i < 10000000; i++ {
		id := node.Generate()
		if _, ok := c[id]; ok {
			t.Errorf("duplicated id: %s, time: %d, counter: %d", id, id.Time(), id.Step())
			dupCount++
			continue
		}
		c[id] = true
	}

	t.Logf("total: %d, dulicated: %d", 10000000, dupCount)
}

func BenchmarkGenerate(b *testing.B) {
	b.StopTimer()
	worker, err := snowflake.New(1)
	if err != nil {
		b.Error(err)
		b.FailNow()
	}
	b.ResetTimer()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		worker.Generate()
	}
}

func Benchmark_bwmarrinSnowflake(b *testing.B) {
	b.StopTimer()
	node, err := thirdsnowflake.NewNode(1)
	if err != nil {
		b.Error(err)
		b.FailNow()
	}
	b.ResetTimer()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		node.Generate()
	}
}

/* Benchmark
Current Repo:
```sh
goos: darwin
goarch: amd64
pkg: github.com/yeqown/snowflake
BenchmarkGenerate-8   	 5000000	       243 ns/op	       0 B/op	       0 allocs/op
```

`bwmarrin/snowflake`
```sh
goos: darwin
goarch: amd64
pkg: github.com/yeqown/snowflake
Benchmark_bwmarrinSnowflake-8   	 5000000	       243 ns/op	       0 B/op	       0 allocs/op
```
*/
