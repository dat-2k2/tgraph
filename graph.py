import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from Lab1 import lab1
from Lab2 import lab2

'''Input handler'''
def input_custom(instruction:str, check = lambda x: x.isnumeric(), convert = lambda x: int(x,10), alert = "Follow the instruction!" ) -> int:
    while(True):
        n = input(instruction)
        if (not check(n)):
            print(alert)
            continue
        n = convert(n)
        return n

'''show paths from LIST OF paths'''
def showpath(list_paths, graph, pos):
    edges = []
    for path in list_paths:
        for i in range (len(path)-1):
            edges.append([path[i],path[i+1]])
    nx.draw_networkx_nodes(graph,pos)
    nx.draw_networkx_labels(graph, pos)
    nx.draw_networkx_edges(graph, pos,arrows=True)
    nx.draw_networkx_edges(graph,pos,edges,edge_color = 'red',arrows=True)
    nx.draw_networkx_edge_labels(graph, pos, edge_labels = nx.get_edge_attributes(graph, 'weight'))
    plt.show(block = False)


'''Path extraction from Previous array'''
def generatePathfromPrev(prev, src, des):
    res = [des]
    while (res[len(res)-1] != src):
        res.append(prev[res[len(res)-1]])
        if (res[len(res)-1] == None):
            return None
    res.reverse()
    return res

'''more clearly showing path'''
def printPaths(path_list, caption = ""):
    if (len(caption)):
        print (caption)
    for path in path_list:
        print (path)




EXIT = False
EXITCODE  = 0
NEWGRAPH = 1
VISUAL = 2
SHIMBELL = 3
REACH = 4
DIJSKTRA = 5
BELLMAN_FORD = 6
FLOYD_WARSHALL = 7

