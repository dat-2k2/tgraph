from copy import deepcopy
import sys
sys.path.append('../Lab2/')
from Lab1 import lab1
#Need to make a copy before passing to the function
# Eulerian Extension
def checkEuler(G):

    '''check if deg is even'''
    is_euler = True
    deg = [0 for i in range(len(G))]
    ''''''
    for i in range (len(G)):
        for j in range (len(G)):
            '''deg'''
            if (G[i][j]):
                deg[i] += 1
    odd = list(filter(lambda x: deg[x]%2, list(range(len(G)))))
    is_euler = bool(len(odd) == 0)
    new_edges = {}
    G_euler = deepcopy(G)
    if (not is_euler):
        
        take_even = False
        even = list(filter(lambda x: deg[x]%2 == 0, list(range(len(G_euler)))))

        while (len(odd)):
            if (not take_even):
                # take out 2 vertices which have odd degree
                u = -1
                v = -1
                for i in odd:
                    for j in odd:
                        if (G_euler[i][j] == 0 and i!= j):
                            u = i
                            v = j

                # if they're still not connected, then connect them
                if (u+1 and v+1):
                    G_euler[u][v] = (lab1.generatorRandom()+1)
                    G_euler[v][u] = G_euler[u][v]
                    deg[u]+=1
                    deg[v]+=1
                    new_edges[(u,v)] = 1
                else:
                    #all odd edges are connected together (u,v not found && len(odd) > 1)
                    take_even = True
                    continue
            else: 
                for u in odd:
                    need_break = False
                    for v in even:
                        if (G_euler[u][v] == 0 and u!= v):
                            G_euler[u][v] = (lab1.generatorRandom()+1)
                            G_euler[v][u] = G_euler[u][v]
                            deg[u]+=1
                            deg[v]+=1
                            new_edges[(u,v)] = 1
                            take_even = False
                            need_break = True
                            break
                    if (need_break):
                        break
                    
            odd = list(filter(lambda x: deg[x]%2, list(range(len(G_euler)))))
            even = list(filter(lambda x: deg[x]%2 == 0, list(range(len(G_euler))))) 
        
        if (len(odd)):
            print("Error")
            #all odd vertices are connected, so we need to choose an even vertex to connect
            

    return is_euler, G_euler, new_edges


 
def checkHamilton(G):
    deg = [0 for i in range(len(G))]
    ''''''
    for i in range (len(G)):
        for j in range (len(G)):
            '''deg'''
            if (G[i][j]):
                deg[i] += 1
    

    is_hamilton = False
    G_hamilton = deepcopy(G)
    new_edges = {}
    def subHamilton(undirected, visited):
        nonlocal is_hamilton
        if (len(visited) < len(undirected)):
            for i in range(len(undirected)):
                if (i not in visited and undirected[visited[-1]][i]):
                    subHamilton(undirected, visited + [i])
                else:
                    continue
        else:
            if (undirected[visited[-1]][visited[0]]):
                is_hamilton = True
                return

    if (not is_hamilton):
        under = list(filter(lambda i: deg[i] < 2, list(range(len(G)))))    
        for u in range(len(G_hamilton)):
            while (deg[u] < len(G_hamilton)//2): # by Dirac
                found = False

                ## find a vertex in under to connect
                for v in under:
                    ##only connect if they're not connected yet. 
                    if (G_hamilton[u][v] == 0 and u != v):
                        G_hamilton[u][v] = (lab1.generatorRandom()+1)
                        G_hamilton[v][u] = G_hamilton[u][v]
                        deg[u]+=1
                        deg[v]+=1
                        new_edges[(u,v)] = G_hamilton[u][v]
                        found = True
                        break
                if (not found):
                    ## randomly pick a vertices

                    v = (lab1.generatorRandom())%(len(G_hamilton))
                    while (G_hamilton[u][v] or u == v):
                        v = (lab1.generatorRandom())%(len(G_hamilton))
                    G_hamilton[u][v] = (lab1.generatorRandom()+1)
                    G_hamilton[v][u] = G_hamilton[u][v]
                    deg[u]+=1
                    deg[v]+=1
                    new_edges[(u,v)] = G_hamilton[u][v]
            '''update the under list'''
            if (u in under):
                under.remove(u)

    return is_hamilton, G_hamilton, new_edges

    



## !! FOR UNDIRECTED ONLY
def Hierholzer(undirected):
    G = deepcopy(undirected)
    curr_path = [0]
    cycle = []
    while (curr_path):
        curr_v = curr_path[-1]
        # find a sub cycle
        neighbors = []
        while (True):
            neighbors = list(filter(lambda x: G[curr_v][x], list(range(len(G)))))
            if (len(neighbors)):
                next = neighbors[0]
                curr_path.append(next)
                # remove the used edge
                G[curr_v][next] = 0
                G[next][curr_v] = 0
                curr_v = next
                continue
            else:
                break

        ## backtrack by the subcycle to find another one
        cycle.append(curr_path.pop())   

    # make path from cycle

    path = {}
    c = 0
    while (len(cycle) > 1):
        path[(cycle.pop(), cycle[-1])] = c
        c+=1

    return path
        


def AllHamiltonCyle(undirected, filepath):
    w = float('inf')
    p = []
    def subHamiltonFile(file, undirected, visited, sum_weight):
        nonlocal w, p

        if (len(visited) < len(undirected)):
            for i in range(len(undirected)):
                if (i not in visited and undirected[visited[-1]][i]):
                    sum_weight += undirected[visited[-1]][i]
                    subHamiltonFile(file, undirected, visited + [i], sum_weight)
        else:
            # only take ending vertex when it's connected to the head
            if (undirected[visited[-1]][visited[0]]):
                visited.append(visited[0])
                sum_weight += undirected[visited[-1]][visited[0]]
                
                if (sum_weight < w):
                    w = sum_weight
                    p = visited
                file.write(str(visited))
                file.write(" %d\n"%sum_weight)
                
    def subHamilton(undirected, visited, sum_weight):
        nonlocal w, p

        if (len(visited) < len(undirected)):
            for i in range(len(undirected)):
                if (i not in visited and undirected[visited[-1]][i]):
                    subHamilton(undirected, visited + [i], sum_weight + undirected[visited[-1]][i])
                else:
                    continue
        else:
            if (undirected[visited[-1]][visited[0]]):
                visited.append(visited[0])
                sum_weight += undirected[visited[-1]][visited[0]]

                print("Цикл:", visited, " Вес: ", sum_weight)

                if (sum_weight < w):
                    w = sum_weight
                    p = visited

    ## O(n!)
    if (len(undirected) > 7):
        f = open(filepath, 'w')
        f.close()
        f = open(filepath,'a')
        subHamiltonFile(f, undirected, [0], 0)
        f.close()
    else:
        subHamilton(undirected, [0], 0)
    return w,p

    

        

                
        






