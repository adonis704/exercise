'''
Testing:
In:
    content = [
        ('adam','123'),
        ('adame','345'),
        ('adamb','567'),
        ('ad','789'),
        ('ben','101'),
        ('adb','102')
    ]
    trie.delNode('adam')
    trie.startWith('ad')
    node = trie.search('adamb')
    node = trie.search('adam')
    trie.update('adamb','12345')
    node = trie.search('adamb')

Out:
ad: 789
adb: 102
adamb: 567
adame: 345
The number of adamb is 567.
No name found.
The number of adamb is 12345.

'''

class TreeNode:
    def __init__(self,key):
        self.key = key
        self.parents = None
        self.children = {}
        self.val = None

    def setParents(self,node):
        self.parents = node

    def addChild(self,node):
        self.children[node.key] = node

    def setValue(self,val):
        self.val = val

class TrieTree:
    def __init__(self):
        self.rootNode = TreeNode(0)
        self.max = 0

    def addNode(self,exp,val):
        curNode = self.rootNode
        temp = ''
        for char in exp:
            temp += char
            if temp not in curNode.children.keys():
                newNode = TreeNode(temp)
                if len(temp) > self.max:
                    self.max = len(temp)
                newNode.setParents(curNode)
                curNode.children[temp] = newNode
            curNode = curNode.children[temp]
        curNode.setValue(val)


    def delNode(self,key):
        find = self.search(key)
        if find.children:
            find.val = None
        else:
            if find.parents:
                find.parents.children.pop(key)
                find.setParents(None)
            else:
                find.setParents(None)

    def search(self,key):
        curNode = self.rootNode
        temp = ''
        for char in key:
            temp += char
            if temp not in curNode.children.keys():
                return None
            curNode = curNode.children[temp]

        return curNode

    def startWith(self,key):
        curNode = self.search(key)
        stack = [curNode]
        while stack:
            curNode = stack.pop()
            if curNode.val:
                print("{}: {}".format(curNode.key, curNode.val))
            for child in curNode.children.values():
                stack.append(child)
    def update(self,key,val):
        curNode = self.search(key)
        curNode.setValue(val)

def test_trie():
    content = [
        ('adam','123'),
        ('adame','345'),
        ('adamb','567'),
        ('ad','789'),
        ('ben','101'),
        ('adb','102')
    ]
    trie = TrieTree()

    for name,phone in content:
        trie.addNode(name,phone)

    trie.delNode('adam')
    trie.startWith('ad')
    node = trie.search('adamb')
    if node and node.val:
        print("The number of {} is {}.".format(node.key,node.val))
    else:
        print("No name found.")

    node = trie.search('adam')
    if node and node.val:
        print("The number of {} is {}.".format(node.key,node.val))
    else:
        print("No name found.")

    trie.update('adamb','12345')
    node = trie.search('adamb')
    if node and node.val:
        print("The number of {} is {}.".format(node.key, node.val))
    else:
        print("No name found.")


if __name__ == '__main__':
    test_trie()

