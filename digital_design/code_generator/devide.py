def devide_rank():
    a = []
    with open('code_4_final.txt', 'r') as f:
        for line in f:
            a.append(line)

    a1, a2, a3, a4= [], [], [], []
    for i in range(int(len(a)/4)):
        a1.append(a[4*i].replace('\n', ','))
        a2.append(a[4*i+1].replace('\n', ','))
        a3.append(a[4*i+2].replace('\n', ','))
        a4.append(a[4*i+3].replace('\n', ','))

    a1[-1] = a1[-1].replace(',', ';')
    a2[-1] = a2[-1].replace(',', ';')
    a3[-1] = a3[-1].replace(',', ';')
    a4[-1] = a4[-1].replace(',', ';')

    with open('1.txt', 'w') as f:
        for line in a1:
            f.write(line)
    with open('2.txt', 'w') as f:
        for line in a2:
            f.write(line)
    with open('3.txt', 'w') as f:
        for line in a3:
            f.write(line)
    with open('4.txt', 'w') as f:
        for line in a4:
            f.write(line)

def devide_mul():
    a = []
    with open('mul_final.txt', 'r') as f:
        for line in f:
            a.append(line)

    a1, a2, a3, a4= [], [], [], []
    for i in range(int(len(a)/4)):
        a1.append(a[4*i].replace('\n', ','))
        a2.append(a[4*i+1].replace('\n', ','))
        a3.append(a[4*i+2].replace('\n', ','))
        a4.append(a[4*i+3].replace('\n', ','))

    a1[-1] = a1[-1].replace(',', ';')
    a2[-1] = a2[-1].replace(',', ';')
    a3[-1] = a3[-1].replace(',', ';')
    a4[-1] = a4[-1].replace(',', ';')

    with open('5.txt', 'w') as f:
        for line in a1:
            f.write(line)
    with open('6.txt', 'w') as f:
        for line in a2:
            f.write(line)
    with open('7.txt', 'w') as f:
        for line in a3:
            f.write(line)
    with open('8.txt', 'w') as f:
        for line in a4:
            f.write(line)

def devide_huibian():
    a = []
    with open('code_4_new1.txt', 'r') as f:
        for line in f:
            a.append(line)

    a1, a2, a3, a4= [], [], [], []
    for i in range(int(len(a)/4)):
        a1.append(a[4*i])
        a2.append(a[4*i+1])
        a3.append(a[4*i+2])
        a4.append(a[4*i+3])

    with open('code_4_new1_devided.txt', 'w') as f:
        f.write('\\\\ALU1\n')
        for i in a1:
            f.write(i)
        f.write('\\\\ALU2\n')
        for i in a2:
            f.write(i)
        f.write('\\\\ALU3\n')
        for i in a3:
            f.write(i)
        f.write('\\\\ALU4\n')
        for i in a4:
            f.write(i)


def test():
    devide_mul()

if __name__ == "__main__":
    test()