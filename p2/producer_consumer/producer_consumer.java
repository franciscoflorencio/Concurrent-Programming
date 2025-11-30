package producer_consumer;
class Buffer {
    private int N;
    private int[] buffer;
    private int count = 0, in = 0, out = 0;
 
    // constructor
    Buffer(int N) {
        this.N = N;
        this.buffer = new int[N];
    }

    // producer
    public synchronized void Insere (int item){
        try {
            while (this.count == N) {
                wait();
            }
            this.buffer[this.in] = item;
            this.in = (this.in + 1) % N;
            this.count++;
            notifyAll();
        } catch (InterruptedException e) {}
    }

    // consumer
    public synchronized int Retira() {
        int item = -1;
        try {
            while (this.count == 0) {
                wait();
            }
            item = this.buffer[this.out];
            this.out = (this.out + 1) % N;
            this.count--;
            notifyAll();
        } catch (InterruptedException e) {}
        return item;
    }
}