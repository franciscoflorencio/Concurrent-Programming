package ThreadPool;

import java.util.LinkedList;

public class SimpleThreadPool {
    private final int nThreads;
    private final WorkerThread[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;
    
    public SimpleThreadPool(int nThreads) {
        this.nThreads = nThreads;
        this.queue = new LinkedList<>();
        this.threads = new WorkerThread[nThreads];
        this.shutdown = false;

        for (int i = 0; i < nThreads; i++) {
            threads[i] = new WorkerThread();
            threads[i].start();
        }
    }

    public void execute(Runnable r) {
        synchronized (queue) {
            // se o pool está parando, NEGAR novos trabalhos
            if (this.shutdown) return;

            queue.addLast(r);
            queue.notify();
        }
    }

    public void shutdown() {
        synchronized (queue) {
            this.shutdown = true;

            // acorda todas as threads para que eles possam checar a flag 'shutdown'
            queue.notifyAll();
        }

        // espera todas as treads finalizarem suas threads atuais e saírem
        for (int i = 0; i < nThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {}
        }
    }

    public class WorkerThread extends Thread {
        public void run() {
            Runnable r;

            while (true) {
                synchronized (queue) {
                    // lógica do wait: espera enquanto a fila está vazia e não estamos dando shutdown
                    while (queue.isEmpty() && !shutdown) {
                        try {
                            queue.wait(); 
                        } catch (InterruptedException e) {}
                    }

                    // lógica de saída: se fila está vazia e shutdown é verdade, pare a thread
                    if (queue.isEmpty() && shutdown) {
                        return;
                    }

                    r = queue.removeFirst();
                }

                // lógica de execução: roda a task fora do lock, o que permite que as outras threads peguem tasks concorrentemente
                try {
                    r.run();
                } catch (RuntimeException e) {}
            }
        }

    } 
}
