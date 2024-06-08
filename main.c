#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <unistd.h>

// Estrutura de dados Dos Menu
typedef struct
{
  char nome[40];
  int numeroDaOpcao;
} Opcoes;

// Estrutura de dados do menu Inicial
Opcoes opcoesIniciais[] = {
    {"Verificar Saldo", 1},
    {"Depositar Dinheiro", 2},
    {"Sacar Dinheiro", 3},
    {"Registrar Usuario", 4},
    {"Sair", 5}
};

// Funções protótipo
int criarTabelasNoBancoDeDados(sqlite3 *db);
void verificaUsuariosNoBd(sqlite3 *db);
int autenticarUsuario(sqlite3 *db, int *userId, int pin);
void verificarSaldo(sqlite3 *db, int userId);
void depositarDinheiro(sqlite3 *db, int userId);
void sacarDinheiro(sqlite3 *db, int userId);
void menu(Opcoes *opcoes, int tamanho_Das_Opcoes);
void registrarUsuario(sqlite3 *db);
void copy_right(void);
void logotipo_Do_Banco(void);


// Função principal
int main() {
    sqlite3 *db;
    int escolha, userId,pin;

    // Abrir o banco de dados
    if (sqlite3_open("atm.db", &db)) {
        fprintf(stderr, "Erro ao abrir banco de dados: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    criarTabelasNoBancoDeDados(db);

    /* verificaUsuariosNoBd(db); */

    // Autenticação de usuário
  /*   if (!autenticarUsuario(db,&userId, pin)) {
        printf("Autenticação falhou! Saindo...\n");
        sqlite3_close(db);
        return 1;
    } */

    logotipo_Do_Banco();
    printf("\tNumero de Usuario: ");
    scanf("%d",&userId);
    printf("\tDigite seu PIN: ");
    scanf("%d", &pin);
    system("clear");
    do {
        logotipo_Do_Banco();
        menu(opcoesIniciais, (sizeof(opcoesIniciais) / sizeof(opcoesIniciais[0])));
        printf("\tDigite sua escolha: ");
        scanf("%d", &escolha);

        system("clear");
        switch (escolha) {
            case 1:
                verificarSaldo(db, userId);
                break;
            case 2:
                depositarDinheiro(db, userId);
                break;
            case 3:
                sacarDinheiro(db, userId);
                break;
            case 4:
                registrarUsuario(db);
                break;
            case 5:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Escolha inválida! Tente novamente.\n");
        }
    } while (escolha != 5);

    // Fechar o banco de dados
    sqlite3_close(db);
    copy_right();
    return 0;
}

int criarTabelasNoBancoDeDados(sqlite3 *db) {
    int rc;

    // Abrir o banco de dados
    /* rc = sqlite3_open("atm.db", &db);
   
    if (rc != SQLITE_OK) {
      fprintf(stderr, "Erro ao abrir banco de dados: %s\n", sqlite3_errmsg(db));
      return 1;
    } */

    // Criar tabela "usuarios"
    char sql[] = "CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, nome TEXT NOT NULL, senha TEXT NOT NULL, pin INTEGER NOT NULL)";
    rc = sqlite3_exec(db, sql, NULL, 0, NULL);

    if (rc != SQLITE_OK) {
      fprintf(stderr, "Erro ao criar tabela: %s\n", sqlite3_errmsg(db));
      system("clear");
    }

    // Criar tabela "balance" (Create table "balance")
    char sqlBalance[] = "CREATE TABLE balance (id INTEGER PRIMARY KEY AUTOINCREMENT, conta INTEGER NOT NULL, saldo REAL NOT NULL, FOREIGN KEY(conta) REFERENCES users(id))";
    rc = sqlite3_exec(db, sqlBalance, NULL, 0, NULL);

    if (rc != SQLITE_OK) {
    fprintf(stderr, "Erro ao criar tabela: %s\n", sqlite3_errmsg(db));
    system("clear");
    }
    
    // Criar tabela "accounts"
   /*  char sqlAccounts[] = "CREATE TABLE accounts (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER NOT NULL, saldo REAL NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id))";
    rc = sqlite3_exec(db, sqlAccounts, NULL, 0, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar tabela: %s\n", sqlite3_errmsg(db));
        system("clear");
    } */

    return rc;

}

//Verificar ser ha usuarios no banco de dados

void verificaUsuariosNoBd(sqlite3 *db){
    char sql[] = "SELECT * FROM users" ;
}

// Funcão responsavel por exibir os elementos da estrutura de dados presentes na lista
void menu(Opcoes *opcoes, int tamanho_Das_Opcoes)
{
  for (int i = 0; i < tamanho_Das_Opcoes; i++)
  {
    printf("\t[%d] %s\n", opcoes[i].numeroDaOpcao, opcoes[i].nome);
    puts("");
  }
}

//Registrar um novo usuário
void registrarUsuario(sqlite3 *db) {
  char nome[50];
  char *errMsg = 0;
  char senha[20];
  int pin;
  double saldo;
  int rc;

  system("clear");
  logotipo_Do_Banco();

  printf("\tDigite seu nome: ");
  scanf("%s", nome);

  printf("\tDigite sua senha: ");
  scanf("%s", senha);

  printf("\tDigite um PIN: ");
  scanf("%d", &pin);

  printf("\tDigite saldo: ");
  scanf("%lf", &saldo);

  // Prepare and execute SQL statement to insert new user data
  char sql[256];
  sprintf(sql, "INSERT INTO users (nome, senha, pin) VALUES ('%s', '%s', %d)", nome, senha, pin);
  rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

  if ( rc != SQLITE_OK) {
    fprintf(stderr, "Erro no SQL: %s\n", errMsg);
    sqlite3_free(errMsg);
  } else {
    sleep(2);
    system("clear");
    logotipo_Do_Banco();
    printf("\t\tUsuário registrado com sucesso!\n");
    sleep(5);
    system("clear");
  }
}

// Função para autenticar usuário
int autenticarUsuario(sqlite3 *db, int *userId,int pin) {
    char sql[256];
    sprintf(sql, "SELECT id FROM users WHERE pin = %d",pin);
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erro no SQL: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    int res = sqlite3_step(stmt);
    if (res == SQLITE_ROW) {
        *userId = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return 1;
    } else {
        sqlite3_finalize(stmt);
        return 0;
    }
}

// Função para verificar o saldo
void verificarSaldo(sqlite3 *db, int userId) {
    logotipo_Do_Banco();
    char sql[256];
    sprintf(sql, "SELECT balance FROM accounts WHERE user_id = %d", userId);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erro no SQL: %s\n", sqlite3_errmsg(db));
        return;
    }

    int res = sqlite3_step(stmt);
    if (res == SQLITE_ROW) {
        double saldo = sqlite3_column_double(stmt, 0);
        printf("Seu saldo atual é: %.2f KZ\n", saldo);
    } else {
        printf("Erro ao verificar saldo.\n");
    }

    sqlite3_finalize(stmt);
}

// Função para depositar dinheiro
void depositarDinheiro(sqlite3 *db, int userId) {
    float deposito;
    char *errMsg = 0;
    
    system("clear");
    logotipo_Do_Banco();
    
    printf("\tDigite o valor para depositar: KZ ");
    scanf("%f", &deposito);

    /* "CREATE TABLE balance (id INTEGER PRIMARY KEY AUTOINCREMENT, conta INTEGER NOT NULL, saldo REAL NOT NULL, FOREIGN KEY(conta) REFERENCES users(id))";
    */
    /* char sqlBalance[256];
    sprintf(sqlBalance, "INSERT INTO balance (conta) VALUES ('%d')", userId);
    
    if (sqlite3_exec(db, sqlBalance, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "Erro no SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
    } */

    if (deposito > 0) { 
        char sql[256];
        sprintf(sql, "UPDATE accounts SET saldo = saldo + %.2f WHERE user_id = %d", deposito, userId);

        if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
            fprintf(stderr, "Erro no SQL: %s\n", errMsg);
            sqlite3_free(errMsg);
        } else {
            printf("\tDepósito realizado com sucesso!\n");
        }
    } else {
        printf("\tValor de depósito inválido!\n");
    }
}

