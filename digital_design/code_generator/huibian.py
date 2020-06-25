from random import shuffle
import math

def sort_asc(a,b,arr,base):
    a = int(a)
    b = int(b)
    base = int(base)
    if arr[a] > arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp
    #     print('asc{} {}'.format(a, b))
    # else:
    #     print('no asc{} {}'.format(a, b))
    if (a+base+1 >= 10 and b+base+1 >= 10):
        newline = 'cpa $t{} $t{}'.format(a+base+1, b+base+1)
    elif (a+base+1 < 10 and b+base+1 >= 10):
        newline = 'cpa $t0{} $t{}'.format(a + base + 1, b + base + 1)
    elif (a+base+1 >= 10 and b+base+1 < 10):
        newline = 'cpa $t{} $t0{}'.format(a + base + 1, b + base + 1)
    else:
        newline = 'cpa $t0{} $t0{}'.format(a + base + 1, b + base + 1)
    print(newline)


def sort_desc(a,b,arr,base):
    a = int(a)
    b = int(b)
    base = int(base)
    if arr[a] < arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp
    #     print('desc{} {}'.format(a, b))
    # else:
    #     print('no desc{} {}'.format(a, b))
    if (a+base+1 >= 10 and b+base+1 >= 10):
        newline = 'cpd $t{} $t{}'.format(a+base+1, b+base+1)
    elif (a+base+1 < 10 and b+base+1 >= 10):
        newline = 'cpd $t0{} $t{}'.format(a + base + 1, b + base + 1)
    elif (a+base+1 >= 10 and b+base+1 < 10):
        newline = 'cpd $t{} $t0{}'.format(a + base + 1, b + base + 1)
    else:
        newline = 'cpd $t0{} $t0{}'.format(a + base + 1, b + base + 1)
    print(newline)

def window(arr,base,rev = 0):
    w = len(arr)
    for i in range(int(w/2)):
        if (not rev):
            sort_asc(i,i+w/2,arr,base)
        else:
            sort_desc(i,i+w/2,arr,base)

def to_DT(arr):
    w = 4
    wt = 2
    while(w <= len(arr)):
        while (wt >= 2):
            base = 0
            for outLoop in range(int(len(arr)/w)):
                base_temp = base
                for i in range(int(w/wt/2)):
                    temp1 = []
                    temp2 = []
                    for j in range(int(wt)):
                        temp1.append(arr[int(base_temp+j)])
                        temp2.append(arr[int(base_temp+w/2+j)])
                    window(temp1,base_temp,0)
                    window(temp2,base_temp+w/2,1)
                    for j in range(len(temp1)):
                        arr[int(base_temp + j)] = temp1[j]
                        arr[int(base_temp + j + w / 2)] = temp2[j]
                    base_temp +=wt
                base +=w
            wt /=2
            # print('mark')
        w *=2
        wt = w/2

def DT(arr,init, rev = 0):
    window = len(arr)
    base = 0
    while(window >=2):
        for times in range(int(len(arr)/window)):
            for i in range(int(window/2)):
                if(not rev):
                    sort_asc(base+i+window*times,base+i+window*times+window/2,arr,init)
                else:
                    sort_desc(base + i + window * times, base + i + window * times + window / 2, arr,init)
        # print('mark')
        window /=2

if __name__ == '__main__':
    # x = [3,5,8,9,10,12,14,20,95,90,60,40,35,23,18,0]
    x = []
    for i in range(64):
        x.append(i)
    shuffle(x)
    print('initial:' + str(x))
    to_DT(x)
    DT(x,0)
    print('after:' + str(x))
