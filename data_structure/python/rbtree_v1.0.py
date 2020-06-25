class TreeNode:
    def __init__(self,key,val = None):
        self.key = key
        self.val = val
        self.color = 'RED'
        self.lchild = None
        self.rchild = None
        self.parent = None
        self.aunt = None
        self.grdpar = None

    def hasLC(self):
        return not self.lchild == None

    def hasRC(self):
        return not self.rchild == None

    def hasPar(self):
        return not self.parent == None

    def setPar(self,node):
        self.parent = node
        if node != None:
            self.grdpar = node.parent
            if node.hasPar():
                if node.isLT():
                    self.aunt = node.parent.rchild
                else:
                    self.aunt = node.parent.lchild

    def setLC(self,node):
        self.lchild = node

    def setRC(self,node):
        self.rchild = node

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

    def out(self):
        del self

    def reinit(self):
        self.setCol("BLACK")
        self.setKey(None)
        self.setVal(None)
        self.setLC(None)
        self.setRC(None)

    def getBro(self):
        if self.isLT():
            return self.parent.rchild
        else:
            return self.parent.lchild

class Rbtree:
    def __init__(self):
        self.root = None

    def _lrotation(self,node):
        if node.hasPar():
            par = node.parent
        else:
            par = None
        temp = node.rchild.lchild
        node.rchild.setPar(par)
        if par != None:
            if node.isLT():
                par.setLC(node.rchild)
            else:
                par.setRC(node.rchild)
        node.rchild.setLC(node)
        if par == None:
            self.root = node.rchild
        node.setPar(node.rchild)
        node.setRC(temp)
        if temp != None:
            temp.setPar(node)


    def _rrotation(self,node):
        if node.hasPar():
            par = node.parent
        else:
            par = None
        temp = node.lchild.rchild
        node.lchild.setPar(par)
        if par != None:
            if node.isLT():
                par.setLC(node.lchild)
            else:
                par.setRC(node.lchild)
        node.lchild.setRC(node)
        if par == None:
            self.root = node.lchild
        node.setPar(node.lchild)
        node.setLC(temp)
        if temp != None:
            temp.setPar(node)

    def _colorflip(self,node):
        if node.hasLC():
            node.lchild.setCol('BLACK')
        if node.hasRC():
            node.rchild.setCol('BLACK')
        if node.hasPar():
            node.setCol('RED')
            self._check(node)

    def _check(self,node):
        if node.parent.color == 'RED' and node.color == 'RED':
            if node.aunt and node.aunt.color == 'RED':
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

    def insert(self,key):
        node = TreeNode(key)
        if not self.root:
            self.root = node
            node.setCol('BLACK')
        else:
            curNode = self.root
            stack = [curNode]
            while stack and stack[-1] != None:
                flag = False
                curNode = stack.pop()
                if key < curNode.key:
                    stack.append(curNode.lchild)
                    flag = True
                else:
                    stack.append(curNode.rchild)
                if not stack[-1] and flag:
                    curNode.setLC(node)
                    print('insert {} under {} as LC.'.format(node.key,curNode.key,))
                    node.setPar(curNode)
                if not stack[-1] and not flag:
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
        if self.root != None:
            stack = []
            node = self.root
            while stack or node:
                while node:
                    stack.append(node)
                    node = node.lchild
                node = stack.pop()
                if node.key == key:
                    return node
                node = node.rchild
            return None
        else:
            return None

    def _changeCol(self,node1,node2):
        temp = node1.color
        node1.setCol(node2.color)
        node2.setCol(temp)

    def remove(self,key):
        succ = self._inorder(key)
        if succ:
            if succ.hasRC() :
                dele = self.findSucc(succ)
                # dele.setKey(succ.key)
                # dele.setVal(succ.val)
            elif succ.hasLC() and not succ.hasRC():
                dele = self.findSucc(succ,'LEFT')
            else:
                dele = None
            if succ.color == 'RED':
                if dele == None:
                    succ.out()
                    return True
                else:
                    dele.copy(succ)
                    succ.out()
                    return True
            else:
                flag = True
                if not succ.hasPar():
                    dele.copy(succ)
                    succ.out()
                    return True
                if dele != None:
                    flag = False
                succ.copy(dele)
                print(succ.key)
                dele.out()
                if succ.parent.color == 'RED' and (succ.getBro() == None or succ.getBro().color == 'BLACK') and(succ.getBro() != None and succ.getBro().lchild.color == 'BLACK') and succ.getBro().rchild.color == 'BLACK':
                    succ.parent.setCol('BLACK')
                    succ.getBro().setCol('RED')
                    return True
                if succ.getBro() != None:
                    if succ.getBro().color == 'BLACK' and succ.getBro().rchild != None and succ.getBro().rchild.color == 'RED':
                        self._lrotation(succ.parent)
                        self._changeCol(succ.parent,succ.grdpar)
                        succ.grdpar.rchild.setCol('BLACK')
                        return True
                    if succ.getBro().color == 'BLACK' and succ.getBro().lchild != None and (succ.getBro().rchild == None or succ.getBro().rchild.color == 'BLACK') and succ.getBro().lchild.color == 'RED':
                        self._rrotation(succ.getBro())
                        self._changeCol(succ.getBro(),succ.getBro().rchild)
                        self._lrotation(succ.parent)
                        self._changeCol(succ.parent, succ.grdpar)
                        succ.grdpar.rchild.setCol('BLACK')
                        return True
                    if  succ.getBro().color == 'RED':
                        self._lrotation(succ.parent)
                        self._changeCol(succ.parent,succ.grdpar)
                        if succ.parent.color == 'RED' and succ.getBro().color == 'BLACK':
                            succ.parent.setCol('BLACK')
                            succ.getBro().setCol('RED')
                            return True
                        if succ.getBro().color == 'BLACK' and succ.getBro().rchild != None and succ.getBro().rchild.color == 'RED':
                            self._lrotation(succ.parent)
                            self._changeCol(succ.parent, succ.grdpar)
                            succ.grdpar.rchild.setCol('BLACK')
                            return True
                        if succ.getBro().color == 'BLACK' and succ.getBro().lchild != None and succ.getBro().rchild.color == 'BLACK' and succ.getBro().lchild.color == 'RED':
                            self._rrotation(succ.getBro())
                            self._changeCol(succ.getBro(), succ.getBro().rchild)
                            self._lrotation(succ.parent)
                            self._changeCol(succ.parent, succ.grdpar)
                            succ.grdpar.rchild.setCol('BLACK')
                            return True
                else:
                    print('123')
                    if succ.getBro() != None:
                        succ.getBro().setCol('RED')
                    succ = succ.parent
                    if succ.parent.color == 'RED' and (succ.getBro() == None or succ.getBro().color == 'BLACK') and (
                            succ.getBro().lchid == None or succ.getBro().lchild.color == 'BLACK') and ( succ.getBro().rchild == None or succ.getBro().rchild.color == 'BLACK'):
                        succ.parent.setCol('BLACK')
                        succ.getBro().setCol('RED')
                        return True
                    if succ.getBro() != None:
                        if succ.getBro().color == 'BLACK' and succ.getBro().rchild != None and succ.getBro().rchild.color == 'RED':
                            self._lrotation(succ.parent)
                            self._changeCol(succ.parent, succ.grdpar)
                            succ.grdpar.rchild.setCol('BLACK')
                            return True
                        if succ.getBro().color == 'BLACK' and succ.getBro().lchild != None and succ.getBro().rchild.color == 'BLACK' and succ.getBro().lchild.color == 'RED':
                            self._rrotation(succ.getBro())
                            self._changeCol(succ.getBro(), succ.getBro().rchild)
                            self._lrotation(succ.parent)
                            self._changeCol(succ.parent, succ.grdpar)
                            succ.grdpar.rchild.setCol('BLACK')
                            return True
                        if succ.getBro().color == 'RED':
                            self._lrotation(succ.parent)
                            self._changeCol(succ.parent, succ.grdpar)
                            if succ.parent.color == 'RED' and succ.getBro().color == 'BLACK':
                                succ.parent.setCol('BLACK')
                                succ.getBro().setCol('RED')
                                return True
                            if succ.getBro().color == 'BLACK' and succ.getBro().rchild != None and succ.getBro().rchild.color == 'RED':
                                self._lrotation(succ.parent)
                                self._changeCol(succ.parent, succ.grdpar)
                                succ.grdpar.rchild.setCol('BLACK')
                                return True
                            if succ.getBro().color == 'BLACK' and succ.getBro().lchild != None and succ.getBro().rchild.color == 'BLACK' and succ.getBro().lchild.color == 'RED':
                                self._rrotation(succ.getBro())
                                self._changeCol(succ.getBro(), succ.getBro().rchild)
                                self._lrotation(succ.parent)
                                self._changeCol(succ.parent, succ.grdpar)
                                succ.grdpar.rchild.setCol('BLACK')
                                return True


                if flag:
                    succ.reinit()




        else:
            print("No node name {}.".format(key))





def test():
    content = [5,7,6,4,12,22,23,8,9,13,20]
    rbtree = Rbtree()
    for i in content:
        rbtree.insert(i)
    print(rbtree.root.lchild.key,rbtree.root.rchild.key)

if __name__ == '__main__':
    test()


