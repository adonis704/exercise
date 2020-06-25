from random import shuffle

def sort_asc(a,b,arr):
    if arr[a] > arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp
    #     print('asc{} {}'.format(a, b))
    # else:
    #     print('no asc{} {}'.format(a, b))
    newline = 'cpa $t{} $t{}'.format(a, b)
    print(newline)


def sort_desc(a,b,arr):
    if arr[a] < arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp
    #     print('desc{} {}'.format(a, b))
    # else:
    #     print('no desc{} {}'.format(a, b))
    newline = 'cpd $t{} $t{}'.format(a, b)
    print(newline)

def DT(arr,ret = 0,reverse = 0):
    if not ret:
        window = len(arr)
        while(window >= 2):
            group = len(arr) / window
            for times in range(int(group)):
                base = times*window
                for j in range(int(window/2)):
                    sort_asc(int(base+j), int(base+j+window/2),arr)
            window = window/2
    elif (ret and not reverse):
        arr = arr
        window = len(arr)
        while(window >= 2):
            group = len(arr) / window
            for times in range(int(group)):
                base = times*window
                for j in range(int(window/2)):
                    sort_asc(int(base+j), int(base+j+window/2),arr)
            window = window/2
        return arr
    else:
        arr = arr
        window = len(arr)
        while(window >= 2):
            group = len(arr) / window
            for times in range(int(group)):
                base = times*window
                for j in range(int(window/2)):
                    sort_desc(int(base+j), int(base+j+window/2),arr)
            window = window/2
        return arr


def to_DT(arr):
    initial(arr)
    window = 4
    while(window <= len(arr)):
        group = len(arr) / (2*window)
        for times in range(int(group)):
            base = int(2*times*window)
            temp_arr = DT(arr[base:base + window],ret = 1)
            for i in range(len(temp_arr)):
                arr[int(base+i)] = temp_arr[i]
            temp_arr = DT(arr[base + window:base + 2*window], ret=1,reverse = 1)
            # temp_arr.reverse()
            for i in range(len(temp_arr)):
                arr[int(base+i+window)] = temp_arr[i]
        window = window*2

def initial(arr):
    window = 4
    group = len(arr)/window
    for times in range(int(group)):
        base = times*window
        sort_asc(int(base), int(base+1), arr)
        sort_desc(int(base+2),int(base+3),arr)

def write_txt():
    f = open('out.txt')


if __name__ == '__main__':
    x = []
    for i in range(64):
        x.append(i)
    shuffle(x)
    to_DT(x)
    DT(x)
    write_txt()

