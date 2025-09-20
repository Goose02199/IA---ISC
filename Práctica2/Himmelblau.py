import math
import random

# 1. Definir la función de Himmelblau
def himmelblau(x, y):
    """
    Calcula el valor de la función de Himmelblau para las coordenadas (x, y).
    f(x, y) = (x^2 + y - 11)^2 + (x + y^2 - 7)^2
    """
    term1 = (x**2 + y - 11)**2
    term2 = (x + y**2 - 7)**2
    return term1 + term2

# 2. Implementar el algoritmo de Recocido Simulado
def simulated_annealing(domain, initial_temp, final_temp, alpha):
    """
    Optimiza la función de Himmelblau usando recocido simulado.

    Args:
        domain (tuple): El rango de búsqueda para x e y, ej: (-5, 5).
        initial_temp (float): La temperatura inicial.
        final_temp (float): La temperatura final a la que se detiene el algoritmo.
        alpha (float): La tasa de enfriamiento (cooling rate).
    """
    # Iniciar en un punto aleatorio dentro del dominio
    current_x = random.uniform(domain[0], domain[1])
    current_y = random.uniform(domain[0], domain[1])
    current_cost = himmelblau(current_x, current_y)
    
    # Guardar la mejor solución encontrada hasta ahora
    best_x, best_y, best_cost = current_x, current_y, current_cost
    
    temp = initial_temp

    # Bucle principal: continuar mientras la temperatura sea alta
    while temp > final_temp:
        # Iterar varias veces a la misma temperatura para estabilizar
        for i in range(100):
            # Generar un vecino cercano al punto actual
            # El tamaño del paso se reduce a medida que baja la temperatura
            step_size = (domain[1] - domain[0]) * (temp / initial_temp)
            new_x = current_x + random.uniform(-step_size, step_size)
            new_y = current_y + random.uniform(-step_size, step_size)

            # Asegurarse de que el nuevo punto esté dentro del dominio
            new_x = max(min(new_x, domain[1]), domain[0])
            new_y = max(min(new_y, domain[1]), domain[0])
            
            new_cost = himmelblau(new_x, new_y)
            
            # Calcular la diferencia de costo (energía)
            cost_diff = new_cost - current_cost

            # Decidir si aceptar el nuevo punto
            # Siempre se acepta si es una mejor solución (cost_diff < 0)
            # A veces se acepta si es una peor solución, con una probabilidad
            if cost_diff < 0 or random.random() < math.exp(-cost_diff / temp):
                current_x, current_y, current_cost = new_x, new_y, new_cost
                
                # Actualizar la mejor solución si es necesario
                if current_cost < best_cost:
                    best_x, best_y, best_cost = current_x, current_y, current_cost

        # Enfriar la temperatura
        temp *= alpha
        
    return best_x, best_y, best_cost

# --- Programa Principal ---
if __name__ == "__main__":
    # Dominio de búsqueda para x e y
    SEARCH_DOMAIN = (-5, 5)
    
    # Hiperparámetros del algoritmo
    INITIAL_TEMPERATURE = 10000.0
    FINAL_TEMPERATURE = 1e-5
    COOLING_RATE = 0.995  # Un enfriamiento lento suele dar mejores resultados

    print("Optimizando la funcion de Himmelblau con Recocido Simulado...")
    
    # Ejecutar el algoritmo
    found_x, found_y, min_value = simulated_annealing(
        SEARCH_DOMAIN, 
        INITIAL_TEMPERATURE, 
        FINAL_TEMPERATURE, 
        COOLING_RATE
    )

    print("\n--- Resultado ---")
    print(f"Coordenadas encontradas (x, y): ({found_x:.6f}, {found_y:.6f})")
    print(f"Valor minimo de la funcion f(x, y): {min_value:.6f}")
    
    print("\nNota: La funcion de Himmelblau tiene 4 minimos globales. Ejecuta el programa de nuevo para encontrar potencialmente otro.")