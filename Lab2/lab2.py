import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

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
#complexity = O(len(G)^2)
def Dijsktra(G, src:int):
    '''generate queue, dist and prep'''
    q = []
    d = []
    pr = []
    iterations = 0

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
            iterations += 1
            if (G[curr][other] > 0):
                if (d[other] > d[curr] + G[curr][other]):
                    d[other] = d[curr] + G[curr][other]
                    pr[other] = curr
    return d,pr,iterations

#complexity (O(V)
def BellmanFord(G, src:int):
    '''dist init'''
    dist = []
    prev = []
    iterations = 0
    for i in range (len(G)):
        dist.append(float('inf'))
        prev.append(None)
    dist[src] = 0
    
    '''update distance'''
    for k in range (len(G)):
        for i in range (len(G)):
            for j in range (len(G)):
                iterations += 1
                if (G[j][i]):
                    if (dist[j] != float('inf')):
                        if (dist[i] > dist[j] + G[j][i]):
                            dist[i] = dist[j]+G[j][i]
                            prev[i] = j
    '''check negative cycle'''
    return dist,prev, iterations, negative_cycle(G,dist)

'''only return '''
def FloydWarshall(G):
    '''distance matrix initialization'''
    dist = []
    prev = []
    iterations = 0
    for i in range (len(G)):
        tmp = []
        tmp_prev = []
        for j in range (len(G)):
            if (G[i][j]):
                tmp.append(G[i][j])
                tmp_prev.append(j)
            elif (i == j):
                tmp.append(0)
                tmp_prev.append(None)
            else:
                tmp.append(float('inf'))
                tmp_prev.append(None)
        dist.append(tmp)
        prev.append(tmp_prev)

    '''iteration'''
    for k in range (len(G)):
        for i in range (len(G)):
            for j in range (len(G)):
                iterations += 1
                if (i != k and k!= j and j!= i):
                    tmp = dist[i][k]+dist[k][j]
                    if (dist[i][j] > tmp):
                        dist[i][j] = tmp
                        '''assign new path to "previous" array '''
                        prev[i][j] = prev[i][k]
    return dist, prev,iterations
