from random import shuffle
x = []
for i in range(64):
    x.append(i)
shuffle(x)

def sort_desc(a,b,arr):
    a = int(a)
    b = int(b)
    if arr[a] < arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp

def sort_asc(a,b,arr):
    a = int(a)
    b = int(b)
    if arr[a] > arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp

def sort():
    global x
    a = []
    with open('code_4_new1.txt', 'r') as f:
        for line in f:
            a.append(line)

    for code in a:
        if code[0] == '/':
            continue
        op, fir, sec = code.split(' ')
        fir = int(fir[2:-1])
        sec = int(sec[2:-1])
        print(op, fir, sec)
        pos1 = 2 * fir - 2
        pos2 = 2 * fir - 2 + 1
        pos3 = 2 * sec - 2
        pos4 = 2 * sec - 2 + 1
        if op == 'cpia':
            sort_asc(pos1,pos2,x)
            sort_asc(pos3,pos4,x)
        elif op == 'cpid':
            sort_desc(pos1,pos2,x)
            sort_desc(pos3,pos4,x)
        elif op == 'cpxa':
            sort_asc(pos1,pos3,x)
            sort_asc(pos2,pos4,x)
        elif op == 'cpxd':
            sort_desc(pos1,pos3,x)
            sort_desc(pos2,pos4,x)
    print(x)

def sort1():
    global x
    a = []
    with open('code_4.txt', 'r') as f:
        for line in f:
            a.append(line)

    for code in a:
        if code[0] == '/':
            continue
        op, fir, sec = code.split(' ')
        fir = int(fir[1:-1])
        sec = int(sec[1:-1])
        print(op, fir, sec)
        pos1 = 2 * fir - 2
        pos2 = 2 * fir - 2 + 1
        pos3 = 2 * sec - 2
        pos4 = 2 * sec - 2 + 1
        if op == 'cpia':
            sort_asc(pos1,pos2,x)
            sort_asc(pos3,pos4,x)
        elif op == 'cpid':
            sort_desc(pos1,pos2,x)
            sort_desc(pos3,pos4,x)
        elif op == 'cpxa':
            sort_asc(pos1,pos3,x)
            sort_asc(pos2,pos4,x)
        elif op == 'cpxd':
            sort_desc(pos1,pos3,x)
            sort_desc(pos2,pos4,x)
    print(x)

def sort2():
    global x
    a = []
    with open('excel_finish.txt', 'r') as f:
        for line in f:
            a.append(line)

    for code in a:
        if code[0] == '/':
            continue
        op, fir, sec = code.split('\t')
        fir = int(fir)
        sec = int(sec[:-1])
        print(op, fir, sec)
        pos1 = 2 * fir - 2
        pos2 = 2 * fir - 2 + 1
        pos3 = 2 * sec - 2
        pos4 = 2 * sec - 2 + 1
        if op == 'cpia':
            sort_asc(pos1,pos2,x)
            sort_asc(pos3,pos4,x)
        elif op == 'cpid':
            sort_desc(pos1,pos2,x)
            sort_desc(pos3,pos4,x)
        elif op == 'cpxa':
            sort_asc(pos1,pos3,x)
            sort_asc(pos2,pos4,x)
        elif op == 'cpxd':
            sort_desc(pos1,pos3,x)
            sort_desc(pos2,pos4,x)
    print(x)


def test():
    sort()

if __name__ == '__main__':
    test()