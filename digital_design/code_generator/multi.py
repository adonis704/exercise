def multi():
    a = []
    for i in range(1,33):
        a.append(i)
    a.reverse()

    b = [33,34,35,36]
    c= [56,57,58,59,60,61,62,63]

    res = []
    for i in range(8):
        pos1 = 4*i+1
        pos2 = 4*i+2
        pos3 = 4*i +3
        pos4 = 4*i +4

        newline1 = 'mulcum $t{}, $t{}, $t{}\n'.format(pos1, b[0], c[i])
        newline2 = 'mulcum $t{}, $t{}, $t{}\n'.format(pos2, b[1], c[i])
        newline3 = 'mulcum $t{}, $t{}, $t{}\n'.format(pos3, b[2], c[i])
        newline4 = 'mulcum $t{}, $t{}, $t{}\n'.format(pos4, b[3], c[i])
        res.append(newline1)
        res.append(newline2)
        res.append(newline3)
        res.append(newline4)

    print(res)
    with open('multi.txt', 'w') as f:
        for line in res:
            f.write(line)

def to_bin(num):
    res = '{:06b}'.format(num)
    return res

def to_mach():
    a = []
    with open('multi.txt') as f:
        for line in f:
            a.append(line)

    head = '00100'
    toe = '000000000\n'
    res = []
    for items in a:
        print(items)
        no,fir, sec, thir = items.split('$t')
        print(fir)
        fir = int(fir[:-2])
        sec = int(sec[:-2])
        thir = int(thir[:-1])
        newline = head + to_bin(fir) + to_bin(sec) + to_bin(thir) +toe
        res.append(newline)

    with open('multi_mach.txt', 'w') as f:
        for line in res:
            f.write(line)

def final():
    head = '00000'
    toe = '000000000000000000000'
    for i in range(56,64):
        newline = '{}{}{}'.format(head, to_bin(i), toe)
        print(newline)



if __name__ == '__main__':
    final()
