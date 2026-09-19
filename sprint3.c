/* =====================================================================
   SISTEMA DE GERENCIAMENTO DE ESTACAO DE RECARGA - SPRINT 3
   Evolucao do simulador de recarga (Sprint 2) para um sistema capaz de
   armazenar, listar, buscar, ordenar e analisar multiplas sessoes de
   recarga de veiculos eletricos.

   Integrantes:
   Anna Luisa Carvalhaes  RM:573330
   Kethelyn Rocha         RM:574016
   Gabriela Batista       RM:573583
   Samara Carvalho        RM:573666
   ===================================================================== */

#include <stdio.h>

#define MAX_SESSOES 100
#define TARIFA_BASE 0.85f   /* R$ / kWh */

/*  
   1) ESTRUTURA DA SESSAO (struct + typedef, conforme exigido)
    */
typedef struct {
    int   id;      /* identificador unico da sessao      */
    int   vaga;    /* numero da vaga/carregador utilizado */
    float energia; /* energia fornecida na sessao (kWh)  */
    float tempo;   /* duracao da sessao (horas)          */
    float tarifa;  /* tarifa aplicada na sessao (R$/kWh) */
    float custo;   /* valor total cobrado (R$)           */
} Sessao;

/*
   Prototipos das funcoes (organizacao do programa em funcoes)
   - */
void exibirMenu(void);
void cadastrarSessao(Sessao sessoes[], int *totalSessoes, int *proximoId);
void listarSessoes(const Sessao sessoes[], int totalSessoes);
void buscarSessao(const Sessao sessoes[], int totalSessoes);
int  buscaLinearPorId(const Sessao sessoes[], int totalSessoes, int idProcurado);
void ordenarSessoes(Sessao sessoes[], int totalSessoes);
void bubbleSort(Sessao sessoes[], int totalSessoes, int criterio, int decrescente);
void mostrarEstatisticas(const Sessao sessoes[], int totalSessoes);
void limparBufferEntrada(void);

/* 
   MAIN - controla o menu principal do sistema
  */
int main(void) {
    Sessao sessoes[MAX_SESSOES];
    int totalSessoes = 0;   /* quantidade de posicoes ocupadas no vetor */
    int proximoId = 1;      /* proximo id a ser atribuido a uma sessao  */
    int opcao;

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                cadastrarSessao(sessoes, &totalSessoes, &proximoId);
                break;
            case 2:
                listarSessoes(sessoes, totalSessoes);
                break;
            case 3:
                buscarSessao(sessoes, totalSessoes);
                break;
            case 4:
                ordenarSessoes(sessoes, totalSessoes);
                break;
            case 5:
                mostrarEstatisticas(sessoes, totalSessoes);
                break;
            case 6:
                printf("\nEncerrando o sistema. Ate a proxima!\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 6);

    return 0;
}

/*
   3) MENU PRINCIPAL
   */
void exibirMenu(void) {
    printf("\n=====================================\n");
    printf("      ESTACAO DE RECARGA\n");
    printf("=====================================\n");
    printf("1 - Nova sessao de recarga\n");
    printf("2 - Listar sessoes\n");
    printf("3 - Buscar sessao\n");
    printf("4 - Ordenar sessoes\n");
    printf("5 - Estatisticas\n");
    printf("6 - Encerrar\n");
    printf("Escolha: ");
}

/* Limpa o buffer do teclado apos uma leitura invalida ou apos scanf de
   numeros, para nao atrapalhar leituras seguintes. */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* descarta */ }
}

/* 
   2) REGISTRO DE MULTIPLAS SESSOES (cadastro em vetor de structs)
   */
