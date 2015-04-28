import matplotlib.pyplot as plt
import networkx as nx
import os

f = open('network.txt', 'r')
tmp = f.readline()
f.close()
size = tmp.split(' ')[1]
root = [int(tmp.split(' ')[2])]
print "SIZE : ", size

nodes = []
for i in range(0,int(size)):
  nodes.append(i) 

print "NODE: " ,nodes

start = False
edges = []
count = 0
with open('network.txt') as fp:
    for line in fp:

        if (start and count < int(size)-1):
          tmp = line.strip()
          tmp = tmp.split(',')
          a1 = int(tmp[0])
          a2 = int(tmp[1])
          tmp = (a1,a2)
          edges.append(tmp)
          count += 1

        if ( '--edges' in line):
          start = True
          size = line.split(' ')[1]
          print "SIZE of edges : ", size

print "EDGES : ", edges       

edgeLabels = {}
edgeLabelsList = []
start = False
count = 0
with open('network.txt') as fp:
    for line in fp:

        if (start and count < int(size)-1):
          tmp = line.strip()
          tmp = int(tmp)
          edgeLabelsList.append(tmp)
          count += 1

        if ( '--weight' in line):
          start = True
          size = line.split(' ')[1]
          print "SIZE of edges weight: ", size


#Mapping edges and labels
i = 0
for e in edges:
  print e
  edgeLabels[e] = edgeLabelsList[i]
  i += 1

print "EDGES LABELS: ", edgeLabels 

#nodes = [0,1,2,3,4,5,6,7,8,9,10]
#edges = [(0,1), (1,2), (3,1), (2,3),(7,8),(9,10),(2,8)]



G = nx.DiGraph()

G.add_nodes_from(nodes)
G.add_edges_from(edges)

position = nx.random_layout(G)

nx.draw_networkx_nodes(G,position, nodelist=nodes, node_color="y")
nx.draw_networkx_nodes(G,position, nodelist=root, node_color="r")

nx.draw_networkx_edges(G,position)
nx.draw_networkx_labels(G,position)
nx.draw_networkx_edge_labels(G,position,edge_labels=edgeLabels)

#plt.axis('off')
plt.show()

 