#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evento.h"
#include <time.h>

// variável global que armazena as rotações realizadas
int totalRotacoes = 0;

// struct de data e hora
struct dataHora {
    int dia, mes, ano;
    int hora, minuto, segundo;
};

// função que retorna data e hora atuais
struct dataHora obterDataHoraAtual() {
    time_t agora;
    struct tm *info;
    struct dataHora dh;
    
    time(&agora);
    info = localtime(&agora);
    
    dh.dia = info->tm_mday;
    dh.mes = info->tm_mon + 1;
    dh.ano = info->tm_year + 1900;
    dh.hora = info->tm_hour;
    dh.minuto = info->tm_min;
    dh.segundo = info->tm_sec;
    
    return dh;
}

// formatação dos enum
const char* NOME_TIPO[] = {"PREENCHIMENTO", "ACIDENTE_TRANSITO", "FALHA_SEMAFORO", "INTERRUPCAO_ENERGIA", "ALAGAMENTO", "INCENDIO"};
const char* NOME_REGIAO[] = {"PREENCHIMENTO", "NORTE", "SUL", "LESTE", "OESTE", "CENTRO"};
const char* NOME_STATUS[] = {"PREENCHIMENTO", "ATIVO", "RESOLVIDO"};

// struct de evento
struct eventNode{
    struct eventNode *leftPtr;
    struct eventNode *rightPtr;
    int altura;
    int id;
    enum tipoEvento tipo;
    enum nivelSeveridade severidade;
    struct dataHora dHora;
    enum regiaoCidade regiao;
    enum statusEvento status;
};

/* gerar id de valor aleatório para garantir que não haja duplicados
ou retornar o valor inserido caso ainda não exista na árvore: */
int gerarCadastroUnico(EventNodePtr eventPtr, int id) {
    if (eventPtr == NULL) return id; // cadastro ainda não existe
    
    if (eventPtr->id == id) {
        return gerarCadastroUnico(eventPtr, rand() % 100);  // tenta outro
    }
    else if (id < eventPtr->id) {
        return gerarCadastroUnico(eventPtr->leftPtr, id);
    }
    else {
        return gerarCadastroUnico(eventPtr->rightPtr, id);
    }
}

// função que retorna novo nó de evento
EventNodePtr novoEvento(int id, enum tipoEvento tipo, enum nivelSeveridade severidade, enum regiaoCidade regiao, enum statusEvento status) {
    EventNodePtr no = (EventNodePtr)malloc(sizeof(EventNode));  
    if (no != NULL) {
        no->id = id;
        no->tipo = tipo;
        no->severidade = severidade;
        no->dHora = obterDataHoraAtual();
        no->regiao = regiao;
        no->status = status;
        no->altura = 0;
        no->leftPtr = NULL;
        no->rightPtr = NULL;
    } 
    return no;
}

// função auxiliar para retornar altura do nó
int altura_NO(EventNodePtr no) {
    if (no == NULL)
        return -1;
    return no->altura;
}

// Função auxiliar para retornar o maior valor entre dois inteiros
int maior(int a, int b) {
    return (a > b) ? a : b;
}

// Função para realizar rotação simples à esquerda (RR)
EventNodePtr rotacaoRR(EventNodePtr raiz) {
    totalRotacoes++;
    EventNodePtr no = raiz->rightPtr;
    raiz->rightPtr = no->leftPtr;
    no->leftPtr = raiz;
    
    raiz->altura = maior(altura_NO(raiz->leftPtr), altura_NO(raiz->rightPtr)) + 1;
    no->altura = maior(altura_NO(no->leftPtr), altura_NO(no->rightPtr)) + 1;
    
    return no;
}

// Função para realizar rotação simples à direita (LL)
EventNodePtr rotacaoLL(EventNodePtr raiz) {
    totalRotacoes++;
    EventNodePtr no = raiz->leftPtr;
    raiz->leftPtr = no->rightPtr;
    no->rightPtr = raiz;
    
    raiz->altura = maior(altura_NO(raiz->leftPtr), altura_NO(raiz->rightPtr)) + 1;
    no->altura = maior(altura_NO(no->leftPtr), altura_NO(no->rightPtr)) + 1;
    
    return no;
}

// Função para realizar rotação dupla à esquerda-direita (LR)
EventNodePtr rotacaoLR(EventNodePtr raiz) {
    raiz->leftPtr = rotacaoRR(raiz->leftPtr);
    return rotacaoLL(raiz);
}