if (__name__ == "__main__"):

    np.random.seed()

    '''Preparation'''
    print("Лабораторная 1: Построение графов")

    '''cache memory'''

    #adjacency matrix
    adjacency_matrix = []

    graph = None
    needtoreopen = False
    pos = None
    #solution for Floyd - Warshall. 
    #Need to save it bc this algo costs alot more than two others.
    FW_cache = []

    ''''''

    while (not EXIT):
        '''initialize graph'''
        if (len(adjacency_matrix) == 0):

            '''user input'''
            print("Случайно построить ациклический граф с задаемым количеством вершин:")
            num_vertices = input_custom( "Вводите количество вершин (0 - выход):", lambda x: x.isnumeric() and int(x,10)!=1,lambda x:int(x,10), "Кол-во вершин должен быть больше 1!")
            if (num_vertices == EXITCODE):
                EXIT = True
                break

            '''generate graph from adjacency matrix'''
            adjacency_matrix =  lab1.generatorAcyclicDirectedGraph(num_vertices)
            graph = nx.DiGraph(np.array(adjacency_matrix))
            pos = nx.circular_layout(graph)

            print ("Граф построен:")
            print(np.array(adjacency_matrix))

        '''modes'''
        print( str(NEWGRAPH) + ". Новый граф")
        print(str(VISUAL) + ". Изображение графа")
        print(str(SHIMBELL) + ". Матрица Шимбелла с задаемым количеством ребер")
        print(str(REACH) + ". Проверить достижимость от одной точкой до другой")
        print(str(DIJSKTRA) + ". Dijsktra")
        print(str(BELLMAN_FORD) + ". Bellman - Ford")
        print(str(FLOYD_WARSHALL) + ". Floyd - Warshall")
        print(str(EXITCODE) + ". Выход")
        choice = input_custom("Выберите задание: ", lambda x: x.isnumeric and int(x,10) in [VISUAL, SHIMBELL, REACH, DIJSKTRA, BELLMAN_FORD, FLOYD_WARSHALL, NEWGRAPH, EXITCODE])

        if (choice == VISUAL):

            '''draw graph'''
            if (needtoreopen):
                plt.close()
                needtoreopen = False
            nx.draw(graph,pos, with_labels = True)
            nx.draw_networkx_edge_labels(graph, pos, edge_labels = nx.get_edge_attributes(graph, 'weight'))
            plt.show(block=False)
        elif (choice == SHIMBELL):

            '''Shimbell'''
            length = input_custom("Вводите количество ребер: ")
            print("Матрица Шимбелла с "+ "%s"%length +" ребер:")
            shimbell = lab1.Shimbell_matrix(length, np.array(adjacency_matrix))
            print(shimbell)

            '''find + show all paths sastified Shimbell'''
            if (not np.array_equal(shimbell,np.full(num_vertices, np.zeros(num_vertices))) and len(shimbell) > 0):
                '''find paths with given length'''
                connected_paths = []
                edges = []
                for src in range (len(adjacency_matrix)):
                    for des in range(len(adjacency_matrix)):
                        if (shimbell[src][des]):
                            #using DFS
                            connected_paths.extend(lab1.DFS_path(src,des,adjacency_matrix))
                if (needtoreopen):
                    plt.close()
                    needtoreopen = False

                '''print all paths with given length'''
                printPaths(connected_paths, "Пути с длиной "+ str(length))

                '''visualize'''
                showpath(connected_paths, graph, pos)
                needtoreopen = True

        elif(choice==REACH):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))

            '''find path from src to des'''
            connected_paths = lab1.DFS_path(src,des,adjacency_matrix)

            '''result'''
            if (len(connected_paths)):
                print ("Кол-во пути от "+"%s"%src+" до "+"%s"%des+": "+"%s"%(len(connected_paths)))
                printPaths(connected_paths, "Пути из вершины " + str(src) + " до "+ str(des)+": ")
            else: 
                print ("Вершина "+str(des)+" недостижима из вершины "+str(src))
            if (needtoreopen):
                plt.close()
                needtoreopen = False
            showpath(connected_paths, graph, pos)
            needtoreopen = True

        elif(choice == DIJSKTRA):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))
            d = 0 
            p = 0 
            iterations = 0
            arr = lab2.Dijsktra(adjacency_matrix,src)
            d = arr[0]
            p = arr[1]
            iterations = arr[2]           
            print("Вектор расстояний: ")
            print(np.array(d))
            print("Кол-во итераций = " +str(iterations))


            '''Print found path'''
            paths = generatePathfromPrev(p,src,des)
            if (paths != None): 
                printPaths([paths], "Кратчайший путь из "+ str(src) + " до " + str(des)+ ": ")
                '''visualize'''
                if (needtoreopen):
                    plt.close()
                    needtoreopen = False
                showpath([paths], graph, pos)
                needtoreopen = True
            else: 
                print ("Вершина "+str(des)+" недостижима из вершины "+str(src))



        elif (choice == BELLMAN_FORD):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))

            print ("Вектор расстояний: ")
            d = 0 
            p = 0 
            iterations = 0
            arr = lab2.BellmanFord(adjacency_matrix,src)
            d = arr[0]
            p = arr[1]
            iterations = arr[2]
            neg_cyc = arr[3]
            print(np.array(d))
            print ("Кол - во итераций: ")
            print (iterations)


            '''Print found path'''
            paths = generatePathfromPrev(p,src,des)
            if (neg_cyc):
                print ("Отрицательный цикл")
            else:
                if (paths != None): 
                    printPaths([paths], "Кратчайший путь из "+ str(src) + " до " + str(des)+ ": ")
                    '''visualize'''
                    if (needtoreopen):
                        plt.close()
                        needtoreopen = False
                    showpath([paths], graph, pos)
                    needtoreopen = True
                else: 
                    print ("Вершина "+str(des)+" недостижима из вершины "+str(src))

        elif (choice == FLOYD_WARSHALL):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))

            d = 0 
            p = 0 
            iterations = 0

            if (len(FW_cache) == 0):
                arr = lab2.FloydWarshall(adjacency_matrix)
                d = arr[0]
                p = arr[1]
                iterations = arr[2]

                '''Save the solution to cache for later using'''
                FW_cache = arr
            else:
                d = FW_cache[0]
                p = FW_cache[1]
                iterations = FW_cache[2]

            print("Матрица расстояний: ")
            print(np.array(d))
            print ("Кол - во итераций: ")
            print (iterations)

            #need a little convert here: the p array is 2D 
            paths = None
            if (p[src][des] != None):
                paths = [src]
                while (paths[len(paths)-1] != des):
                    paths.append(p[paths[len(paths)-1]][des])
                    
            '''Print found path'''
            if (paths != None): 
                printPaths([paths], "Кратчайший путь из "+ str(src) + " до " + str(des)+ ": ")
    
                '''visualize'''
                if (needtoreopen):
                    plt.close()
                    needtoreopen = False
                showpath([paths], graph, pos)
                needtoreopen = True
                
            else: 
                '''Not found, announce'''
                print ("Вершина "+str(des)+" недостижима из вершины "+str(src))


        elif(choice == NEWGRAPH):
            plt.close()
            '''clear cache'''
            adjacency_matrix = []
            graph = None
            needtoreopen = False
            pos = None
            FW_cache = []
            ''''''
            continue
        elif(choice == EXITCODE):
            EXIT=True
            break