f = open('code_2.txt','r')
a = []
b = []
for line in f:
    a.append(line)
base = 0
for i in range(168):
    if a[base][0:3] == 'cpa' and a[base+1][0:3] =='cpd' and a[base+2][0:3] == 'cpa'and a[base+3][0:3] =='cpd':
        r1 = int((int(a[base][6:8]) + 1) / 2)
        r2 = int((int(a[base+2][6:8]) + 1) / 2)
        r3 = int((int(a[base+1][6:8]) + 1) / 2)
        r4 = int((int(a[base+3][6:8]) + 1) / 2)
        b.append('cpia ${}, ${}'.format(r1,r2))
        b.append('cpid ${}, ${}'.format(r3,r4))
    elif a[base][0:3] == 'cpa' and a[base+1][0:3] =='cpa' and a[base+2][0:3] == 'cpa'and a[base+3][0:3] =='cpa':
        r1 = int((int(a[base][6:8]) + 1) / 2)
        r2 = int((int(a[base][11:13]) + 1) / 2)
        r3 = int((int(a[base+2][6:8]) + 1) / 2)
        r4 = int((int(a[base+2][11:13]) + 1) / 2)
        if (r1 == r2 or r3 == r4):
            r1 = int((int(a[base][6:8]) + 1) / 2)
            r2 = int((int(a[base + 2][6:8]) + 1) / 2)
            r3 = int((int(a[base + 1][6:8]) + 1) / 2)
            r4 = int((int(a[base + 3][6:8]) + 1) / 2)
            b.append('cpia ${}, ${}'.format(r1, r2))
            b.append('cpia ${}, ${}'.format(r3, r4))
        else:
            b.append('cpxa ${}, ${}'.format(r1,r2))
            b.append('cpxa ${}, ${}'.format(r3,r4))
            pass
    elif a[base][0:3] == 'cpd' and a[base+1][0:3] =='cpd' and a[base+2][0:3] == 'cpd' and a[base+3][0:3] =='cpd':
        r1 = int((int(a[base][6:8]) + 1) / 2)
        r2 = int((int(a[base][11:13]) + 1) / 2)
        r3 = int((int(a[base+2][6:8]) + 1) / 2)
        r4 = int((int(a[base+2][11:13]) + 1) / 2)
        b.append('cpxd ${}, ${}'.format(r1,r2))
        b.append('cpxd ${}, ${}'.format(r3,r4))
    elif a[base][0:3] == 'cpa' and a[base+1][0:3] =='cpa' and a[base+2][0:3] == 'cpd' and a[base+3][0:3] =='cpd':
        r1 = int((int(a[base][6:8]) + 1) / 2)
        r2 = int((int(a[base][11:13]) + 1) / 2)
        r3 = int((int(a[base+2][6:8]) + 1) / 2)
        r4 = int((int(a[base+2][11:13]) + 1) / 2)
        b.append('cpxa ${}, ${}'.format(r1,r2))
        b.append('cpxd ${}, ${}'.format(r3,r4))
    else:
        print('error')
    base +=4
f.close()

with open("data.txt","w") as f:
     for i in b:
         f.write(i+'\n')



