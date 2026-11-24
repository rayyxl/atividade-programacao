#include <stdio.h>
#include <stdlib.h>
#include "meu_banco.h"

int main() {
    int opcao;
    Cliente *clientes = NULL;
    int quantidade = 0;

    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar clientes\n");
        printf("2. Consultar cliente por numero da conta\n");
        printf("3. Realizar emprestimo\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                printf("Quantos clientes deseja cadastrar? ");
                scanf("%d", &quantidade);

                if (quantidade <= 0) {
                    printf("Quantidade invalida.\n");
                    break;
                }

                free(clientes);
                clientes = malloc((size_t)quantidade * sizeof(*clientes));

                if (clientes == NULL) {
                    printf("Erro ao alocar memoria para os clientes.\n");
                    return 1;
                }

                cadastrar_clientes(clientes, quantidade);
                break;
            }

            case 2: {
                if (clientes == NULL || quantidade == 0) {
                    printf("Nenhum cliente cadastrado.\n");
                    break;
                }

                char numero_conta[8];
                printf("Digite o numero da conta: ");
                scanf(" %7s", numero_conta);

                Cliente *cliente = procurar_cliente_por_numero(clientes, quantidade,
                                                               numero_conta);
                if (cliente == NULL) {
                    printf("Cliente nao encontrado.\n");
                } else {
                    printf("Nome: %s\n", cliente->nome);
                    printf("Saldo total: R$ %.2f\n", cliente->saldo_total);

                    for (int i = 0; i < cliente->quantidade_contas; i++) {
                        printf("Conta: %s | Tipo: %.0f | Saldo: R$ %.2f\n",
                               cliente->numeros_conta[i],
                               cliente->contas[i][0],
                               cliente->contas[i][1]);
                    }
                }
                break;
            }

            case 3: {
                if (clientes == NULL || quantidade == 0) {
                    printf("Nenhum cliente cadastrado.\n");
                    break;
                }

                char numero_conta[8];
                float valor;

                printf("Digite o numero da conta: ");
                scanf(" %7s", numero_conta);
                printf("Digite o valor do emprestimo: ");
                scanf("%f", &valor);

                if (valor < 0) {
                    printf("Valor invalido.\n");
                } else {
                    realizar_emprestimos(clientes, quantidade, numero_conta, valor);
                }
                break;
            }

            case 4:
                printf("Encerrando o programa.\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 4);

    free(clientes);
    return 0;
}