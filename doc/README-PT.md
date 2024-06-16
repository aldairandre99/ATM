# Bank Monami
Projecto de final de semestre da disciplina de Laboratorio de Informatica do 1º ano 2023/2024.

Docente : Malaquias Victor Filipe
<br/>
<br/>

## Criar um simulador de ATM

Que permita ao usuário realizar operações como:

[ x ] Retirar o dinheiro

[ x ] Depósito

[ x ] Verificação de saldo.
<br/>
<br/>

## Desafios

[ x ] Verificação de saldo suficiente para saques

[ x ] Autenticação de usuário através de uma palavra passe.
<br/>
<br/>

## Melhorias para as proximas versões

[ ] Fazer login com nome de usuario e palavra passe

[ ] Adicionar funcionalidade no menu para trocar de usuario

[ ] Fazer transferencia via IBAN
<br/>
<br/>

## Requesitos para rodar o programa

GCC

Sqlite3

Linux Machine
<br/>
<br/>

## Compilando o programa para usuarios Linux

In atm-project dir

```
  mv atm-env.db atm.db
  
  gcc main.c -I${url_to_sqlite_header_file} -lsqlite3 -o main

  ./main
```
