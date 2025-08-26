# Variaveis do Compilador
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -pthread -lm

# Variaveis dos Executaveis
SEQ_EXEC = sequencial_gen
CONC_EXEC = concorrente_run

# Variaveis do Benchmark
DIMENSOES = 1000000 10000000 50000000
THREADS = 1 2 4
N_EXECUCOES = 5
ARQUIVO_BIN = dados_vetores.bin

# Alvo padrao: executa o benchmark
all: run

# Alvo para rodar o benchmark completo
run: $(SEQ_EXEC) $(CONC_EXEC)
	@echo "========================================================"
	@echo "Iniciando Benchmark..."
	@echo "========================================================"
	@for n in $(DIMENSOES); do \
		echo "\nIniciando testes para a Dimensão N = $$n"; \
		echo "Gerando novo arquivo de dados..."; \
		./$(SEQ_EXEC) $$n; \
		for t in $(THREADS); do \
			soma_tempo=0; \
			soma_variacao=0; \
			echo "--> Testando com $$t thread(s)..."; \
			for i in $$(seq 1 $(N_EXECUCOES)); do \
				saida=$$("./$(CONC_EXEC)" $(ARQUIVO_BIN) $$t); \
				tempo=$$(echo "$$saida" | grep 'TEMPO:' | awk '{print $$2}'); \
				variacao=$$(echo "$$saida" | grep 'VARIACAO:' | awk '{print $$2}'); \
				soma_tempo=$$(echo "$$soma_tempo + $$tempo" | bc -l); \
				soma_variacao=$$(echo "$$soma_variacao + $$variacao" | bc -l); \
			done; \
			media_tempo=$$(echo "scale=6; $$soma_tempo / $(N_EXECUCOES)" | bc -l); \
			media_variacao=$$(echo "scale=12; $$soma_variacao / $(N_EXECUCOES)" | bc -l); \
			echo "   (a) Dimensão dos vetores: $$n"; \
			echo "   (b) Número de threads: $$t"; \
			echo "   (c) Tempo de execução (média): $$media_tempo segundos"; \
			echo "   (d) Variação relativa do resultado: $$media_variacao"; \
			echo "--------------------------------------------------------"; \
		done; \
	done
	@echo "\nBenchmark concluído."

# Regra de compilacao para o programa sequencial
$(SEQ_EXEC): sequencial.c
	$(CC) $(CFLAGS) -o $@ $<

# Regra de compilacao para o programa concorrente
$(CONC_EXEC): concorrente.c timer.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Alvo para limpar os arquivos gerados
clean:
	rm -f $(SEQ_EXEC) $(CONC_EXEC) $(ARQUIVO_BIN)

# Declara alvos que nao sao arquivos
.PHONY: all run clean