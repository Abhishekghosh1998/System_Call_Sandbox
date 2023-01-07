import sys
import angr


exec_name=sys.argv[1]

proj=angr.Project(exec_name, auto_load_libs=False)

cfg=proj.analyses.CFGFast(normalize=True)

import os
script_dir = os.path.dirname(__file__) #<-- absolute dir the script is in
rel_database_path = "database"
abs_database_path = os.path.join(script_dir, rel_database_path)

proj1=angr.Project(abs_database_path,  auto_load_libs=False)

cfg1=proj1.analyses.CFGFast()



functionlist=cfg1.kb.functions
functionObjList=[]
functionDict=dict(functionlist)
for addr in functionDict:
  functionObjList.append(functionDict[addr])
functionNames=[]
for f in functionObjList:
  functionNames.append(f.name)
functionNames.remove('main')


abs_libc_path=os.path.join(script_dir,"libc.txt")
f=open(abs_libc_path,"r")
libcFunctionNames=[]
for line in f:
  libcFunctionNames.append(line.split(":")[0][1:])
libcFunctionNames
libcFunctionNames.remove('main')


cfg_nodes=cfg.nodes()
#len(cfg_nodes)



entry_point=proj.entry
#print(hex(entry_point))



functions=cfg.kb.functions
#print(functions)


# In[14]:


def driver(fname):
    blk=cfg.get_node(functions[fname].addr)
    s=exploreBlock_2(blk)
    processNodeLabels()


def getFname(blk):
  if blk.name==None:
    return blk.addr
  return blk.name.split("+")[0]


from graphviz import Digraph
g=Digraph('G',filename=exec_name+".dot", graph_attr={'rankdir':'LR'})
g.attr('node', shape='circle', color='black')#, style ="filled")



g.node(str(functions['main'].addr),label="")


def addEdge(x,y,l):
    x=str(x)
    y=str(y)
    l=str(l)
    if (x,y,l) not in EdgeList:
        g.edge(x,y,label=l)
        EdgeList.add((x,y,l))

def processNodeLabels():
    for e in blks_to_add_labels:
        g.node(str(e[0]),label=str(e[1]))


visitedBlocks=set()
EdgeList=set()
blks_to_add_labels=[]

def exploreBlock_2(blk): 
  # receives a cfg
  # if this node is being visited for the first time, then it generates the corresponding 
  #graph nodes as needed and returns the start node identifier which should be used be used 
  #by some other node using it
    
    print("Exploring Block ",blk)
  #if blk.name is None:
  #    return (False, None, None)

    if blk.addr in visitedBlocks: # here where you shall use the color concept to get loop edges
        #print (str(blk)+" already visited !!!\n")
        return
    visitedBlocks.add(blk.addr)
    
    successors=blk.successors
    
    if(len(successors)==1 
       and successors[0].name is not None 
       and len(successors[0].name.split("+"))==1 
       and getFname(blk) in functions 
       and functions[getFname(blk)].get_call_target(blk.addr)==successors[0].addr):
        #function call or system call
        blk2=None
        blklist=[x.addr for x in list(functions[getFname(blk)].blocks)]
        blklist.sort()
        index=blklist.index(blk.addr)+1
        if index < len(blklist):
            blk2=cfg.get_node(blklist[index])
            exploreBlock_2(blk2)

        if ( (getFname(blk) not in functionNames) 
            and getFname(blk) not in libcFunctionNames 
            and (successors[0].is_syscall 
                 or successors[0].name in functionNames 
                 or successors[0].name in libcFunctionNames)
           and blk2 is not None):
            blks_to_add_labels.append((blk.addr,blk2.addr)) #adding return address
        
        if successors[0].is_syscall:
            #blk makes a syscall
            #we need to get the block to which blk returns after system call
            startNode=str(blk.addr)
            endNode=str(blk.addr)+".2"
            if blk2 is not None:
                endNode=str(blk2.addr)
            addEdge(startNode, endNode, successors[0].name )
           
        else:
            #usual function call or jump to blk of some other function
            startNode=str(blk.addr)
            endNode=str(successors[0].addr)
            addEdge(startNode, endNode, "epsilon")
            exploreBlock_2(successors[0])
            
    else:
        startNode=str(blk.addr)
        for s in successors:
            endNode=str(s.addr)
            addEdge(startNode, endNode, "epsilon" )
            exploreBlock_2(s)
    print("Finished Exploring Block", blk)



import sys
sys.setrecursionlimit(100000)
driver('main')


g.save()

