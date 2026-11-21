# Compilacion de todo el proyecto con un solo comando: make
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -MMD -MP -Isrc
TARGET   := pyme

# Fuentes del programa (todo menos main.cpp, que se reutiliza en las pruebas)
SRC_LIB  := $(wildcard src/modelo/*.cpp src/vista/*.cpp src/controlador/*.cpp)
SRC_MAIN := src/main.cpp
OBJ_LIB  := $(SRC_LIB:src/%.cpp=build/%.o)
OBJ_MAIN := $(SRC_MAIN:src/%.cpp=build/%.o)

# Pruebas: cada pruebas/prueba_*.cpp se convierte en un ejecutable
SRC_TEST := $(wildcard pruebas/prueba_*.cpp)
BIN_TEST := $(SRC_TEST:pruebas/%.cpp=build/pruebas/%)

# Pruebas solo con el Modelo (sin Vista ni Controlador)
OBJ_MODELO := $(filter build/modelo/%,$(OBJ_LIB))

all: $(TARGET)

$(TARGET): $(OBJ_LIB) $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/pruebas/%: pruebas/%.cpp $(OBJ_MODELO)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< $(OBJ_MODELO) -o $@

run: all
	./$(TARGET)

pruebas: $(BIN_TEST)
	@set -e; if [ -z "$(BIN_TEST)" ]; then echo "No hay pruebas todavia en pruebas/"; fi; \
	for t in $(BIN_TEST); do echo "== $$t"; ./$$t; done

clean:
	rm -rf build $(TARGET)

-include $(OBJ_LIB:.o=.d) $(OBJ_MAIN:.o=.d)

.PHONY: all run pruebas clean
