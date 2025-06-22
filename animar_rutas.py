import matplotlib.pyplot as plt
import glob
import csv
import os
import imageio

colores = [
    "#FF0000",  # Rojo
    "#0000FF",  # Azul
    "#006400",  # Verde oscuro
    "#FFA500",  # Naranja
    "#FF69B4",  # Rosa
    "#00CED1",  # Celeste
    "#8B4513",  # Marrón
    "#8A2BE2",  # Violeta
]

def obtener_route_ids(archivos):
    route_ids = set()
    for archivo in archivos:
        with open(archivo) as f:
            reader = csv.reader(f)
            for row in reader:
                route_id = int(row[6])
                route_ids.add(route_id)
    return sorted(route_ids)

def leer_rutas_csv(filename):
    rutas = []
    nodos = set()
    with open(filename) as f:
        reader = csv.reader(f)
        for row in reader:
            x1, y1, x2, y2, id1, id2, route_id = row
            x1, y1, x2, y2 = map(float, (x1, y1, x2, y2))
            id1, id2 = int(id1), int(id2)
            rutas.append(((x1, y1), (x2, y2), int(route_id)))
            nodos.add((x1, y1, id1))
            nodos.add((x2, y2, id2))
    return rutas, nodos

def animar_rutas():
    archivos = sorted(glob.glob("rutas_*.csv"))
    route_ids = obtener_route_ids(archivos)
    route_id_to_color = {rid: colores[i % len(colores)] for i, rid in enumerate(route_ids)}

    # Junta todos los nodos de todos los archivos para posiciones y límites fijos
    all_nodos = set()
    for archivo in archivos:
        _, nodos = leer_rutas_csv(archivo)
        all_nodos.update(nodos)
    xs = [x for x, y, _ in all_nodos]
    ys = [y for x, y, _ in all_nodos]
    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)
    padding_x = (max_x - min_x) * 0.3  # Más margen horizontal
    padding_y = (max_y - min_y) * 0.3  # Más margen vertical

    for idx, archivo in enumerate(archivos):
        plt.clf()
        rutas, nodos = leer_rutas_csv(archivo)
        # Dibuja las aristas de las rutas
        for (x1, y1), (x2, y2), route_id in rutas:
            if int(route_id) != 0 and (x1 != x2 or y1 != y2):
                color = route_id_to_color.get(route_id, "#000000")
                plt.plot([x1, x2], [y1, y2], color=color, linewidth=2)
        # Dibuja todos los nodos SIEMPRE en la misma posición, solo como puntitos pequeños
        xs = [x for x, y, _ in all_nodos]
        ys = [y for x, y, _ in all_nodos]
        plt.scatter(xs, ys, s=10, color='black', zorder=5)
        plt.axis('equal')
        plt.axis('off')
        plt.xlim(min_x - padding_x, max_x + padding_x)
        plt.ylim(min_y - padding_y, max_y + padding_y)
        plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
        plt.savefig(f"rutas_{idx:03d}.png", bbox_inches='tight', pad_inches=0)
        print(f"Imagen guardada: rutas_{idx:03d}.png")

def crear_gif(desde="rutas_*.png", salida="rutas.gif", duracion=1.5):
    archivos = sorted(glob.glob(desde))
    imagenes = [imageio.imread(f) for f in archivos]
    imageio.mimsave(salida, imagenes, duration=duracion)
    print(f"GIF guardado como {salida}")
    # Borra los PNG y CSV
    for f in archivos:
        os.remove(f)
    for f in glob.glob("rutas_*.csv"):
        os.remove(f)
    print("Archivos PNG y CSV eliminados.")

if __name__ == "__main__":
    animar_rutas()
    crear_gif()