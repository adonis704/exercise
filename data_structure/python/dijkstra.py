'''
Testing:
In:
    content = [
        ('1','2',10),
        ('1','4',20),
        ('2','4',5),
        ('2','5',100), #set cost to 100 so that the way should not be choosen.
        ('3','1',5),
        ('3','6',20),
        ('4','3',10),
        ('4','5',5),
        ('4','6',10),
        ('4','7',20),
        ('5','7',30),
        ('7','6',10)
    ]
Out:
['3', '1'] cost 5
['3', '1', '2'] cost 15
['3', '1', '2', '4'] cost 20
['3', '1', '2', '4', '5'] cost 25
['3', '6'] cost 20
['3', '1', '2', '4', '7'] cost 40
'''

class PriorityQueue:
    def __init__(self):
        self.heapArray = [(0,0)]
        self.pqSize = 0

    def __contains__(self, item):
        for i in self.heapArray:
            if i[1] == item:
                return True
        return None

    def newHeap(self,list):
        self.pqSize = len(list)
        self.heapArray = [(0,0)]
        for i in list:
            self.heapArray.append(i)

        i = len(list) //2
        while i>0:
            self._percDown(i)
            i -= 1

    def insert(self,tuple):
        self.heapArray.append(tuple)
        self.pqSize +=1
        self._percUp(self.pqSize)

    def _minChlid(self,i):
        if i*2 + 1 > self.pqSize:
            return i*2
        else:
            if self.heapArray[i * 2][0] > self.heapArray[i * 2 + 1][0]:
                return i*2+1
            else:
                return i*2

    def _percDown(self,i):
        while(i*2 <= self.pqSize):
            mc = self._minChlid(i)
            if self.heapArray[i][0] > self.heapArray[mc][0]:
                temp = self.heapArray[mc]
                self.heapArray[mc] = self.heapArray[i]
                self.heapArray[i] = temp
            i = mc

    def _percUp(self,i):
        while i // 2 > 0:
            if self.heapArray[i][0] < self.heapArray[i // 2][0]:
                temp = self.heapArray[i // 2]
                self.heapArray[i // 2] = self.heapArray[i]
                self.heapArray[i] = temp
            i = i // 2

    def update(self,vertex,distance):
        done = False
        key = 0
        i = 1
        while not done and i <= self.pqSize:
            if self.heapArray[i][1] == vertex:
                key = i
                done = True
            else:
                i += 1

        if key:
            self.heapArray[key] = (distance,self.heapArray[key][1])
            self._percUp(key)

    def pop(self):
        returnValue = self.heapArray[1][1]
        self.heapArray[1] = self.heapArray[-1]
        self.pqSize -=1
        self.heapArray.pop()
        self._percDown(1)
        return returnValue

    def isEmpty(self):
        return self.pqSize == 0

class Vertex:
    def __init__(self,key):
        self.distance = float('inf')
        self.predecessor = None
        self.color = 'white'
        self.ID = key
        self.connectedTo = {}

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

def newGraph(content):
    g = Graph()
    for start,end,cost in content:
        g.addEdge(start,end,cost)

    return g

def dijkstra(g,start,end):
    start.setDistance(0)
    start.setPred(None)
    pq = PriorityQueue()
    pq.insert((start.distance,start))

    while not pq.isEmpty():
        curVert = pq.pop()
        for vert in curVert.getConnections():
            temp = curVert.distance + curVert.connectedTo[vert]
            if vert not in pq and vert.color != 'black':
                pq.insert((vert.distance,vert))
            if vert.distance > temp:
                vert.setDistance(temp)
                vert.setPred(curVert)
                pq.update(vert,temp)
                # print('Vert {} updated to {}'.format(vert.ID,temp))
                # print(pq.heapArray)
        curVert.setColor('black')

    temp = end.ID
    way = []
    while temp != start.ID:
        way.insert(0,temp)
        temp = g.vertDict[temp].predecessor.ID
    way.insert(0,temp)

    return way, end.distance

def test_dijkstra():
    content = [
        ('1','2',10),
        ('1','4',20),
        ('2','4',5),
        ('2','5',100),
        ('3','1',5),
        ('3','6',20),
        ('4','3',10),
        ('4','5',5),
        ('4','6',10),
        ('4','7',20),
        ('5','7',30),
        ('7','6',10)
    ]

    g = newGraph(content)

    start = '3'
    for key in g.vertDict:
        if key != start:
            way,cost = dijkstra(g,g.vertDict[start],g.vertDict[key])
            print('{} cost {}'.format(way,cost))

if __name__ == '__main__':
    test_dijkstra()




