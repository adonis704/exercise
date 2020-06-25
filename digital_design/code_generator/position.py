f = open('code1.txt','r')
a = []
for line in f:
    a.append(line)
print(a[0][11:13])