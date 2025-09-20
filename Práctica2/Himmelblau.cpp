#include <iostream>
#include <cmath>
#include <random>
#include <algorithm> // Para std::min y std::max

// Estructura para devolver el resultado
struct Solution {
    double x;
    double y;
    double cost;
};

// 1. Definir la función de Himmelblau
double himmelblau(double x, double y) {
    // f(x, y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2
    double term1 = std::pow(x*x + y - 11, 2);
    double term2 = std::pow(x + y*y - 7, 2);
    return term1 + term2;
}

// 2. Implementar el algoritmo de Recocido Simulado
Solution simulated_annealing(double domain_min, double domain_max, double initial_temp, double final_temp, double alpha) {
    // Motor de números aleatorios de alta calidad
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Iniciar en un punto aleatorio dentro del dominio
    double current_x = domain_min + dis(gen) * (domain_max - domain_min);
    double current_y = domain_min + dis(gen) * (domain_max - domain_min);
    double current_cost = himmelblau(current_x, current_y);

    // Guardar la mejor solución encontrada
    double best_x = current_x;
    double best_y = current_y;
    double best_cost = current_cost;

    double temp = initial_temp;

    // Bucle principal
    while (temp > final_temp) {
        // Iterar varias veces a la misma temperatura
        for (int i = 0; i < 100; ++i) {
            // Generar un vecino cercano
            double step_size = (domain_max - domain_min) * (temp / initial_temp);
            double new_x = current_x + (dis(gen) * 2 - 1) * step_size;
            double new_y = current_y + (dis(gen) * 2 - 1) * step_size;

            // Asegurarse de que el nuevo punto esté dentro del dominio
            new_x = std::max(domain_min, std::min(new_x, domain_max));
            new_y = std::max(domain_min, std::min(new_y, domain_max));

            double new_cost = himmelblau(new_x, new_y);
            double cost_diff = new_cost - current_cost;
            
            // Decidir si aceptar el nuevo punto
            if (cost_diff < 0 || dis(gen) < std::exp(-cost_diff / temp)) {
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
    
    return {best_x, best_y, best_cost};
}

// --- Programa Principal ---
int main() {
    // Dominio de búsqueda
    const double SEARCH_DOMAIN_MIN = -5.0;
    const double SEARCH_DOMAIN_MAX = 5.0;
    
    // Hiperparámetros del algoritmo
    const double INITIAL_TEMPERATURE = 10000.0;
    const double FINAL_TEMPERATURE = 1e-5;
    const double COOLING_RATE = 0.995;

    std::cout << "Optimizando la funcion de Himmelblau con Recocido Simulado..." << std::endl;

    // Ejecutar el algoritmo
    Solution result = simulated_annealing(
        SEARCH_DOMAIN_MIN,
        SEARCH_DOMAIN_MAX,
        INITIAL_TEMPERATURE,
        FINAL_TEMPERATURE,
        COOLING_RATE
    );

    // Configurar la precisión de salida para std::cout
    std::cout.precision(6);
    std::cout << std::fixed;

    std::cout << "\n--- Resultado ---" << std::endl;
    std::cout << "Coordenadas encontradas (x, y): (" << result.x << ", " << result.y << ")" << std::endl;
    std::cout << "Valor minimo de la funcion f(x, y): " << result.cost << std::endl;
    std::cout << "\nNota: La funcion de Himmelblau tiene 4 minimos globales. Ejecuta el programa de nuevo para encontrar potencialmente otro." << std::endl;

    return 0;
}