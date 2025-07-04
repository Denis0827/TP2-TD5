import matplotlib.pyplot as plt
import glob
import csv
import os
import imageio.v2 as imageio
import re

CSV_DIR = "animaciones/csv_exportados"
GIF_DIR = "animaciones/gif"

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

def extraer_numero_iteracion_csv(nombre):
    m = re.search(r'_(\d+)\.csv$', nombre)
    return int(m.group(1)) if m else -1

def extraer_numero_iteracion_png(nombre):
    m = re.search(r'_(\d+)\.png$', nombre)
    return int(m.group(1)) if m else -1

def obtener_route_ids(archivos):
    route_ids = set()
    for archivo in archivos:
        with open(archivo) as f:
            reader = csv.reader(f)
            for row in reader:
                if not row or row[0].startswith('#'):
                    continue  # Salta la línea de comentario o vacía
                if len(row) < 7:
                    continue  # Salta líneas que no tienen suficientes columnas
                route_id = int(row[6])
                route_ids.add(route_id)
    return sorted(route_ids)

def leer_rutas_csv(filename):
    rutas = []
    nodos = set()
    with open(filename) as f:
        reader = csv.reader(f)
        for row in reader:
            if row[0].startswith('#'):
                continue  # Salta la línea de comentario
            x1, y1, x2, y2, id1, id2, route_id = row
            x1, y1, x2, y2 = map(float, (x1, y1, x2, y2))
            id1, id2 = int(id1), int(id2)
            rutas.append(((x1, y1), (x2, y2), int(route_id)))
            nodos.add((x1, y1, id1))
            nodos.add((x2, y2, id2))
    return rutas, nodos

def leer_info_csv(filename):
    with open(filename) as f:
        lines = []
        for _ in range(3):
            line = f.readline()
            if line.startswith("#"):
                lines.append(line[1:].strip())
        return "\n".join(lines)
    return ""

def get_instancia_algoritmo_pairs():
    archivos = glob.glob(os.path.join(CSV_DIR, "*.csv"))
    pares = set()
    patron = re.compile(r"([A-Za-z0-9\-]+)_([A-Za-z0-9_]+)_\d+\.csv")
    for archivo in archivos:
        base = os.path.basename(archivo)
        m = patron.match(base)
        if m:
            instancia, algoritmo = m.group(1), m.group(2)
            pares.add((instancia, algoritmo))
    return sorted(pares)

def animar_todas():
    pares = get_instancia_algoritmo_pairs()
    for instancia, algoritmo in pares:
        print(f"Animando {instancia} - {algoritmo}")
        animar_rutas(instancia, algoritmo)
        crear_gif(instancia, algoritmo)

def animar_rutas(instancia, algoritmo):
    patron_csv = f"{instancia}_{algoritmo}_*.csv"
    archivos = glob.glob(os.path.join(CSV_DIR, patron_csv))
    archivos = sorted(archivos, key=lambda x: extraer_numero_iteracion_csv(x))
    if not archivos:
        print(f"No hay CSV para {instancia} - {algoritmo}")
        return
    route_ids = obtener_route_ids(archivos)
    route_id_to_color = {rid: colores[i % len(colores)] for i, rid in enumerate(route_ids)}
    all_nodos = set()
    for archivo in archivos:
        _, nodos = leer_rutas_csv(archivo)
        all_nodos.update(nodos)
    xs = [x for x, y, _ in all_nodos]
    ys = [y for x, y, _ in all_nodos]
    min_x, max_x = min(xs), max(xs)
    min_y, max_y = min(ys), max(ys)
    padding_x = (max_x - min_x) * 0.3
    padding_y = (max_y - min_y) * 0.3
    for idx, archivo in enumerate(archivos):
        plt.clf()
        rutas, nodos = leer_rutas_csv(archivo)
        for (x1, y1), (x2, y2), route_id in rutas:
            if int(route_id) != 0 and (x1 != x2 or y1 != y2):
                color = route_id_to_color.get(route_id, "#000000")
                plt.plot([x1, x2], [y1, y2], color=color, linewidth=2)
        xs = [x for x, y, _ in all_nodos]
        ys = [y for x, y, _ in all_nodos]
        plt.scatter(xs, ys, s=10, color='black', zorder=5)
        plt.axis('equal')
        plt.axis('off')
        plt.xlim(min_x - padding_x, max_x + padding_x)
        plt.ylim(min_y - padding_y, max_y + padding_y)
        plt.subplots_adjust(left=0, right=1, top=0.83, bottom=0)
        info = leer_info_csv(archivo)
        if info:
            plt.title('\n' + info, fontsize=12, loc='center', pad=5, linespacing=1.5)
        plt.gcf().set_size_inches(6, 6)
        nombre_png = f"{instancia}_{algoritmo}_{idx+1:03d}.png"
        plt.savefig(os.path.join(GIF_DIR, nombre_png))
        print(f"Imagen guardada: {os.path.join(GIF_DIR, nombre_png)}")

def crear_gif(instancia, algoritmo, duracion=3.5):
    patron_png = f"{instancia}_{algoritmo}_*.png"
    archivos = glob.glob(os.path.join(GIF_DIR, patron_png))
    archivos = sorted(archivos, key=lambda x: extraer_numero_iteracion_png(x))
    salida = os.path.join(GIF_DIR, f"{instancia}_{algoritmo}.gif")
    if not archivos:
        print(f"No se encontraron imágenes PNG para crear el GIF de {instancia} - {algoritmo}.")
        return
    imagenes = [imageio.imread(f) for f in archivos]
    imageio.mimsave(salida, imagenes, duration=duracion)
    print(f"GIF guardado como {salida}")
    # Borra los PNG y CSV de esta instancia/algoritmo
    for f in archivos:
        os.remove(f)
    patron_csv = f"{instancia}_{algoritmo}_*.csv"
    for f in glob.glob(os.path.join(CSV_DIR, patron_csv)):
        os.remove(f)
    print(f"Archivos PNG y CSV eliminados para {instancia} - {algoritmo}.")

if __name__ == "__main__":
    animar_todas()