void cadastrarSessao(Sessao sessoes[], int *totalSessoes, int *proximoId) {
    if (*totalSessoes >= MAX_SESSOES) {
        printf("\nLimite maximo de %d sessoes atingido!\n", MAX_SESSOES);
        return;
    }

    Sessao nova;
    nova.id = (*proximoId)++;

    printf("\n--- Nova sessao de recarga (ID %d) ---\n", nova.id);

    printf("Vaga utilizada (1 a 10): ");
    scanf("%d", &nova.vaga);

    printf("Energia fornecida (kWh): ");
    scanf("%f", &nova.energia);

    printf("Tempo de recarga (horas): ");
    scanf("%f", &nova.tempo);

    /* Tarifa dinamica simples: sessoes mais longas (>1h) pagam tarifa
       um pouco maior, simulando maior demanda da rede - mantem a ideia
       de precificacao dinamica trazida da Sprint 2. */
    nova.tarifa = (nova.tempo > 1.0f) ? TARIFA_BASE * 1.15f : TARIFA_BASE;
    nova.custo  = nova.energia * nova.tarifa;

    sessoes[*totalSessoes] = nova;
    (*totalSessoes)++;

    printf("\nSessao cadastrada com sucesso!\n");
    printf("Tarifa aplicada: R$ %.2f/kWh | Custo total: R$ %.2f\n",
           nova.tarifa, nova.custo);
}

/* 
   LISTAGEM DE SESSOES
   */
void listarSessoes(const Sessao sessoes[], int totalSessoes) {
    if (totalSessoes == 0) {
        printf("\nNenhuma sessao cadastrada ainda.\n");
        return;
    }

    printf("\n=================== SESSOES CADASTRADAS ===================\n");
    printf("%-4s %-6s %-12s %-10s %-12s %-10s\n",
           "ID", "Vaga", "Energia(kWh)", "Tempo(h)", "Tarifa(R$)", "Custo(R$)");

    for (int i = 0; i < totalSessoes; i++) {
        printf("%-4d %-6d %-12.2f %-10.2f %-12.2f %-10.2f\n",
               sessoes[i].id, sessoes[i].vaga, sessoes[i].energia,
               sessoes[i].tempo, sessoes[i].tarifa, sessoes[i].custo);
    }
    printf("=============================================================\n");
}

/* =======================================================================
   4) BUSCA DE SESSOES
   Algoritmo escolhido: BUSCA LINEAR por ID.
   Justificativa: o vetor nao esta necessariamente ordenado (as sessoes
   sao inseridas na ordem em que o cliente chega), entao a busca linear
   e a forma direta e correta de localizar qualquer ID sem exigir uma
   ordenacao previa.

   Complexidade: O(n) -> no pior caso (ID inexistente, ou na ultima
   posicao), o laco percorre as n sessoes do vetor uma unica vez, sem
   lacos aninhados nem recursao.
   ======================================================================= */
int buscaLinearPorId(const Sessao sessoes[], int totalSessoes, int idProcurado) {
    for (int i = 0; i < totalSessoes; i++) {
        if (sessoes[i].id == idProcurado) {
            return i; /* retorna a posicao encontrada */
        }
    }
    return -1; /* nao encontrado */
}

void buscarSessao(const Sessao sessoes[], int totalSessoes) {
    if (totalSessoes == 0) {
        printf("\nNenhuma sessao cadastrada ainda.\n");
        return;
    }

    int idBuscado;
    printf("\nDigite o ID da sessao: ");
    scanf("%d", &idBuscado);

    int pos = buscaLinearPorId(sessoes, totalSessoes, idBuscado);

    if (pos == -1) {
        printf("\nSessao com ID %d nao encontrada.\n", idBuscado);
        return;
    }

    printf("\n--- Sessao encontrada ---\n");
    printf("ID.......: %d\n", sessoes[pos].id);
    printf("Vaga.....: %d\n", sessoes[pos].vaga);
    printf("Energia..: %.2f kWh\n", sessoes[pos].energia);
    printf("Tempo....: %.2f h\n", sessoes[pos].tempo);
    printf("Tarifa...: R$ %.2f/kWh\n", sessoes[pos].tarifa);
    printf("Custo....: R$ %.2f\n", sessoes[pos].custo);
}

/* =======================================================================
   5) ORDENACAO DE SESSOES
   Algoritmo escolhido: BUBBLE SORT, implementado manualmente (sem usar
   qsort() ou qualquer funcao pronta da biblioteca padrao).
   O usuario escolhe o criterio: ID, energia, custo ou tempo.
   ======================================================================= */
