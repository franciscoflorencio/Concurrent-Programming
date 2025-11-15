# Define o compilador e a JVM
JCC = javac
JVM = java

# Constantes de teste
# Número de primos esperado para N=10.000.000
PRIMO_COUNT_ESPERADO = 664579
# Valor de N usado no FutureHello.java
N_VALOR = 10000000

# Alvo padrão: compila todas as classes
all: MyPool.class FutureHello.class

# --- Regras de Compilação ---
MyPool.class: MyPool.java
	@echo "Compilando MyPool.java..."
	@$(JCC) MyPool.java

FutureHello.class: FutureHello.java
	@echo "Compilando FutureHello.java..."
	@$(JCC) FutureHello.java

# --- Alvo Principal de Teste ---
# Roda todos os testes de corretude
test: test-mypool test-futurehello
	@echo "\n✅ Todos os testes de corretude passaram com sucesso!"

# --- Teste da Atividade 1 (MyPool) ---
test-mypool: MyPool.class
	@echo "\nExecutando teste para MyPool (Atividade 1)..."
	@# Executa o programa e salva a saída em um arquivo
	@$(JVM) MyPool > mypool.output
	
	@# Verifica se a saída contém resultados-chave esperados.
	@# O grep -q é silencioso e falha o make se não encontrar o padrão.
	@grep -q "O número 2 é primo!" mypool.output
	@grep -q "O número 9 não é primo!" mypool.output
	@grep -q "O número 23 é primo!" mypool.output
	@grep -q "O número 24 não é primo!" mypool.output
	@grep -q "Terminou" mypool.output
	@echo "-> Teste MyPool: OK"

# --- Teste da Atividade 3 (FutureHello) ---
test-futurehello: FutureHello.class
	@echo "\nExecutando teste para FutureHello (Atividade 3)..."
	@# Executa o programa e salva a saída
	@$(JVM) FutureHello > futurehello.output
	
	@# Verifica se a contagem de primos está correta
	@grep -q "Quantidade de números primos (no intervalo de 1 a $(N_VALOR)): $(PRIMO_COUNT_ESPERADO)" futurehello.output
	@echo "-> Teste FutureHello: OK"

# --- Alvos de Execução (sem teste) ---
run-mypool: MyPool.class
	@$(JVM) MyPool

run-futurehello: FutureHello.class
	@$(JVM) FutureHello

# --- Limpeza ---
# Remove os arquivos gerados
clean:
	@echo "Limpando arquivos .class e .output..."
	@rm -f *.class *.output