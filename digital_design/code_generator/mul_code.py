reg1 = [1, 4, 6, 7, 33, 36, 38, 39]
reg3 = [10] * 8
reg5 = [18] * 8
reg7 = [25, 28, 30, 31, 41, 44, 46, 47]
reg2 = [2, 3, 5, 8, 34, 35, 37, 40]
reg4 = [9] * 8
reg6 = [17] * 8
reg8 = [26, 27, 29, 32, 42, 43, 45, 48]
reg1.reverse()
reg2.reverse()
reg3.reverse()
reg4.reverse()

group1 = ['11', '13', '21', '23', '51', '53', '61', '63']
group2 = ['12', '14', '22', '24', '52', '54', '62', '64']
group3 = ['b2']
group4 = ['b1']
group5 = ['b4']
group6 = ['b3']
group7 = ['31', '33', '41', '43', '71', '73', '81', '83']
group8 = ['32', '34', '42', '44', '72', '74', '82', '84']

def check(str):
    global reg1
    global reg2
    global reg3
    global reg4
    global reg5
    global reg6
    global reg7
    global reg8

    if str in group1:
        return reg1.pop()
    elif str in group2:
        return reg2.pop()
    elif str in group3:
        return reg3.pop()
    elif str in group4:
        return reg4.pop()
    elif str in group5:
        return reg5.pop()
    elif str in group6:
        return reg6.pop()
    elif str in group7:
        return reg7.pop()
    elif str in group8:
        return reg8.pop()
    else:
        return

def to_bin(num):
    res = '{:06b}'.format(num)
    return res

def to_mul():
    dic = {
        'c1': 56,
        'c2': 58,
        'c3': 60,
        'c4': 62,
        'c5': 57,
        'c6': 59,
        'c7': 61,
        'c8': 63

    }
    a = []
    with open('mul_code_raw.txt', 'r') as f:
        for line in f:
            a.append(line)

    res = [[] for i in range(len(a))]

    count = 0
    for items in a:
        for item in items.split('\t'):
            res[count].append(item)
        res[count][2] = res[count][2][:-1]
        count +=1
    print(res)

    res_code = []
    res_mach = []
    head = '00100'
    toe = '000000000\n'
    for item in res:
        pos1 = check(item[0])
        pos2 = check(item[1])
        pos3 = dic[item[2]]
        newline1 = 'mulcum $t{}, $t{}, $t{}\n'.format(pos1,pos2,pos3)
        newline2 = head + to_bin(pos1) + to_bin(pos2) +to_bin(pos3) +toe
        res_code.append(newline1)
        res_mach.append(newline2)
    print(res_code)

    with open('mul_code.txt', 'w') as f:
        for line in res_code:
            f.write(line)

    with open('mul_mach.txt', 'w') as f:
        for line in res_mach:
            f.write(line)



def test():
    to_mul()


if __name__ == '__main__':
    test()