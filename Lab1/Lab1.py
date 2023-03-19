import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

ALPHA = 0.3
MUY = 50

def generatorRandom(alpha, muy) -> int:
    b = float(1-alpha)
    c = muy/(1+alpha*muy)
    p = np.float_power((1+alpha*muy), -1/alpha)
    r = np.random.rand()
    x = 0
    while (r > 0):
        r = r - p 
        x = x+1
        p = p*c*(alpha+b/x)
        r = r - p
    return x 


def generatorUndirectedGraph(vertices):
    M = np.full((vertices,vertices), 0)
    edges = vertices
    for k in range (edges):
        while(True):
            N = generatorRandom(ALPHA,MUY)
            i = int(N/vertices)
            j = int(N%vertices)
            if (M[i][j] != 1 and i != j):
                M[i][j] = 1
                M[j][i] = 1
                break
    return M

def generatorAcyclicDirectedGraph(vertices):
    M = np.full((vertices,vertices), 0)
    edges = generatorRandom(ALPHA,MUY)%(int(vertices*(vertices-1)/2))+1
    for k in range (edges):
            N = generatorRandom(ALPHA,MUY)
            i = int(N/vertices)
            j = int(N%vertices)
            if (M[i][j] != 1):
                if (i < j):
                    M[i][j] = 1
                    break
    return M

def 

