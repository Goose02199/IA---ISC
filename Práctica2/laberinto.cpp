#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm> // Para std::find_if
#include <cmath>     // Para abs
#include <limits>    // Para std::numeric_limits
#include <conio.h>   // Para _getch
#include <windows.h> // Para SetConsoleTextAttribute

// --- Definiciones para los tipos de casilla ---
enum class TipoCasilla {
    VACIO,
    MURO,
    INICIO,
    FIN,
    CAMINO_BFS,
    CAMINO_DFS,
    CAMINO_ASTAR
};

// --- Estructura para Nodo ---
struct Nodo {
    int x, y;

    // Operador de comparación para poder buscar en contenedores
    bool operator==(const Nodo& otro) const {
        return x == otro.x && y == otro.y;
    }
};

const int N = 20;

// --- Prototipos de funciones ---
void setColor(int color);
void dibujarMatriz(const std::vector<std::vector<TipoCasilla>>& matriz, int cX, int cY, const std::string& titulo);
void resolverLaberintoBFS(std::vector<std::vector<TipoCasilla>> matriz, Nodo inicio, Nodo fin);
void resolverLaberintoDFS(std::vector<std::vector<TipoCasilla>> matriz, Nodo inicio, Nodo fin);
bool resolverDFS_recursivo(std::vector<std::vector<TipoCasilla>>& matriz, std::vector<std::vector<bool>>& visitados, Nodo actual, Nodo fin);
void resolverLaberintoAStar(std::vector<std::vector<TipoCasilla>> matriz, Nodo inicio, Nodo fin);

// --- Función para cambiar el color de la consola ---
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// --- Función de Dibujo ---
void dibujarMatriz(const std::vector<std::vector<TipoCasilla>>& matriz, int cursorX, int cursorY, const std::string& titulo) {
    system("cls");
    std::cout << titulo << std::endl;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            if (x == cursorX && y == cursorY) {
                std::cout << "[]";
            } else {
                switch (matriz[y][x]) {
                    case TipoCasilla::MURO:         std::cout << "##"; break;
                    case TipoCasilla::INICIO:       std::cout << "IN"; break;
                    case TipoCasilla::FIN:          std::cout << "FN"; break;
                    case TipoCasilla::CAMINO_BFS:   setColor(10); std::cout << "**"; setColor(7); break;
                    case TipoCasilla::CAMINO_DFS:   setColor(14); std::cout << "++"; setColor(7); break;
                    case TipoCasilla::CAMINO_ASTAR: setColor(13); std::cout << "@@"; setColor(7); break;
                    default:                        std::cout << " ."; break;
                }
            }
        }
        std::cout << std::endl;
    }
    if (cursorX != -1) {
        std::cout << "\n--- MODO EDICION ---\n"
                  << "Usa W/A/S/D para moverte.\n"
                  << "ESPACIO: Muro | I: Inicio | F: Fin\n"
                  << "Q: Guardar y Resolver con todos los algoritmos.\n";
    }
}

// --- BFS ---
void resolverLaberintoBFS(std::vector<std::vector<TipoCasilla>> matriz, Nodo inicio, Nodo fin) {
    std::queue<Nodo> frontera;
    std::vector<std::vector<bool>> visitados(N, std::vector<bool>(N, false));
    std::vector<std::vector<Nodo>> padres(N, std::vector<Nodo>(N, {-1, -1}));

    frontera.push(inicio);
    visitados[inicio.y][inicio.x] = true;
    bool solucionEncontrada = false;

    while (!frontera.empty()) {
        Nodo actual = frontera.front();
        frontera.pop();

        if (actual == fin) {
            solucionEncontrada = true;
            break;
        }

        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; ++i) {
            Nodo vecino = {actual.x + dx[i], actual.y + dy[i]};
            if (vecino.x >= 0 && vecino.x < N && vecino.y >= 0 && vecino.y < N &&
                !visitados[vecino.y][vecino.x] && matriz[vecino.y][vecino.x] != TipoCasilla::MURO) {
                visitados[vecino.y][vecino.x] = true;
                padres[vecino.y][vecino.x] = actual;
                frontera.push(vecino);
            }
        }
    }

    if (solucionEncontrada) {
        Nodo actual = fin;
        while (!(actual == inicio)) {
            if (matriz[actual.y][actual.x] != TipoCasilla::FIN) {
                matriz[actual.y][actual.x] = TipoCasilla::CAMINO_BFS;
            }
            actual = padres[actual.y][actual.x];
        }
    }
    dibujarMatriz(matriz, -1, -1, solucionEncontrada ? "--- Solucion con BFS (Camino mas corto: **) ---" : "--- BFS: No se encontro solucion ---");
}

// --- DFS ---
void resolverLaberintoDFS(std::vector<std::vector<TipoCasilla>> matriz, Nodo inicio, Nodo fin) {
    std::vector<std::vector<bool>> visitados(N, std::vector<bool>(N, false));
    bool solucionEncontrada = resolverDFS_recursivo(matriz, visitados, inicio, fin);
    dibujarMatriz(matriz, -1, -1, solucionEncontrada ? "--- Solucion con DFS (Primer camino encontrado: ++) ---" : "--- DFS: No se encontro solucion ---");
}