// Função para sacar dinheiro
void sacarDinheiro(sqlite3 *db, int userId) {
    float saque;
    printf("Digite o valor para sacar: KZ ");
    scanf("%f", &saque);

    if (saque > 0) {
        // Verificar saldo
        char sql[256];
        sprintf(sql, "SELECT balance FROM accounts WHERE user_id = %d", userId);

        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
            fprintf(stderr, "Erro no SQL: %s\n", sqlite3_errmsg(db));
            return;
        }

        int res = sqlite3_step(stmt);
        if (res == SQLITE_ROW) {
            double saldo = sqlite3_column_double(stmt, 0);
            sqlite3_finalize(stmt);

            if (saque <= saldo) {
                sprintf(sql, "UPDATE accounts SET balance = balance - %.2f WHERE user_id = %d", saque, userId);
                char *errMsg = 0;
                if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
                    fprintf(stderr, "Erro no SQL: %s\n", errMsg);
                    sqlite3_free(errMsg);
                } else {
                    printf("Saque realizado com sucesso!\n");
                }
            } else {
                printf("Saldo insuficiente!\n");
            }
        } else {
            printf("Erro ao verificar saldo.\n");
        }
    } else {
        printf("Valor de saque inválido!\n");
    }
}

void copy_right()
{
  system("clear");
  logotipo_Do_Banco();
  puts("\t\tObrigado por usar o nosso App");
  puts("");
  puts("");
  sleep(3);
  system("clear");
  logotipo_Do_Banco();
  puts("\tAutores");
  puts("");
  puts("\tAldair André");
  puts("\tDébora Félix");
  puts("\tRafael Vumbi");
  puts("\tGeovani Muanha");
  puts("");
  puts("\tRepositorio Github: https://github.com/aldairandre/banking_app");
  puts("");
  puts("");
  exit(0);
}

void logotipo_Do_Banco()
{
  puts("\t============================================");
  puts("\t\t\tBANCO MONAMI");
  puts("\t============================================");
  puts("");
  puts("");
}