import sys
import numpy as np
sys.path.append('../Lab2/')
from Lab2 import lab2
from copy import deepcopy
'''Ford-Fulkerson'''
'''take an empty matrix F to store the flow result, function return maximum flow'''


def dflow_path(F, C, prev, sink):
    end = sink
    dflow = float('inf')
    while (prev[end] != None):
        if (C[prev[end]][end] - F[prev[end]][end] < dflow):
            dflow = C[prev[end]][end] - F[prev[end]][end]
        end = prev[end]
    return dflow

NOTFOUND = None

def EdmondKarps(G, src, sink, C):
    '''g - adj matrix'''
    '''F - adj flow'''
    '''init flow matrix'''
    f_matrix = []
    for i in range (len(G)):
        tmp = []
        for j in range (len(G)):
            tmp.append(0)
        f_matrix.append(tmp)
    '''C - capac'''
    total_flow = 0
    print(np.array(C))
    '''main body'''
    while (True):

        '''bfs'''
        prev = []
        for i in range (len(G)):
            prev.append(NOTFOUND)
        q = []
        q.append(src)
        while (len(q)):
            cur = q.pop(0)
            for neighbor in range(len(G)):
                if (G[cur][neighbor]):
                    if (prev[neighbor] == NOTFOUND and neighbor != src and C[cur][neighbor] > f_matrix[cur][neighbor]):
                        prev[neighbor] = cur
                        q.append(neighbor)

                        
        '''path found'''
        if (prev[sink] != NOTFOUND):
            '''find '''
            df = dflow_path(f_matrix, C, prev, sink)
            '''add path flow to total flow'''
            total_flow += df

            '''update flow'''
            cur = sink
            while (cur!=src):
                f_matrix[prev[cur]][cur]+=df
                f_matrix[cur][prev[cur]]-=df
                cur = prev[cur]
            
        else:
            '''break if no path is found'''
            break
    return total_flow, f_matrix



def flow_path(C, prev, sink):
    end = sink
    flow = float('inf')
    while (prev[end] != None):
        if (C[prev[end]][end] < flow):
            flow = C[prev[end]][end]
    return flow



def mincostflow(G_inp, C_inp, src, sink, threshold):
    G = deepcopy(G_inp)
    Available = deepcopy(C_inp)
    '''init flow matrix'''
    F = []
    for i in range (len(G)):
        tmp = []
        for j in range (len(G)):
            tmp.append(0)
        F.append(tmp)


    tmp = lab2.BellmanFord(G,src)
    prev = tmp[1]
    neg_cyc = tmp[3]
    total_flow = 0

    if (neg_cyc):
        print('neg_cyc')
        return NOTFOUND, F, "отр. цикл"

    while (prev[sink] != None):

        '''if found, assume that the path is intense so we can increase total flow
        up to a certain amount'''

        df = dflow_path(F, Available, prev, sink)
    
        '''update sent flow'''
        cur = sink
        while (prev[cur] != None):
            F[prev[cur]][cur] += df
            cur = prev[cur]

        total_flow += df
        if (total_flow >= threshold):
            cost = 0
            for i in range(len(F)):
                for j in range(len(F)):
                        if (F[i][j]):
                            cost += F[i][j] * G[i][j]
            return total_flow, F, cost 
        
        '''if current flow isn't the wanted one then update'''
        
        '''update flow'''
        for i in range (len(F)):
            for j in range (len(F)):
                '''rule 3'''
                if (F[i][j]):
                    G[i][j] = -G[j][i]
                    Available[i][j] = Available[j][i]
                    if (F[j][i] == 0):
                        if (Available[i][j]):
                            '''rule 4'''
                            Available[i][j] -= F[i][j]
                        else:
                            '''rule 5'''
                            G[i][j] = 0
                
        '''reset'''
        tmp = lab2.BellmanFord(G,src)
        prev = tmp[1]
        neg_cyc = tmp[3]

    return NOTFOUND, F, "макс. поток найден: "+str(total_flow)
