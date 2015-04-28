'''
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

G = nx.DiGraph()
G.add_edges_from(
    [('A', 'B'), ('A', 'C'), ('D', 'B'), ('E', 'C'), ('E', 'F'),
     ('B', 'H'), ('B', 'G'), ('B', 'F'), ('C', 'G')],weight=1)

val_map = {'A': 1.0,
           'D': 0.5714285714285714,
           'H': 0.0}

values = [val_map.get(node, 0.25) for node in G.nodes()]

# Specify the edges you want here
red_edges = [('A', 'C'), ('E', 'C')]
edge_colours = ['black' if not edge in red_edges else 'red'
                for edge in G.edges()]
black_edges = [edge for edge in G.edges() if edge not in red_edges]

# Need to create a layout when doing
# separate calls to draw nodes and edges
pos = nx.spring_layout(G)
nx.draw_networkx_nodes(G, pos, cmap=plt.get_cmap('jet'), node_color = values)
nx.draw_networkx_edges(G, pos, edgelist=red_edges, edge_color='r', arrows=True)
nx.draw_networkx_edges(G, pos, edgelist=black_edges, arrows=True)
plt.show()
'''
'''
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
import pylab
import random

G = nx.DiGraph()

G.add_edges_from([('A', 'B'),('C','D'),('G','D')], weight=1)
G.add_edges_from([('D','A'),('D','E'),('B','D'),('D','E')], weight=2)
G.add_edges_from([('B','C'),('E','F')], weight=3)
G.add_edges_from([('C','F')], weight=4)


val_map = {'A': 1.0,
                   'D': 0.5714285714285714,
                              'H': 0.0}

values = [val_map.get(node, 0.25) for node in G.nodes()]
#values = [(random(), random(), random()) for _i in range(10)]
edge_labels=dict([((u,v,),d['weight'])
                 for u,v,d in G.edges(data=True)])
red_edges = [('C','D'),('D','A')]
edge_colors = ['black' if not edge in red_edges else 'red' for edge in G.edges()]

pos=nx.spring_layout(G)
nx.draw_networkx_edge_labels(G,pos,edge_labels=edge_labels)
nx.draw(G,pos, node_color = values, node_size=1500,edge_color=edge_colors,edge_cmap=plt.cm.Reds)
pylab.show()
'''
import matplotlib.pyplot as plt
import networkx as nx

nodes = [0,1,2,3,4,5,6,7,8,9,10]
edges = [(0,1), (1,2), (3,1), (2,3),(7,8),(9,10),(2,8)]
#edges = [([0,1],[4,5] ),([1,2],[5,2]),([7,8],[9,10]),([7,2],[1,8])]
nodeListA = [0,1,5]
nodeListB = [2,3,4]  
nodeListC = [6,7,8,9,10]  

edgeLabels = {}

G = nx.DiGraph()

G.add_nodes_from(nodes)
G.add_edges_from(edges)

for e in edges:
  print e
  edgeLabels[e] = 34
  

position = nx.circular_layout(G)

nx.draw_networkx_nodes(G,position, nodelist=nodes, node_color="y")
#nx.draw_networkx_nodes(G,position, nodelist=nodeListB, node_color="r")
#nx.draw_networkx_nodes(G,position, nodelist=nodeListC, node_color="y")

nx.draw_networkx_edges(G,position)
nx.draw_networkx_labels(G,position)
nx.draw_networkx_edge_labels(G,position,edge_labels=edgeLabels)

plt.show()
 