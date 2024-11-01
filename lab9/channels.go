//introduz canais na aplicação Go
package main

import (
	"fmt"
)

func tarefa(str chan string) {
	//escreve no canal
	str <- "Bom dia"

	fmt.Printf("Terminando...\n")
}

func main() {
	var msg = "" //string vazia
	
	//cria um canal de comunicacao nao-bufferizado
	str := make(chan string)
	
	//cria um goroutine que executará a função 'tarefa'
	go tarefa(str)

	//lê do canal
	msg = <-str

	fmt.Println("Mensagem recebida: ", msg)
}
