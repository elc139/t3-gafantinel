# Disciplina: Programação Paralela
# Aluno: Gustavo Arrua Fantinel

Link: ![OpenMPDemoABC.cpp](https://github.com/elc139/t3-gafantinel/blob/master/OpenMPDemoABC.cpp)

## Static
Quando o schedulling-type escolhido é static sem um chunk-size definido, faz-se com que o OpenMP divida as iterações em chunks que são aproximadamente iguais em tamanho e os distribui igualmente para cada thread:
```
1: static sem chunksize
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
A=34 B=33 C=33
```
Definindo um chunk-size, o programa se comporta de forma similar, sendo que dessa forma a atividade de cada thread é delimitada de acordo com o tamanho do chunk:
```
2: static com chunksize 4
AAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAA
A=36 B=32 C=32 
```

## Dynamic
Utilizando o schedulling type dynamic, cada thread executa uma parcela das iterações e então requisita outra parcela, até que a tarefa acabe. se não for especificado o chunk-size, o OpenMP usa ```chunk-size = 1```:
```
3: dynamic sem chunksize
CABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABC
A=33 B=33 C=34 
```
Não há uma ordem específica para a distribuição dos chunks. Aqui definimos o chunk como 4:
```
4: dynamic com chunksize 4
CCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCCAAAABBBBCCCC
A=32 B=32 C=36
```

## Guided
Semelhante ao tipo dynamic, no tipo guided o OpenMP divide a iteração do loop em chunks e os divide nas threads disponiveis. Cada thread executa o seu chunk e requisita outro até que não haja mais chunks disponíveis. Difere-se do tipo dynamic na medida em que o tamanho do chunk é proporcional ao número de iterações não designadas dividido pelo número de threads.
```
5: guided sem chunksize
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAABBBBBBBBBBCCCCCCCAAAABBBCCABC
A=20 B=48 C=32 
```
Isso quer dizer que os chunks são expressivamente maiores ao início das iterações do que ao final. Se definirmos um chunk-size na diretiva do OpenMP, quer dizer que estamos definindo o MENOR tamanho do chunk.
```
6: guided com chunksize 4
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCBBBBBBBBBBAAAAAAACCCCBBBBAAAA
A=33 B=48 C=19 
```

## Runtime
O tipo runtime significa que o OpenMP só decidirá a respeito do tipo de schedulling durante o runtime do programa, o que pode acontecer setando a variável ambiente ```OMP_SCHEDULE``` durante a execução do programa ou utilizando a função ```omp_set_schedule```.
```
7: runtime
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCBBBBBBBBBBAAAAAAACCCCBBBBAAAA
A=33 B=48 C=19 
```

## Auto
O schedulling type auto deixa a decisão quanto ao tipo para o compilador ou para o sistema que está rodando o programa.
```
8: auto
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
A=34 B=33 C=33 
```

## Sem exclusão mútua
Quando não especificamos uma seção crítica para o programa ou não utilizamos algum meio de proteção de leitura/escrita dos dados compartilhados entre as threads, o programa pode resultar em outputs erráticos e imprevisíveis.
```
9: dynamic sem mutex
B--BC-BC--B--C--C--CA-C-BC-BC-BC-BC-BC-BC-BCABC-BC-BC-BC-BC-BC-BC-BCABC-BC-BC-BC-BC-BC-BC-BC-BC-BC--
A=3 B=29 C=31
```