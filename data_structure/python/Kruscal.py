'''
Testing:
In:
content = [
        (5,('A','D')),
        (7,('A','B')),
        (9,('B','D')),
        (7,('B','E')),
        (15,('D','E')),
        (6,('D','F')),
        (11,('F','G')),
        (8,('F','E')),
        (9,('E','G')),
        (8,('B','C')),
        (5,('C','E'))
    ]

Out:
[('A', 'D'), ('C', 'E'), ('D', 'F'), ('B', 'E'), ('A', 'B'), ('E', 'G')]
'''

class TreeNode:
    def __init__(self,key):
        self.parent = None
        self.key = key
        self.lChild = None
        self.rChild = None
        self.isRoot = True

    def setParent(self,node):
        self.parent = node
        self.isRoot = False

class PriorityQueue:
    def __init__(self):
        self.heapArray = [(0,(0,0))]
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
            if self.heapArray[i*2][0] > self.heapArray[i*2+1][0]:
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

    def update(self,cost,vertecies):
        done = False
        key = 0
        i = 1
        while not done and i <= self.pqSize:
            if self.heapArray[i][1] == vertecies:
                key = i
                done = True
            else:
                i += 1

        if key:
            self.heapArray[key] = (cost,self.heapArray[key][1])
            self._percDown(key)

    def pop(self):
        returnValue = self.heapArray[1][1]
        returnDistance = self.heapArray[1][0]
        self.heapArray[1] = self.heapArray[-1]
        self.pqSize -=1
        self.heapArray.pop()
        self._percDown(1)
        return returnValue,returnDistance

    def isEmpty(self):
        return self.pqSize == 0


def newPq(content):
    pq = PriorityQueue()

    for items in content:
        pq.insert(items)

    return pq

def quick_uniton(items):
    tree = {}
    done = []
    for item in items:
        for key in item:
            if key not in done:
                tree[key] = TreeNode(key)
                done.append(key)
        key = item[0]
        while not tree[key].isRoot:
            key = tree[key].parent.key
        tree[key].setParent(tree[item[1]])

    key = done[0]
    while not tree[key].isRoot:
        key = tree[key].parent.key

    for i in done:
        key_temp = i
        while not tree[key_temp].isRoot:
            key_temp = tree[key_temp].parent.key
        if key_temp != key:
            return False

    return True

def kruscal(content,vertNum):
    pq = newPq(content)
    way = []
    way_parse = []

    while not pq.isEmpty():
        edge, dist = pq.pop()
        if dist == float('inf'):
            break
        vert1 = edge[0]
        vert2 = edge[1]
        flag1 = False
        flag2 = False

        for edges in way:
            if edges == vert1:
                flag1 = True
            elif edges == vert2:
                flag2 = True

            if flag1:
                for vert in way:
                    if (vert2,vert) in pq:
                        pq.update(float('inf'),(vert2,vert))
                    if (vert,vert2) in pq:
                        pq.update(float('inf'), (vert, vert2))

            elif flag2:
                for vert in way:
                    if (vert2,vert) in pq:
                        pq.update(float('inf'),(vert1,vert))
                    if (vert,vert2) in pq:
                        pq.update(float('inf'), (vert, vert1))

        way_parse.append(edge)
        for vert in edge:
            way.append(vert)

        if quick_uniton(way_parse) and len(set(way)) == vertNum:
            break

    return way_parse

def test_kruscal():
    content = [
        (5,('A','D')),
        (7,('A','B')),
        (9,('B','D')),
        (7,('B','E')),
        (15,('D','E')),
        (6,('D','F')),
        (11,('F','G')),
        (8,('F','E')),
        (9,('E','G')),
        (8,('B','C')),
        (5,('C','E'))
    ]
    vertNum = 7

    way = kruscal(content,vertNum)
    print(way)

if __name__ == '__main__':
    test_kruscal()