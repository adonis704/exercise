def to_bin(num):
    res = '{:06b}'.format(num)
    return res

def to_mach():
    a = []
    dic = {
        'cpia': '00000',
        'cpid': '00001',
        'cpxa': '00010',
        'cpxd': '00011'
    }
    toe = '000000000000000'
    with open('code_4_new1.txt') as f:
        for line in f:
            a.append(line)

    res = []
    for item in a:
        temp = []
        op, st, sd = item.split(' ')
        st = int(st.split('t')[1][:-1])
        sd = int(sd.split('t')[1][:-1])
        st = to_bin(st)
        sd = to_bin(sd)
        # print(dic[op] + st + sd + toe)
        res.append(dic[op] + st + sd + toe + ',')

    with open('code_4_mach2.txt', 'w') as f:
        for item in res:
            f.write(item)

def test():
    to_mach()

if __name__ == '__main__':
    test()