typedef struct eventNode EventNode;
typedef EventNode *EventNodePtr;

// Enumeração dos tipos possíveis de eventos urbanos
enum tipoEvento {
    ACIDENTE_TRANSITO = 1,
    FALHA_SEMAFORO = 2,
    INTERRUPCAO_ENERGIA = 3,
    ALAGAMENTO = 4,
    INCENDIO = 5
};

// Enumeração dos níveis de severidade (inicia em 1 para melhor legibilidade)
enum nivelSeveridade {
    BAIXA = 1,
    MEDIA = 2,
    ALTA = 3,
    MUITO_ALTA = 4,
    CRITICA = 5
};

// Enumeração das regiões da cidade
enum regiaoCidade {
    NORTE = 1,
    SUL = 2,
    LESTE = 3,
    OESTE = 4,
    CENTRO= 5
};

// Enumeração do status do evento
enum statusEvento {
    ATIVO = 1,
    RESOLVIDO = 2
};

// interfaces
int gerarCadastroUnico(EventNodePtr eventPtr, int id);
int maior(int a, int b);
EventNodePtr insertNode(EventNodePtr *eventPtr, int id, enum tipoEvento tipo, enum nivelSeveridade severidade, enum regiaoCidade regiao, enum statusEvento status);
void preOrder(EventNodePtr eventPtr);
void preOrderAtivos(EventNodePtr eventPtr);
void preOrderResolvidos(EventNodePtr eventPtr);
void buscarEventoId(EventNodePtr eventPtr, int id);
void listarEventosIntervaloSeveridade(EventNodePtr eventPtr, int inicio, int fim);
void listarEventosRegiao(EventNodePtr eventPtr, enum regiaoCidade regiao);
void listarEventosIntervaloID(EventNodePtr eventPtr, int inicio, int fim);
EventNodePtr deletarEvento(EventNodePtr *eventPtr, int id);
EventNodePtr alterarStatus(EventNodePtr *eventPtr, int id);
EventNodePtr alterarSeveridade(EventNodePtr *eventPtr, int id, int severidade);
void metricasArvore(EventNodePtr eventPtr);
