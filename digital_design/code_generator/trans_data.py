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
        return '1'
    elif str in group2:
        return '2'
    elif str in group3:
        return '3'
    elif str in group4:
        return '4'
    elif str in group5:
        return '5'
    elif str in group6:
        return '6'
    elif str in group7:
        return '7'
    elif str in group8:
        return '8'
    else:
        return

def trans_mul():
    a = []
    with open('mul_data.txt', 'r') as f:
        for line in f:
            if line[0] != 'M':
                a.append(line)

    dic = {}
    pos = [
        '11', '12', '13', '14',
        '21', '22', '23', '24',
        '31', '32', '33', '34',
        '41', '42', '43', '44',
        '51', '52', '53', '54',
        '61', '62', '63', '64',
        '71', '72', '73', '74',
        '81', '82', '83', '84',
        'b1', 'b2', 'b3', 'b4'
    ]

    for i in range(len(pos)):
        dic[pos[i]] = a[i][:-2]
        print(a[i][:-2])

    res = []
    with open('mul_code_raw.txt', 'r') as f:
        for line in f:
            fir, sec, thir = line.split('\t')
            res.append(dic[fir])
            res.append(dic[sec])
    print(res)

    with open('mul_data_rank.txt', 'w') as f:
        for line in res:
            f.write(line + '\n')

def rank_transed():
    a = []
    with open('mul_data_rank.txt', 'r') as f:
        for line in f:
            if line[0] != 'M':
                a.append(line)

    b = []
    with open('mul_code_raw.txt', 'r') as f:
        for line in f:
            fir, sec, thir = line.split('\t')
            b.append(fir)
            b.append(sec)
    res = [[] for i in range(8)]

    for i in range(len(a)):
        pos = check(b[i])
        pos = int(pos)
        res[pos-1].append(a[i].replace('\n', ',\n'))
    print(res)

    with open('mul_code1.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[0]:
            f.write(item)
    with open('mul_code2.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[1]:
            f.write(item)
    with open('mul_code3.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[2]:
            f.write(item)
    with open('mul_code4.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[3]:
            f.write(item)
    with open('mul_code5.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[4]:
            f.write(item)
    with open('mul_code6.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[5]:
            f.write(item)
    with open('mul_code7.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[6]:
            f.write(item)
    with open('mul_code8.txt', 'w') as f:
        f.write('MEMORY_INITIALIZATION_RADIX=2;\n')
        f.write('MEMORY_INITIALIZATION_VECTOR=\n')
        for item in res[7]:
            f.write(item)





def test():
    rank_transed()

if __name__ == '__main__':
    test()

