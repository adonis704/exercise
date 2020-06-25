path ='lgr_test.txt'
WIDTH = 32
DEPTH = 10
STAGE = 10

count32 = 0
count233 = 0
count63 = 0
count153 = 0

def matrix(width, depth):
    res = []
    for i in range(width):
        temp = []
        for i in range(depth):
            temp.append(i+1)
        temp.reverse()
        res.append(temp)
    return res
matx1 = matrix(WIDTH,DEPTH)
matx2 = matrix(WIDTH,DEPTH)

def reset():
    global count32
    global count233
    global count63
    global count153
    global matx1
    global matx2
    
    count32 = 0
    count233 = 0
    count63 = 0
    count153 = 0
    matx1 = matrix(WIDTH, 10)
    matx2 = matrix(WIDTH, 10)


def txt(path):
    res = []
    with open(path,'r') as f:
        for line in f:
           res.append(line)
    return res

def form(arr):
    fir = []
    sec = []
    res = []
    fir_ed = []
    sec_ed = []
    for item in arr:
        fir.append(item.split(')')[0])
        sec.append(item.split(')')[1])

    for item1 in fir:
        temp1 = item1.split(',')
        temp2 = []
        for item2 in temp1:
            temp2.append(int(item2.split(' ')[-1]))
        fir_ed.append(temp2)

    for item in sec:
        temp = item.lstrip()[:-1]
        temp = float(temp)
        temp = int(temp)
        sec_ed.append(temp)

    for i in range(len(sec_ed)):
        temp = []
        for j in fir_ed[i]:
            temp.append(j)
        temp.append(sec_ed[i])
        res.append(temp)

    # res1 = [[] for i in range(WIDTH)]
    res1 = []
    for item in res:
        if item[3] != 0:
            res1.append(item)
    return res1

def handle(arr):
    global count32
    global count233
    global count63
    global count153

    stage = arr[0]
    typ = arr[1]
    col = arr[2]-1
    num = arr[3]
    if typ == 1:
        for i in range(num):
            pos1 = matx1[col].pop()
            pos2 = matx2[col].pop()
            newline = 'assign s{}[{}][{}] = s{}[{}][{}];'.format(stage+1,col,pos2,stage,col,pos1 )
            print(newline)

    elif typ == 2:
        for i in range(num):
            pos11 = matx1[col].pop()
            matx1[col].pop()
            pos13 = matx1[col].pop()
            pos2 = matx2[col].pop()
            pos3 = matx2[col+1].pop()
            newline = 'comp32 comp32_u_s{}_{}(\n\t.a_i(s{}[{}][{}:{}]),\n\t.o({{s{}[{}][{}],s{}[{}][{}]}})\n);'\
                .format(stage,count32,stage,col,pos13,pos11,stage+1,col,pos2,stage+1,col+1,pos3)
            count32 +=1
            print(newline)

    elif typ == 3:
        for i in range(num):
            pos11 = matx1[col].pop()
            for i in range(4):
                matx1[col].pop()
            pos16 = matx1[col].pop()
            pos2 = matx2[col].pop()
            pos3 = matx2[col+1].pop()
            pos4 = matx2[col+2].pop()
            newline = 'comp63 comp63_u_s{}_{}(\n\t.a_i(s{}[{}][{}:{}]),\n\t.o({{s{}[{}][{}],s{}[{}][{}],s{}[{}][{}]}})\n);'\
                .format(stage,count63,stage,col,pos16,pos11,stage+1,col,pos2,stage+1,col+1,pos3,stage+1,col+2,pos4)
            count63 +=1
            print(newline)

    elif typ == 4:
        for i in range(num):
            pos11 = matx1[col].pop()
            for i in range(3):
                matx1[col].pop()
            pos15 = matx1[col].pop()
            pos16 = matx1[col+1].pop()
            pos2 = matx2[col].pop()
            pos3 = matx2[col+1].pop()
            pos4 = matx2[col+2].pop()
            newline = 'comp153 comp153_u_s{}_{}(\n\t.a0_i(s{}[{}][{}:{}]),\n\t.a1_i(s{}[{}][{}]),\n\t.o({{s{}[{}][{}],s{}[{}][{}],s{}[{}][{}]}})\n);'\
                .format(stage,count153,stage,col,pos15,pos11,stage,col+1,pos16,stage+1,col,pos2,stage+1,col+1,pos3,stage+1,col+2,pos4)
            count153 +=1
            print(newline)

    elif typ == 5:
        for i in range(num):
            pos11 = matx1[col].pop()
            matx1[col].pop()
            pos13 = matx1[col].pop()
            pos14 = matx1[col+1].pop()
            pos15 = matx1[col+1].pop()
            pos2 = matx2[col].pop()
            pos3 = matx2[col+1].pop()
            pos4 = matx2[col+2].pop()
            newline = 'comp233 comp233_u_s{}_{}(\n\t.a0_i(s{}[{}][{}:{}]),\n\t.a1_i({{s{}[{}][{}:{}]}}),\n\t.o({{s{}[{}][{}],s{}[{}][{}],s{}[{}][{}]}})\n);'\
                .format(stage,count233,stage,col,pos13,pos11,stage,col+1,pos15,pos14,stage+1,col,pos2,stage+1,col+1,pos3,stage+1,col+2,pos4)

            count233 +=1
            print(newline)
    else:
        print('error')

def depart(arr):
    res = [[] for i in range(STAGE)]
    for item in arr:
        res[item[0]].append(item)
    return res

def s4_handle():
        a = []
        with open('mul.txt') as f:
            for line in f:
                if line[0] == 'a' and line[8] == '4':
                    a.append(line)
        print(a)
        arr = []
        for item in a:
            temp = item.split('=')[0]
            temp = temp.split('[')
            temp1 = int(temp[1][:-1])
            temp2 = int(temp[2][:-2])
            arr_temp = [temp1,temp2]
            arr.append(arr_temp)
        print(arr)
        res = []
        for i in range(32):
            for j in range(3):
                if [i,j+1] in arr:
                    newline = 'assign s4[{}][{}] = s3[{}][{}];\n'.format(i,j+1,i,j+1)
                    res.append(newline)
                else:
                    newline = 'assign s4[{}][{}] = 0;\n'.format(i, j + 1, i, j + 1)
                    res.append(newline)
        print(res)

        with open('mul_s4.txt', 'w') as f:
            for item in res:
                f.write(item)

def test():
    content = txt(path)
    content = form(content)
    content = depart(content)
    # for item in content:
    #     print(item)
    for items in content:
        if(len(items) != 0):
            for item in items:
                handle(item)
            reset()
        else:pass



if __name__ == '__main__':
    s4_handle()