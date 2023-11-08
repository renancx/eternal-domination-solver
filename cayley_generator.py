from sage.all import *
from sage.graphs.connectivity import is_connected
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
while test <= last_test:
    nb_verts = random.randrange(min_nb_verts, (max_nb_verts + 1))
    perc_edges = random.uniform(min_perc_edges, max_perc_edges)

    g = graphs.RandomGNP(nb_verts, perc_edges)
    if g.is_cayley():
        write_test_to_file(g, f"{test:02}.in")
        g.plot().save(f"{test:02}_in.png")

        test += 1
