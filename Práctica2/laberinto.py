import os
import msvcrt
import collections
import heapq
import copy

# --- Constantes ---
N = 20

# --- Tipos de Casilla (usando constantes) ---
VACIO = 0
MURO = 1
INICIO = 2
FIN = 3
CAMINO_BFS = 4
CAMINO_DFS = 5
CAMINO_ASTAR = 6

# --- Colores ANSI ---
class Colors:
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    MAGENTA = '\033[95m'
    RESET = '\033[0m'

class Laberinto:
    def __init__(self, n):
        self.n = n
        self.matriz = [[VACIO for _ in range(n)] for _ in range(n)]
        self.inicio = None
        self.fin = None
        self.cursor_x = 0
        self.cursor_y = 0

    def dibujar(self, titulo):
        os.system('cls' if os.name == 'nt' else 'clear')
        print(titulo)
        for y in range(self.n):
            linea = ""
            for x in range(self.n):
                if x == self.cursor_x and y == self.cursor_y:
                    linea += "[]"
                else:
                    casilla = self.matriz[y][x]
                    if casilla == MURO:
                        linea += "##"
                    elif casilla == INICIO:
                        linea += "IN"
                    elif casilla == FIN:
                        linea += "FN"
                    elif casilla == CAMINO_BFS:
                        linea += f"{Colors.GREEN}**{Colors.RESET}"
                    elif casilla == CAMINO_DFS:
                        linea += f"{Colors.YELLOW}++{Colors.RESET}"
                    elif casilla == CAMINO_ASTAR:
                        linea += f"{Colors.MAGENTA}@@{Colors.RESET}"
                    else:
                        linea += " ."
            print(linea)
        
        if self.cursor_x is not None:
            print("\n--- MODO EDICION ---")
            print("Usa W/A/S/D para moverte.")
            print("ESPACIO: Muro | I: Inicio | F: Fin")
            print("Q: Guardar y Resolver con todos los algoritmos.")

    def modo_edicion(self):
        self.dibujar("Construye tu laberinto")
        while True:
            tecla = msvcrt.getch().lower()
            if tecla == b'w' and self.cursor_y > 0: self.cursor_y -= 1
            elif tecla == b's' and self.cursor_y < self.n - 1: self.cursor_y += 1
            elif tecla == b'a' and self.cursor_x > 0: self.cursor_x -= 1
            elif tecla == b'd' and self.cursor_x < self.n - 1: self.cursor_x += 1
            elif tecla == b' ':
                if self.matriz[self.cursor_y][self.cursor_x] == VACIO:
                    self.matriz[self.cursor_y][self.cursor_x] = MURO
                elif self.matriz[self.cursor_y][self.cursor_x] == MURO:
                    self.matriz[self.cursor_y][self.cursor_x] = VACIO
            elif tecla == b'i':
                if self.inicio:
                    self.matriz[self.inicio[1]][self.inicio[0]] = VACIO
                self.matriz[self.cursor_y][self.cursor_x] = INICIO
                self.inicio = (self.cursor_x, self.cursor_y)
            elif tecla == b'f':
                if self.fin:
                    self.matriz[self.fin[1]][self.fin[0]] = VACIO
                self.matriz[self.cursor_y][self.cursor_x] = FIN
                self.fin = (self.cursor_x, self.cursor_y)
            elif tecla == b'q':
                if not self.inicio or not self.fin:
                    print("\nError: Debes definir un INICIO ('I') y un FIN ('F') para resolver.")
                    return False
                self.cursor_x = None # Salir del modo edición
                self.cursor_y = None
                return True
            self.dibujar("Construye tu laberinto")

    def resolver_bfs(self):
        matriz_copia = copy.deepcopy(self.matriz)
        frontera = collections.deque([self.inicio])
        visitados = {self.inicio}
        padres = {self.inicio: None}
        solucion_encontrada = False

        while frontera:
            x, y = frontera.popleft()
            if (x, y) == self.fin:
                solucion_encontrada = True
                break
            
            for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
                nx, ny = x + dx, y + dy
                if 0 <= nx < self.n and 0 <= ny < self.n and (nx, ny) not in visitados and matriz_copia[ny][nx] != MURO:
                    visitados.add((nx, ny))
                    padres[(nx, ny)] = (x, y)
                    frontera.append((nx, ny))

        if solucion_encontrada:
            actual = self.fin
            while actual != self.inicio:
                x, y = actual
                if matriz_copia[y][x] not in [INICIO, FIN]:
                    matriz_copia[y][x] = CAMINO_BFS
                actual = padres[actual]
        
        self.matriz = matriz_copia
        self.dibujar("--- Solucion con BFS (Camino mas corto: **) ---" if solucion_encontrada else "--- BFS: No se encontro solucion ---")

    def resolver_dfs(self):
        matriz_copia = copy.deepcopy(self.matriz)
        visitados = set()
        
        def dfs_recursivo(x, y):
            if not (0 <= x < self.n and 0 <= y < self.n) or (x, y) in visitados or matriz_copia[y][x] == MURO:
                return False
            
            visitados.add((x, y))
            
            if (x, y) == self.fin:
                return True

            for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
                if dfs_recursivo(x + dx, y + dy):
                    if matriz_copia[y][x] != INICIO:
                        matriz_copia[y][x] = CAMINO_DFS
                    return True
            return False

        solucion_encontrada = dfs_recursivo(self.inicio[0], self.inicio[1])
        self.matriz = matriz_copia
        self.dibujar("--- Solucion con DFS (Primer camino encontrado: ++) ---" if solucion_encontrada else "--- DFS: No se encontro solucion ---")

    def resolver_a_star(self):
        matriz_copia = copy.deepcopy(self.matriz)
        
        def manhattan(a, b):
            return abs(a[0] - b[0]) + abs(a[1] - b[1])

        frontera = [(0, self.inicio)] # (f_score, (x, y))
        padres = {self.inicio: None}
        g_score = { (x,y): float('inf') for y in range(self.n) for x in range(self.n) }
        g_score[self.inicio] = 0
        f_score = { (x,y): float('inf') for y in range(self.n) for x in range(self.n) }
        f_score[self.inicio] = manhattan(self.inicio, self.fin)
        
        solucion_encontrada = False

        while frontera:
            _, actual = heapq.heappop(frontera)
            x, y = actual

            if actual == self.fin:
                solucion_encontrada = True
                break

            for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0)]:
                vecino = (x + dx, y + dy)
                nx, ny = vecino

                if not (0 <= nx < self.n and 0 <= ny < self.n) or matriz_copia[ny][nx] == MURO:
                    continue
                
                temp_g = g_score[actual] + 1
                if temp_g < g_score[vecino]:
                    padres[vecino] = actual
                    g_score[vecino] = temp_g
                    f_score[vecino] = temp_g + manhattan(vecino, self.fin)
                    heapq.heappush(frontera, (f_score[vecino], vecino))
        
        if solucion_encontrada:
            actual = self.fin
            while actual != self.inicio:
                x, y = actual
                if matriz_copia[y][x] not in [INICIO, FIN]:
                    matriz_copia[y][x] = CAMINO_ASTAR
                actual = padres[actual]

        self.matriz = matriz_copia
        self.dibujar("--- Solucion con A* (Camino Optimo: @@) ---" if solucion_encontrada else "--- A*: No se encontro solucion ---")

if __name__ == "__main__":
    laberinto = Laberinto(N)
    
    if laberinto.modo_edicion():
        laberinto.resolver_bfs()
        input("\nPresiona Enter para ver la solucion con DFS...")
        
        laberinto_original = copy.deepcopy(laberinto.matriz) # Guardar estado original
        laberinto.resolver_dfs()
        input("\nPresiona Enter para ver la solucion con A*...")

        laberinto.matriz = laberinto_original # Restaurar para A*
        laberinto.resolver_a_star()

        print(f"{Colors.RESET}\nPrograma terminado.")