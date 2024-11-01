package main

import (
	"fmt"
)

func tarefa(fromMain chan string, toMain chan string) {
	// Recebe a primeira mensagem da main e imprime
	msg := <-fromMain
	fmt.Println(msg)

	// Responde para a main
	toMain <- "Oi Main, bom dia, tudo bem?"

	// Recebe a segunda mensagem da main e imprime
	msg = <-fromMain
	fmt.Println(msg)

	// Responde para a main
	toMain <- "Certo, entendido."

	// Envia uma última mensagem para informar que a goroutine finalizou
	toMain <- "finalizando"
}

func main() {
	// Cria os canais para comunicação bidirecional
	fromMain := make(chan string) // Canal de Main para Goroutine
	toMain := make(chan string)   // Canal de Goroutine para Main

	// Inicia a goroutine com os canais
	go tarefa(fromMain, toMain)

	// Envia a primeira mensagem para a goroutine
	fromMain <- "Olá, Goroutine, bom dia!"

	// Recebe a resposta da goroutine e imprime
	msg := <-toMain
	fmt.Println(msg)

	// Envia a segunda mensagem para a goroutine
	fromMain <- "Tudo bem! Vou terminar tá?"

	// Recebe a segunda resposta da goroutine e imprime
	msg = <-toMain
	fmt.Println(msg)

	// Aguarda a última mensagem da goroutine indicando que finalizou
	msg = <-toMain
	fmt.Println(msg)

	// Imprime a mensagem de finalização da main
	fmt.Println("finalizando")
}
