package main

import (
	"fmt"
	"io"
	"log"
	"os"

	"github.com/yeqown/snowflake"

	"github.com/urfave/cli"
)

func main() {
	app := initApp(genActionFunc)

	if err := app.Run(os.Args); err != nil {
		log.Fatal(err.Error())
	}
}

func initApp(genAction cli.ActionFunc) *cli.App {
	app := cli.NewApp()
	app.Version = "1.0.0"
	app.Author = "yeqown@gmail.com"
	app.Flags = []cli.Flag{}
	app.Commands = []cli.Command{
		cli.Command{
			Name: "gen",
			Flags: []cli.Flag{
				cli.Int64Flag{
					Name:  "machineID, m",
					Value: 0,
					Usage: "specify the machine id",
				},
				cli.Int64Flag{
					Name:  "count, c",
					Value: 1,
					Usage: "count of ids to generate",
				},
				cli.StringFlag{
					Name:  "output, o",
					Value: "stdout",
					Usage: "output stream, default output to stdout",
				},
			},
			Action: genAction,
		},
	}

	return app
}

// genActionFunc ...
func genActionFunc(c *cli.Context) error {
	var (
		output io.Writer = os.Stdout
		count            = 1
		// machineID           = 0
	)

	// generate id worker
	worker, err := snowflake.New(c.Int64("machineID"))
	if err != nil {
		return err
	}

	// use output
	if out := c.String("output"); out != "stdout" {
		if output, err = os.OpenFile(out, os.O_WRONLY|os.O_CREATE, 0644); err != nil {
			return err
		}
	}

	// get count of wanted ids
	count = c.Int("count")

	// generate and send to output
	for i := 0; i < count; i++ {
		id := worker.Generate()
		fmt.Fprintf(output, "%d\n", id)
		// output.Write(id.Bytes())
	}

	return nil
}
