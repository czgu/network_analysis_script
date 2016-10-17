#!/usr/bin/python

import sys
import collections

def parse_add_edge(content, fout):
    with open(fout, 'w') as f:
        # From given id to a compressed id
        nodes_id_map = {}
        # Store the graph as a dictionary
        nodes_graph_map = {}

        for line in content:
            if line.strip()[0] == '#':
                continue
            nodes = line.replace('\r\n', '').split('\t')
            for node in nodes:
                if node not in nodes_id_map:
                    nodes_id_map[node] = len(nodes_id_map)
                    nodes_graph_map[nodes_id_map[node]] = set()

            from_node = nodes_id_map[nodes[0]]
            to_node = nodes_id_map[nodes[1]]

            # We do not allow self edge
            if from_node == to_node:
                continue

            if from_node > to_node:
                from_node, to_node = to_node, from_node

            nodes_graph_map[from_node].add(
                to_node)

        # order the graph
        odict = collections.OrderedDict(sorted(nodes_graph_map.items()))
        for node, edges in odict.items():
            for edge in sorted(list(edges)):
                f.write(str(node) + ' ' + str(edge) + ' 1\r\n')


def main():
    fname = sys.argv[1]
    fout = sys.argv[2]
    parsed = []

    with open(fname) as f:
        content = f.readlines()
        parse_add_edge(content, fout)


    return 0

main()
