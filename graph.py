import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
from termcolor import colored
from enum import Enum
from copy import deepcopy
from Lab1 import lab1
from Lab2 import lab2
from Lab3 import lab3
from Lab4 import lab4
from Lab5 import lab5

'''Input handler'''
def input_custom(instruction:str, check = lambda x: x.isnumeric(), convert = lambda x: int(x,10), alert = colored("Follow the instruction!","yellow") ) -> int:
    while(True):
        n = input(colored(instruction,"light_yellow"))
        if (not check(n)):
            print(alert)
            continue
        n = convert(n)
        return n


'''show paths from LIST OF paths'''
def showpath(list_paths, graph, pos, color='r'):
    '''put new edges into a dict to avoid duplication'''
    edges = {}
    '''make a collection of edges by LineCollection'''
    for path in list_paths:
        for i in range (len(path)-1):
            edges[(path[i],path[i+1])] = color

    #May update?
    '''cannot partly remove edges, so draw on top to fit the style'''
    ##NOTE: return [FancyArrowPatch]
    tmp = nx.draw_networkx_edges(graph,pos,edges,edge_color = color,arrows=True)
    plt.show(block = False)
    '''return the newest layer'''
    return tmp


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
        print (colored(caption,"green"))
    for path in path_list:
        print (path)

            
        
def resetLabels(EdgeLabelArtist):
    if (EdgeLabelArtist):
        for label in EdgeLabelArtist.values():
            label.remove()

def resetEdgeLayer(EdgeArtistStack):
    while (len(EdgeArtistStack) > 1):
        tmp = EdgeArtistStack.pop()
        if (type(tmp).__name__ == 'list'):
            for i in tmp:
                i.remove()
        else:
            tmp.remove()

def hardresetEdgeLayer(EdgeArtistStack):
    while (len(EdgeArtistStack)):
        tmp = EdgeArtistStack.pop()
        if (type(tmp).__name__ == 'list'):
            for i in tmp:
                i.remove()
        else:
            tmp.remove()

def message(str):
    print(colored(str,'cyan'))

def alert(str):
    print(colored(str,'red'))

def success(str):
    print(colored(str,'green'))

EXIT = False

CHOICES = Enum('CHOICES', ['EXITCODE', 'NEWGRAPH', 'VISUAL','UNDIRECTED_VISUAL','SHIMBELL', 'REACH', 'DIJSKTRA', 'BELLMAN_FORD', 'FLOYD_WARSHALL', 'FORD_FULKERSON', 'MINCOST_FLOW','KRUSKAL','PRIM', 'KIRCHHOFF','EULER_PATH','HAMILTON_PATH', 'CYCLE', 'SALESMAN'],start=0)

