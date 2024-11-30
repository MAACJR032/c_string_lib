# c_string_lib: Uma Biblioteca de strings em C

[English](README.md) | Português

c_string_lib é uma biblioteca personalizada de strings em C que providencia uma forma dinâmica e eficiente de gerenciar strings. Ela abstrai a complexidade de alocação de memória e null-termination (`'\0`), permitindo que os usuários foquem nas operações de string. 

A biblioteca inclui uma vasta gama de funções para criar, modificar, comparar e manipular strings.
Funções que tipicamente aceitam `const char*` também possuem uma versão que funciona com a struct `string.

## Features

- Gerenciamento de memória automático para crescimento e redimensionamento da string.
- Null-termination sempre é gerenciado internamente.
- Possui suporte várias operações:
  - **Append**, **Insert**, **Resize**, **Clear**, **Compare**, **Reverse**, **Convert to Upper/Lowercase**.
  - **Substring extraction**, **Splitting** strings, e **Joining** strings com delimitadores.
- Códigos de erro para gerenciar casos extremos com eficiência (e.g., argumentos null, erros de allocação).
- API Consistente para ambos inputs `const char*` e struct `string`.

## Clone Esse Repositório

```
git clone https://github.com/MAACJR032/c_string_lib.git
```
