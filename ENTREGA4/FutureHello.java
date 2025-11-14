/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;


//classe runnable
class MyCallable implements Callable<Long> {
  //construtor
  MyCallable() {}
 
  //método para execução
  public Long call() throws Exception {
    long s = 0;
    for (long i=1; i<=100; i++) {
      s++;
    }
    return s;
  }
}

class ChecaPrimo implements Callable<Integer> {
    private final long number;

    ChecaPrimo (long number){
        this.number = number;
    }

    @Override
    public Integer call() throws Exception {
        if (ehPrime(number)) {
            return 1;
        } else {
            return 0;
        }
    }

    private boolean ehPrime(long n) {
        if (n <= 1) return false;
        if (n <= 3) return true;

        if (n % 2 == 0 || n % 3 == 0) return false;

        for (long i = 5; i * i <= n; i = i + 6) {
            if (n % i == 0 || n % (i + 2) == 0)
              return false;
        }

        return true;
    }
}

//classe do método main
public class FutureHello  {
  private static final long N = 10000000;
  private static final int NTHREADS = 10;

  public static void main(String[] args) {
    //cria um pool de threads (NTHREADS)
    ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
    //cria uma lista para armazenar referencias de chamadas assincronas
    List<Future<Long>> list = new ArrayList<Future<Long>>();
    List<Future<Integer>> primeList = new ArrayList<Future<Integer>>();

    for (int i = 0; i < N; i++) {
      Callable<Long> worker = new MyCallable();
      Callable<Integer> primeWorker = new ChecaPrimo(i);
      /*submit() permite enviar tarefas Callable ou Runnable e obter um objeto Future para acompanhar o progresso e recuperar o resultado da tarefa
       */
      Future<Long> submit = executor.submit(worker);
      Future<Integer> primeSubmit = executor.submit(primeWorker);
      list.add(submit);
      primeList.add(primeSubmit);
    }

    System.out.println(list.size());
    System.out.println(primeList.size());
    //pode fazer outras tarefas...

    //recupera os resultados e faz o somatório final
    long sum = 0;
    int primeCount = 0;
    for (Future<Long> future : list) {
      try {
        sum += future.get(); //bloqueia se a computação nao tiver terminado
      } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
      } catch (ExecutionException e) {
        e.printStackTrace();
      }
    }
    for (Future<Integer> future : primeList) {
      try {
        primeCount += future.get();
      } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
      } catch (ExecutionException e) {
        e.printStackTrace();
      }
    }
    System.out.println(sum);
    System.out.println("Quantidade de números primos (no intervalo de 1 a "+N+"): "+primeCount);
    executor.shutdown();
  }
}
