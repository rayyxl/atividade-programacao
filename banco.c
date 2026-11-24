#include "meu_banco.h"
#include <stdio.h>
#include <string.h>

void cadastrar_clientes(Cliente *clientes, int quantidade) {
    int digitos = 1000;
    int i, j, linha;

    for (i = 0; i < quantidade; i++) {
        printf("\nCadastro do cliente %d:\n", i + 1);

        printf("Nome: ");
        scanf(" %99[^\n]", clientes[i].nome);

        clientes[i].saldo_total = 0.0f;
        clientes[i].quantidade_contas = 0;
        clientes[i].emprestimo_realizado = 0;

        int quantidade_contas;
        do {
            printf("Quantas contas deseja cadastrar (1 ou 2)? ");
            scanf("%d", &quantidade_contas);
        } while (quantidade_contas < 1 || quantidade_contas > 2);

        for (linha = 0; linha < quantidade_contas; linha++) {
            int tipo;

            do {
                printf("Tipo da conta %d (1 - Corrente, 2 - Salario, 3 - Poupanca): ",
                       linha + 1);
                scanf("%d", &tipo);

                if (tipo < 1 || tipo > 3) {
                    printf("Tipo invalido. Digite novamente.\n");
                    continue;
                }

                int tipo_repetido = 0;
                for (j = 0; j < linha; j++) {
                    if ((int)clientes[i].contas[j][0] == tipo) {
                        tipo_repetido = 1;
                    }
                }

                if (tipo_repetido) {
                    printf("Este cliente ja possui esse tipo de conta.\n");
                    tipo = 0;
                } else if (linha > 0) {
                    int primeiro_tipo = (int)clientes[i].contas[0][0];

                    if ((primeiro_tipo == 1 && tipo == 2) ||
                        (primeiro_tipo == 2 && tipo == 1)) {
                        printf("Conta corrente e conta salario nao podem ser usadas ao mesmo tempo.\n");
                        tipo = 0;
                    }
                }
            } while (tipo < 1 || tipo > 3);

            printf("Saldo da conta: ");
            float saldo;
            scanf("%f", &saldo);

            clientes[i].contas[linha][0] = (float)tipo;
            clientes[i].contas[linha][1] = saldo;
            clientes[i].saldo_total += saldo;

            snprintf(clientes[i].numeros_conta[linha],
                     sizeof(clientes[i].numeros_conta[linha]),
                     "%04d-%02d", digitos, tipo);

            digitos++;
        }

        clientes[i].quantidade_contas = quantidade_contas;

        printf("Saldo total do cliente: R$ %.2f\n", clientes[i].saldo_total);
    }
}

Cliente *procurar_cliente_por_numero(Cliente *clientes, int quantidade,
                                     const char *numero_conta) {
    int i, linha;
    for (i = 0; i < quantidade; i++) {
        for (linha = 0; linha < clientes[i].quantidade_contas; linha++) {
            if (strcmp(clientes[i].numeros_conta[linha], numero_conta) == 0) {
                return &clientes[i];
            }
        }
    }

    return NULL;
}

void realizar_emprestimos(Cliente *clientes, int quantidade,
                          const char *numero_conta,
                          float quantidade_emprestimo) {
    Cliente *cliente = procurar_cliente_por_numero(clientes, quantidade, numero_conta);

    if (cliente == NULL) {
        printf("Cliente com numero de conta %s nao encontrado.\n", numero_conta);
        return;
    }

    if (cliente->emprestimo_realizado) {
        printf("Este cliente ja realizou um emprestimo.\n");
        return;
    }

    int possui_corrente = 0;
    int possui_poupanca = 0;
    float saldo_poupanca = 0.0f;

    for (int linha = 0; linha < cliente->quantidade_contas; linha++) {
        int tipo = (int)cliente->contas[linha][0];

        if (tipo == 1) {
            possui_corrente = 1;
        } else if (tipo == 3) {
            possui_poupanca = 1;
            saldo_poupanca = cliente->contas[linha][1];
        }
    }

    if (!possui_corrente || !possui_poupanca) {
        printf("Emprestimo permitido apenas para clientes com conta corrente e poupança.\n");
        return;
    }

    if (saldo_poupanca <= 0.0f) {
        printf("O cliente precisa possuir saldo na conta poupança.\n");
        return;
    }

    float limite = 0.25f * (10.0f * cliente->saldo_total);

    if (quantidade_emprestimo <= 0.0f) {
        printf("Valor invalido para emprestimo.\n");
        return;
    }

    if (quantidade_emprestimo > limite) {
        printf("Emprestimo excede o limite de R$ %.2f.\n", limite);
        return;
    }

    if (quantidade_emprestimo > saldo_poupanca) {
        printf("O saldo da conta poupanca e insuficiente.\n");
        return;
    }

    for (int linha = 0; linha < cliente->quantidade_contas; linha++) {
        if (strcmp(cliente->numeros_conta[linha], numero_conta) == 0) {
            cliente->contas[linha][1] -= quantidade_emprestimo;
            cliente->saldo_total -= quantidade_emprestimo;
            cliente->emprestimo_realizado = 1;

            printf("Emprestimo realizado para %s. Novo saldo da conta: R$ %.2f\n",
                   cliente->nome, cliente->contas[linha][1]);
            printf("Novo saldo total do cliente: R$ %.2f\n", cliente->saldo_total);
            printf("Limite utilizado: R$ %.2f de R$ %.2f\n",
                   quantidade_emprestimo, limite);
            return;
        }
    }
}