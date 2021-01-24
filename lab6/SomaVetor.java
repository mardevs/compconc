/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Nome: Domenica Maria Cioci Ferreira Gonçalves */
/* Laboratório: 6 */
/* Codigo: Implementação de um programa concorrente em Java para soma de dois vetores */
/* -------------------------------------------------------------------*/

class Vetor {
    private double[] vet;

    public Vetor(int tam) {
        this.vet = new double[tam];
    }

    public void set(int i, double valor) {
        vet[i] = valor;
    }

    public double get(int i) {
        return vet[i];
    }

    public int tamanho() {
        return vet.length;
    }

    public void imprime() {
        for(double d : vet) {
            System.out.print(d + " ");
        }
        System.out.print("\n");
    }
}

class ThreadSoma extends Thread {
    private int n_threads;
    private int thread_id;
    private final Vetor a;
    private final Vetor b;
    private final Vetor c;

    private final int inicio;
    private final int fim;

    ThreadSoma(int n_threads, int thread_id, Vetor a, Vetor b, Vetor c) {
        this.n_threads = n_threads;
        this.thread_id = thread_id;
        this.a = a;
        this.b = b;
        this.c = c;

        // Os 3 vetores devem ter o mesmo tamanho para podermos somá-los. Vamos utilizar o tamanho de 'a'.
        int tam_bloco = a.tamanho() / n_threads;
        this.inicio = thread_id * tam_bloco;

        if(thread_id != n_threads - 1) {
            this.fim = this.inicio + tam_bloco;
        } else {
            this.fim = a.tamanho();
        }
    }

    @Override
    public void run() {
        for(int i = inicio; i < fim; i++) {
            double s = a.get(i) + b.get(i);
            c.set(i, s);
        }
    }
}

public class SomaVetor {
    public static void main(String[] args) {
        int N_THREADS = 4;
        int TAM_VETOR = 100000;

        Thread[] threads = new Thread[N_THREADS];
        Vetor a = new Vetor(TAM_VETOR);
        Vetor b = new Vetor(TAM_VETOR);
        Vetor c = new Vetor(TAM_VETOR);

        // Inicializando Vetores
        for(int i = 0; i < TAM_VETOR; i++) {
            a.set(i, i);
            b.set(i, TAM_VETOR - i);
        }

        // Criando Threads;
        for(int i = 0; i < N_THREADS; i++) {
            threads[i] = new ThreadSoma(N_THREADS, i, a, b, c);
        }

        // Iniciando Threads
        for(int i = 0; i < N_THREADS; i++) {
            threads[i].start();
        }

        // Esperando Threads
        for(int i = 0; i < N_THREADS; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        System.out.print("A: ");
        a.imprime();

        System.out.print("B: ");
        b.imprime();

        System.out.print("C: ");
        c.imprime();
    }
}
