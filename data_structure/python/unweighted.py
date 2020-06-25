'''
Testing:
['3', '1'] cost 1
['3', '1', '2'] cost 2
['3', '1', '4'] cost 2
['3', '1', '2', '5'] cost 3
['3', '6'] cost 1
['3', '1', '4', '7'] cost 3
'''

class Queue:
    def __init__(self):
        self.items = []

    def isEmpty(self):
        return self.items == []

    def enqueue(self,value):
        self.items.insert(0,value)

    def dequeue(self):
        return self.items.pop()

    def size(self):
        return len(self.items)

class Vertex:
    def __init__(self,key):
        self.distance = 0
        self.predecessor = None
        self.color = 'white'
        self.ID = key
        self.connectedTo = {}

    def __str__(self):
        return str(self.ID) + ' connected to ' + str([x.ID for x in self.connectedTo])

    def setDistance(self,value):
        self.distance = value

    def setPred(self,key):
        self.predecessor = key

    def setColor(self,color):
        if color in ['white','gray','black']:
            self.color = color
        else:
            print("Only white, gray and black are avalialbe.")

    def addNeighbor(self,nbr,weight = 0):
        self.connectedTo[nbr] = weight

    def dropNeighbor(self,nbr):
        try:
            self.connectedTo.pop(nbr)
        except Exception:
            print("No key named {}.".format(nbr))

    def getConnections(self):
        return self.connectedTo.keys()

    def getWeight(self,nbr):
        return self.connectedTo[nbr]



class Graph:
    def __init__(self):
        self.vertDict = {}
        self.vertNumber = 0

    def addVertex(self,key):
        newVertex = Vertex(key)
        self.vertDict[key] = newVertex
        self.vertNumber += 1
        return newVertex

    def getVertex(self,key):
        try:
            return self.vertDict[key]
        except Exception:
            return None

    def getVertices(self):
        return self.vertDict.keys()

    def addEdge(self,start,end,cost = 0):
        if start not in self.vertDict.keys():
            temp = self.addVertex(start)
        if end not in self.vertDict.keys():
            temp = self.addVertex(end)

        self.vertDict[start].addNeighbor(self.vertDict[end],cost)

def new_graph(content):
    graph = Graph()
    for start,end in content:
        graph.addEdge(start,end)

    return graph

def bfs(graph,start,end):
    start.setDistance(0)
    start.setPred(None)
    vertQueue = Queue()
    vertQueue.enqueue(start)

    while not vertQueue.isEmpty():
        currentVertex = vertQueue.dequeue()
        for nbr in currentVertex.getConnections():
            if nbr.color == 'white':
                # print(nbr.ID)
                nbr.setColor('gray')
                nbr.setDistance(currentVertex.distance + 1)
                nbr.setPred(currentVertex)
                vertQueue.enqueue(nbr)

        currentVertex.setColor('black')

    temp = end.ID
    way = []
    while temp != start.ID:
        way.insert(0,temp)
        temp = graph.vertDict[temp].predecessor.ID
    way.insert(0,temp)

    return end.distance,way

def test_graph():
    content = [
        ('1','2'),
        ('1','4'),
        ('2','4'),
        ('2','5'),
        ('3','1'),
        ('3','6'),
        ('4','3'),
        ('4','5'),
        ('4','6'),
        ('4','7'),
        ('5','7'),
        ('7','6')
    ]

    graph = new_graph(content)
    # print(graph.vertDict['1'])
    start = '3'
    for key in graph.vertDict:
        if key != start:
            cost,way = bfs(graph,graph.vertDict[start],graph.vertDict[key])
            print('{} cost {}'.format(way,cost))

if __name__ == '__main__':
    test_graph()

