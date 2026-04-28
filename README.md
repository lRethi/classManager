# classManager
Um aplicativo firmware feito para manipular e controlar um ambiente de sala de aula.

1] Senha: Array[4], rand gera 1 número de 0 à 9 pra cada ponto do array pra senha.
Teclado p/Senha: Teclado estilo celular antigo 4x3, com um botão de deletar e um de OK pra continuar.

2] Def de Alunos: Botões somam ou subtraem 5 ou 1 pra quantidade de alunos, e apertar dois botões ao mesmo tempo prossegue. (Ou repetir o teclado p/senha nao sei !)

// COMEÇOU AULA DEPOIS DAQUI

3] (BOTÃO 12) Controle de Presença: Mesma coisa acima, mas bate o limite dado pelo Def de Alunos.

4] [BOTÃO 9 e 11) Controle de Retorno dos Alunos: Um soma saída e o outro marca retorno dos alunos. (Cada apertada salva o GetTick atual num array de Entrada e Saída, e depois divide os dois, pro desafio bônus.)

5] Interface Visual: Mostra quantidade de alunos registrados no centro (tipo, grandão 20/40), na direita mostra quantos espaços disponíveis pra Saída em VERDE e na Esquerda mostra quantos alunos tão fora da sala em VERMELHO. Erro e coisa do tipo aparece EMBAIXO.

6] Encerramento e Relatório: Toda saída soma um int, e ai mostra aqui.

DESAFIOS EXTRAS]
Tempo de Aula: Assim que passar da Config Inicial, pega o Tick atual, e quando Encerrar, pega o Tick final.
Tempo Médio fora da Sala: Toda vez que alguém sair e voltar, salva o tick da saída e da volta em um array, e no fim, soma todos os valores dentro do array e divide tudo pra pegar a média.
Sistema de Tentativas de Senha: For(i < 3) simples.
