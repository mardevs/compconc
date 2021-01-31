import java.util.LinkedList;

class Buffer {
    private final LinkedList<Double> buffer;
    private final int tamanho;

    public Buffer(int tamanho) {
        buffer = new LinkedList<>();
        this.tamanho = tamanho;

        System.out.println ("buffer = verificaBufferLE.Buffer(" + tamanho + ")");
    }

    public synchronized boolean cheio() {
        return buffer.size() == tamanho;
    }

    public synchronized boolean vazio() {
        return buffer.isEmpty();
    }

    public synchronized boolean insere(double valor) {
        if(cheio()) {
            return false;
        }

        System.out.println ("buffer.insere()");
        buffer.add(valor);
        return true;
    }

    public synchronized Double retira() {
        if(vazio()) {
            return null;
        }

        System.out.println ("buffer.remove()");
        return buffer.removeFirst();
    }

    public void imprimeBuffer() {
        for(Double valor : buffer) {
            System.out.print(valor + " ");
        }
    }
}

// Monitor
class MonitorBuffer {
    private final Buffer buffer;

    // Construtor
    MonitorBuffer(Buffer buffer) {
        this.buffer = buffer;
    }

    // Entrada para leitores
    public synchronized double Le(int id) {
        while (this.buffer.vazio()) {
            try {
                System.out.println("le.leitorBloqueado(" + id + ")");
                wait();  //bloqueia pela condicao logica da aplicacao
            } catch (InterruptedException e) { }
        }

        System.out.println("le.leitorLendo(" + id + ")");
        double valor = buffer.retira();

        notifyAll(); // libera leitores e escritores (caso existam leitores ou escritores bloqueados)

        return valor;
    }

    // Entrada para escritores
    public synchronized void Escreve (int id, double valor) {
        while (buffer.cheio()) {
            try {
                System.out.println("le.escritorBloqueado(" + id + ")");
                wait();  //bloqueia pela condicao logica da aplicacao
            } catch (InterruptedException e) { }
        }

        System.out.println("le.escritorEscrevendo(" + id + ")");
        buffer.insere(valor);

        notifyAll(); // libera leitores e escritores (caso existam leitores ou escritores bloqueados)
    }
}

class Leitor extends Thread {
    private final int id;
    private final int delay;
    private final MonitorBuffer monitor;

    public Leitor(int id, int delay, MonitorBuffer monitor) {
        this.id = id;
        this.delay = delay;
        this.monitor = monitor;
    }

    @Override
    public void run() {
        while (true) {
            double v = monitor.Le(id);
            for (int i = 0; i < 1000000; i++) {v /= 2;} //...loop bobo para simbolizar o tempo de leitura
            try {
                sleep(delay); //atraso bobo...
            } catch (InterruptedException e) { }
        }
    }
}

class Escritor extends Thread {
    private final int id;
    private final int delay;
    private final MonitorBuffer monitor;

    public Escritor(int id, int delay, MonitorBuffer monitor) {
        this.id = id;
        this.delay = delay;
        this.monitor = monitor;
    }

    @Override
    public void run() {
        double j = 777777777.7;
        while (true) {
            double valor = Math.random();
            for (int i = 0; i < 1000000; i++) { j = j/2; } //...loop bobo para simbolizar o tempo de escrita
            monitor.Escreve(id, valor);
            try {
                sleep(delay); //atraso bobo...
            } catch (InterruptedException e) { }
        }
    }
}

//--------------------------------------------------------
// Classe principal
public class Main {
    static final int L = 1;
    static final int E = 1;
    static final int TAM_BUFFER = 2;

    public static void main(String[] args) {
        System.out.println ("import verificaBufferLE");

        Buffer buffer = new Buffer(TAM_BUFFER);

        MonitorBuffer monitor = new MonitorBuffer(buffer);  // Monitor (objeto compartilhado entre leitores e escritores)
        Leitor[] l = new Leitor[L];                         // Threads leitores
        Escritor[] e = new Escritor[E];                     // Threads escritores

        System.out.println ("le = verificaBufferLE.LE(buffer)");

        for (int i = 0; i < L; i++) {
            l[i] = new Leitor(i+1, (i + 1) * 100, monitor);
            l[i].start();
        }

        for (int i = 0; i < E; i++) {
            e[i] = new Escritor(i+1, (i + 1) * 100, monitor);
            e[i].start();
        }

        try {
            for (int i = 0; i < L; i++) {
                l[i].join();
            }

            for (int i = 0; i < E; i++) {
                e[i].join();
            }

        } catch (InterruptedException interruptedException) {
            interruptedException.printStackTrace();
        }
    }
}
