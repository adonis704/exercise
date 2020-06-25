def func1():
    f = open('excel_finish.txt', 'r')
    a = []
    for line in f:
        a.append(line)

    res = []
    for item in a :
        temp = item.split('\t')
        newline = '{} $t{}, $t{}\n'.format(temp[0], temp[1], temp[2][:-1])
        res.append(newline)

    with open('code_4_new1.txt','w') as f:
        for item in res:
            f.write(item)

def func2():
    f = open('code_4_copy.txt', 'r')
    a = []
    a_str = []
    a_res = []
    for line in f:
        a.append(line)
    arr = [1,4,6,7,10,11,13,16,18,19,21,24,25,28,30,31]
    for item in arr:
        a_str.append(str(item))
    for line in a:
        if line[0] == '/':
            a_res.append(line)
        else:
            if line.split('$')[1][:-2] not in a_str:
                temp = line.split('$')
                if temp[0][2:4] == 'xa':
                    temp[0] = temp[0].replace('a','d')
                elif temp[0][2:4] == 'xd':
                    temp[0] = temp[0].replace('d', 'a')
                newline = '{}\t{}\t{}\n'.format(temp[0][:-1],temp[2][:-1],temp[1][:-2])

                a_res.append(newline)
            else:
                temp = line.split('$')
                newline = '{}\t{}\t{}\n'.format(temp[0][:-1],temp[1][:-2],temp[2][:-1])
                a_res.append(newline)
    print(a_res)
    with open('excel_rank.txt','w') as f:
        for line in a_res:
            f.write(line)

def func3():
    f = open('excel.txt', 'r')
    a = []
    a_str = []
    a_res = []
    for line in f:
        a.append(line)
    arr = [1,4,6,7,10,11,13,16,18,19,21,24,25,28,30,31]
    for item in arr:
        a_str.append(str(item))
    for line in a:
        if line.split('\t')[1] not in a_str:
            temp = line.split('\t')
            if temp[0][3] == 'a':
                temp[0] = temp[0].replace('a','d')
            else:
                temp[0] = temp[0].replace('d', 'a')
            newline = '{} $t{}, $t{}\n'.format(temp[0],temp[2][:-1],temp[1])
            a_res.append(newline)
        else:
            temp = line.split('\t')
            newline = '{} $t{}, $t{}\n'.format(temp[0],temp[1],temp[2][:-1])
            a_res.append(newline)
    print(a_res)
    with open('code_4_new.txt','w') as f:
        for line in a_res:
            f.write(line)

def func4():
    a1, a2, a3, a4 = [],[],[],[]
    for i in range(1,9):
        a1.append(str(i))
    for i in range(9,17):
        a2.append(str(i))
    for i in range(17,25):
        a3.append(str(i))
    for i in range(25,33):
        a4.append(str(i))

    a = []
    a1_res, a2_res, a3_res, a4_res, = [],[],[],[]
    with open('excel_rank.txt') as f:
        for line in f:
            a.append(line)

    round = 1
    count1 = 0
    count2 = 0
    count3 = 0
    count4 = 0
    for item in a:
        if item[0] !='/':
            op, fir, sec = item.split('\t')
            if fir in a1:
                a1_res.append(item)
                count1 +=1
            elif fir in a2:
                a2_res.append(item)
                count2 += 1
            elif fir in a3:
                a3_res.append(item)
                count3 += 1
            elif fir in a4:
                a4_res.append(item)
                count4 += 1
        else:
            round = int(item[2:]) + 1
            count1 = 0
            count2 = 0
            count3 = 0
            count4 = 0
        if (count1 > 4 or count2 > 4 or count3 > 4 or count4 > 4):
            print(round, count1, count2, count3, count4)
    print(len(a1_res))
    print(len(a2_res))
    print(len(a3_res))
    print(len(a4_res))

    res = []
    for i in range(len(a1_res)):
        res.append(a1_res[i])
        res.append(a2_res[i])
        res.append(a3_res[i])
        res.append(a4_res[i])

    with open('excel_finish.txt', 'w') as f:
        for line in res:
            f.write(line)


def func5():
    a1, a2, a3, a4 = [],[],[],[]
    for i in range(1,9):
        a1.append(str(i))
    for i in range(9,17):
        a2.append(str(i))
    for i in range(17,25):
        a3.append(str(i))
    for i in range(25,33):
        a4.append(str(i))

    a = []
    a1_res, a2_res, a3_res, a4_res, = [],[],[],[]
    with open('code_4_copy.txt', 'r') as f:
        for line in f:
            a.append(line)
    round = 1
    count1 = 0
    count2 = 0
    count3 = 0
    count4 = 0
    for item in a:
        if item[0] !='/':
            op, fir, sec = item.split('$')
            if fir[:-2] in a1:
                a1_res.append(item)
                count1 +=1
            elif fir[:-2] in a2:
                a2_res.append(item)
                count2 += 1
            elif fir[:-2] in a3:
                a3_res.append(item)
                count3 += 1
            elif fir[:-2] in a4:
                a4_res.append(item)
                count4 += 1
        else:
            round = int(item[2:])+1
            count1 = 0
            count2 = 0
            count3 = 0
            count4 = 0
        if (count1 >4 or count2 >4 or count3 >4 or count4 >4):
            print(round, count1,count2,count3,count4)
    print(len(a1_res))
    print(len(a2_res))
    print(len(a3_res))
    print(len(a4_res))



def test():
    func1()

if __name__ == '__main__':
    test()