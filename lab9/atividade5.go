package main

import (
	"fmt"
	"math"
	"sync"
)

// Função para verificar se um número é primo
func ehPrimo(numero int64) bool {
	if numero <= 1 {
		return false
	}
	if numero == 2 {
		return true
	}
	if numero%2 == 0 {
		return false
	}
	for i := int64(3); i <= int64(math.Sqrt(float64(numero)))+1; i += 2 {
		if numero%i == 0 {
			return false
		}
	}
	return true
}

// Função worker para processar números e enviar os primos encontrados
func worker(tarefas <-chan int64, contador chan<- int64, grupo *sync.WaitGroup) {
	defer grupo.Done() // Decrementa o contador ao final da goroutine
	for numero := range tarefas {
		if ehPrimo(numero) {
			contador <- numero // Envia o número primo encontrado para o canal de contador
		}
	}
}

func main() {
	var limite int64       // Limite até onde queremos encontrar primos
	M := 4        // Número de goroutines (workers)

	// Solicita o valor do limite ao usuário
	fmt.Print("Digite o valor do limite: ")
	fmt.Scan(&limite)

	tarefas := make(chan int64, limite)       // Canal para enviar números
	contador := make(chan int64, limite)      // Canal para receber os números primos encontrados
	var grupo sync.WaitGroup                  // Sincronização das goroutines

	// Lança M goroutines para verificar a primalidade dos números
	for i := 0; i < M; i++ {
		grupo.Add(1)
		go worker(tarefas, contador, &grupo)
	}

	// Envia os números de 1 até o limite para o canal tarefas
	for i := int64(1); i <= limite; i++ {
		tarefas <- i
	}
	close(tarefas) // Fechar o canal tarefas indica que não há mais números a processar

	// Aguarda todas as goroutines terminarem e fecha o canal contador
	go func() {
		grupo.Wait()
		close(contador) // Fechar o canal contador ao terminar todas as goroutines
	}()

	// Exibe os números primos coletados das goroutines
	fmt.Print("Primos: ")

	totalPrimos := 0 // Contador de números primos

	for primo := range contador {
		fmt.Print(primo, " ")
		totalPrimos++ // Incrementa o contador para cada primo encontrado
	}

	// Nova linha e exibe o total de números primos
	fmt.Printf("\nTotal de números primos: %d\n", totalPrimos)
}
