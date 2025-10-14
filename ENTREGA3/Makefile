CC = gcc
CFLAGS = -Wall -pthread -lm

TARGET = main
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

test: $(TARGET)
	@echo "\n=== Teste 1: Intervalo pequeno com buffer pequeno (N=50, M=5) ==="
	@echo "Esperado: Deve encontrar os números primos: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47"
	./$(TARGET) 50 5
	
	@echo "\n=== Teste 2: Intervalo maior com buffer médio (N=100, M=10) ==="
	@echo "Condição M << N satisfeita (10 << 100)"
	./$(TARGET) 100 10
	
	@echo "\n=== Teste 3: Testando restrição do buffer (N=20, M=30) ==="
	@echo "Deve falhar porque M > N não é permitido"
	./$(TARGET) 20 30 || echo "Teste passou - detectou corretamente que M > N"
	
	@echo "\n=== Teste 4: Caso limite (N=10, M=2) ==="
	@echo "Testando tamanho mínimo viável do buffer"
	./$(TARGET) 10 2
	
	@echo "\n=== Teste 5: Teste de estresse (N=1000, M=20) ==="
	@echo "Testando com números maiores para verificar a coordenação entre threads"
	./$(TARGET) 1000 20

clean:
	rm -f $(TARGET)

.PHONY: all test clean