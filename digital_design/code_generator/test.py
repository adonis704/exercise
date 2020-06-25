from random import shuffle

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
    newline = 'cpa $t{} $t{}'.format(a+base, b+base)
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
    newline = 'cpd $t{} $t{}'.format(a+base, b+base)
    print(newline)

def initial(arr):
    window = 4
    group = len(arr)/window
    for times in range(int(group)):
        base = times*window
        sort_asc(int(base), int(base+1), arr)
        sort_desc(int(base+2),int(base+3),arr)

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
        print('mark')
        window /=2


def to_DT(arr):
    window = 4
    while(window <= len(arr)):
        base = 0
        for i in range(int(len(arr)/window)):
            temp1 = []
            temp2 =[]
            for j in range(int(window/2)):
                temp1.append(arr[int(base+j)])
                temp2.append(arr[int(base+j+window/2)])
            DT(temp1,base,0)
            DT(temp2,base+window/2,1)
            for j in range(int(window/2)):
                arr[int(base+j)] = temp1[j]
                arr[int(base+j+window/2)] = temp2[j]
            base +=window
        window *=2


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