// Função para realizar rotação dupla à direita-esquerda (RL)
EventNodePtr rotacaoRL(EventNodePtr raiz) {
    raiz->rightPtr = rotacaoLL(raiz->rightPtr);
    return rotacaoRR(raiz);
}

// Função para calcular o fator de balanceamento
int fatorBalanceamentoEvento(EventNodePtr no) {
    if (no == NULL)
        return 0; 
    return altura_NO(no->leftPtr) - altura_NO(no->rightPtr);
}

// inserir nó de evento na árvore:
EventNodePtr insertNode(EventNodePtr *eventPtr, int id, enum tipoEvento tipo, enum nivelSeveridade severidade, enum regiaoCidade regiao, enum statusEvento status){
    // caso não haja cadastro no nó atual:
    if (*eventPtr == NULL) {
        return novoEvento(id, tipo, severidade, regiao, status);
    }
    // caso o id passado tenha valor menor que o id do nó atual:
    if (id < (*eventPtr)->id) {
        // chamada recursiva - CAPTURA O RETORNO
        (*eventPtr)->leftPtr = insertNode(&((*eventPtr)->leftPtr), id, tipo, severidade, regiao, status);
    } 
    // caso o id passado tenha valor maior que o id do nó atual:
    else if (id > (*eventPtr)->id) {
        // chamada recursiva - CAPTURA O RETORNO
        (*eventPtr)->rightPtr = insertNode(&((*eventPtr)->rightPtr), id, tipo, severidade, regiao, status);
    }
    // opção de segurança, caso de fato seja passado um id já existente
    else {
        printf("duplicado\n");
        return *eventPtr;
    }
    
    // reatribuição de altura
    (*eventPtr)->altura = maior(altura_NO((*eventPtr)->leftPtr), altura_NO((*eventPtr)->rightPtr)) + 1;
    int fb = fatorBalanceamentoEvento(*eventPtr);

    // Casos de desbalanceamento
    if (fb > 1 && id < (*eventPtr)->leftPtr->id)
        return rotacaoLL(*eventPtr);

    if (fb < -1 && id > (*eventPtr)->rightPtr->id)
        return rotacaoRR(*eventPtr);

    if (fb > 1 && id > (*eventPtr)->leftPtr->id)
        return rotacaoLR(*eventPtr);

    if (fb < -1 && id < (*eventPtr)->rightPtr->id)
        return rotacaoRL(*eventPtr);

    return *eventPtr;
}

// exibir todos os eventos da árvore pelo método pre-Order:
void preOrder(EventNodePtr eventPtr){
    if (eventPtr != NULL) {
        printf("ID: %d, Altura: %d, Tipo de Evento: %s, Severidade: %d, Regiao: %s, Status: %s, Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n", 
               eventPtr->id,
               eventPtr->altura,
               NOME_TIPO[eventPtr->tipo],
               eventPtr->severidade,
               NOME_REGIAO[eventPtr->regiao],
               NOME_STATUS[eventPtr->status],
               eventPtr->dHora);
        preOrder(eventPtr->leftPtr);
        preOrder(eventPtr->rightPtr);
    }
}

// exibir todos os eventos ativos da árvore pelo método pre-Order:
void preOrderAtivos(EventNodePtr eventPtr){
    if (eventPtr != NULL) {
        if (eventPtr->status == ATIVO)
        printf("ID: %d, Altura: %d, Tipo de Evento: %s, Severidade: %d, Regiao: %s, Status: %s, Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n", 
               eventPtr->id,
               eventPtr->altura,
               NOME_TIPO[eventPtr->tipo],
               eventPtr->severidade,
               NOME_REGIAO[eventPtr->regiao],
               NOME_STATUS[eventPtr->status],
               eventPtr->dHora);
        preOrder(eventPtr->leftPtr);
        preOrder(eventPtr->rightPtr);
    }
}

// exibir todos os eventos resolvidos da árvore pelo método pre-Order:
void preOrderResolvidos(EventNodePtr eventPtr){
    if (eventPtr != NULL) {
        if (eventPtr->status == RESOLVIDO)
            printf("ID: %d, Altura: %d, Tipo de Evento: %s, Severidade: %d, Regiao: %s, Status: %s, Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n", 
                   eventPtr->id,
                   eventPtr->altura,
                   NOME_TIPO[eventPtr->tipo],
                   eventPtr->severidade,
                   NOME_REGIAO[eventPtr->regiao],
                   NOME_STATUS[eventPtr->status],
                   eventPtr->dHora);
        preOrderResolvidos(eventPtr->leftPtr);
        preOrderResolvidos(eventPtr->rightPtr);
    }
}

