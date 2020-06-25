import tkinter as tk
from tkinter import ttk
from tkinter import messagebox

OK = 0
NOT_OK = 1
GUEST = 2
VALUE_VALID = "■"
VALUE_SPACE = "□"
VALUE_START = "▣"
VALUE_USED = "◈"

IMAGE_MAP = {
    "left_left": "═",
    "left_up": "╚",
    "left_down": "╔",
    "right_up": "╝",
    "right_down": "╗",
    "right_right": "═",
    "up_up": "║",
    "up_left": "╗",
    "up_right": "╔",
    "down_down": "║",
    "down_left": "╝",
    "down_right": "╚",
    "down": '▼',
    "up": '▲',
    "left": '◀',
    "right": '▶',
    "end": '■',
}

class Game:
    def __init__(self,arr,end = None):
        self._rows = len(arr)
        self._cols = len(arr[0])
        self._array = arr
        self._start = None
        self._tmpStart = None
        self._tmpEnd = end
        self.length = 0  # the number of avaliable nodes
        self.nodeList = []  # not passed nodes
        self.endList = []
        self.passList = []  # passed nodes
        self._procInit()
        if self._tmpEnd:
            self.endList.append(end)

    def _copy(self):
        copyArr = []
        for row in self._array:
            copyArr.append(row.copy())
        return copyArr

    def _procInit(self):
        for x in range(self._rows):
            line = ""
            for y in range(self._cols):
                v = self._array[x][y]
                line += str(v)
                if v == VALUE_VALID:
                    self.nodeList.append((x, y))
                    self.length += 1
                if v == VALUE_START:
                    self.nodeList.append((x, y))
                    self.length += 1
                    self._start = (x, y)
                    self._tmpStart = (x, y)
                    self.passList = [(x, y)]
                    self.nodeUsed((x, y))
                    
    def nodeUsed(self, node):
        self._array[node[0]][node[1]] = VALUE_USED
        self.nodeList.remove(node)

    def nodeUsedStart(self, node):
        self.nodeUsed(node)
        self.passList.append(node)
        self._tmpStart = node

    def nodeUsedend(self, node):
        self.nodeUsed(node)
        self.endList.append(node)
        self._tmpEnd = node
        
    def getNeighbors(self, node):
        neighbors = []
        if (node[0] - 1) >= 0 and self._array[node[0] - 1][node[1]] == VALUE_VALID:
            neighbors.append(["left", (node[0] - 1, node[1])])
        if (node[0] + 1) < self._rows and self._array[node[0] + 1][node[1]] == VALUE_VALID:
            neighbors.append(["right", (node[0] + 1, node[1])])
        if (node[1] - 1) >= 0 and self._array[node[0]][node[1] - 1] == VALUE_VALID:
            neighbors.append(["up", (node[0], node[1] - 1)])
        if (node[1] + 1) < self._cols and self._array[node[0]][node[1] + 1] == VALUE_VALID:
            neighbors.append(["down", (node[0], node[1] + 1)])
        return neighbors

    def isNeighbor(self,node1, node2):
        if (node1[0] == node2[0] and abs(node1[1] - node2[1]) == 1) or (
                node1[1] == node2[1] and abs(node1[0] - node2[0]) == 1):
            return True
        return False
    
    def result(self, extend=None):
        if extend:
            self.passList += extend
        if self.endList and len(self.passList) < self.length:
            self.endList.reverse()
            for node in self.endList:
                if self.passList.count(node) == 0:
                    self.passList.append(node)
        return self.passList
    
    def resultReturn(self):
        self.result()
        resultArr = self._array.copy()
        nodeStart = self.passList[0]
        startArrow = self.nodeToString(nodeStart, self.passList[1])
        resultArr[nodeStart[0]][nodeStart[1]] = IMAGE_MAP[startArrow]
        node_end = self.passList[-1]
        resultArr[node_end[0]][node_end[1]] = IMAGE_MAP["end"]
        for i in range(1, len(self.passList) - 1):
            node0 = self.passList[i - 1]
            node1 = self.passList[i]
            node2 = self.passList[i + 1]
            arrow = "%s_%s" % (self.nodeToString(node0, node1), self.nodeToString(node1, node2))
            x, y = node1
            resultArr[x][y] = IMAGE_MAP.get(arrow, VALUE_SPACE)
        return resultArr
    
    def nodeToString(self, node1, node2):
        if node1[0] + 1 == node2[0]:
            return "down"
        elif node1[0] - 1 == node2[0]:
            return "up"
        elif node1[1] + 1 == node2[1]:
            return "right"
        elif node1[1] - 1 == node2[1]:
            return "left"
        return "end"
    
    def procStart(self):
        neighbors = self.getNeighbors(self._tmpStart)
        if len(neighbors) == 1:
            neighborNode = neighbors[0][1]
            self.nodeUsedStart(neighborNode)
            self.procStart()

    def procEnd(self):
        if self._tmpEnd:
            neighbors = self.getNeighbors(self._tmpEnd)
            if len(neighbors) == 1:
                neighborNode = neighbors[0][1]
                self.nodeUsedend(neighborNode)
                self.procEnd()
                
    def process(self):
        self.procStart()
        self.procEnd()

        for node in self.nodeList:
            neighbors = self.getNeighbors(node)
            if len(neighbors) == 1:
                neighborNode = neighbors[0][1]
                if not self._tmpEnd:
                    if not self.isNeighbor(node, self._tmpStart):
                        self.nodeUsedend(node)
                        self.nodeUsedend(neighborNode)
                        return self.process()
                    else:
                        pass

                elif self.isNeighbor(node, self._tmpStart) and not self.isNeighbor(node, self._tmpEnd):
                    self.nodeUsedStart(node)
                    self.nodeUsedStart(neighborNode)
                    return self.process()

                elif not self.isNeighbor(node, self._tmpStart) and self.isNeighbor(node, self._tmpEnd):
                    self.nodeUsedend(node)
                    self.nodeUsedend(neighborNode)
                    return self.process()

                elif self.isNeighbor(node, self._tmpStart) and self.isNeighbor(node, self._tmpEnd):
                    pass

                else:
                    return NOT_OK
                
        if len(self.nodeList) == 0:
            if not self._tmpEnd:
                return OK

            elif self.isNeighbor(self._tmpEnd, self._tmpStart):
                self.passList = self.result()
                return OK

            else:
                return NOT_OK
            
        else:
            return GUEST
        
    def guess(self):
        start = self._tmpStart
        passFlag = False
        neighbors = self.getNeighbors(start)

        for neighbor in neighbors:
            tmpArr = self._copy()
            neighborNode = neighbor[1]
            tmpArr[neighborNode[0]][neighborNode[1]] = VALUE_START
            tmpArrNum = Game(tmpArr, end=self._tmpEnd)
            ret = tmpArrNum.process()
            if ret == OK:
                self.result(tmpArrNum.passList)
                passFlag = True
                break

            elif ret == GUEST:
                ret1 = tmpArrNum.guess()
                if ret1 == OK:
                    self.result(tmpArrNum.passList)
                    passFlag = True
                    break
                else:
                    del tmpArrNum
                    continue

            elif ret == NOT_OK:
                del tmpArrNum
                continue
                
        if passFlag:
            return OK
        else:
            return NOT_OK

    #FIXME
    def compute(self):
        ret = self.process()
        if ret == OK:
            result = self.resultReturn()
        elif ret == NOT_OK:
            result = '没有结果'
        else:
            ret1 = self.guess()
            if ret1 == OK:
                result = self.resultReturn()
            else:
                result = '没有结果'

        return result
                
