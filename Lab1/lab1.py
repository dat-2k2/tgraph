import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import copy 
ALPHA = 2
MUY = 20
def generatorRandom(alpha = ALPHA, muy = MUY) -> int:
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

#Directed graph doesn't have any circle 
#<=> well - ordered
#build downward, like a lattice
#recursion
def generatorAcyclicDirectedGraph(vertices:int):
    if (vertices == 2):
        return [[0,1],[0,0]]
    else:
        '''randomly choose the new deg'''
        num_des = generatorRandom()%((vertices-1)//2)+1
        available = []
        new = []
        '''create a new vector data for new vertex'''
        for i in range (vertices-1):
            new.append(0)
        for i in range (vertices-1):
            available.append(i)
        '''randomly choose source vertices for the new'''
        while (num_des):
            index = available[generatorRandom()%(len(available))]
            new[index] = (generatorRandom()+1)/10
            available.remove(index)
            num_des= num_des - 1
        '''recursion'''
        adjmt = generatorAcyclicDirectedGraph(vertices-1)
        for i in range(vertices-1):
            adjmt[i].append(0)
        new.append(0)
        adjmt.append(new)
        return adjmt



'''multiplication 2 adj matrices by Shimbell method'''
'''simultaneously store the found path. This path can contain cycle. '''





"""Connecting-path finding"""
"""Return a list of paths"""
def all_paths(i:int, j:int,G, visited_ = []):
    res = []
    visited = copy.deepcopy(visited_)
    #special case
    visited.append(i)
    if (G[i][j]):
        res.append([i,j])
        visited.append(j)
    '''for indirectly connected'''
    for next in range (len(G)):
        if (not (next in visited)):
            if (G[i][next]):
                #recursion at each neighbor node. 
                for tail in all_paths(next,j,G, visited+[next]):
                    res.append([i]+tail)
    #filter
    filtered_arr = []
    for path in res:
        if (path[len(path)-1] == j):
            filtered_arr.append(path)
    return filtered_arr

#calculate Shimbell matrix, deg = length 
#Shimbell[i,j] = number of walk(маршрут) (not path (путь)) with length from i to j 
def Shimbell_matrix(length: int, G ,max_min = True):
    '''Shimbell method requires unweighted''' 
    if (length == 1):
        return G
    elif (length == 0):
        return []
    else:
        shimbell = copy.deepcopy(G)
        '''shimbell = G^ length'''
        while (length > 1):
            res = []
            for row in range (len(G)):
                row_ = []
                for col in range (len(G)):
                    '''intialize based on mode'''
                    if (max_min):
                        cell = float('inf')
                    else:
                        cell = 0
                    for index in range (len(G)):
                        '''find the minimum / maximum path from i to j through any index vertex'''
                        tmp = (shimbell[row][index] + G[index][col]) if (shimbell[row][index] * G[index][col]) else 0 
                        '''if tmp == 0 skip'''
                        if (tmp):
                            if (max_min):
                                if (cell > tmp):
                                    cell = tmp
                            else:
                                if (cell < tmp):
                                    cell = tmp
                    '''set infty to 0'''
                    if (cell == float('inf')):
                        cell = 0
                    row_.append(cell)
                res.append(row_)
            shimbell = copy.deepcopy(res)
            length -= 1
        return shimbell