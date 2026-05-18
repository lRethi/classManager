# Class Manager

Projeto integrado do primeiro trimestre da disciplina de Linguagem de Programações e Sistemas Embarcados.

O **Class Manager** é um sistema embarcado desenvolvido em **STM32F103C8** com interface gráfica em **display ST7789 240x240**, voltado para o controle de presença, quantidade de alunos em sala e tempo médio fora da sala.

Feito por **Fred** && **Livia**.

## Visão geral

A aplicação utiliza quatro botões em **pull-up** para navegação e entrada de dados. O fluxo principal do sistema é:

1. tela inicial;
2. validação por senha;
3. definição da quantidade de alunos matriculados;
4. tela principal de controle;
5. registro de saídas e entradas;
6. encerramento da aula com exibição de estatísticas.

### 🎥 Demonstração do projeto
[![Demo do projeto](https://img.youtube.com/vi/YprnkfKEc30/0.jpg)](https://www.youtube.com/watch?v=YprnkfKEc30)

## Hardware utilizado

- STM32F103C8
- Display ST7789 240x240 via SPI1
- 4 botões físicos com resistor pull-up
- Clock do MCU configurado em **64 MHz**

## Funcionalidades

- tela de boas-vindas;
- acesso protegido por senha;
- definição da quantidade total de alunos da turma;
- controle da quantidade de alunos presentes;
- controle da quantidade de alunos fora da sala;
- registro de horários de saída e entrada;
- cálculo do tempo total de aula;
- cálculo do tempo médio fora da sala;
- exibição final dos resultados no display.

## Como funciona

### 1. Inicialização

Ao ligar o sistema, o STM32 inicializa o HAL, configura o clock, habilita os periféricos necessários e inicializa o display ST7789.

### 2. Tela inicial

A interface inicial mostra o nome do projeto e aguarda a ativação do botão de confirmação para continuar.

### 3. Senha de acesso

O sistema gera uma senha aleatória de 4 dígitos com base nas direções dos botões.

Os botões representam os números:

- cima = 1
- direita = 2
- baixo = 3
- esquerda = 4

O usuário tem até 3 tentativas para acertar a sequência.

### 4. Definição da turma

Depois do acesso, a tela permite definir a quantidade total de alunos da sala.

Os botões ajustam o valor em passos de:

- `+1`
- `-1`
- `+5`
- `-5`

A seleção é encerrada com o pressionamento simultâneo de dois botões.

### 5. Tela principal

Na tela principal, os botões passam a controlar:

- presença total em sala;
- quantidade de alunos fora;
- registro de horários de saída e entrada.

O sistema também faz controle de limites para impedir valores inválidos.

### 6. Encerramento

Quando o programa é encerrado, a tela final exibe:

- quantidade de alunos presentes;
- total de saídas;
- duração total da aula;
- tempo médio fora da sala.

## Organização do código

As principais funções do projeto são:

- `iniciarPrograma()`
- `entrarSenha()`
- `definirAlunos()`
- `interfacePrincipal()`
- `controlePresenca()`
- `controleSaida()`
- `encerrarPrograma()`

## Bibliotecas utilizadas

- `stm32f1xx_hal.h`
- biblioteca do display `ST7789`
- `stdlib.h`
- `stdio.h`
- `stdbool.h`
- `time.h`

## Configurações importantes

- Clock do STM32 configurado em **64 MHz**
- Botões configurados em **Pull-Up**
- Comunicação com o display feita por **SPI1**
- Memória dinâmica usada para armazenar os tempos de entrada e saída

## Estrutura de dados

O projeto utiliza vetores dinâmicos para registrar os tempos das saídas e entradas dos alunos.

Isso permite armazenar apenas os eventos necessários e redimensionar a memória conforme o uso.

## Observações

- O projeto foi desenvolvido para ambiente embarcado com interface visual no display.
- O comportamento depende da configuração correta dos GPIOs, do SPI e da biblioteca do ST7789.
- O sistema foi pensado para uso acadêmico e demonstra controle de estado, interface e memória dinâmica em C.

## Licença

Código acadêmico desenvolvido para fins educacionais.
