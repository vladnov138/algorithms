import heapq

import matplotlib.pyplot as plt
import networkx as nx
from networkx.drawing.nx_pydot import graphviz_layout
import json
import shapely
import math

def get_sp_tree(graph):
    edges, nodes = [], []
    tree = nx.Graph()
    nodes.append(list(graph.nodes)[0])
    for _ in graph.nodes:
        connected_edges = filter(lambda edge: edge not in nodes and edge[1] not in nodes, graph.edges(nodes))
        try:
            min_edge = min(connected_edges, key=lambda edge: graph.get_edge_data(*edge)["weight"])
        except ValueError:
            continue
        edges.append(min_edge + (graph.get_edge_data(*min_edge)["weight"],))
        nodes.append(min_edge[1])
    tree.add_nodes_from(nodes)
    tree.add_weighted_edges_from(edges)
    return

def get_span_tree_by_heapq(graph):
    edges, nodes = [], []
    tree = nx.Graph()
    start_node = list(graph.nodes)[0]
    nodes.append(start_node)
    connected_edges = [(start_node, new_node, graph.get_edge_data(start_node, new_node)['weight'])
                                      for new_node in graph.neighbors(start_node)]
    heapq.heapify(connected_edges)
    while connected_edges:
        min_edge = heapq.heappop(connected_edges)
        if min_edge[1] not in nodes:
            edges.append(min_edge)
            nodes.append(min_edge[1])
            for node in graph.neighbors(min_edge[1]):
                if node not in nodes:
                    edge = (min_edge[1], node, graph.get_edge_data(min_edge[1], node)['weight'])
                    heapq.heappush(connected_edges, edge)
    tree.add_weighted_edges_from(edges)
    tree.add_nodes_from(nodes)
    return tree

def get_clusters(spanning_tree: nx.Graph, n_clusters: int):
    """Splits the tree into n_clusters clusters"""
    # Sort edges of the spanning_tree
    sorted_edges = sorted(spanning_tree.edges(data=True), key=lambda x: x[2]['weight'], reverse=True)
    copy_tree = spanning_tree.copy()
    for i in range(n_clusters - 1):
        if len(sorted_edges) > i:
            copy_tree.remove_edge(*sorted_edges[i][:2])
    return list(nx.connected_components(copy_tree))

def draw_graph(graph):
    pos = nx.spring_layout(graph)
    plt.figure(figsize=(7,6))
    nx.draw_networkx_nodes(graph, pos)
    nx.draw_networkx_edges(graph, pos, graph.edges)
    # nx.draw_networkx_labels(graph, pos, font_size = 20)
    plt.show()

def distance(site1, site2):
    loc1 = site1["location"]
    loc2 = site2["location"]
    r = 6371
    phi1 = math.radians(loc1["lat"])
    phi2 = math.radians(loc2["lat"])
    dphi = math.radians(loc2["lat"] - loc1["lat"])
    dlambda = math.radians(loc2["lon"] - loc1["lon"])
    a = math.sin(dphi/2) ** 2 + math.cos(phi1) * math.cos(phi2) * math.sin(dlambda/2) ** 2
    c = 2 * math.atan2(a ** 0.5, (1-a)**0.5)
    return r * c

def make_graph(sites, max_distance = 400):
    edges = []
    nodes = []
    for i, site in enumerate(sites[:-1]):
        # print(f"{i+1}/{len(sites)-1}")
        nodes.append(site["code"])
        min_distance = 10 ** 16
        min_dest = None
        min_dest_flag = True
        for dest in sites[i+1:]:
            d = distance(site, dest)
            if d <= max_distance:
                edges.append((site["code"], dest["code"], d))
                min_dest_flag = False
            if d < min_distance:
                min_dest = dest
                min_distance = d
        if min_dest_flag:
            edges.append((site["code"], dest["code"], min_distance))
    return edges, nodes

with open("base.json", "r") as f:
    sites = json.load(f)
    # print(len(sites))
    lons = []
    lats = []
    for site in sites:
        lons.append(site["location"]["lon"])
        lats.append(site["location"]["lat"])

with open("continents.json", "r") as f:
    continents = json.load(f)
    north_america_polygon = continents["features"][1]["geometry"]["coordinates"]
# print(north_america_polygon[0][0])

max_polygon = []
max_area = 0

for i in range(len(north_america_polygon)):
    points = north_america_polygon[i][0]
    area = shapely.Polygon(points).area
    if area > max_area:
        max_polygon = points
        max_area = area

xs, ys = zip(*max_polygon)

north_america = shapely.Polygon(max_polygon)
north_america_sites = []
for site in sites:
    p = shapely.Point((site["location"]["lon"]), (site["location"]["lat"]))
    if north_america.contains(p):
        north_america_sites.append(site)

flons = []
flats = []
for site in north_america_sites:
    flons.append(site["location"]["lon"])
    flats.append(site["location"]["lat"])

edges, nodes = make_graph(north_america_sites)
graph = nx.Graph()
graph.add_nodes_from(nodes)
graph.add_weighted_edges_from(edges)

my_span_tree = get_span_tree_by_heapq(graph)
span_tree = nx.minimum_spanning_tree(graph)
print("My function: ", my_span_tree)
print("Networkx function: ", span_tree)
clusters = get_clusters(my_span_tree, 40)
# print(clusters)
print(len(clusters))


