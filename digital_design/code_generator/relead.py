from random import shuffle
flag = 0

def crash_check():
    a = []
    with open('code_4_new1.txt', 'r') as f:
        for line in f:
            a.append(line)

    a1, a2, a3, a4 = [],[],[],[]
    for i in range(1,9):
        a1.append(str(i))
    for i in range(9,17):
        a2.append(str(i))
    for i in range(17,25):
        a3.append(str(i))
    for i in range(25,33):
        a4.append(str(i))
    a11, a21, a31, a41 = [], [], [], []
    a12, a22, a32, a42 = [], [], [], []
    for i in range(int(len(a)/4)):
        a11.append(a[4*i].split('$t')[1][:-2])
        a12.append(a[4*i].split('$t')[2][:-1])
        a21.append(a[4*i+1].split('$t')[1][:-2])
        a22.append(a[4*i+1].split('$t')[2][:-1])
        a31.append(a[4*i+2].split('$t')[1][:-2])
        a32.append(a[4*i+2].split('$t')[2][:-1])
        a41.append(a[4*i+3].split('$t')[1][:-2])
        a42.append(a[4*i+3].split('$t')[2][:-1])
        # a1.append(a[4*i])
        # a2.append(a[4*i +1])
        # a3.append(a[4*i+2])
        # a4.append(a[4*i+3])
    print(a11)
    print(a12)
    print(a21)
    print(a22)
    print(a31)
    print(a32)
    print(a41)
    print(a42)
    # print(a1[0].split('$t')[0][:-1])
    # print(a1[0].split('$t')[1][:-2])
    # print(a1[0].split('$t')[2][:-1])

    newline1 = ''
    newline2 = ''
    newline3 = ''
    newline4 = ''
    newline5 = ''
    newline6 = ''
    newline7 = ''
    newline8 = ''
    for i in range(len(a11)):
        newline1 += (a11[i] + '\t')
        newline2 += (a12[i] + '\t')
        newline3 += (a21[i] + '\t')
        newline4 += (a22[i] + '\t')
        newline5 += (a31[i] + '\t')
        newline6 += (a32[i] + '\t')
        newline7 += (a41[i] + '\t')
        newline8 += (a42[i] + '\t')
    newline1 += '\n'
    newline2 += '\n'
    newline3 += '\n'
    newline4 += '\n'
    newline5 += '\n'
    newline6 += '\n'
    newline7 += '\n'
    newline8 += '\n'

    res = [newline1, newline2, newline3, newline4, newline5, newline6, newline7, newline8]
    with open('crash.txt', 'w') as f:
        for line in res:
            f.write(line)

    count = 0
    for i in range(len(a11)):
        if (a12[i] not in a1 or a22[i] not in a2 or a32[i] not in a3 or a42[i] not in a4 ):
            print(count)
        count+=1

def crash_check1():
    a = []
    with open('crash1.txt', 'r') as f:
        for line in f:
            a.append(line)

    res = [[] for i in range(len(a))]
    count = 0
    for item in a:
        for temp in item.split('\t'):
            res[count].append(temp)
        count +=1

    for item in res:
        print(item)

    count1 = 0
    count2 = 0
    for i in range(len(res) - 1):
        temp1 = res[i]
        temp2 = res[i+1]
        for j in temp1:
            if j in temp2:
                print('0:{}'. format(i))

    for i in range(len(res) - 2):
        temp1 = res[i]
        temp2 = res[i+2]
        flag = 0
        for j in temp1:
            if j in temp2:
                print('1:{}'. format(i))
                flag = 1
        if (flag):
            count1 += 1
            flag = 0

    for i in range(len(res) - 3):
        temp1 = res[i]
        temp2 = res[i+3]
        flag = 0
        for j in temp1:
            if j in temp2:
                print('2:{}'. format(i))
                flag = 1
        if (flag):
            count2 +=1
            flag=0

    print(count1,count2)

def crash_shuffle(before):
    global flag
    a = []
    with open('crash1.txt', 'r') as f:
        for line in f:
            a.append(line)

    print(before)
    fail = 0
    restart = 0
    for i in range(4,int(len(a)/4 - 1)):
        time = 0
        temp1 = before
        temp2 = []
        sec = [[] for i in range(4)]
        for j in range(4):
            temp2.append(a[4 * (i+1) + j])
        count = 0
        for items in temp2:
            for item in items.split('\t'):
                sec[count].append(item)
            sec[count][7] = sec[count][7][:-1]
            count+=1
        no_crush = 0
        while (not no_crush):
            shuffle(sec)
            flag1 = 1
            flag2 = 1
            flag3 = 1
            for item in temp1[2]:
                if item in sec[0]:
                    flag1 = 0
                    break
            for item in temp1[3]:
                if item in sec[0]:
                    flag2 = 0
                    break
                if item in sec[1]:
                    flag3 = 0
                    break
            if (flag1 and flag2 and flag3):
                no_crush = 1
                before = sec
                print(before)
            else:
                time +=1
            if time >=500:
                fail = 1
                break
        if fail:
            restart = 1
            break
    if not restart:
        flag = 1
    else:
        print('fail')


def test():
    a = []
    with open('crash1.txt', 'r') as f:
        for line in f:
            a.append(line)
    before = [[] for i in range(4)]
    for i in range(4):
        temp = a[i+12]
        for item in temp.split('\t'):
            before[i].append(item)
        before[i][7] = before[i][7][:-1]
    while(not flag):
        # shuffle(before)
        crash_shuffle(before)
    # crash_check1()

if __name__ == '__main__':
    test()