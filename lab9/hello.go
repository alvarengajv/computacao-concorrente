//Hello World com goroutines em Go
package main

import (
	"fmt"
	"time"
)

func olaMundo(i int) {
	fmt.Printf("Olá da goroutine %d!\n", i)
}

func main() {
	for i := 0; i < 10; i++ {
	   go olaMundo(i) //dispara a goroutine
	}
	time.Sleep(time.Second) //bloqueio na main
	//quando a main termina, o programa inteiro termina
}
