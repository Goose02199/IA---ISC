#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <limits.h> // Para INT_MAX

#define N 20

// --- Definiciones para los tipos de casilla ---
#define VACIO 0
#define MURO 1
#define INICIO 2
#define FIN 3
#define CAMINO_BFS 4 // Verde
#define CAMINO_DFS 5 // Amarillo
#define CAMINO_ASTAR 6 // Magenta

// --- Estructuras para Nodo y Cola (usadas por BFS) ---
typedef struct {
    int x, y;
} Nodo;

typedef struct {
    Nodo items[N * N];
    int frente, final;
} Cola;

// --- Prototipos de funciones ---
void setColor(int color);
void dibujarMatriz(int matriz[N][N], int cX, int cY, const char* titulo);

// --- Función para cambiar el color de la consola ---
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// --- Funciones de la Cola para BFS ---
void inicializarCola(Cola *q) { q->frente = -1; q->final = -1; }
int estaVacia(Cola *q) { return q->frente == -1; }
void encolar(Cola *q, Nodo valor) {
    if (q->final == N * N - 1) return;
    if (q->frente == -1) q->frente = 0;
    q->final++;
    q->items[q->final] = valor;
}
Nodo desencolar(Cola *q) {
    Nodo item = {-1, -1};
    if (estaVacia(q)) return item;
    item = q->items[q->frente];
    q->frente++;
    if (q->frente > q->final) q->frente = q->final = -1;
    return item;
}

// --- Función de Dibujo (actualizada para todos los caminos) ---
void dibujarMatriz(int matriz[N][N], int cursorX, int cursorY, const char* titulo) {
    system("cls");
    printf("%s\n", titulo);
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            if (x == cursorX && y == cursorY) printf("[]");
            else {
                switch (matriz[y][x]) {
                    case MURO:       printf("##"); break;
                    case INICIO:     printf("IN"); break;
                    case FIN:        printf("FN"); break;
                    case CAMINO_BFS: setColor(10); printf("**"); setColor(7); break;
                    case CAMINO_DFS: setColor(14); printf("++"); setColor(7); break;
                    case CAMINO_ASTAR:setColor(13); printf("@@"); setColor(7); break;
                    default:         printf(" ."); break;
                }
            }
        }
        printf("\n");
    }
    if (cursorX != -1) {
        printf("\n--- MODO EDICION ---");
        printf("\nUsa W/A/S/D para moverte.");
        printf("\nESPACIO: Muro | I: Inicio | F: Fin");
        printf("\nQ: Guardar y Resolver con todos los algoritmos.\n");
    }
}

// --- ALGORITMOS DE BÚSQUEDA ---

// BFS (sin cambios)
void resolverLaberintoBFS(int matriz[N][N], Nodo inicio, Nodo fin);

// DFS (sin cambios)
void resolverLaberintoDFS(int matriz[N][N], Nodo inicio, Nodo fin);
int resolverDFS_recursivo(int matriz[N][N], int v[N][N], Nodo a, Nodo f);

