#ifndef MEU_BANCO_H
#define MEU_BANCO_H

typedef struct {
    char nome[100];

    // Cada linha representa uma conta que contém seu número e seu saldo.
    float contas[2][2];

    char numeros_conta[2][8];
    int quantidade_contas;
    float saldo_total;
    int emprestimo_realizado;
} Cliente;

void cadastrar_clientes(Cliente *clientes, int quantidade);
Cliente *procurar_cliente_por_numero(Cliente *clientes, int quantidade,
                                     const char *numero_conta);
void realizar_emprestimos(Cliente *clientes, int quantidade,
                          const char *numero_conta,
                          float quantidade_emprestimo);

#endif