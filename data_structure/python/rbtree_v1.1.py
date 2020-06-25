'''
Testing:
in:
    content = [
        (5,'a'),
        (7,'b'),
        (6,'c'),
        (4,'d'),
        (12,'e'),
        (22,'f'),
        (23,'g'),
        (8,'h'),
        (9,'i'),
        (13,'j'),
        (20,'k')
    ]
    rbtree = Rbtree()
    for key, val in content:
        rbtree.insert(key,val)
    print('Done.')
out:
insert 7 under 5 as RC.
insert 6 under 7 as LC.
RL
insert 4 under 5 as LC.
Colorflip
insert 12 under 7 as RC.
insert 22 under 12 as RC.
L
insert 23 under 22 as RC.
Colorflip
insert 8 under 7 as RC.
insert 9 under 8 as RC.
L
insert 13 under 22 as LC.
insert 20 under 13 as RC.
Colorflip
L
Done.
(The process is exactly the same as the one shown in https://www.cs.usfca.edu/~galles/visualization/RedBlack.html,
which is an online visual data struture website.)

in:
    node = rbtree.search(8)
    print('Key {} has val {}'.format(node.key,node.val))
out:
    Key 8 has val h

in:
    rbtree.remove(8)
    node = rbtree.search(8)
    print(node)
out:
    None

in:
    rbtree.insert(8,'old')
    rbtree.update(8,'new')
    node = rbtree.search(8)
    print('Key {} has val {}'.format(node.key,node.val))
out:
    Key 8 has val new
'''

class TreeNode:
    def __init__(self,key,val = None):
        self.key = key
        self.val = val
        self.color = 'RED'
        self.parent = None
        self.aunt = None
        self.grdpar = None
        self.brother = None
        if self.key != None:
            self.lchild = TreeNode(None)
            self.rchild = TreeNode(None)
            self.rchild.setPar(self)
            self.lchild.setPar(self)
            self.lchild.setCol('BLACK')
            self.rchild.setCol('BLACK')


    def hasLC(self):
        return not self.lchild.key == None

    def hasRC(self):
        return not self.rchild.key == None

    def hasPar(self):
        return not self.parent == None

    def setPar(self,node):
        self.parent = node
        if node != None:
            if self.isLT():
                self.brother = node.rchild
            else:
                self.brother = node.lchild
            if node.hasPar():
                self.grdpar = node.parent
                if node.isLT():
                    self.aunt = node.parent.rchild
                if node.isRT():
                    self.aunt = node.parent.lchild

    def setLC(self,node):
        del self.lchild
        self.lchild = node
        node.setPar(self)


    def setRC(self,node):
        del self.rchild
        self.rchild = node
        node.setPar(self)

    def setCol(self,col):
        self.color = col

    def setVal(self,val):
        self.val = val

    def setKey(self,key):
        self.key = key

    def isRT(self):
        if self.hasPar():
            if self.parent.lchild == self:
                return False
            if self.parent.rchild == self:
                return True
        else:
            return False
    def isLT(self):
        if self.hasPar():
            if self.parent.lchild == self:
                return True
            if self.parent.rchild == self:
                return False
        else:
            return False
    def copy(self,node):
        self.setKey(node.key)
        self.setVal(node.val)

    def reinit(self):
        self.setCol("BLACK")
        self.setKey(None)
        self.setVal(None)
        del self.rchild
        del self.lchild

