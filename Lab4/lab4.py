## using Disjoint set
## from now using set of edge


## FOR UNDIRECTED GRAPH ONLY

from copy import deepcopy
from functools import reduce

class dsu:
    def __init__(self, n: int):
        self.par = [x for x in range(n)]
    def find(self, u: int):
        if (self.par[u] != u):
            self.par[u] = self.find(self.par[u])
        return self.par[u]
    def union(self, i, j):
        self.par[self.find(i)] = self.find(j)

class edge:
    def __init__(self, u, v, w):
        self.src = u
        self.sink = v
        self.w = w


## connect 2 disconnected trees (2 disjont set) gonna make another disjoint set and still keep disjoint property 
def Kruskal(G):
    edge_list = []
    totalWeight = 0
    skeleton = dsu(len(G))
    res = [[0 for i in range (len(G))] for j in range (len(G))]
    complexity = 0
    '''init list of edge'''
    for i in range(len(G)):
        for j in range (len(G)):
            if (G[i][j]):
                tmp = edge(i,j,G[i][j])
                edge_list.append(tmp)

    '''sort them '''
    edge_list.sort(key = lambda x: x.w)
    for e in edge_list:
        # try including this edge, if it doesn't make a cycle then add it
        x = skeleton.find(e.src)
        y = skeleton.find(e.sink)
        if (x != y):
            res[e.src][e.sink] = G[e.src][e.sink]
            res[e.sink][e.src] = G[e.src][e.sink]
            totalWeight += e.w
            skeleton.union(x,y)
 

    return totalWeight, res


def Prim(G):
    skeleton = [[0 for i in range (len(G))] for j in range (len(G))]
    unvisited  = [i for i in range (1,len(G))]

    edge_queue = []
    cur = [0]
    for j in unvisited:
        if (G[0][j]):
            edge_queue.append(edge(0,j,G[0][j]))
            
    while (len(unvisited)):
        u = min(edge_queue, key=lambda x: x.w)
        edge_queue.remove(u)
        '''add new edge to watch'''
        cur.append(u.sink)
        unvisited.remove(u.sink)
        edge_queue = list(filter(lambda x: x.sink != u.sink, edge_queue))

        for j in unvisited:
            if (G[u.sink][j]):
                edge_queue.append(edge(u.sink,j,G[u.sink][j]))

        skeleton[u.src][u.sink] = G[u.src][u.sink]
        skeleton[u.sink][u.src] = G[u.src][u.sink]

    return skeleton


def notnullCount(arr):
    count = 0
    for i in range (len(arr)):
        if (arr[i]): 
            count += 1
    return count

def remove1(a: list, x):
    b = deepcopy(a)
    b.remove(x)
    return b

def subMatrix(G, notcell):
    rows = remove1(list(range(len(G))),notcell[0])
    cols = remove1(list(range(len(G))),notcell[1])
    return [[G[row][col] for col in cols] for row in rows]


def determinant(G):
    if (len(G) == 0):
        return None
    elif (len(G) == 1):
        return G[0][0]
    else:
        s = 0 
        for i in range (len(G)):    
            if (i % 2):
                if (G[i][0]):
                    s -= G[i][0]*determinant([[G[row][col] for col in list(range(1,len(G)))] for row in list(range(0,i)) + list(range(i+1,len(G)))])
            else: 
                if (G[i][0]):
                    s += G[i][0]*determinant([[G[row][col] for col in list(range(1,len(G)))] for row in list(range(0,i)) + list(range(i+1,len(G)))])
        return s
        

        
def Kirchhoff(L1):
    '''num of spanning trees = cofactor of any element'''
    L = deepcopy(L1)
    '''get the cofactor'''
    for c in L:
        c.pop()
    L.pop()

    cofact = determinant(L)
    '''number of spanning tree of G = 1/#vertices*product(eigenvalues)'''
    return abs(cofact)

        
def isLeaf(G, v):
    count = 0
    for i in range (len(G)):
        if (G[i][v]):
            count +=1
    return (count == 1),
''''''
def PruferEncode(G):
    if (len(G) >= 2):
        '''init queue of vertices'''
        deg = {i:0 for i in range (len(G))}
        for i in range (len(G)):
            for j in range (i+1,len(G)):
                if (G[i][j]):
                    deg[i]+=1
                    deg[j]+=1

        q = list(filter(lambda x: deg[x] == 1, list(range(len(G)))))
        res = []
        weight = []
        while (len(deg) > 2):
            '''remove the LEAF with the smallest label'''
            q = list(filter(lambda x: deg[x] == 1, deg.keys()))
            l = q.pop(0)
            deg.pop(l)
            for j in deg.keys():
                if (G[l][j]):
                    res.append(j)
                    weight.append(G[l][j])
                    deg[j] -= 1

        '''remain 2 vertices - 1 edge'''
        weight.append(G[list(deg.keys())[0]][list(deg.keys())[1]])
        return res, weight
    elif (len(G) == 1):
        return [], [G[0][0]]
    else:
        return [], []


def PruferDecode(arr, weight):
    if (len(weight) >= 2):
        G = [[0 for i in range(len(arr)+2)] for j in range (len(arr)+2)]
        deg = [1 for i in range(len(G))]

        '''firstly set deg = num appear + 1'''
        for i in arr:
            deg[i] += 1
        for i in range(len(arr)):
            for j in range(len(G)):
                if (deg[j] == 1):
                    G[arr[i]][j] = weight[i]
                    G[j][arr[i]] = weight[i]
                    deg[arr[i]] -= 1
                    deg[j] -= 1
                    break
        '''remain 2 nodes u v with deg = 1'''
        last = list(filter(lambda x: deg[x] > 0, list(range(len(G)))))

        '''insert [u,v] to G'''
        G[last[0]][last[1]] = weight[len(weight)-1]
        G[last[1]][last[0]] = weight[len(weight)-1]
        return G
    elif (len(weight)):
        return [[0, weight[0]], [weight[0], 0]]
    else:
        return [[0]]