class App:
    def __init__(self,root):
        self.wd = root
    
    def windowInit(self):
        self.wd.title('不走重复格子 By 罗成学')
        self.fir = ttk.Label(self.wd, text="请输入需要计算的图")
        self.fir.grid(row=0, column=0)
        self.sec = ttk.Label(self.wd, text="0代表不可经过方块，1代表可经过方块，2代表起点，每一行以回车结束")
        self.sec.grid(row=1, column=0)
        self.inText = tk.Text(self.wd,
                         width=40,
                         height=30,
                         )
        self.inText.grid(row=2, column=0)
        self.scroll = tk.Scrollbar(self.wd, command=self.inText.yview)
        self.scroll.grid(row=2, sticky=tk.N + tk.S + tk.E)
        self.inText.configure(yscrollcommand=self.scroll.set)

        self.thir = ttk.Label(self.wd, text='输入的图像')
        self.thir.grid(row=0, column=1)
        self.four = ttk.Label(self.wd, text="□代表不可经过方块，■代表可经过方块，▣代表起点")
        self.four.grid(row=1, column=1)
        self.outText1 = tk.Text(self.wd,
                          width=40,
                          height=30,
                          )
        self.outText1.grid(row=2, column=1)

        self.five = ttk.Label(self.wd, text='计算结果')
        self.five.grid(row=0, column=2)
        self.six = ttk.Label(self.wd, text="实心三角代表起点方向，双线代表路径，■代表终点")
        self.six.grid(row=1, column=2)
        self.outText2 = tk.Text(self.wd,
                          width=40,
                          height=30,
                          )
        self.outText2.grid(row=2, column=2)

        self.but1 = tk.Button(self.wd, text='确认', command=lambda: self._butClick1(self.inText, self.outText1,self.outText2))
        self.but1.grid(row=3, column=0)
        self.but2 = tk.Button(self.wd, text='清除', command=lambda: self._butClick2(self.inText, self.outText1,self.outText2))
        self.but2.grid(row=5, column=2)

    def _toArr(self,inText):
        text = inText.get(1.0, 'end')
        init = ''
        arr = []
        for i in text:
            init += i
        for i in init.split('\n'):
            if i:
                newRow = []
                for j in i:
                    newRow.append(int(j))
                arr.append(newRow)
        return arr

    def _toArrPic(self,arr):
        arrPic = []
        for row in arr:
            newRow = []
            for value in row:
                if value == 1:
                    newRow.append(VALUE_VALID)
                elif value == 0:
                    newRow.append(VALUE_SPACE)
                else:
                    newRow.append(VALUE_START)
            arrPic.append(newRow)
        return arrPic

    def _butClick1(self,inText, outText1,outText2):
        try:
            arr = self._toArr(inText)
            arrPic = self._toArrPic(arr)
        except:
            messagebox.showerror('Error', '输入不正确')
            self._butClick2(inText,outText1,outText2)

        outText1.delete(1.0, 'end')
        for row in arrPic:
            outText1.insert('insert',"".join(row) +'\n')
        try:
            game = Game(arrPic)
            result = game.compute()
            outText2.delete(1.0, 'end')
            if result == '没有结果':
                outText2.insert('insert',result)
            else:
                for row in result:
                    outText2.insert('insert', "".join(row)+'\n')
        except:
            messagebox.showerror('Error', '输入不正确')
            self._butClick2(inText,outText1,outText2)

    def _butClick2(self,inText, outText1,outText2):
        inText.delete(1.0, 'end')
        outText1.delete(1.0, 'end')
        outText2.delete(1.0, 'end')

    def run(self):
        self.wd.mainloop()

def test():
    wd = tk.Tk()
    gameTest = App(wd)
    gameTest.windowInit()
    gameTest.run()

if __name__ == '__main__':
    test()