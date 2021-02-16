# Lab 8 - Atividade 1

a. _Quais devem ser os valores iniciais dos semáforos (`em_e`, `em_l`, `escr`, `leit`) para que o algoritmo funcione corretamente?_

Os semáforos `em_e` e `em_l` são, respectivamente, os semáforos de exclusão mútua dos escritores e dos leitores. Por esse motivo, eles são iniciados com o valor `1`.
Já os semáforos `escr` e `leit` são semáforos de condição, representando, respectivamente, se escritores e leitores podem escrever ou ler. Como inicialmente tanto os escritores podem escrever como os leitores podem ler, esses semáforos também são iniciados com o valor `1`.

b. _É possível simplificar esse codigo (reduzir o uso dos mecanismos de sincronização)?_

Podemos substituir os semáforos binários de exclusão mútua separados para leitores e escritores por apenas um que faça a exclusão mútua de forma mais parecida com a versão de solução que vimos antes de aprender a usar semáforos. Isso muda um pouco a solução do problema, mas torna possível a simplificação.

c. no código

d. no código

e. _Descreva se os locks usados são recursivos ou não e justifique a escolha feita._

Os locks não são recursivos, pois, como cada thread chama pthread_mutex_lock apenas uma vez antes de chamar pthread_mutex_unlock (ou seja, como cada thread não faz chamadas recursivas a pthread_mutex_lock) os locks não precisam ser recursivos.