void ordenarSessoes(Sessao sessoes[], int totalSessoes) {
    if (totalSessoes < 2) {
        printf("\nE necessario ter pelo menos 2 sessoes para ordenar.\n");
        return;
    }

    int criterio, ordem;

    printf("\nOrdenar por:\n");
    printf("1 - ID\n");
    printf("2 - Energia consumida\n");
    printf("3 - Custo da sessao\n");
    printf("4 - Tempo de recarga\n");
    printf("Escolha o criterio: ");
    scanf("%d", &criterio);

    if (criterio < 1 || criterio > 4) {
        printf("\nCriterio invalido.\n");
        return;
    }

    printf("Ordem: 1-Crescente  2-Decrescente: ");
    scanf("%d", &ordem);

    int decrescente = (ordem == 2);

    bubbleSort(sessoes, totalSessoes, criterio, decrescente);

    printf("\nSessoes ordenadas com sucesso!\n");
    listarSessoes(sessoes, totalSessoes);
}

/* Retorna o valor da sessao correspondente ao criterio escolhido, para
   simplificar a comparacao dentro do Bubble Sort. */
static float valorPorCriterio(const Sessao *s, int criterio) {
    switch (criterio) {
        case 1: return (float) s->id;
        case 2: return s->energia;
        case 3: return s->custo;
        case 4: return s->tempo;
        default: return (float) s->id;
    }
}

/* Implementacao MANUAL do Bubble Sort.
   Para cada passagem, os elementos adjacentes sao comparados e trocados
   de posicao caso estejam fora de ordem. A cada passagem completa, o
   maior (ou menor, se decrescente) elemento "flutua" ate sua posicao
   final correta - por isso o nome "bolha".

   Complexidade: O(n^2) -> dois lacos aninhados. O laco externo executa
   n-1 vezes e, para cada passagem, o laco interno executa ate n-1-i
   comparacoes. O total de comparacoes e proporcional a n*(n-1)/2, ou
   seja, cresce de forma quadratica com o numero de sessoes. */
void bubbleSort(Sessao sessoes[], int totalSessoes, int criterio, int decrescente) {
    for (int i = 0; i < totalSessoes - 1; i++) {
        for (int j = 0; j < totalSessoes - 1 - i; j++) {

            float atual  = valorPorCriterio(&sessoes[j], criterio);
            float proximo = valorPorCriterio(&sessoes[j + 1], criterio);

            int trocar = decrescente ? (atual < proximo) : (atual > proximo);

            if (trocar) {
                Sessao temp = sessoes[j];
                sessoes[j] = sessoes[j + 1];
                sessoes[j + 1] = temp;
            }
        }
    }
}

/* =======================================================================
   6) ESTATISTICAS DA ESTACAO
   ======================================================================= */
void mostrarEstatisticas(const Sessao sessoes[], int totalSessoes) {
    if (totalSessoes == 0) {
        printf("\nNenhuma sessao cadastrada ainda.\n");
        return;
    }

    float energiaTotal = 0.0f;
    float faturamentoTotal = 0.0f;
    float maiorConsumo = sessoes[0].energia;
    float menorConsumo = sessoes[0].energia;

    for (int i = 0; i < totalSessoes; i++) {
        energiaTotal += sessoes[i].energia;
        faturamentoTotal += sessoes[i].custo;

        if (sessoes[i].energia > maiorConsumo) maiorConsumo = sessoes[i].energia;
        if (sessoes[i].energia < menorConsumo) menorConsumo = sessoes[i].energia;
    }

    float ticketMedio = faturamentoTotal / totalSessoes;

    printf("\n========= ESTATISTICAS ==========\n\n");
    printf("Sessoes realizadas: %d\n", totalSessoes);
    printf("Energia fornecida: %.2f kWh\n", energiaTotal);
    printf("Faturamento: R$ %.2f\n", faturamentoTotal);
    printf("Ticket medio: R$ %.2f\n\n", ticketMedio);
    printf("Maior consumo: %.2f kWh\n", maiorConsumo);
    printf("Menor consumo: %.2f kWh\n", menorConsumo);
    printf("==================================\n");
}
