import math

def DT(arr):
    length = len(arr)
    L = 2
    while (L <= length):
        L_temp = L
        L_wd = L
        flag = 1
        for i in range(int(math.log(L,2))):
            for k in range(int(length/(2*L_temp))):
                base = int(k*L_temp*2)
                for j in range(int(L_wd/2)):
                    sort_asc(j+base,int(j+base+L_temp/2),arr)
                if L_temp == length:
                    flag = 0
                    print('L_temp = {} flag = 0'.format(L_temp))
                if flag:
                    for j in range(int(L_wd/2)):
                        sort_desc(base+int(L_temp+j),base+int(L_temp+j+L_temp/2),arr)
                if not flag and L_temp != length:
                    for j in range(int(L_wd / 2)):
                        sort_asc(base+int(L_temp+j),base+int(L_temp+j+L_temp/2),arr)
            L_temp = L_temp/2
        L = L*2
        print('L = {}'.format(L))

def sort_asc(a,b,arr):
    if arr[a] > arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp
        print('asc{} {}'.format(arr[a], arr[b]))
    else:
        print('no asc{} {}'.format(arr[a], arr[b]))


def sort_desc(a,b,arr):
    if arr[a] < arr[b]:
        temp = arr[b]
        arr[b] = arr[a]
        arr[a] = temp
        print('desc{} {}'.format(arr[a], arr[b]))
    else:
        print('no desc{} {}'.format(arr[a], arr[b]))

if __name__ == '__main__':
    arr = [2,3,1,4,5,7,8,6]
    DT(arr)
    for i in arr:
        print(i)