// --- A* (A-Estrella) ---
int calcularManhattan(Nodo a, Nodo b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void resolverLaberintoAStar(int matriz[N][N], Nodo inicio, Nodo fin) {
    Nodo frontera[N * N];
    int frontera_count = 0;
    
    int en_frontera[N][N] = {0};
    int visitados[N][N] = {0};

    Nodo padres[N][N];
    memset(padres, -1, sizeof(padres));

    int g_score[N][N];
    int f_score[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            g_score[i][j] = INT_MAX;
            f_score[i][j] = INT_MAX;
        }
    }

    g_score[inicio.y][inicio.x] = 0;
    f_score[inicio.y][inicio.x] = calcularManhattan(inicio, fin);

    frontera[frontera_count++] = inicio;
    en_frontera[inicio.y][inicio.x] = 1;

    int solucionEncontrada = 0;

    while (frontera_count > 0) {
        int actual_idx = 0;
        for (int i = 1; i < frontera_count; i++) {
            if (f_score[frontera[i].y][frontera[i].x] < f_score[frontera[actual_idx].y][frontera[actual_idx].x]) {
                actual_idx = i;
            }
        }
        Nodo actual = frontera[actual_idx];

        if (actual.x == fin.x && actual.y == fin.y) {
            solucionEncontrada = 1;
            break;
        }

        // Mover de frontera a visitados
        frontera[actual_idx] = frontera[--frontera_count];
        en_frontera[actual.y][actual.x] = 0;
        visitados[actual.y][actual.x] = 1;

        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; i++) {
            Nodo vecino = {actual.x + dx[i], actual.y + dy[i]};
            
            if (vecino.x < 0 || vecino.x >= N || vecino.y < 0 || vecino.y >= N) continue;
            if (matriz[vecino.y][vecino.x] == MURO || visitados[vecino.y][vecino.x]) continue;

            int temp_g_score = g_score[actual.y][actual.x] + 1;

            if (temp_g_score < g_score[vecino.y][vecino.x]) {
                padres[vecino.y][vecino.x] = actual;
                g_score[vecino.y][vecino.x] = temp_g_score;
                f_score[vecino.y][vecino.x] = temp_g_score + calcularManhattan(vecino, fin);
                
                if (!en_frontera[vecino.y][vecino.x]) {
                    frontera[frontera_count++] = vecino;
                    en_frontera[vecino.y][vecino.x] = 1;
                }
            }
        }
    }

    if (solucionEncontrada) {
        Nodo actual = fin;
        while (actual.x != inicio.x || actual.y != inicio.y) {
            if (matriz[actual.y][actual.x] != FIN) matriz[actual.y][actual.x] = CAMINO_ASTAR;
            actual = padres[actual.y][actual.x];
        }
    }
    dibujarMatriz(matriz, -1, -1, solucionEncontrada ? "--- Solucion con A* (Camino Optimo: @@) ---" : "--- A*: No se encontro solucion ---");
}

// --- MAIN ---
int main() {
    int matriz_original[N][N] = {0};
    int cursorX = 0, cursorY = 0;
    Nodo inicio = {-1, -1}, fin = {-1, -1};
    char tecla;

    dibujarMatriz(matriz_original, cursorX, cursorY, "Construye tu laberinto");

    while ((tecla = _getch()) != 'q' && tecla != 'Q') {
        switch (tecla) {
            case 'w': case 'W': if (cursorY > 0) cursorY--; break;
            case 's': case 'S': if (cursorY < N - 1) cursorY++; break;
            case 'a': case 'A': if (cursorX > 0) cursorX--; break;
            case 'd': case 'D': if (cursorX < N - 1) cursorX++; break;
            case ' ':
                if (matriz_original[cursorY][cursorX] == VACIO) matriz_original[cursorY][cursorX] = MURO;
                else if (matriz_original[cursorY][cursorX] == MURO) matriz_original[cursorY][cursorX] = VACIO;
                break;
            case 'i': case 'I':
                if (inicio.x != -1) matriz_original[inicio.y][inicio.x] = VACIO;
                matriz_original[cursorY][cursorX] = INICIO;
                inicio = (Nodo){cursorX, cursorY};
                break;
            case 'f': case 'F':
                if (fin.x != -1) matriz_original[fin.y][fin.x] = VACIO;
                matriz_original[cursorY][cursorX] = FIN;
                fin = (Nodo){cursorX, cursorY};
                break;
        }
        dibujarMatriz(matriz_original, cursorX, cursorY, "Construye tu laberinto");
    }

    if (inicio.x == -1 || fin.x == -1) {
        printf("\nError: Debes definir un INICIO ('I') y un FIN ('F') para resolver.\n");
        return 1;
    }

    // --- Resolver con BFS ---
    int matriz_bfs[N][N];
    memcpy(matriz_bfs, matriz_original, sizeof(matriz_original));
    resolverLaberintoBFS(matriz_bfs, inicio, fin);
    printf("\nPresiona una tecla para ver la solucion con DFS...");
    _getch();

    // --- Resolver con DFS ---
    int matriz_dfs[N][N];
    memcpy(matriz_dfs, matriz_original, sizeof(matriz_original));
    resolverLaberintoDFS(matriz_dfs, inicio, fin);
    printf("\nPresiona una tecla para ver la solucion con A*...");
    _getch();

    // --- Resolver con A* ---
    int matriz_astar[N][N];
    memcpy(matriz_astar, matriz_original, sizeof(matriz_original));
    resolverLaberintoAStar(matriz_astar, inicio, fin);

    setColor(7);
    printf("\nPrograma terminado.\n");
    return 0;
}


