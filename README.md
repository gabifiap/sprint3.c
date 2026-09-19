# Sistema de Gerenciamento de Estação de Recarga

Sprint 3 — Estruturas de Dados e Algoritmos | FIAP

Evolução do simulador de recarga de veículos elétricos da Sprint 2. O sistema deixou de trabalhar com uma única recarga em tempo real e passou a **registrar, listar, buscar, ordenar e analisar múltiplas sessões de recarga**, aplicando os conteúdos pedidos no enunciado: struct, typedef, vetores de estruturas, funções, busca, ordenação e análise de complexidade (Big-O).

## Integrantes

| Nome | RM |
|---|---|
| Anna Luisa Carvalhaes | 573330 |
| Kethelyn Rocha | 574016 |
| Gabriela Batista | 573583 |
| Samara Carvalho | 573666 |

## O que o programa faz

Simula o painel de controle de uma estação de recarga. A cada sessão registrada, o programa calcula tarifa e custo e guarda em memória durante a execução. O usuário pode listar, buscar por ID, reordenar por diferentes critérios e ver estatísticas de todas as sessões já realizadas.



## Itens do enunciado e como foram atendidos

### 1. Estrutura da sessão (`struct` + `typedef`)

```c
typedef struct {
    int   id;      // identificador único da sessão
    int   vaga;    // número da vaga/carregador utilizado
    float energia; // energia fornecida na sessão (kWh)
    float tempo;   // duração da sessão (horas)
    float tarifa;  // tarifa aplicada na sessão (R$/kWh)
    float custo;   // valor total cobrado (R$)
} Sessao;
```

Além dos campos mínimos pedidos (`id`, `energia`, `tempo`, `custo`), acrescentamos `vaga` (qual carregador foi usado) e `tarifa` (valor por kWh aplicado, que varia conforme o tempo de recarga).

### 2. Registro de múltiplas sessões (vetor de structs)

```c
Sessao sessoes[100];
```

Controlado pela variável `totalSessoes`, que indica quantas posições do vetor estão ocupadas. Cada cadastro é feito pela função `cadastrarSessao`.

### 3. Menu principal

Implementado em `exibirMenu` + `switch` dentro de um `do...while` no `main`, ficando em loop até a opção 6 ("Encerrar").

### 4. Busca de sessões — **Busca Linear**

Função `buscaLinearPorId`: percorre o vetor posição por posição comparando o `id` até encontrar (ou chegar ao fim).

Optamos pela busca linear em vez da binária porque as sessões são cadastradas na ordem de chegada dos clientes (não por ID), e o vetor pode ser reordenado por outros critérios — exigir ordenação prévia por ID só para viabilizar a busca binária não fazia sentido para o uso real do sistema.

### 5. Ordenação — **Bubble Sort implementado manualmente**

Função `bubbleSort`, sem uso de `qsort` ou qualquer função pronta da biblioteca. O usuário escolhe o critério (ID, energia, custo ou tempo) via `ordenarSessoes`, e também a ordem (crescente/decrescente).

### 6. Estatísticas da estação

Calculadas em `mostrarEstatisticas`, percorrendo o vetor uma vez:
- Sessões realizadas
- Energia total fornecida (kWh)
- Faturamento total (R$)
- Ticket médio
- Maior consumo registrado
- Menor consumo registrado

### 7. Funções

O programa é dividido em 9 funções, cada uma com uma responsabilidade: `exibirMenu`, `cadastrarSessao`, `listarSessoes`, `buscarSessao`, `buscaLinearPorId`, `ordenarSessoes`, `bubbleSort`, `valorPorCriterio`, `mostrarEstatisticas`.

### 8. Análise de algoritmos (Big-O)

**Busca Linear — O(n)**

```c
for (int i = 0; i < totalSessoes; i++) {
    if (sessoes[i].id == idProcurado) return i;
}
```

Um único laço, sem aninhamento. No pior caso (ID inexistente ou na última posição), percorre todas as `n` sessões uma única vez → o número de comparações cresce **linearmente** com `n`.

**Bubble Sort — O(n²)**

```c
for (int i = 0; i < totalSessoes - 1; i++) {
    for (int j = 0; j < totalSessoes - 1 - i; j++) {
        // comparação e possível troca
    }
}
```

Dois laços aninhados: o laço externo roda `n-1` vezes e, a cada passagem, o laço interno faz até `n-1-i` comparações. Somando todas as passagens, o total de comparações é proporcional a `n × (n-1) / 2` — termo dominado pelo `n²`, ou seja, complexidade **quadrática**. Se o número de sessões dobrar, o número de comparações tende a quadruplicar.

