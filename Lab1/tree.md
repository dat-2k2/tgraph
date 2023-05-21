Tree- [Definition](#definition)
- [Definition](#definition)
  - [Forest](#forest)
  - [Tree](#tree)
- [Properties](#properties)
  - [Center of tree](#center-of-tree)
- [DiTree](#ditree)
  - [Definition](#definition-1)



# Definition
## Forest
- Graph without cycle
## Tree
- If a graph is connnected, it's called a **free** tree
=> Component of a forest - trees

# Properties
- z(G) (#cycle) = 0
- If connecting 2 nonadjacent vertices makes z(G+x) = 1, the graph G is called **subacyclic**

Defined *p* - num of vertices, *q* - num of edges, *k* - connecting components, *z* - num of cycles
 
- $ k(G) = 1, z(G) = 0 $
- $ \forall (u,v) \vert \#(u,v)\vert = 1 $
- $ k(G) = 1 $
- if G is a tree, so $ k(G) = 1$ & $\forall e \in E, \space k(G-e) = 1$
- ... 
watch more https://tema.spbstu.ru/userfiles/files/courses/2019-theory_graph/TG10.pdf


## Center of tree
- from 1 vertex or 2 connected vertices

# DiTree
## Definition
DiGraph with below properties:
- $\exist !$ node r, $deg^+(r)=0$, called **root** of graph 
- $\forall v \in V - root, ( d^+(v) = 1 )$