// buscar e exibir um evento de acordo com seu id:
void buscarEventoId(EventNodePtr eventPtr, int id) {
    // caso árvore vazia ou id não existente:
    if (eventPtr == NULL) {
        printf("Evento nao encontrado\n");
        return;
    }
    // caso encontre:
    if (eventPtr->id == id) {
        printf("ID: %d, Altura: %d, Tipo de Evento: %s, Severidade: %d, Regiao: %s, Status: %s, Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n", 
               eventPtr->id,
               eventPtr->altura,
               NOME_TIPO[eventPtr->tipo],
               eventPtr->severidade,
               NOME_REGIAO[eventPtr->regiao],
               NOME_STATUS[eventPtr->status],
               eventPtr->dHora);
    }
    // caso o id atual seja maior que o buscado → vai para ESQUERDA (menores)
    else if (eventPtr->id > id) {
        buscarEventoId(eventPtr->leftPtr, id);
    }
    // caso o id atual seja menor que o buscado → vai para DIREITA (maiores)
    else {
        buscarEventoId(eventPtr->rightPtr, id);
    }
}

// listar eventos ativos de acordo com seu intervalo de severidade em pre-order:
void listarEventosIntervaloSeveridade(EventNodePtr eventPtr, int inicio, int fim) {
    // Caso base: se o nó atual não é nulo, continua a busca
    if (eventPtr != NULL) {
        // Verifica se o evento está ATIVO
        if (eventPtr->status == ATIVO) {
            // Verifica se a severidade está dentro do intervalo [inicio, fim]
            if (eventPtr->severidade >= inicio && eventPtr->severidade <= fim) {
                // Exibe os dados do evento que atende aos critérios
                printf("ID: %d, Altura: %d, Tipo de Evento: %s, Severidade: %d, Regiao: %s, Status: %s, Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n", 
                       eventPtr->id,
                       eventPtr->altura,
                       NOME_TIPO[eventPtr->tipo],
                       eventPtr->severidade,
                       NOME_REGIAO[eventPtr->regiao],
                       NOME_STATUS[eventPtr->status],
                       eventPtr->dHora);
            }
        }
        // Percorre a subárvore ESQUERDA (IDs menores)
        listarEventosIntervaloSeveridade(eventPtr->leftPtr, inicio, fim);
        // Percorre a subárvore DIREITA (IDs maiores)
        listarEventosIntervaloSeveridade(eventPtr->rightPtr, inicio, fim);
    }
    // Quando eventPtr == NULL, a função simplesmente retorna (fim da recursão)
}

// listar eventos ativos de uma região específica em ordem (IDs crescentes)
void listarEventosRegiao(EventNodePtr eventPtr, enum regiaoCidade regiao) {
    if (eventPtr != NULL) {
        // Percorre a subárvore ESQUERDA (IDs menores)
        listarEventosRegiao(eventPtr->leftPtr, regiao);
        
        // Verifica o nó atual
        if (eventPtr->status == ATIVO && eventPtr->regiao == regiao) {
            // Exibe os dados do evento que atende aos critérios
            printf("ID: %d, Altura: %d, Tipo de Evento: %s, Severidade: %d, Regiao: %s, Status: %s, Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n", 
                   eventPtr->id,
                   eventPtr->altura,
                   NOME_TIPO[eventPtr->tipo],
                   eventPtr->severidade,
                   NOME_REGIAO[eventPtr->regiao],
                   NOME_STATUS[eventPtr->status],
                   eventPtr->dHora);
        }
        
        // Percorre a subárvore DIREITA (IDs maiores)
        listarEventosRegiao(eventPtr->rightPtr, regiao);
    }
}

// listar eventos de acordo com seu intervalo de id em pre-order:
void listarEventosIntervaloID(EventNodePtr eventPtr, int inicio, int fim) {
    // Caso base: se o nó atual não é nulo, continua a busca
    if (eventPtr != NULL) {
        // Verifica se o id está dentro do intervalo [inicio, fim]
        if (eventPtr->id >= inicio && eventPtr->id <= fim) {
            // Exibe os dados do evento que atende aos critérios
            printf("ID: %d, Altura: %d, Tipo de Evento: %s, Severidade: %d, Regiao: %s, Status: %s, Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n", 
                    eventPtr->id,
                    eventPtr->altura,
                    NOME_TIPO[eventPtr->tipo],
                    eventPtr->severidade,
                    NOME_REGIAO[eventPtr->regiao],
                    NOME_STATUS[eventPtr->status],
                    eventPtr->dHora);
        }
        // Percorre a subárvore ESQUERDA (IDs menores)
        listarEventosIntervaloID(eventPtr->leftPtr, inicio, fim);
        // Percorre a subárvore DIREITA (IDs maiores)
        listarEventosIntervaloID(eventPtr->rightPtr, inicio, fim);
    }
}

