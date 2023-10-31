from sage.all import *
import random

def write_test_to_file(g, filename):
    with open(filename, "w") as output_file:
        output_file.write(f"p edge {g.order()} {g.size()}\n")
        for u, v, l in g.edges():
            output_file.write(f"e {u} {v}\n")

first_test = int(input())
last_test = int(input())
min_nb_verts = int(input())
max_nb_verts = int(input())
min_perc_edges = float(input())
max_perc_edges = float(input())

test = first_test
generated_graphs = []

while test <= last_test:
    nb_verts = random.randrange(min_nb_verts, (max_nb_verts + 1))
    perc_edges = random.uniform(min_perc_edges, max_perc_edges)

    grid_dim_x = random.randint(3, 13)
    grid_dim_y = random.randint(1, 12)
    grid_dim_z = random.randint(2, 19)

    # Create a 3D grid graph with nb_verts vertices
    #grid_dim = int(round(nb_verts ** (1/3)))
    g = graphs.GridGraph([grid_dim_x, grid_dim_y, grid_dim_z])

    g.relabel([i+1 for i in range(g.order())])

    write_test_to_file(g, f"{test:02}.in")
    g.plot().save(f"{test:02}_in.png")

    test += 1
