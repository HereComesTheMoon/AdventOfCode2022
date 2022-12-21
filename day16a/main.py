import matplotlib.pyplot as plt
import networkx as nx

def read():
    with open('./../data/16.txt') as f:
        lines = f.readlines()
        nodes = []
        edges = []
        for row in lines:
            name = row[6:8]
            flow = int(row[row.index('=')+1:row.index(';')]);
            tail = row[row.find("valve") + 6:]
            close = [x.strip() for x in tail.split(',')]

            # inp.append((name, flow, edges))
            nodes.append((name, {"flow": flow}))
            edges.extend((name, x) for x in close)

        return nodes, edges

# def build_graph(inp: list[tuple[str, int, list[str]]]):


if __name__ == '__main__':
    nodes, edges = read()

    G = nx.Graph()
    G.add_nodes_from(nodes)
    G.add_edges_from(edges)

    # Nonzero flow subset
    # H = [n if d['flow'] != 0 else for n, d in nodes]
    sub = list(filter(lambda l: l[1]['flow'] != 0, nodes))
    sub = [n for n, d in sub]
    print(sub)

    pos = nx.spring_layout(G, seed=13538314, k = 0.19)
    # pos = nx.planar_layout(G) # Not planar!
    nx.draw(G, pos=pos, node_size=200)

    nx.draw_networkx_nodes(G, pos, sub, node_color="tab:red", node_size=200)
    nx.draw_networkx_nodes(G, pos, ['AA'], node_color="tab:green", node_size=200)
    nx.draw_networkx_labels(G, pos, font_size=8)
    plt.savefig('figure.pdf')