if (__name__ == "__main__"):

    np.random.seed()

    '''cache memory'''

    #adjacency matrix
    adjacency_matrix = []
    capacity_matrix = []
    undirected = []
    tocycle_edges = []

    graph = None
    pos = None
    #solution for Floyd - Warshall. 
    #Need to save it bc this algo costs alot more than two others.
    FW_cache = []
    
    #max flow by Ford-Fulkerson
    max_flow = 0
    
    # is Euler?
    is_Euler = None
    eulerian_extended_edges = {}
    eulerian_matrix = []

    # is Hamilton?
    is_Hamilton = None
    hamiltonian_extended_edges = {}
    hamiltonian_matrix = []

    #artist object to control items:
    Edge_label_artist = None
    Edge_artist_stack = []
    Edge_added_stack = []
    ''''''
    '''VIEW MODE FLAG'''
    directed_view = True
    while (not EXIT):

        '''initialize graph'''
        if (len(adjacency_matrix) == 0):

            '''user input'''
            message("Случайно построить ациклический граф с задаемым количеством вершин:")
            num_vertices = input_custom("Вводите количество вершин (0 - выход):", lambda x: x.isnumeric() and int(x,10)!=1,lambda x:int(x,10), "Кол-во вершин должен быть больше 1!")
            if (num_vertices == CHOICES.EXITCODE.value):
                EXIT = True
                break

            '''generate graph from adjacency matrix'''
            adjacency_matrix =  lab1.generatorAcyclicDirectedGraphWithFlow(num_vertices)
            capacity_matrix = lab1.generatorCapacities(adjacency_matrix)
            undirected = [[max(adjacency_matrix[i][j],adjacency_matrix[j][i]) for j in range(num_vertices)] for i in range(num_vertices)]


            graph = nx.DiGraph(np.array(adjacency_matrix))
            pos = nx.circular_layout(graph)

            '''draw graph'''
            nx.draw_networkx_nodes(graph,pos)
            nx.draw_networkx_labels(graph,pos)

            '''2 modes: directed and undirected'''
            Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,arrows=True))
            Edge_label_artist = nx.draw_networkx_edge_labels(graph, pos, label_pos=0.3, edge_labels={(u,v): "%d"%adjacency_matrix[u][v] for u,v in graph.edges})
            

            success("Граф построен:",)
            print(np.array(adjacency_matrix))

        plt.show(block=False)
    
        '''modes'''
        print("=====================================================\t\t")
        print(colored("\nСОЗДАНИЕ:\n",'light_yellow'))
        print("%d. Новый граф" % CHOICES.NEWGRAPH.value)

        print(colored("\nИЗОБРАЖЕНИЯ:\n",'light_yellow'))
        print("%d. Изображение графа" %CHOICES.VISUAL.value)
        print("%d. Изображение графа (неориентированный)" %CHOICES.UNDIRECTED_VISUAL.value)

        print(colored("\nАЛГОРИТМЫ НА ОРИЕНТИРОВАННОМ ГРАФЕ:\n",'light_yellow'))
        print("%d. Матрица Шимбелла с задаемым количеством ребер" %CHOICES.SHIMBELL.value)
        print("%d. Проверить достижимость от одной точкой до другой" %CHOICES.REACH.value)
        print("%d. Dijsktra" %CHOICES.DIJSKTRA.value)
        print("%d. Bellman - Ford" %CHOICES.BELLMAN_FORD.value)
        print("%d. Floyd - Warshall" %CHOICES.FLOYD_WARSHALL.value)
        print("%d. Ford - Fulkerson" %CHOICES.FORD_FULKERSON.value)
        print("%d. Поток минимальной стоймости" %CHOICES.MINCOST_FLOW.value)

        print(colored("\nАЛГОРИТМЫ НА НЕОРИЕНТИРОВАННОМ ГРАФЕ:\n",'light_yellow'))
        print("%d. Kruskal" %CHOICES.KRUSKAL.value)
        print("%d. Prim" %CHOICES.PRIM.value)
        print("%d. Kirchhoff" %CHOICES.KIRCHHOFF.value)
        print("%d. Путь Euler" %CHOICES.EULER_PATH.value)
        print("%d. Путь Hamilton" %CHOICES.HAMILTON_PATH.value)
        print("%d. Цикл Euler" %CHOICES.CYCLE.value)
        print("%d. Задача коммивояжера"%CHOICES.SALESMAN.value)
        print("%d. Выход" %CHOICES.EXITCODE.value)
        choice = input_custom("Выберите задание: ", lambda x: x.isnumeric and int(x,10) in [x.value for x in CHOICES])
        


        ## Reset top layers to add new edge
        if (choice in [CHOICES.UNDIRECTED_VISUAL.value, CHOICES.KRUSKAL.value, CHOICES.PRIM.value, CHOICES.KIRCHHOFF.value, CHOICES.EULER_PATH.value, CHOICES.HAMILTON_PATH.value, CHOICES.CYCLE.value,CHOICES.SALESMAN.value]):
        
            ## UNDIRECTED MODE
            if (directed_view):
                ## need to toggle mode
                hardresetEdgeLayer(Edge_artist_stack)
                Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,arrows=False))   
                directed_view = False

                resetLabels(Edge_label_artist)
                Edge_label_artist = nx.draw_networkx_edge_labels(graph, pos, label_pos=0.3, edge_labels={(u,v): adjacency_matrix[u][v] for u,v in graph.edges})
                
            else:
                ## no toggle
                resetEdgeLayer(EdgeArtistStack=Edge_artist_stack)
        else:
            ## DIRECTED MODE
            if (not directed_view):
                ## need to toggle mode
                hardresetEdgeLayer(Edge_artist_stack)
                Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,arrows=True))           

                resetLabels(Edge_label_artist) 
                Edge_label_artist = nx.draw_networkx_edge_labels(graph, pos, label_pos=0.3, edge_labels={(u,v): "%d"%(adjacency_matrix[u][v]) for u,v in graph.edges})
                    
                directed_view = True 
            else:
                ## no toggle
                resetEdgeLayer(EdgeArtistStack=Edge_artist_stack)
        
        plt.show(block=False)
        ##Remove all new upper layout of edges, only keep the base
        if (choice == CHOICES.VISUAL.value): 

            plt.show(block=False)
        
        elif (choice == CHOICES.UNDIRECTED_VISUAL.value):


            plt.show(block=False)

        elif (choice == CHOICES.SHIMBELL.value):

            '''Shimbell'''
            length = input_custom("Вводите количество ребер: ")
            max_min = bool(input_custom("Максимум - 0/Минимум - 1?: ", lambda x: x.isnumeric and int(x) in [0,1]))
            message("Матрица Шимбелла с "+ "%s"%length +" ребер:")
            shimbell = lab1.Shimbell_matrix(length, np.array(adjacency_matrix), max_min)
            print(np.array(shimbell))



        elif (choice == CHOICES.REACH.value):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))

            '''find path from src to des'''
            connected_paths = lab1.all_paths(src,des,adjacency_matrix)

            '''result'''
            if (len(connected_paths)):
                print ("Кол-во пути от "+"%s"%src+" до "+"%s"%des+": "+"%s"%(len(connected_paths)))
                printPaths(connected_paths, "Пути из вершины " + str(src) + " до "+ str(des)+": ")
                alert("Вершина "+str(des)+" недостижима из вершины "+str(src))
            resetEdgeLayer(Edge_artist_stack)
            Edge_artist_stack.append(showpath(connected_paths, graph, pos))
            resetLabels(Edge_label_artist)
            Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.5,edge_labels={(u,v):adjacency_matrix[u][v] for u, v in graph.edges()})


        elif (choice == CHOICES.DIJSKTRA.value):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))
            arr = lab2.Dijsktra(adjacency_matrix,src)

            '''result'''
            d = arr[0]
            p = arr[1]
            iterations = arr[2]           
            
            print(np.array(d))

            '''Print found path'''
            paths = generatePathfromPrev(p,src,des)
            if (paths != None): 
                printPaths([paths], "Кратчайший путь из "+ str(src) + " до " + str(des)+ ": ")
                resetEdgeLayer(Edge_artist_stack)
                Edge_artist_stack.append(showpath([paths], graph, pos))
                resetLabels(Edge_label_artist)
                Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.3,edge_labels={(u,v):adjacency_matrix[u][v] for u, v in graph.edges()})
            else:
                alert("Вершина "+str(des)+" недостижима из вершины "+str(src))


        elif (choice == CHOICES.BELLMAN_FORD.value):
            src = input_custom("Выверите отправление: ", lambda x: int(x) in range(num_vertices))
            des = input_custom("Выверите прибытие: ", lambda x: int(x) in range(num_vertices))

            arr = lab2.BellmanFord(adjacency_matrix,src)

            d = arr[0]
            p = arr[1]
            iterations = arr[2]
            neg_cyc = arr[3]

            message("Вектор расстояний: ")
            print(np.array(d))
            message("Кол-во итераций = " +str(iterations))

            '''Print found path'''            
            if (neg_cyc):
                print ("Отрицательный цикл")
            else:
                paths = generatePathfromPrev(p,src,des)
                if (paths != None): 
                    printPaths([paths], "Кратчайший путь из "+ str(src) + " до " + str(des)+ ": ")
                    '''visualize'''
                    resetEdgeLayer(Edge_artist_stack)
                    Edge_artist_stack.append(showpath([paths], graph, pos))
                    resetLabels(Edge_label_artist)
                    Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.3,edge_labels={(u,v):adjacency_matrix[u][v] for u, v in graph.edges()})                    
                else: 
                    alert("Вершина "+str(des)+" недостижима из вершины "+str(src))


        elif (choice == CHOICES.FLOYD_WARSHALL.value):
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

            message("Матрица расстояний: ")
            print(np.array(d))
            message("Кол - во итераций: ")
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
                resetEdgeLayer(Edge_artist_stack)
                Edge_artist_stack.append(showpath([paths], graph, pos))
                
                resetLabels(Edge_label_artist)
                Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.3,edge_labels={(u,v):adjacency_matrix[u][v] for u, v in graph.edges()})
                
            else: 
                '''Not found, announce'''
                alert ("Вершина "+str(des)+" недостижима из вершины "+str(src))



        elif (choice == CHOICES.FORD_FULKERSON.value):

            resetLabels(Edge_label_artist)
            flow_labels = {(u,v):"%d/%d"%(0,capacity_matrix[u][v]) for u,v in graph.edges}
            Edge_label_artist = nx.draw_networkx_edge_labels(graph, pos, edge_labels = flow_labels, label_pos = 0.3)
        

            src = input_custom("Выверите исток: ", lambda x: int(x) in range(num_vertices))
            sink = input_custom("Выверите сток: ", lambda x: int(x) in range(num_vertices))
            # src = len(adjacency_matrix)-1
            # sink = 0
            result = lab3.EdmondKarps(adjacency_matrix, src, sink, capacity_matrix)

            '''maximum flow'''
            max_flow = result[0]
            print("Максимальный поток: %d" %result[0])
            
            '''flow map'''
            flow_matrix = result[1]
            print("Матрица потока: ")
            print(np.array(flow_matrix))

            '''visualize'''
            edges_flow = list(filter(lambda uv: flow_matrix[uv[0]][uv[1]], graph.edges()))

            resetEdgeLayer(Edge_artist_stack)
            Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,edges_flow,edge_color='light_green'))
            '''show flow'''
            '''reset label'''
            resetLabels(Edge_label_artist)
            flow_labels = {(u,v):"%d/%d"%(flow_matrix[u][v],capacity_matrix[u][v]) for u,v in graph.edges}
            Edge_label_artist = nx.draw_networkx_edge_labels(graph, pos, edge_labels = flow_labels, label_pos = 0.2)
            plt.show(block=False)



        elif (choice == CHOICES.MINCOST_FLOW.value):

            resetLabels(Edge_label_artist)
            flow_labels = {(u,v):"c:%d,w:%d"%(capacity_matrix[u][v],adjacency_matrix[u][v]) for u,v in graph.edges}
            Edge_label_artist = nx.draw_networkx_edge_labels(graph, pos, edge_labels = flow_labels, label_pos = 0.3)

            if (max_flow == 0):
                alert("Максимальный поток пока не определен")
                continue
            
            src = input_custom("Выверите исток: ", lambda x: int(x) in range(num_vertices))
            sink = input_custom("Выверите сток: ", lambda x: int(x) in range(num_vertices))
            # src = len(adjacency_matrix)-1
            # sink = 0
            
            '''max_flow still undefined'''

            
            Threshold = max_flow*2/3
            message("Желаемый поток: %d"%Threshold)
            result = lab3.mincostflow(adjacency_matrix, capacity_matrix, src, sink, Threshold)

            flow = result[0]
            cost = result[2]
            m = result[1]
            if (result[0]):
                    
                message("Актуальный поток: " +str(flow))
                message("Стоймость: " +str(cost))
                message("Матрица потока: ")
                print(np.array(m))
                
                '''show new flow by redraw label'''

                edges_flow = list(filter(lambda uv: m[u][v], graph.edges()))
                resetEdgeLayer(Edge_artist_stack)
                Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,edges_flow,edge_color='light_green'))
                resetLabels(Edge_label_artist)
                flow_labels = {(u,v):"%d(%d)"%(m[u][v], adjacency_matrix[u][v]) for u,v in graph.edges}
                Edge_label_artist = nx.draw_networkx_edge_labels(graph, pos, label_pos=0.2, edge_labels=flow_labels)
                plt.show(block=False)
        
                ''''''
            else:
                alert("Нет потока: " + str(result[2]))



        elif (choice == CHOICES.KRUSKAL.value):
 
            result = lab4.Kruskal(undirected)
            spanning_tree = result[1]
            print(np.array(spanning_tree))
            
            e = []

            for i in range (num_vertices):
                for j in range (num_vertices):
                    if (adjacency_matrix[i][j] and spanning_tree[i][j]):
                        e.append((i,j))
                        

            resetEdgeLayer(Edge_artist_stack)

            Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,e,edge_color = 'red',arrows=False))
            plt.show(block=False)
            resetLabels(Edge_label_artist)
            Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.5,edge_labels={(u,v):adjacency_matrix[u][v] for u, v in graph.edges()})


            '''show on graph'''
            result = lab4.PruferEncode(spanning_tree)
            code = result[0]
            w = result[1]
            message("Код Прюфера: ")
            print(str(code))
            message("Веса: ")
            print(str(w))
            check =  lab4.PruferDecode(code, w)

            message("Проверка: ")
            print(np.array(check))
            if (check == spanning_tree):
                success("Кодирование верно")
                alert("Неверное кодирование")



        elif (choice == CHOICES.PRIM.value):
            
            result = lab4.Prim(undirected)
            spanning_tree = result
            e = []
            for i in range (num_vertices):
                for j in range (num_vertices):
                    if (adjacency_matrix[i][j] and spanning_tree[i][j]):
                        e.append((i,j))


            resetEdgeLayer(Edge_artist_stack)
            Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,e,edge_color = 'red',arrows=False))
            plt.show(block=False)
            resetLabels(Edge_label_artist)
            Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.5,edge_labels={(u,v):adjacency_matrix[u][v] for u, v in graph.edges()})
            
            '''show on graph'''
            result = lab4.PruferEncode(spanning_tree)
            code = result[0]
            w = result[1]
            message("Код Прюфера: ")
            print(str(code))
            message("Веса: ")
            print(str(w))
            check =  lab4.PruferDecode(code, w)

            message("Проверка: ")
            print(np.array(check))
            if (check == spanning_tree):
                success("Кодирование верно")
                alert("Неверное кодирование")



        elif (choice == CHOICES.KIRCHHOFF.value):

            laplaceMatrix = [[0 for i in range(num_vertices)] for j in range (num_vertices)]
            for i in range(num_vertices):
                for j in range(num_vertices):
                    if (adjacency_matrix[i][j]):
                        laplaceMatrix[i][i] += 1
                        laplaceMatrix[i][j] = -1
                        laplaceMatrix[j][i] = -1
            message("Матрица Лапласа:")
            print(np.matrix(laplaceMatrix))
            print("По теореме Киргхоффа, число остовных деревьев равно любому (i,i) кофактору матрицы Лапласа")
            result = lab4.Kirchhoff(laplaceMatrix)
            message("Число дерев: %d"%result)


        
        elif (choice == CHOICES.EULER_PATH.value):

            result = lab5.checkEuler(undirected)
            is_Euler = result[0]
            eulerian_extended_edges = result[2]
            eulerian_matrix = deepcopy(undirected)
            labels = {(u,v):adjacency_matrix[u][v] for u, v in graph.edges()}

            if (is_Euler):
                success("Эйлеровым")
            else:
                alert("Не эйлеровым. Модификация:")
                print(np.array(result[1]))
                '''make an adj matrix for eulerian extension'''
                for edge in eulerian_extended_edges:
                    eulerian_matrix[edge[0]][edge[1]] = eulerian_extended_edges[edge]
                    eulerian_matrix[edge[1]][edge[0]] = eulerian_extended_edges[edge]
                '''visual'''

                resetEdgeLayer(Edge_artist_stack)
                Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,list(eulerian_extended_edges.keys()),edge_color = 'orange',arrows=False))
                labels.update(eulerian_extended_edges)

            resetLabels(Edge_label_artist)
            
            Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.5,edge_labels=labels)
            plt.show(block=False)


        elif (choice == CHOICES.HAMILTON_PATH.value):
            
            result = lab5.checkHamilton(undirected)

            is_Hamilton = result[0]
            hamiltonian_extended_edges = result[2]
            hamiltonian_matrix = deepcopy(undirected)
            labels = {(u,v):adjacency_matrix[u][v] for u, v in graph.edges()}

            if (is_Hamilton):
                success("Гамильтоновым")
            else:
                alert("Не гамильтоновым. Модификация:")
                print(np.array(result[1]))
                '''make an adj matrix for hamiltonian extension'''
                for edge in hamiltonian_extended_edges:
                    hamiltonian_matrix[edge[0]][edge[1]] = hamiltonian_extended_edges[edge]
                    hamiltonian_matrix[edge[1]][edge[0]] = hamiltonian_extended_edges[edge]

                '''visual'''
                resetEdgeLayer(Edge_artist_stack)
                Edge_artist_stack.append(nx.draw_networkx_edges(graph,pos,list(hamiltonian_extended_edges.keys()),edge_color = 'orange',arrows=False))
                labels.update(hamiltonian_extended_edges)

            resetLabels(Edge_label_artist)
            Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.5,edge_labels=labels)
            plt.show(block=False)


        elif (choice == CHOICES.CYCLE.value):

            if (is_Euler == None):
                ## modify if graph is not determined as eulerian yet. 
                message("Эйлеровность пока не определяют. Определяем:")
                result = lab5.checkEuler(undirected)
                is_Euler = result[0]
                eulerian_matrix = result[1]
                eulerian_extended_edges = result[2]

                if (is_Euler):
                    success("Эйлеровым")
                else:
                    alert("Не эйлеровым. Модификация:")
                    print(np.array(result[1]))
            
            path = lab5.Hierholzer(eulerian_matrix)
            # path : (u,v): index
            message("Цикл Euler:")
            print(np.array(list(path.keys())))

            '''visual'''
            resetEdgeLayer(Edge_artist_stack)

            Edge_artist_stack.append(nx.draw_networkx_edges(graph, pos,path.keys(),edge_color = 'red', arrows = True)) ## arrow to show the path

            resetLabels(Edge_label_artist)
            Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.5,edge_labels=path)

            plt.show(block=False)


        elif (choice == CHOICES.SALESMAN.value):
            if (is_Hamilton == None):
                ## modify if graph is not determined as Hamiltonian yet. 
                message("Галмильтоновность пока не определяют. Определяем:")
                result = lab5.checkHamilton(undirected)
                is_Hamilton = result[0]
                hamiltonian_matrix = result[1]
                hamiltonian_extended_edges = result[2]

                if (is_Hamilton):
                    success("Гамильтоновым")
                else:
                    alert("Не гамильтоновым. Модификация:")
                    print(np.array(hamiltonian_matrix))
            file_path = "hamilton.txt"
            res = lab5.AllHamiltonCyle(hamiltonian_matrix, file_path)
            w = res[0]
            cycle = res[1]
            # path : (u,v): index
            message("Решение (цикл):")
            print(cycle)

            path = {(cycle[i],cycle[i+1]): hamiltonian_matrix[cycle[i]][cycle[i+1]] for i in range(len(cycle)-1)}
            message("Вес: "+str(w))

            success("Процесс решения написан в файле " + file_path)
            '''visual'''
            resetEdgeLayer(Edge_artist_stack)
            Edge_artist_stack.append(nx.draw_networkx_edges(graph, pos,path.keys(),edge_color = 'red', arrows = True)) ## arrow to show the path

            resetLabels(Edge_label_artist)
            Edge_label_artist = nx.draw_networkx_edge_labels(graph,pos,label_pos=0.5,edge_labels=path)

            plt.show(block=False)



        elif (choice == CHOICES.NEWGRAPH.value):
            '''cache memory'''
            plt.clf()
            #adjacency matrix
            adjacency_matrix = []
            capacity_matrix = []
            undirected = []
            tocycle_edges = []

            graph = None
            pos = None
            #solution for Floyd - Warshall. 
            #Need to save it bc this algo costs alot more than two others.
            FW_cache = []
            
            #max flow by Ford-Fulkerson
            max_flow = 0
            
            # is Euler?
            is_Euler = None
            eulerian_extended_edges = {}
            eulerian_matrix = []

            # is Hamilton?
            is_Hamilton = None
            hamiltonian_extended_edges = {}
            hamiltonian_matrix = []

            #artist object to control items:
            Edge_label_artist = None
            Edge_artist_stack = []
            Edge_added_stack = []
            ''''''
            '''VIEW MODE FLAG'''
            directed_view = True
            continue
        
        
        elif (choice == CHOICES.EXITCODE.value):
            EXIT=True
            break