// deletar evento de acordo com id
EventNodePtr deletarEvento(EventNodePtr *eventPtr, int id){
    // caso árvore vazia ou id não existente:
    if (*eventPtr == NULL) {
        printf("Evento nao encontrado\n");
        return *eventPtr;
    }
    
    // caso encontre:
    if ((*eventPtr)->id == id) {
        // caso não esteja resolvido
        if ((*eventPtr)->status != RESOLVIDO) {
            printf("Evento nao pode ser removido - status nao e RESOLVIDO\n");
            return *eventPtr;
        }
        // Caso 1: nó folha
        if ((*eventPtr)->leftPtr == NULL && (*eventPtr)->rightPtr == NULL) {
            free(*eventPtr);
            *eventPtr = NULL;
            printf("Evento removido com sucesso!");
            return *eventPtr;
        }
        // Caso 2: tem filho à direita
        else if ((*eventPtr)->leftPtr == NULL) {
            EventNodePtr temp = (*eventPtr)->rightPtr;
            free(*eventPtr);
            *eventPtr = temp;
            printf("Evento removido com sucesso!");
            return *eventPtr;
        }
        // Caso 3: tem filho à esquerda
        else if ((*eventPtr)->rightPtr == NULL) {
            EventNodePtr temp = (*eventPtr)->leftPtr;
            free(*eventPtr);
            *eventPtr = temp;
            printf("Evento removido com sucesso!");
            return *eventPtr;
        }
        // Caso 4: tem dois filhos
        else {
            // Encontra o sucessor in-order (menor da direita)
            EventNodePtr *sucessorPtr = &((*eventPtr)->rightPtr);
            while ((*sucessorPtr)->leftPtr != NULL) {
                sucessorPtr = &((*sucessorPtr)->leftPtr);
            }
            
            // Copia os valores do sucessor
            (*eventPtr)->id = (*sucessorPtr)->id;
            (*eventPtr)->tipo = (*sucessorPtr)->tipo;
            (*eventPtr)->severidade = (*sucessorPtr)->severidade;
            (*eventPtr)->dHora = (*sucessorPtr)->dHora;
            (*eventPtr)->regiao = (*sucessorPtr)->regiao;
            (*eventPtr)->status = (*sucessorPtr)->status;

            // Remove o sucessor (que terá 0 ou 1 filho)
            int idSucessor = (*sucessorPtr)->id;
            (*eventPtr)->rightPtr = deletarEvento(&((*eventPtr)->rightPtr), idSucessor);
        }
    }
    // caso o id atual seja maior que o buscado → vai para ESQUERDA (menores)
    else if ((*eventPtr)->id > id) {
        (*eventPtr)->leftPtr = deletarEvento(&((*eventPtr)->leftPtr), id);
    }
    // caso o id atual seja menor que o buscado → vai para DIREITA (maiores)
    else {
        (*eventPtr)->rightPtr = deletarEvento(&((*eventPtr)->rightPtr), id);
    }

    // Se a árvore estiver vazia após a remoção
    if (*eventPtr == NULL)
        return *eventPtr;

    // Atualiza a altura
    (*eventPtr)->altura = maior(altura_NO((*eventPtr)->leftPtr), altura_NO((*eventPtr)->rightPtr)) + 1;

    // Rebalanceamento
    int fb = fatorBalanceamentoEvento(*eventPtr);

    // LL
    if (fb > 1 && fatorBalanceamentoEvento((*eventPtr)->leftPtr) >= 0)
        return rotacaoLL(*eventPtr);

    // LR
    if (fb > 1 && fatorBalanceamentoEvento((*eventPtr)->leftPtr) < 0)
        return rotacaoLR(*eventPtr);

    // RR
    if (fb < -1 && fatorBalanceamentoEvento((*eventPtr)->rightPtr) <= 0)
        return rotacaoRR(*eventPtr);

    // RL
    if (fb < -1 && fatorBalanceamentoEvento((*eventPtr)->rightPtr) > 0)
        return rotacaoRL(*eventPtr);

    return *eventPtr;
}

