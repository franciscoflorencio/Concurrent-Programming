package readers_writers;
class LE {
    private int leitoresLendo = 0;
    private int escritoresEscrevendo = 0;
    private int escritoresEsperando = 0;

    // Entrada Leitor
    public synchronized void EntraLeitor() {
        try {
            while (this.escritoresEscrevendo > 0 ||
                    this.escritoresEsperando > 0)
                wait();
            this.leitoresLendo++;
        } catch (InterruptedException e) {}
    }

    // Saida Leitor
    public synchronized void SaiLeitor() {
        this.leitoresLendo--;
        if (this.leitoresLendo == 0)
            notifyAll();
    }

    // Entrada Escritor
    public synchronized void EntraEscritor() {
        try {
            this.escritoresEsperando++;
            while (this.leitoresLendo > 0 ||
                    this.escritoresEscrevendo > 0)
                wait();
            this.escritoresEscrevendo--;
            this.escritoresEscrevendo++;
        } catch (InterruptedException e) {}
    }

    // Saida Escritor
    public synchronized void SaiEscritor() {
        this.escritoresEscrevendo--;
        notifyAll();
    }
}
