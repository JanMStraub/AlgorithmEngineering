import networkx as nx
import matplotlib.pyplot as plt

# Define the edges and their weights
edges = [
    (1, 3, 2),
    (1, 2, 1),
    (2, 3, 1),
    (2, 5, -2),
    (2, 4, 1),
    (3, 1, 2),
    (3, 2, 1),
    (3, 4, 2),
    (3, 5, 1),
    (4, 2, 1),
    (4, 3, 2),
    (4, 6, 2),
    (4, 7, 3),
    (5, 3, 1),
    (5, 4, 1),
    (6, 4, 2),
    (6, 7, 9),
    (7, 1, -10),
    (7, 6, 9),
]

# Create a undirected graph
G = nx.Graph()

# Add edges with weights to the graph
for edge in edges:
    G.add_edge(edge[0], edge[1], weight=edge[2])

# Draw the graph
pos = nx.spring_layout(G)
nx.draw(
    G,
    pos,
    with_labels=True,
    node_size=700,
    node_color="skyblue",
    font_size=10,
    font_weight="bold",
)
edge_labels = nx.get_edge_attributes(G, "weight")
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)

# Show the plot
plt.title("Graph with Weights")
plt.show()