// alterar status atual do evento passando seu ID
EventNodePtr alterarStatus(EventNodePtr *eventPtr, int id) {
    // Caso 1: árvore vazia ou ID não encontrado (chegou ao fim da busca)
    if (*eventPtr == NULL) {
        printf("Evento nao encontrado\n");
        return *eventPtr;
    }
    
    // Caso 2: encontrou o nó com o ID buscado
    if ((*eventPtr)->id == id) {
        // Se o status atual é ATIVO, muda para RESOLVIDO e retorna
        if ((*eventPtr)->status == ATIVO) {
            (*eventPtr)->status = RESOLVIDO;
            printf("Status alterado com sucesso!\n");
            return *eventPtr;
        }
        // Se o status atual é RESOLVIDO, muda para ATIVO e retorna
        if ((*eventPtr)->status == RESOLVIDO) {
            (*eventPtr)->status = ATIVO;
            printf("Status alterado com sucesso!\n");
            return *eventPtr;
        }
    }
    // Caso 3: ID buscado é MENOR que o ID atual → vai para subárvore ESQUERDA
    else if ((*eventPtr)->id > id) {
        return alterarStatus(&((*eventPtr)->leftPtr), id);
    }
    // Caso 4: ID buscado é MAIOR que o ID atual → vai para subárvore DIREITA
    else {
        return alterarStatus(&((*eventPtr)->rightPtr), id);
    }
}

// alterar severidade do evento ativo passando seu ID
EventNodePtr alterarSeveridade(EventNodePtr *eventPtr, int id, int severidade) {
    // Caso 1: árvore vazia ou ID não encontrado (chegou ao fim da busca)
    if (*eventPtr == NULL) {
        printf("Evento nao encontrado\n");
        return *eventPtr;  // retorna NULL
    }
    
    // Caso 2: encontrou o nó com o ID buscado
    if ((*eventPtr)->id == id) {
        // Verifica se o evento está ATIVO (só ativos podem ter severidade alterada)
        if ((*eventPtr)->status != ATIVO) {
            printf("Severidade nao pode ser alterada - evento nao ATIVO\n");
            return *eventPtr;  // retorna o nó sem alterar
        }
        // Altera a severidade para o novo valor passado
        (*eventPtr)->severidade = severidade;
        printf("Severidade alterada com sucesso!\n");
        return *eventPtr;  // retorna o nó alterado
    }
    // Caso 3: ID buscado é MENOR que o ID atual → vai para subárvore ESQUERDA
    else if ((*eventPtr)->id > id) {
        return alterarSeveridade(&((*eventPtr)->leftPtr), id, severidade);
    }
    // Caso 4: ID buscado é MAIOR que o ID atual → vai para subárvore DIREITA
    else {
        return alterarSeveridade(&((*eventPtr)->rightPtr), id, severidade);
    }
}

// Função auxiliar recursiva para percorrer a árvore (fora da metricasArvore)
void percorrerMetricas(EventNodePtr no, int *totalNos, int *ativos, int *somaFatores, int *totalNosFator) {
    if (no != NULL) {
        (*totalNos)++;
        
        if (no->status == ATIVO) {
            (*ativos)++;
        }
        
        int fb = fatorBalanceamentoEvento(no);
        (*somaFatores) += fb;
        (*totalNosFator)++;
        
        percorrerMetricas(no->leftPtr, totalNos, ativos, somaFatores, totalNosFator);
        percorrerMetricas(no->rightPtr, totalNos, ativos, somaFatores, totalNosFator);
    }
}

// calcular e exibir métricas da árvore
void metricasArvore(EventNodePtr eventPtr) {
    if (eventPtr == NULL) {
        printf("Arvore vazia\n");
        return;
    }
    
    int alturaTotal = eventPtr->altura;
    int totalNos = 0;
    int ativos = 0;
    int somaFatores = 0;
    int totalNosFator = 0;
    
    percorrerMetricas(eventPtr, &totalNos, &ativos, &somaFatores, &totalNosFator);
    
    float fbMedio = (float)somaFatores / totalNosFator;
    
    printf("\n========== METRICAS DA ARVORE ==========\n");
    printf("Altura total da arvore: %d\n", alturaTotal);
    printf("Numero total de nos: %d\n", totalNos);
    printf("Numero de eventos ativos: %d\n", ativos);
    printf("Fator de balanceamento medio: %.2f\n", fbMedio);
    printf("Total de rotacoes realizadas: %d\n", totalRotacoes);
    printf("========================================\n");
}