class Rbtree:
    def __init__(self):
        self.root = None

    def _update(self,node):
        node.setPar(node.parent)

    def _lrotation(self,node):
        if node.hasPar():
            par = node.parent
        else:
            par = None
        temp = node.rchild.lchild
        if par != None:
            if node.isLT():
                par.setLC(node.rchild)
            else:
                par.setRC(node.rchild)
        else:
            node.rchild.setPar(par)
            self.root = node.rchild
        node.rchild.setLC(node)
        node.setRC(temp)
        self._update(node)
        self._update(node.parent.rchild)
        #Maybe it need more _update

    def _rrotation(self,node):
        if node.hasPar():
            par = node.parent
        else:
            par = None
        temp = node.lchild.rchild
        if par != None:
            if node.isLT():
                par.setLC(node.lchild)
            else:
                par.setRC(node.lchild)
        else:
            node.lchild.setPar(par)
            self.root = node.lchild
        node.lchild.setRC(node)
        node.setLC(temp)
        self._update(node)
        self._update(node.parent.lchild)


    def _colorflip(self,node):
        if node.hasLC():
            node.lchild.setCol('BLACK')
        if node.hasRC():
            node.rchild.setCol('BLACK')
        if node.hasPar():
            node.setCol('RED')
            self._check(node)

    def _check(self,node):
        if not node.hasPar():
            node.setCol('BLACK')
        elif node.parent.color == 'RED' and node.color == 'RED':
            if node.aunt.color == 'RED':
                print('Colorflip')
                self._colorflip(node.grdpar)
            else:
                if node.isLT() and node.parent.isLT():
                    print('R')
                    self._rrotation(node.grdpar)
                    node.parent.setCol('BLACK')
                    node.parent.rchild.setCol('RED')
                elif node.isRT() and node.parent.isRT():
                    print('L')
                    self._lrotation(node.grdpar)
                    node.parent.setCol('BLACK')
                    node.parent.lchild.setCol('RED')
                elif node.isLT() and node.parent.isRT():
                    print('RL')
                    self._rrotation(node.parent)
                    self._lrotation(node.parent)
                    node.setCol('BLACK')
                    node.lchild.setCol('RED')
                else:
                    print('LR')
                    self._lrotation(node.parent)
                    self._rrotation(node.parent)
                    node.setCol('BLACK')
                    node.rchild.setCol('RED')

    def insert(self,key,val = None):
        node = TreeNode(key,val)
        if not self.root:
            self.root = node
            node.setCol('BLACK')
        else:
            curNode = self.root
            stack = [curNode]
            flag = False
            while stack and stack[-1].key != None:
                flag = False
                curNode = stack.pop()
                if key < curNode.key:
                    stack.append(curNode.lchild)
                    flag = True
                else:
                    stack.append(curNode.rchild)
            if  flag:
                curNode.setLC(node)
                print('insert {} under {} as LC.'.format(node.key,curNode.key,))
            else:
                curNode.setRC(node)
                print('insert {} under {} as RC.'.format(node.key,curNode.key,))
                node.setPar(curNode)

            self._check(node)

    def findSucc(self,node,pattern = 'RIGHT'):
        if pattern == 'RIGHT':
            stack = [node.rchild]
            while stack:
                curNode = stack.pop()
                if curNode.hasLC():
                    stack.append(curNode.lchild)
                else:
                    return curNode
        else:
            stack = [node.lchild]
            while stack:
                curNode = stack.pop()
                if curNode.hasRC():
                    stack.append(curNode.rchild)
                else:
                    return curNode

    def _inorder(self,key):
        stack = []
        node = self.root
        while stack or node.key != None:
            while node.key != None:
                stack.append(node)
                node = node.lchild
            node = stack.pop()
            temp = node.key
            if node.key == key:
                return node
            node = node.rchild
        return None

    def _goThro(self):
        stack = []
        node = self.root
        while stack or node.key != None:
            while node.key != None:
                stack.append(node)
                node = node.lchild
            node = stack.pop()
            self._update(node)
            node = node.rchild
        return None

    def _changeCol(self,node1,node2):
        temp = node1.color
        node1.setCol(node2.color)
        node2.setCol(temp)

    def search(self,key):
        try:
            node = self._inorder(key)
            return node
        except Exception:
            return None

    def update(self,key,val):
        node = self.search(key)
        if node:
            node.setVal(val)
        else:
            print('No node name {}'.format(key))

    def _replace(self,replace,replaced,case = '1'):
        if case == '1':
            if replaced.isLT():
                replaced.parent.setLC(replace)
                del replaced
            elif replaced.isRT():
                replaced.parent.setRC(replace)
                del replaced
            self._goThro()
            return replace
        else:
            replace.setLC(replace.brother)
            if replaced.isLT():
                replace.grdpar.setLC(replace)
                del replaced
            elif replaced.isRT():
                replace.grdpar.setRC(replace)
                del replaced
            self._goThro()
            return replace

    def remove(self,key):
        dele = self._inorder(key)
        print(dele.key)
        color = 'WHITE'
        node = None
        if dele:
            if dele.hasRC() and not dele.hasLC():
                color = dele.color
                node = self._replace(dele.rchild,dele)
            elif dele.hasLC() and not dele.hasRC():
                color = dele.color
                node = self._replace(dele.lchild,dele)
            elif not dele.hasLC() and not dele.hasRC():
                color = dele.color
                dele.reinit()
                node = dele
            else:
                succ = self.findSucc(dele)
                color = succ.color
                if dele.rchild == succ:
                    node = self._replace(succ,dele,'2').rchild
                else:
                    dele.copy(succ)
                    node = self._replace(succ.rchild,succ)
                    #a empty may occur
            if color == 'BLACK':
                self._removeFix(node)
        else:
            print('No node named {}!!'.format(key))

    def _removeFix(self,node):
        flag = True
        while True:
            if node.color == 'RED' and flag:
                node.setCol('BLACK')
                node.parent.setCol('RED')
                print('Case1')
                break
            elif node.brother.color == 'RED' and flag:
                node.parent.setCol('RED')
                node.brother.setCol('BLACK')
                self._lrotation(node.parent)
                print('Case2')
            elif node.brother.rchild.color == 'BLACK' and node.brother.lchild.color == 'BLACK' and flag:
                bre = False
                if node.parent.color == 'RED':
                    bre = True
                node.parent.setCol('BLACK')
                node.brother.setCol('RED')
                print('Case3')
                if bre:
                    break
            elif node.brother.lchild.color == 'RED' and node.brother.rchild.color == 'BLACK':
                node.brother.setCol('RED')
                node.brother.lchild.setCol('BLACK')
                self._rrotation(node.brother)
                flag = False
                print('Case4')
            elif node.brother.rchild.color == 'RED':
                node.brother.setCol(node.parent.color)
                node.brother.rchild.setCol('BLACK')
                node.parent.setCol('BLACK')
                self._lrotation(node.parent)
                print('Case5')
                break

