#include <stdio.h>
#include <stdlib.h>
#include "evento.h"

int main() {
    EventNodePtr rootPtr = NULL; // ponteiro para a raiz da árvore
    printf("========== SISTEMA DE GERENCIAMENTO ==========\n");
    
    int opcao = -1;
    while (opcao != 0) {  // loop principal - só sai quando opcao = 0
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("\n0.Sair\n1. Adicionar evento\n2. Menu de Consultas\n3. Menu de Alteracoes\n4. Deletar Evento Resolvido\n5.Exibir Metricas Da Arvore\n");
        printf("Insira a opcao desejada:\n");
        
        // validação da opção principal
        if (scanf("%d", &opcao) != 1) {
            printf("ENTRADA INVALIDA!\n");
            while (getchar() != '\n'); // limpa buffer
            continue;
        }
        
        switch (opcao) {
        case 1: {  // ADICIONAR EVENTO
            printf("Insira o ID (0 para ID aleatorio):\n");
            int id;
            if (scanf("%d", &id) != 1) {
                printf("ENTRADA INVALIDA!\n");
                while (getchar() != '\n');
                break;
            }
            if (id == 0) id = gerarCadastroUnico(rootPtr, rand() % 100);

            // validação do tipo (1 a 5)
            int tipo = -1;
            while (tipo < 1 || tipo > 5){
                printf("\nEscolha o tipo de evento:\n");
                printf("1.ACIDENTE de TRANSITO\n2.FALHA de SEMAFORO\n3.INTERRUPCAO de ENERGIA\n4.ALAGAMENTO\n5.INCENDIO\n");
                printf("\nDigite:");
                if (scanf("%d", &tipo) != 1) {
                    printf("ENTRADA INVALIDA!\n");
                    while (getchar() != '\n');
                    tipo = -1;
                    continue;
                }
                if (tipo < 1 || tipo > 5) printf("DIGITE UM VALOR VALIDO!\n");
            }
            
            // validação da severidade (1 a 5)
            int severidade = -1;
            while (severidade < 1 || severidade > 5) {
                printf("\nEscolha a severidade do evento:\n");
                printf("1.BAIXA\n2.MEDIA\n3.ALTA\n4.MUITO_ALTA\n5.CRITICA\n");
                printf("\nDigite:");
                if (scanf("%d", &severidade) != 1) {
                    printf("ENTRADA INVALIDA!\n");
                    while (getchar() != '\n');
                    severidade = -1;
                    continue;
                }
                if (severidade < 1 || severidade > 5) printf("DIGITE UM VALOR VALIDO!\n");                
            }

            // validação da região (1 a 5)
            int regiao = -1;
            while (regiao < 1 || regiao > 5) {
                printf("\nEscolha a regiao do evento:\n");
                printf("1.NORTE\n2.SUL\n3.LESTE\n4.OESTE\n5.CENTRO\n");
                printf("\nDigite:");
                if (scanf("%d", &regiao) != 1) {
                    printf("ENTRADA INVALIDA!\n");
                    while (getchar() != '\n');
                    regiao = -1;
                    continue;
                }
                if (regiao < 1 || regiao > 5) printf("DIGITE UM VALOR VALIDO!\n");                
            }

            // insere o evento (status = 1 = ATIVO)
            rootPtr = insertNode(&rootPtr, gerarCadastroUnico(rootPtr, id), tipo, severidade, regiao, 1);
            printf("Evento cadastrado com sucesso!\n");
            break;
        }
        
        case 2: {  // MENU DE CONSULTAS
            int opcao2 = -1;
            while (opcao2 < 1 || opcao2 > 7) {
                printf("\n---------- MENU DE CONSULTAS ----------\n");
                printf("1.Listar todos os eventos (pre-ordem)\n"
                       "2.Listar eventos ativos\n"
                       "3.Listar eventos resolvidos\n"
                       "4.Buscar evento por ID\n"
                       "5.Listar eventos ativos por intervalo de severidade\n"
                       "6.Listar os eventos ativos de uma regiao especifica (em-ordem)\n"
                       "7.Listar eventos por intervalo de ID\n");
                printf("Insira a opcao desejada:\n");
                
                if (scanf("%d", &opcao2) != 1) {
                    printf("ENTRADA INVALIDA!\n");
                    while (getchar() != '\n');
                    opcao2 = -1;
                    continue;
                }
                
                switch (opcao2) {
                case 1: {
                    printf("\nEventos (pre-ordem):\n");
                    preOrder(rootPtr);
                    break;
                }
                case 2: {
                    printf("\nEventos ativos:\n");
                    preOrderAtivos(rootPtr);
                    break;
                }
                case 3: {
                    printf("\nEventos Resolvidos:\n");
                    preOrderResolvidos(rootPtr);
                    break;
                }
                case 4: {
                    printf("\nDigite o id do evento:\n");
                    int id = -1;
                    if (scanf("%d", &id) != 1) {
                        printf("ENTRADA INVALIDA!\n");
                        while (getchar() != '\n');
                        break;
                    }
                    printf("\nEvento encontrado:\n");
                    buscarEventoId(rootPtr, id);
                    break;
                }
                case 5: {
                    int inicio = -1, fim = -1;
                    while (inicio >= fim || inicio < 1 || fim > 5) {
                        printf("\nDigite o inicio do intervalo (1 a 5):\n");
                        if (scanf("%d", &inicio) != 1) {
                            printf("ENTRADA INVALIDA!\n");
                            while (getchar() != '\n');
                            inicio = -1;
                            continue;
                        }
                        printf("\nDigite o fim do intervalo (1 a 5):\n");
                        if (scanf("%d", &fim) != 1) {
                            printf("ENTRADA INVALIDA!\n");
                            while (getchar() != '\n');
                            fim = -1;
                            continue;
                        }
                        if (inicio >= fim) printf("O INICIO PRECISA SER MENOR QUE O FIM!\n");
                        if (inicio < 1 || fim > 5) printf("VALORES DEVEM ESTAR ENTRE 1 E 5!\n");
                    }
                    printf("\nEventos encontrados no intervalo:\n");
                    listarEventosIntervaloSeveridade(rootPtr, inicio, fim);
                    break;
                }
                case 6: {
                    int regiao = -1;
                    while (regiao < 1 || regiao > 5) {
                        printf("\nESCOLHA A REGIAO:\n");
                        printf("1.NORTE\n2.SUL\n3.LESTE\n4.OESTE\n5.CENTRO\n");
                        printf("Digite:");
                        if (scanf("%d", &regiao) != 1) {
                            printf("ENTRADA INVALIDA!\n");
                            while (getchar() != '\n');
                            regiao = -1;
                            continue;
                        }
                        if (regiao < 1 || regiao > 5) printf("DIGITE UM VALOR VALIDO!\n");
                    }
                    printf("\nEventos encontrados na regiao:\n");
                    listarEventosRegiao(rootPtr, regiao);
                    break;
                }
                case 7: {
                    int inicio = -1, fim = -1;
                    while (inicio >= fim || inicio < 0) {
                        printf("\nDigite o inicio do intervalo:\n");
                        if (scanf("%d", &inicio) != 1) {
                            printf("ENTRADA INVALIDA!\n");
                            while (getchar() != '\n');
                            inicio = -1;
                            continue;
                        }
                        printf("\nDigite o fim do intervalo:\n");
                        if (scanf("%d", &fim) != 1) {
                            printf("ENTRADA INVALIDA!\n");
                            while (getchar() != '\n');
                            fim = -1;
                            continue;
                        }
                        if (inicio >= fim) printf("O INICIO PRECISA SER MENOR QUE O FIM!\n");
                    }
                    printf("\nEventos encontrados no intervalo:\n");
                    listarEventosIntervaloID(rootPtr, inicio, fim);
                    break;
                }
                }
                if (opcao2 < 1 || opcao2 > 7) printf("DIGITE UM VALOR VALIDO!\n");
            }
            break;
        }
        
        case 3: {  // MENU DE ALTERAÇÕES
            int opcao3 = -1;
            while (opcao3 < 1 || opcao3 > 2) {
                printf("\n---------- MENU DE ALTERACOES ----------\n");
                printf("\n1.Alterar Status de um evento\n2.Alterar severidade de um evento ativo\n");
                printf("Escolha a opcao:\n");
                
                if (scanf("%d", &opcao3) != 1) {
                    printf("ENTRADA INVALIDA!\n");
                    while (getchar() != '\n');
                    opcao3 = -1;
                    continue;
                }
            
                switch (opcao3) {
                case 1: {  // alterar status
                    printf("Listagem de todos os eventos:\n");
                    preOrder(rootPtr);
                    printf("\nDigite o id do evento cujo status deve ser alterado:\n");
                    int id = -1;
                    if (scanf("%d", &id) != 1) {
                        printf("ENTRADA INVALIDA!\n");
                        while (getchar() != '\n');
                        break;
                    }
                    alterarStatus(&rootPtr, id);
                    break;
                }
                case 2: {  // alterar severidade
                    printf("Listagem de todos os eventos ativos:\n");
                    preOrderAtivos(rootPtr);
                    printf("\nDigite o id do evento cuja severidade deve ser alterada:\n");
                    int id = -1;
                    if (scanf("%d", &id) != 1) {
                        printf("ENTRADA INVALIDA!\n");
                        while (getchar() != '\n');
                        break;
                    }
                    
                    int novaSeveridade = -1;
                    while (novaSeveridade < 1 || novaSeveridade > 5) {
                        printf("\nNiveis de severidade:\n");
                        printf("1.BAIXA\n2.MEDIA\n3.ALTA\n4.MUITO ALTA\n5.CRITICA\n");
                        printf("\nDigite o novo valor de severidade (1 a 5):\n");
                        if (scanf("%d", &novaSeveridade) != 1) {
                            printf("ENTRADA INVALIDA!\n");
                            while (getchar() != '\n');
                            novaSeveridade = -1;
                            continue;
                        }
                        if (novaSeveridade < 1 || novaSeveridade > 5) 
                            printf("DIGITE UM VALOR VALIDO (1 A 5)!\n");
                    }
                    alterarSeveridade(&rootPtr, id, novaSeveridade);
                    printf("Severidade alterada com sucesso!\n");
                    break;
                }
                }
                if (opcao3 < 1 || opcao3 > 2) 
                    printf("DIGITE UM VALOR VALIDO!\n");
            }
            break;
        }
        
        case 4: {  // DELETAR EVENTO RESOLVIDO
            printf("\nListagem dos eventos resolvidos:\n");
            preOrderResolvidos(rootPtr);
            int id = -1;
            printf("Digite o id do evento a ser removido: ");
            if (scanf("%d", &id) != 1) {
                printf("ENTRADA INVALIDA!\n");
                while (getchar() != '\n');
                break;
            }
            rootPtr = deletarEvento(&rootPtr, id);
            break;
        }
        
        case 5: {  // EXIBIR MÉTRICAS
            metricasArvore(rootPtr);
            break;
        }
        }
        
        if (opcao < 0 || opcao > 5) 
            printf("\nDIGITE UM VALOR VALIDO (0 A 5)!");
    }
    return 0;
}