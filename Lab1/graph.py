import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import functools as func
ALPHA = 0.3
MUY = 50
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
        num_src = generatorRandom()%(vertices-1)+1
        available = []
        new = []
        '''create a new vector data for new vertex'''
        for i in range (vertices-1):
            new.append(0)
        for i in range (num_src):
            available.append(i)
        '''randomly choose source vertices for the new'''
        while (num_src):
            index = available[generatorRandom()%(len(available))]
            new[index] = 1
            available.remove(index)
            num_src = num_src - 1
        '''recursion'''
        adjmt = generatorAcyclicDirectedGraph(vertices-1)
        for i in range(vertices-1):
            adjmt[i].append(0)
        new.append(0)
        adjmt.append(new)
        return adjmt



#plus on graph, by Shimbell method
def plus_graph(P:np.ndarray, Q:np.ndarray):
    ''' a+b = max (a,b)'''
    if (P.shape != Q.shape):
        return []
    else :
        res = np.full(P.shape,0)
        for row in range (P.shape[0]):
            for col in range (P.shape[1]):
                res[row][col] = max(P[row][col], Q[row][col])
        return res
    
#multiply on graph, by Shimbell method
def multiply_graph(P:np.ndarray,Q:np.ndarray):
    if (P.shape[0]!= P.shape[1] or Q.shape[0] != Q.shape[1] or P.shape != Q.shape):
        return []
    else: 
        res = np.full(P.shape,0)
        for row in range (P.shape[0]):
            for col in range (P.shape[1]):
                for index in range (P.shape[0]):
                    if (index == 0):
                        res[row][col] = P[row][index] * Q[index][col]
                    else:
                        if (res[row][col] < P[row][index] * Q[index][col]):
                            res[row][col] = P[row][index] * Q[index][col]
        return res

#calculate Shimbell matrix, deg = length 
#Shimbell[i,j] = number of walk(маршрут) (not path (путь)) with length from i to j 
def Shimbell_matrix(length: int, G):
    if (length ==1):
        return G
    else:
        return multiply_graph(G, Shimbell_matrix(length - 1, G))

"""Connecting-path finding"""
def DFS_path(i:int, j:int, visited, G):
        res = []
        #special case
        if (G[i][j]):
            res.append([i,j])
        visited.append(i)
        '''for undirectly connected'''
        for next in range (len(G)):
            if (not (next in visited)):
                if (G[i][next]):
                    #recursion here
                    for tail in DFS_path(next,j,visited+[next],G):
                        res.append([i]+tail)
        #filter
        filtered_arr = []
        for path in res:
            if j in path:
                filtered_arr.append(path)
        return  filtered_arr



EXITCODE  = 0
'''Input handler'''
def input_custom(instruction:str, check = lambda x: x.isnumeric(), convert = lambda x: int(x,10), alert = "Follow the instruction!" ) -> int:
    while(True):
        n = input(instruction)
        if (not check(n)):
            print(alert)
            continue
        n = convert(n)
        return n
EXIT = False


if (__name__ == "__main__"):

    np.random.seed()

    print("Лабораторная 1: Построение графов")
    adjacency_matrix = []
    graph = None
    needtoreopen = False
    while (not EXIT):

        if (len(adjacency_matrix) == 0):
            print("Случайно построить ациклический граф с задаемым количеством вершин:")
            num_vertices = input_custom( "Вводите количество вершин (0 - выход):", lambda x: x.isnumeric() and int(x,10)>1,lambda x:int(x,10), "Кол-во вершин должен быть больше 1!")
            adjacency_matrix =  generatorAcyclicDirectedGraph(num_vertices)
            graph = nx.MultiDiGraph(np.array(adjacency_matrix))
            print ("Граф построен:")
            print(np.array(adjacency_matrix))
            pos = nx.circular_layout(graph)
        print("1. Изображение графа")
        print("2. Матрица Шимбелла с задаемым количеством ребер")
        print("3. Проверить достижимость от одной точкой до другой")
        print("4. Новый граф")
        print("0. Выход")
        choice = input_custom("Выберите задание: ", lambda x: x in ['1','2','3','4','0'])
        if (choice == 1):
            if (needtoreopen):
                plt.close()
                needtoreopen = False
            nx.draw(graph,pos, with_labels = True)
            plt.show(block=False)
        elif (choice ==2):
            length = input_custom("Вводите количество ребер (0-выход): ")
            if (length == EXITCODE):
                EXIT = True
                break
            print("Матрица Шимбелла с "+ "%s"%length +" ребер:")
            print(Shimbell_matrix(length, np.array(adjacency_matrix)))
        elif(choice==3):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))

            #nx.draw(graph,nx.shell_layout(graph), with_labels = True, font_weight ='bold',node_color = "white")
            connected_paths = DFS_path(src,des,[],adjacency_matrix)
            
            print ("Кол-во пути от "+"%s"%src+" до "+"%s"%des+": "+"%s"%(len(connected_paths)))
            print(connected_paths)
            edges = []
            for path in connected_paths:
                for i in range (len(path)-1):
                    edges.append([path[i],path[i+1]])
            #
            if (needtoreopen):
                plt.close()
                needtoreopen = False
            nx.draw_networkx_nodes(graph,pos)
            nx.draw_networkx_labels(graph, pos)
            nx.draw_networkx_edges(graph, pos,arrows=True)
            nx.draw_networkx_edges(graph,pos,edges,edge_color = 'red',arrows=True)

            plt.show(block = False)
            needtoreopen = True
        elif(choice == 4):
            plt.close()
            adjacency_matrix = []
            continue
        elif(choice == EXITCODE):
            EXIT=True
            break