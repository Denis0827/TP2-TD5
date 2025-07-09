import pandas as pd
import matplotlib.pyplot as plt
import os

# Obtener la ruta de la carpeta csv_exportados relativa al script
carpeta = os.path.join(os.path.dirname(__file__), 'csv_exportados')
# Carpeta de salida para los gráficos
output_dir = os.path.join(os.path.dirname(__file__), 'graficos')
os.makedirs(output_dir, exist_ok=True)

def graficar_algoritmos(algoritmos, tipo='costo'):
    """
    Grafica la métrica (tiempo o costo) por instancia para los algoritmos dados.
    Si una solución no es factible (Factibilidad == 0), se marca con punto rojo,
    pero la línea se mantiene continua para todo el algoritmo.
    """

    global carpeta
    global output_dir

    # Colores asignados por algoritmo (sin rojo)
    colores_disponibles = [
        'blue', 'green', 'orange', 'purple', 'brown',
        'pink', 'gray', 'olive', 'cyan', 'magenta',
        'teal', 'gold'  # nuevos colores
    ]
    color_map = {alg: colores_disponibles[i % len(colores_disponibles)] for i, alg in enumerate(algoritmos)}

    # Cargar datos desde CSVs
    dataframes = []
    for archivo in os.listdir(carpeta):
        if archivo.endswith('.csv'):
            instancia = archivo.replace('.csv', '')
            ruta = os.path.join(carpeta, archivo)
            df_temp = pd.read_csv(ruta)
            df_temp.columns = df_temp.columns.str.strip()  # Elimina espacios antes/después de los nombres

            df_temp['Instancia'] = instancia
            dataframes.append(df_temp)

    df_total = pd.concat(dataframes, ignore_index=True)
    df_filtrado = df_total[df_total['Algoritmo'].isin(algoritmos)]

    # Elegir eje Y
    if tipo == 'tiempo':
        eje_y = 'Tiempo_us'
        etiqueta_y = 'Tiempo de ejecución (us)'
    else:
        eje_y = 'Distancia'
        etiqueta_y = 'Distancia Total'

    # Crear gráfico
    plt.figure(figsize=(12, 6))

    for algoritmo in algoritmos:
        df_alg = df_filtrado[df_filtrado['Algoritmo'] == algoritmo]
        color = color_map[algoritmo]
        df_alg = df_alg.sort_values(by='Instancia')
        plt.plot(df_alg['Instancia'], df_alg[eje_y], '-', label=algoritmo, color=color)
        df_nofact = df_alg[df_alg['Factibilidad'] == 0]
        plt.scatter(df_nofact['Instancia'], df_nofact[eje_y], color='red', zorder=5)

    plt.xlabel('Instancia')
    plt.ylabel(etiqueta_y)
    plt.title(f'{etiqueta_y} por instancia para algoritmos seleccionados')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()

    # Guardar el gráfico en la carpeta 'graficos'
    algs_str = '_'.join([alg.replace('+', '').replace(' ', '') for alg in algoritmos])
    filename = f"{algs_str}_{tipo}.png"
    filepath = os.path.join(output_dir, filename)
    plt.savefig(filepath)
    plt.close()

def graficar_lote():
    global carpeta
    global output_dir
    
    graficos = [
        {
            "titulo": "Comparación de heurísticas golosas en función de distancia total, algoritmos CW y NN",
            "algoritmos": ["CW", "NN"],
            "tipo": "costo"
        },
        {
            "titulo": "Comparación de heurísticas golosas en función de tiempo de ejecución, algoritmos CW y NN",
            "algoritmos": ["CW", "NN"],
            "tipo": "tiempo"
        },
        {
            "titulo": "Comparación de First improvement, Best improvements y operadores locales en función de distancia total",
            "algoritmos": ["NN+SW_First", "NN+SW_Best", "NN+RL_First", "NN+RL_Best"],
            "tipo": "costo"
        },
        {
            "titulo": "Comparación de First/Best improvement en operadores locales en función de tiempo de ejecución",
            "algoritmos": ["NN+SW_First", "NN+SW_Best"],
            "tipo": "tiempo"
        },
        {
            "titulo": "Operadores con Clarke Wright en función de distancia total",
            "algoritmos": ["CW+SW_Best", "CW+RL_Best"],
            "tipo": "costo"
        },
        {
            "titulo": "Operadores con Clarke Wright",
            "algoritmos": ["CW+SW_Best", "CW+RL_Best"],
            "tipo": "tiempo"
        },
        {
            "titulo": "NN distancia total",
            "algoritmos": ["NN+SW_Best+RL_Best", "NN+RL_Best+SW_Best", "NN+SW_Best", "NN+RL_Best"],
            "tipo": "costo"
        },
        {
            "titulo": "NN con tiempo de ejecución",
            "algoritmos": ["NN+SW_Best+RL_Best", "NN+RL_Best+SW_Best", "NN+SW_Best", "NN+RL_Best"],
            "tipo": "tiempo"
        },
        {
            "titulo": "Final con distancia total",
            "algoritmos": ["CW", "NN", "NN+SW_Best", "NN+RL_Best", "GRASP+SW_Best", "GRASP+RL_Best"],
            "tipo": "costo"
        },
        {
            "titulo": "Final con tiempo de ejecución",
            "algoritmos": ["CW", "NN", "NN+SW_Best", "NN+RL_Best", "GRASP+SW_Best", "GRASP+RL_Best"],
            "tipo": "tiempo"
        }
    ]

    for g in graficos:
        print(f"Generando gráfico: {g['titulo']}")
        graficar_algoritmos(
            algoritmos=g['algoritmos'],
            tipo=g['tipo']
        )

# Llamada
graficar_lote()