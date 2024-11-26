package main

import (
    "fmt"
    "sort"
    "strings"
    "sync"
    "time"
)

// Representa um grafo não direcionado
type Graph struct {
    nodes map[int][]int
    mu    sync.RWMutex
}

// Cria um novo grafo
func NovoGrafo() *Graph {
    return &Graph{
        nodes: make(map[int][]int),
    }
}

// Adiciona uma aresta entre dois nós
func (g *Graph) AdicionarAresta(u, v int) {
    g.mu.Lock()
    defer g.mu.Unlock()

    if u == v {
        return
    }

    // Evita arestas duplicadas
    if !contemValor(g.nodes[u], v) {
        g.nodes[u] = append(g.nodes[u], v)
        g.nodes[v] = append(g.nodes[v], u)
    }
}

// Remove uma aresta entre dois nós
func (g *Graph) RemoverAresta(u, v int) {
    g.mu.Lock()
    defer g.mu.Unlock()

    g.nodes[u] = removerValor(g.nodes[u], v)
    g.nodes[v] = removerValor(g.nodes[v], u)
}

// Remove um valor específico de um slice
func removerValor(slice []int, value int) []int {
    result := []int{}
    for _, v := range slice {
        if v != value {
            result = append(result, v)
        }
    }
    return result
}

// Verifica se um valor está no slice
func contemValor(slice []int, value int) bool {
    for _, v := range slice {
        if v == value {
            return true
        }
    }
    return false
}

// Cria uma aresta ordenada para garantir consistência
func criarAresta(u, v int) [2]int {
    if u < v {
        return [2]int{u, v}
    }
    return [2]int{v, u}
}

// Calcula a intermediariedade de arestas
func calcularIntermediariedadeArestaConcorrente(g *Graph) map[[2]int]float64 {
    intermediariedadeAresta := make(map[[2]int]float64)
    var mu sync.Mutex
    var wg sync.WaitGroup

    // Obtem lista de nós
    nos := func() []int {
        g.mu.RLock()
        defer g.mu.RUnlock()
        nodeList := make([]int, 0, len(g.nodes))
        for node := range g.nodes {
            nodeList = append(nodeList, node)
        }
        return nodeList
    }()

    for _, no := range nos {
        wg.Add(1)
        go func(no int) {
            defer wg.Done()
            
            // Calcula intermediariedade para um nó específico
            intermediariedadeLocal := make(map[[2]int]float64)
            fonteUnica(g, no, intermediariedadeLocal)
            
            // Atualiza mapa global com lock
            mu.Lock()
            for aresta, valor := range intermediariedadeLocal {
                intermediariedadeAresta[aresta] += valor
            }
            mu.Unlock()
        }(no)
    }

    wg.Wait()
    return intermediariedadeAresta
}

// Calcula a intermediariedade para um único nó fonte
func fonteUnica(g *Graph, fonte int, intermediariedadeAresta map[[2]int]float64) {
    g.mu.RLock()
    defer g.mu.RUnlock()

    caminhos := make(map[int]int)
    distancias := make(map[int]int)
    predecessores := make(map[int][]int)
    pilha := []int{}
    fila := []int{fonte}

    // Inicializa distâncias
    for no := range g.nodes {
        distancias[no] = -1
    }
    distancias[fonte] = 0
    caminhos[fonte] = 1

    // Busca em largura para encontrar caminhos
    for len(fila) > 0 {
        atual := fila[0]
        fila = fila[1:]
        pilha = append(pilha, atual)

        for _, vizinho := range g.nodes[atual] {
            if distancias[vizinho] == -1 {
                fila = append(fila, vizinho)
                distancias[vizinho] = distancias[atual] + 1
            }
            if distancias[vizinho] == distancias[atual]+1 {
                caminhos[vizinho] += caminhos[atual]
                predecessores[vizinho] = append(predecessores[vizinho], atual)
            }
        }
    }

    // Calcula dependências
    dependencias := make(map[int]float64)
    for len(pilha) > 0 {
        no := pilha[len(pilha)-1]
        pilha = pilha[:len(pilha)-1]

        for _, predecessor := range predecessores[no] {
            aresta := criarAresta(predecessor, no)
            peso := float64(caminhos[predecessor]) / float64(caminhos[no])
            dependencias[predecessor] += peso * (1 + dependencias[no])
            intermediariedadeAresta[aresta] += dependencias[no]
        }

        if no != fonte {
            dependencias[no] += 1.0
        }
    }
}

// Encontra a aresta com maior intermediariedade
func encontrarArestaMáxima(intermediariedadeAresta map[[2]int]float64) [2]int {
    var arestaMáxima [2]int
    valorMáximo := -1.0

    for aresta, valor := range intermediariedadeAresta {
        if valor > valorMáximo {
            arestaMáxima = aresta
            valorMáximo = valor
        }
    }

    return arestaMáxima
}

// Detecta comunidades usando busca em profundidade
func detectarComunidades(g *Graph) [][]int {
    visitados := make(map[int]bool)
    var comunidades [][]int

    for no := range g.nodes {
        if !visitados[no] {
            comunidade := []int{}
            buscaProfundidade(g, no, &comunidade, visitados)
            if len(comunidade) > 0 {
                comunidades = append(comunidades, comunidade)
            }
        }
    }
    return comunidades
}

// Realiza a busca em profundidade
func buscaProfundidade(g *Graph, no int, comunidade *[]int, visitados map[int]bool) {
    visitados[no] = true
    *comunidade = append(*comunidade, no)
    
    g.mu.RLock()
    defer g.mu.RUnlock()
    
    for _, vizinho := range g.nodes[no] {
        if !visitados[vizinho] {
            buscaProfundidade(g, vizinho, comunidade, visitados)
        }
    }
}