bool resolverDFS_recursivo(std::vector<std::vector<TipoCasilla>>& matriz, std::vector<std::vector<bool>>& visitados, Nodo actual, Nodo fin) {
    if (actual.x < 0 || actual.x >= N || actual.y < 0 || actual.y >= N ||
        matriz[actual.y][actual.x] == TipoCasilla::MURO || visitados[actual.y][actual.x]) {
        return false;
    }

    visitados[actual.y][actual.x] = true;

    if (actual == fin) {
        return true;
    }

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; ++i) {
        Nodo siguiente = {actual.x + dx[i], actual.y + dy[i]};
        if (resolverDFS_recursivo(matriz, visitados, siguiente, fin)) {
            if (matriz[actual.y][actual.x] != TipoCasilla::INICIO) {
                matriz[actual.y][actual.x] = TipoCasilla::CAMINO_DFS;
            }
            return true;
        }
    }
    return false;
}

// --- A* ---
int calcularManhattan(Nodo a, Nodo b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

void resolverLaberintoAStar(std::vector<std::vector<TipoCasilla>> matriz, Nodo inicio, Nodo fin) {
    std::vector<Nodo> frontera;
    std::vector<std::vector<Nodo>> padres(N, std::vector<Nodo>(N, {-1, -1}));
    
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> g_score(N, std::vector<int>(N, INF));
    std::vector<std::vector<int>> f_score(N, std::vector<int>(N, INF));

    g_score[inicio.y][inicio.x] = 0;
    f_score[inicio.y][inicio.x] = calcularManhattan(inicio, fin);
    frontera.push_back(inicio);

    bool solucionEncontrada = false;

    while (!frontera.empty()) {
        auto it_actual = std::min_element(frontera.begin(), frontera.end(), [&](const Nodo& a, const Nodo& b) {
            return f_score[a.y][a.x] < f_score[b.y][b.x];
        });
        Nodo actual = *it_actual;
        frontera.erase(it_actual);

        if (actual == fin) {
            solucionEncontrada = true;
            break;
        }

        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; ++i) {
            Nodo vecino = {actual.x + dx[i], actual.y + dy[i]};

            if (vecino.x < 0 || vecino.x >= N || vecino.y < 0 || vecino.y >= N || matriz[vecino.y][vecino.x] == TipoCasilla::MURO) {
                continue;
            }

            int temp_g_score = g_score[actual.y][actual.x] + 1;
            if (temp_g_score < g_score[vecino.y][vecino.x]) {
                padres[vecino.y][vecino.x] = actual;
                g_score[vecino.y][vecino.x] = temp_g_score;
                f_score[vecino.y][vecino.x] = temp_g_score + calcularManhattan(vecino, fin);

                if (std::find_if(frontera.begin(), frontera.end(), [&](const Nodo& n){ return n == vecino; }) == frontera.end()) {
                    frontera.push_back(vecino);
                }
            }
        }
    }

    if (solucionEncontrada) {
        Nodo actual = fin;
        while (!(actual == inicio)) {
            if (matriz[actual.y][actual.x] != TipoCasilla::FIN) {
                matriz[actual.y][actual.x] = TipoCasilla::CAMINO_ASTAR;
            }
            actual = padres[actual.y][actual.x];
        }
    }
    dibujarMatriz(matriz, -1, -1, solucionEncontrada ? "--- Solucion con A* (Camino Optimo: @@) ---" : "--- A*: No se encontro solucion ---");
}


// --- MAIN ---
int main() {
    std::vector<std::vector<TipoCasilla>> matriz(N, std::vector<TipoCasilla>(N, TipoCasilla::VACIO));
    int cursorX = 0, cursorY = 0;
    Nodo inicio = {-1, -1}, fin = {-1, -1};
    char tecla;

    dibujarMatriz(matriz, cursorX, cursorY, "Construye tu laberinto");

    while ((tecla = _getch()) != 'q' && tecla != 'Q') {
        switch (tecla) {
            case 'w': case 'W': if (cursorY > 0) cursorY--; break;
            case 's': case 'S': if (cursorY < N - 1) cursorY++; break;
            case 'a': case 'A': if (cursorX > 0) cursorX--; break;
            case 'd': case 'D': if (cursorX < N - 1) cursorX++; break;
            case ' ':
                if (matriz[cursorY][cursorX] == TipoCasilla::VACIO) matriz[cursorY][cursorX] = TipoCasilla::MURO;
                else if (matriz[cursorY][cursorX] == TipoCasilla::MURO) matriz[cursorY][cursorX] = TipoCasilla::VACIO;
                break;
            case 'i': case 'I':
                if (inicio.x != -1) matriz[inicio.y][inicio.x] = TipoCasilla::VACIO;
                matriz[cursorY][cursorX] = TipoCasilla::INICIO;
                inicio = {cursorX, cursorY};
                break;
            case 'f': case 'F':
                if (fin.x != -1) matriz[fin.y][fin.x] = TipoCasilla::VACIO;
                matriz[cursorY][cursorX] = TipoCasilla::FIN;
                fin = {cursorX, cursorY};
                break;
        }
        dibujarMatriz(matriz, cursorX, cursorY, "Construye tu laberinto");
    }

    if (inicio.x == -1 || fin.x == -1) {
        std::cerr << "\nError: Debes definir un INICIO ('I') y un FIN ('F') para resolver.\n";
        return 1;
    }

    resolverLaberintoBFS(matriz, inicio, fin);
    std::cout << "\nPresiona una tecla para ver la solucion con DFS...";
    _getch();

    resolverLaberintoDFS(matriz, inicio, fin);
    std::cout << "\nPresiona una tecla para ver la solucion con A*...";
    _getch();

    resolverLaberintoAStar(matriz, inicio, fin);

    setColor(7);
    std::cout << "\nPrograma terminado.\n";
    return 0;
}