def test():
    content = [
        (5,'a'),
        (7,'b'),
        (6,'c'),
        (4,'d'),
        (12,'e'),
        (22,'f'),
        (23,'g'),
        (8,'h'),
        (9,'i'),
        (13,'j'),
        (20,'k')
    ]
    rbtree = Rbtree()
    for key, val in content:
        rbtree.insert(key,val)
    print('Done.')

    node = rbtree.search(8)
    print('Key {} has val {}'.format(node.key,node.val))

    rbtree.remove(8)
    node = rbtree.search(8)
    print(node)

    rbtree.insert(8,'old')
    rbtree.update(8,'new')
    node = rbtree.search(8)
    print('Key {} has val {}'.format(node.key,node.val))

def deletest():
    content = [
        (5,'a'),
        (7,'b'),
        (6,'c'),
        (4,'d'),
        (12,'e'),
        (22,'f'),
        (23,'g'),
        (8,'h'),
        (9,'i'),
        (13,'j'),
        (20,'k')
    ]
    rbtree = Rbtree()
    for key, val in content:
        rbtree.insert(key,val)
    print('Done.')

    keys = [5,7,6,4,12,22,23,8,9,13,20]
    # keys = [5,7,6,4]
    for i in keys:
        print('Removing {}'.format(i))
        rbtree.remove(i)

if __name__ == '__main__':
    deletest()