// Ordena as comunidades
func ordenarComunidades(comunidades [][]int) [][]int {
    for _, comunidade := range comunidades {
        sort.Ints(comunidade)
    }
    sort.Slice(comunidades, func(i, j int) bool {
        return comunidades[i][0] < comunidades[j][0]
    })
    return comunidades
}

// Implementa o algoritmo Girvan-Newman para detecção de comunidades
func girvanNewman(g *Graph) [][]int {
    var comunidades [][]int

    for {
        // Calcula intermediariedade de arestas
        intermediariedadeAresta := calcularIntermediariedadeArestaConcorrente(g)

        if len(intermediariedadeAresta) == 0 {
            break
        }

        // Encontra e remover a aresta com maior intermediariedade
        arestaMáxima := encontrarArestaMáxima(intermediariedadeAresta)
        g.RemoverAresta(arestaMáxima[0], arestaMáxima[1])

        // Detecta comunidades atuais
        comunidades = detectarComunidades(g)

        // Para se múltiplas comunidades
        if len(comunidades) > 1 {
            break
        }
    }

    return ordenarComunidades(comunidades)
}

// Mede o tempo de execução de uma função
func medirTempoExecução(nome string, f func()) time.Duration {
    inicio := time.Now()
    f()
    decorrido := time.Since(inicio)
    return decorrido
}

func main() {
    // Defini grafos de teste
    grafos := []struct {
        nome  string
        grafo *Graph
    }{
        {"Cíclico com Conexão", func() *Graph {
            g := NovoGrafo()
            g.AdicionarAresta(1, 2)
            g.AdicionarAresta(2, 3)
            g.AdicionarAresta(3, 4)
            g.AdicionarAresta(4, 1)
            g.AdicionarAresta(3, 5)
            return g
        }()},
        
        {"Grafo Desconexo", func() *Graph {
            g := NovoGrafo()
            g.AdicionarAresta(1, 2)
            g.AdicionarAresta(3, 4)
            return g
        }()},

        {"Grafo Linear", func() *Graph {
            g := NovoGrafo()
            g.AdicionarAresta(1, 2)
            g.AdicionarAresta(2, 3)
            g.AdicionarAresta(3, 4)
            return g
        }()},

        {"Grafo Completo", func() *Graph {
            g := NovoGrafo()
            g.AdicionarAresta(1, 2)
            g.AdicionarAresta(1, 3)
            g.AdicionarAresta(1, 4)
            g.AdicionarAresta(2, 3)
            g.AdicionarAresta(2, 4)
            g.AdicionarAresta(3, 4)
            return g
        }()},
    }

    fmt.Println("\nComparação de Desempenho entre Métodos Sequencial e Concorrente para Detecção de Comunidades em Grafos utilizando o Algoritmo Girvan-Newman")

    // Executa testes para cada grafo
    for _, teste := range grafos {
        fmt.Printf("\n\n")
        fmt.Println(strings.Repeat("-", 40))        
        fmt.Printf("%s\n", teste.nome)
        fmt.Println(strings.Repeat("-", 40))

        grafoSequencial := teste.grafo
        grafoConcorrente := teste.grafo

        fmt.Println("Método Sequencial:")
        tempoSequencial := medirTempoExecução("Sequencial", func() {
            comunidades := girvanNewman(grafoSequencial)
            imprimirComunidades(comunidades)
        })
        fmt.Printf("    Tempo: %s\n", tempoSequencial)

        fmt.Println("\nMétodo Concorrente:")
        tempoConcorrente := medirTempoExecução("Concorrente", func() {
            comunidades := girvanNewman(grafoConcorrente)
            imprimirComunidades(comunidades)
        })
        fmt.Printf("    Tempo: %s\n", tempoConcorrente)
    }

    // Teste de desempenho com grafo 
    fmt.Printf("\n\n")
    fmt.Println(strings.Repeat("-", 40))
    fmt.Println("Grafo Grande (1000 nós)")
    fmt.Println(strings.Repeat("-", 40))

    grafoGrande := func() *Graph {
        g := NovoGrafo()
        for i := 1; i <= 1000; i++ {
            g.AdicionarAresta(i, i+1)
            if i%5 == 0 && i < 1000 {
                g.AdicionarAresta(i, i+5)
            }
        }
        return g
    }()

    fmt.Println("Método Sequencial:")
    tempoSequencial := medirTempoExecução("Sequencial", func() {
        comunidades := girvanNewman(grafoGrande)
        imprimirComunidades(comunidades)
    })
    fmt.Printf("   Tempo: %s\n", tempoSequencial)

    // Reseta grafo grande
    grafoGrande = func() *Graph {
        g := NovoGrafo()
        for i := 1; i <= 1000; i++ {
            g.AdicionarAresta(i, i+1)
            if i%5 == 0 && i < 1000 {
                g.AdicionarAresta(i, i+5)
            }
        }
        return g
    }()

    fmt.Println("\nMétodo Concorrente:")
    tempoConcorrente := medirTempoExecução("Concorrente", func() {
        comunidades := girvanNewman(grafoGrande)
        imprimirComunidades(comunidades)
    })
    fmt.Printf("    Tempo: %s\n\n", tempoConcorrente)
}

// Nova função para imprimir comunidades de forma mais legível
func imprimirComunidades(comunidades [][]int) {
    fmt.Printf("    Total de Comunidades: %d\n", len(comunidades))
    for i, comunidade := range comunidades {
        fmt.Printf("        Comunidade %d: %v (Tamanho: %d)\n", 
                   i+1, comunidade, len(comunidade))
    }
}
