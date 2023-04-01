import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from typing import List
import sys
sys.path.append("F:/git/tgraph/Lab1")
#Tree implement
class node:
    ''' node attribute'''
    data: any
    parent: None
    children: None
    '''instance attribute'''
def __init__(self, data = None, parent = None, children = []):
    self.data = data
    self.parent = parent
    self.children = children
def printTree(self):
    print(self.data)

def negative_cycle(G, dist):
    for src in range (0, len(G)):
        for des in range (0, len(G)):
            if (dist[des] > dist[src] + G[src][des]):
                return False
    return True

def min_index(d):
    min = d[0]
    res = 0
    for i in range (len(d)):
        if (d[i] < min):
            min = d[i]
            res = i
    return res



'''for non-negative weight only'''
def Dijsktra(G, src:int):
    '''generate queue, dist and prep'''
    q = []
    d = []
    pr = []
    for i in range (len(G)):
        d.append(float('inf'))
        pr.append(None)
        q.append(i)
    d[src] = 0
    '''iterate'''
    while (len(q)):
        '''find min d(u) in Q'''
        curr = q[0]
        for i in q:
            if (d[i] < d[curr]):
                curr = i
        q.remove(curr)
        for other in q:
            if (G[curr][other] > 0):
                if (d[other] > d[curr] + G[curr][other]):
                    d[other] = d[curr] + G[curr][other]
                    pr[other] = curr
    return [d,pr]

def BellmanFord(G, src:int):
    '''dist init'''
    dist = []
    prev = []
    for i in range (len(G)):
        dist.append(float('inf'))
        prev.append(None)
    dist[src] = 0
    
    '''update distance'''
    for k in range (len(G)):
        for i in range (len(G)):
            for j in range (len(G)):
                if (G[j][i]):
                    if (dist[j] != float('inf')):
                        if (dist[i] > dist[j] + G[j][i]):
                            dist[i] = dist[j]+G[j][i]
                            prev[i] = j
    '''check negative cycle'''
    return [dist,prev, negative_cycle(G,dist)]

'''only return '''
def FloydWarshall(G):
    '''distance matrix initialization'''
    dist = []
    prev = []
    for i in range (len(G)):
        tmp = []
        prev.append(0)
        for j in range (len(G)):
            if (G[i][j]):
                tmp.append(G[i][j])
            elif (i == j):
                tmp.append(0)
            else:
                tmp.append(float('inf'))
        dist.append(tmp)

    '''iteration'''
    for k in range (len(G)):
        for i in range (len(G)):
            for j in range (len(G)):
                tmp = dist[i][k]+dist[k][j]
                if (dist[i][j] > tmp):
                    dist[i][j] = tmp
                    '''assign new path to "previous" array '''
                    prev[j] = k
                    prev[k] = i

    return dist, prev


# if (__name__ == "__main__"):
#     print(Dijsktra([[0.,0.2,1.,0.],[0.,0.,0.,1.],[0.,0.,0.,0.2],[0.,0.,0.,0.]],0))