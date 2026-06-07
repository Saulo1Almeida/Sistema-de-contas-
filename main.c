#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numeroConta;
    char nome[50];
    float saldo;
    int ativo;
} Cliente;

void cadastrarCliente(FILE *arquivo) {
    Cliente c;
    int posicao;

    printf("Posicao do registro: ");
    scanf("%d", &posicao);

    if (posicao < 0) {
        printf("Posicao invalida.\n");
        return;
    }

    printf("Numero da conta: ");
    scanf("%d", &c.numeroConta);

    getchar();

    printf("Nome: ");
    fgets(c.nome, sizeof(c.nome), stdin);
    c.nome[strcspn(c.nome, "\n")] = '\0';

    printf("Saldo: ");
    scanf("%f", &c.saldo);

    c.ativo = 1;

    fseek(arquivo, posicao * sizeof(Cliente), SEEK_SET);
    fwrite(&c, sizeof(Cliente), 1, arquivo);

    fflush(arquivo);

    printf("Cliente cadastrado com sucesso!\n");
}

void consultarCliente(FILE *arquivo) {
    Cliente c;
    int conta;
    int encontrado = 0;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arquivo);

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {
        if (c.ativo == 1 && c.numeroConta == conta) {
            printf("\nConta: %d\n", c.numeroConta);
            printf("Nome: %s\n", c.nome);
            printf("Saldo: %.2f\n", c.saldo);

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Cliente nao encontrado.\n");
    }
}

void atualizarSaldo(FILE *arquivo) {
    Cliente c;
    int conta;
    float novoSaldo;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arquivo);

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {

        if (c.ativo == 1 && c.numeroConta == conta) {

            printf("Saldo atual: %.2f\n", c.saldo);

            printf("Novo saldo: ");
            scanf("%f", &novoSaldo);

            c.saldo = novoSaldo;

            fseek(arquivo, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arquivo);

            fflush(arquivo);

            printf("Saldo atualizado com sucesso!\n");
            return;
        }
    }

    printf("Cliente nao encontrado.\n");
}

void removerCliente(FILE *arquivo) {
    Cliente c;
    int conta;

    printf("Numero da conta: ");
    scanf("%d", &conta);

    rewind(arquivo);

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {

        if (c.ativo == 1 && c.numeroConta == conta) {

            c.ativo = 0;

            fseek(arquivo, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arquivo);

            fflush(arquivo);

            printf("Conta encerrada com sucesso!\n");
            return;
        }
    }

    printf("Cliente nao encontrado.\n");
}

void listarClientes(FILE *arquivo) {
    Cliente c;

    printf("\n===== LISTA DE CLIENTES =====\n");

    while (fread(&c, sizeof(Cliente), 1, arquivo) == 1) {

        if (c.ativo == 1) {

            printf("Conta: %d\n", c.numeroConta);
            printf("Nome : %s\n", c.nome);
            printf("Saldo: %.2f\n", c.saldo);
            printf("---------------------------\n");
        }
    }

    printf("Fim da leitura do arquivo.\n");
    printf("Caso nao tenha encontrado algum usuario use a opcao 6 para reposicionar o ponteiro e repetir a listagem.\n");
}

int main() {

    FILE *arquivo;
    int opcao;

    arquivo = fopen("clientes.dat", "r+b");

    if (arquivo == NULL) {
        arquivo = fopen("clientes.dat", "w+b");
    }

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    do {

        printf("\n===== MENU =====\n");
        printf("1. Cadastrar novo cliente\n");
        printf("2. Consultar cliente\n");
        printf("3. Atualizar saldo\n");
        printf("4. Encerrar conta\n");
        printf("5. Listar clientes\n");
        printf("6. Restaurar leitura com rewind()\n");
        printf("7. Encerrar programa\n");
        printf("Opcao: ");

        scanf("%d", &opcao);

        switch(opcao) {

            case 1:
                cadastrarCliente(arquivo);
                break;

            case 2:
                consultarCliente(arquivo);
                break;

            case 3:
                atualizarSaldo(arquivo);
                break;

            case 4:
                removerCliente(arquivo);
                break;

            case 5:
                listarClientes(arquivo);
                break;

            case 6:
                rewind(arquivo);
                printf("Ponteiro reposicionado para o inicio do arquivo.\n");
                break;

            case 7:
                printf("Programa encerrado.\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 7);

    fclose(arquivo);

    return 0;
}