// --- Implementaciones de BFS y DFS (sin cambios, necesarias para compilar) ---

void resolverLaberintoBFS(int matriz[N][N], Nodo inicio, Nodo fin) {
    Cola nodos_frontera;
    inicializarCola(&nodos_frontera);
    int visitados[N][N]; memset(visitados, 0, sizeof(visitados));
    Nodo padres[N][N]; memset(padres, -1, sizeof(padres));
    encolar(&nodos_frontera, inicio);
    visitados[inicio.y][inicio.x] = 1;
    int solucionEncontrada = 0;
    while (!estaVacia(&nodos_frontera)) {
        Nodo nodo_actual = desencolar(&nodos_frontera);
        if (nodo_actual.x == fin.x && nodo_actual.y == fin.y) { solucionEncontrada = 1; break; }
        int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};
        for (int i = 0; i < 4; i++) {
            Nodo hijo = {nodo_actual.x + dx[i], nodo_actual.y + dy[i]};
            if (hijo.x >= 0 && hijo.x < N && hijo.y >= 0 && hijo.y < N) {
                if (!visitados[hijo.y][hijo.x] && matriz[hijo.y][hijo.x] != MURO) {
                    visitados[hijo.y][hijo.x] = 1;
                    padres[hijo.y][hijo.x] = nodo_actual;
                    encolar(&nodos_frontera, hijo);
                }
            }
        }
    }
    if (solucionEncontrada) {
        Nodo actual = fin;
        while (actual.x != inicio.x || actual.y != inicio.y) {
            if(matriz[actual.y][actual.x] != FIN) matriz[actual.y][actual.x] = CAMINO_BFS;
            actual = padres[actual.y][actual.x];
        }
    }
    dibujarMatriz(matriz, -1, -1, solucionEncontrada ? "--- Solucion con BFS (Camino mas corto: **) ---" : "--- BFS: No se encontro solucion ---");
}

int resolverDFS_recursivo(int matriz[N][N], int visitados[N][N], Nodo actual, Nodo fin) {
    if (actual.x<0 || actual.x>=N || actual.y<0 || actual.y>=N || matriz[actual.y][actual.x]==MURO || visitados[actual.y][actual.x]) return 0;
    visitados[actual.y][actual.x] = 1;
    if (actual.x == fin.x && actual.y == fin.y) return 1;
    int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};
    for (int i = 0; i < 4; i++) {
        Nodo siguiente = {actual.x + dx[i], actual.y + dy[i]};
        if (resolverDFS_recursivo(matriz, visitados, siguiente, fin)) {
            if (matriz[actual.y][actual.x] != INICIO) matriz[actual.y][actual.x] = CAMINO_DFS;
            return 1;
        }
    }
    return 0;
}

void resolverLaberintoDFS(int matriz[N][N], Nodo inicio, Nodo fin) {
    int visitados[N][N];
    memset(visitados, 0, sizeof(visitados));
    int solucionEncontrada = resolverDFS_recursivo(matriz, visitados, inicio, fin);
    dibujarMatriz(matriz, -1, -1, solucionEncontrada ? "--- Solucion con DFS (Primer camino encontrado: ++) ---" : "--- DFS: No se encontro solucion ---");
}