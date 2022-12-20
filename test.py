import numpy as np
from itertools import chain, combinations
from subprocess import Popen, PIPE

n = 5

low = 0
high = 100
num_tests = 10

for test in range(num_tests):
	graph = np.random.randint(low=low, high=high, size=(n, n))
	p = 0.5
	mask = np.random.choice(a=[False, True], size=(n, n), p=[p, 1-p])
	graph[mask] = 0
	graph[np.diag_indices_from(graph)] = -1
	graph = np.maximum(graph, graph.T)

	x_indices, y_indices = np.triu_indices_from(graph, 1)
	count = 0
	edges = []

	for x, y in zip(x_indices, y_indices):
	    if graph[x][y] != 0:
	        edges.append((x, y))

	program_path = "./build/main"

	p = Popen([program_path], stdout=PIPE, stdin=PIPE, shell=True)
	p.stdin.write(bytes("{} {}\n".format(n, len(edges)), 'UTF-8'))
	for edge in edges:
	    p.stdin.write(bytes("{} {} {}\n".format(edge[0] + 1, edge[1] + 1, graph[edge]), 'UTF-8'))
	num_terminals = np.random.randint(3, n)
	terminals = np.arange(1, n + 1)
	np.random.shuffle(terminals)
	terminals = sorted(terminals[:num_terminals])
	p.stdin.write(bytes("{}\n".format(num_terminals), 'UTF-8'))
	for terminal in terminals:
	    p.stdin.write(bytes("{} ".format(terminal), 'UTF-8'))
	p.stdin.flush()
	result = p.stdout.readline().strip()
	algo_weight = result.decode("utf-8").split()[-2]

	subsets = list(chain.from_iterable(combinations(edges, r) for r in range(len(edges)+1)))

	total_weight = graph[np.triu_indices_from(graph, 1)].sum()

	program_path = "./build/check_cut"

	min_weight = total_weight

	for subset in subsets:
	    if len(subset) == 0:
	        continue
	    p = Popen([program_path], stdout=PIPE, stdin=PIPE, shell=True)
	    p.stdin.write(bytes("{} {}\n".format(n, len(subset)), 'UTF-8'))
	    weight = 0
	    for edge in subset:
	        p.stdin.write(bytes("{} {} {}\n".format(edge[0] + 1, edge[1] + 1, graph[edge]), 'UTF-8'))
	        weight += graph[edge]
	    p.stdin.write(bytes("{}\n".format(num_terminals), 'UTF-8'))
	    for terminal in terminals:
	        p.stdin.write(bytes("{} ".format(terminal), 'UTF-8'))
	    p.stdin.flush()
	    result = p.stdout.readline().strip()
	    if result == b"1":
	        if total_weight - weight < min_weight:
	            min_weight = total_weight - weight
	if int(algo_weight) <= 2 * min_weight:
		print("Test {} passed".format(test + 1))
	else:
		print("Test {} failed!".format(test + 1))