#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Estructura para devolver el resultado
typedef struct {
    double x;
    double y;
    double cost;
} Solution;

// 1. Definir la función de Himmelblau
double himmelblau(double x, double y) {
    // f(x, y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2
    double term1 = pow(x*x + y - 11, 2);
    double term2 = pow(x + y*y - 7, 2);
    return term1 + term2;
}

// Función auxiliar para generar un double aleatorio en un rango [min, max]
double random_double(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// 2. Implementar el algoritmo de Recocido Simulado
Solution simulated_annealing(double domain_min, double domain_max, double initial_temp, double final_temp, double alpha) {
    // Iniciar en un punto aleatorio dentro del dominio
    double current_x = random_double(domain_min, domain_max);
    double current_y = random_double(domain_min, domain_max);
    double current_cost = himmelblau(current_x, current_y);

    // Guardar la mejor solución encontrada
    double best_x = current_x;
    double best_y = current_y;
    double best_cost = current_cost;

    double temp = initial_temp;

    // Bucle principal
    while (temp > final_temp) {
        // Iterar varias veces a la misma temperatura
        for (int i = 0; i < 100; i++) {
            // Generar un vecino cercano
            double step_size = (domain_max - domain_min) * (temp / initial_temp);
            double new_x = current_x + random_double(-step_size, step_size);
            double new_y = current_y + random_double(-step_size, step_size);

            // Asegurarse de que el nuevo punto esté dentro del dominio
            if (new_x > domain_max) new_x = domain_max;
            if (new_x < domain_min) new_x = domain_min;
            if (new_y > domain_max) new_y = domain_max;
            if (new_y < domain_min) new_y = domain_min;
            
            double new_cost = himmelblau(new_x, new_y);
            double cost_diff = new_cost - current_cost;

            // Decidir si aceptar el nuevo punto
            if (cost_diff < 0 || random_double(0, 1) < exp(-cost_diff / temp)) {
                current_x = new_x;
                current_y = new_y;
                current_cost = new_cost;

                // Actualizar la mejor solución si es necesario
                if (current_cost < best_cost) {
                    best_x = current_x;
                    best_y = current_y;
                    best_cost = current_cost;
                }
            }
        }
        // Enfriar la temperatura
        temp *= alpha;
    }
    
    Solution result = {best_x, best_y, best_cost};
    return result;
}

// --- Programa Principal ---
int main() {
    // Inicializar la semilla para números aleatorios
    srand((unsigned int)time(NULL));

    // Dominio de búsqueda
    const double SEARCH_DOMAIN_MIN = -5.0;
    const double SEARCH_DOMAIN_MAX = 5.0;

    // Hiperparámetros del algoritmo
    const double INITIAL_TEMPERATURE = 10000.0;
    const double FINAL_TEMPERATURE = 1e-5;
    const double COOLING_RATE = 0.995;

    printf("Optimizando la funcion de Himmelblau con Recocido Simulado...\n");

    // Ejecutar el algoritmo
    Solution result = simulated_annealing(
        SEARCH_DOMAIN_MIN,
        SEARCH_DOMAIN_MAX,
        INITIAL_TEMPERATURE,
        FINAL_TEMPERATURE,
        COOLING_RATE
    );

    printf("\n--- Resultado ---\n");
    printf("Coordenadas encontradas (x, y): (%.6f, %.6f)\n", result.x, result.y);
    printf("Valor minimo de la funcion f(x, y): %.6f\n", result.cost);
    printf("\nNota: La funcion de Himmelblau tiene 4 minimos globales. Ejecuta el programa de nuevo para encontrar potencialmente otro.\n");

    return 0;
}