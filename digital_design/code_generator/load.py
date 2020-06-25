reg1 = [1, 4, 6, 7, 33, 36, 38, 39]
reg3 = [10] * 8
reg5 = [18] * 8
reg7 = [25, 28, 30, 31, 41, 44, 46, 47]
reg2 = [2, 3, 5, 8, 34, 35, 37, 40]
reg4 = [9] * 8
reg6 = [17] * 8
reg8 = [26, 27, 29, 32, 42, 43, 45, 48]

group1 = ['11', '13', '21', '23', '51', '53', '61', '63']
group2 = ['12', '14', '22', '24', '52', '54', '62', '64']
group3 = ['b2']
group4 = ['b1']
group5 = ['b4']
group6 = ['b3']
group7 = ['31', '33', '41', '43', '71', '73', '81', '83']
group8 = ['32', '34', '42', '44', '72', '74', '82', '84']

we5 = [1,4,6,7,10,11,13,16,18,19,21,24,25,28,30,31]
we52 = [25,28,30,31]
we62 = [26,27,28,29]
r1 = [56, 57]
r2 = [58, 59]
r3 = [60, 61]
r4 = [62, 63]
before = '0001'
after = '1000'
no = '0000'

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

def load_data():
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

    res_mach = []
    for items in res:
        for item in items:
            if item[0] != 'c':
                reg = check(item)
                if reg in we5:
                    if reg not in we52:
                        pos1 = before
                        pos2 = no
                    else:
                        pos1 = after
                        pos2 = no
                else:
                    if reg not in we62:
                        pos1 = no
                        pos2 = before
                    else:
                        pos1 = no
                        pos2 = after
                newline = to_bin(reg) + pos1 +pos2 + '\n'
                res_mach.append(newline)
    print(res_mach)

    with open('load_data.txt', 'w') as f:
        for line in res_mach:
            f.write(line)

def load_rank():
    res = []
    for i in range(1,33):
        if i in we5:
            if i not in we52:
                pos = '0001'
            else:
                pos = '0010'
        else:
            if i not in we62:
                pos = '0100'
            else:
                pos = '1000'
        newline = to_bin(i) + pos + ',\n'
        res.append(newline)

    with open('load_rank_onehot.txt', 'w') as f:
        for line in res:
            f.write(line)

def output_data():
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

    res_rank = []
    for i in range(1,33):
        if i in we5:
            if i not in we52:
                pos = '000'
            else:
                pos = '001'
        else:
            if i not in we62:
                pos = '010'
            else:
                pos = '011'
        newline = to_bin(i) + pos + '\n'
        res_rank.append(newline)

    res_mul =  []
    for i in range(1,9):
        str = 'c{}'.format(i)
        reg = dic[str]
        if reg in r1:
            pos = '100'
        elif reg in r2:
            pos = '101'
        elif reg in r3:
            pos = '110'
        elif reg in r4:
            pos = '111'
        newline = to_bin(reg) + pos + '\n'
        res_mul.append(newline)

    with open('output_data.txt', 'w') as f:
        for line in res_rank:
            f.write(line)

        for line in res_mul:
            f.write(line)



def test():
    load_rank()

if __name__ == '__main__':
    test()
