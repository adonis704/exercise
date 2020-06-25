'''
search testing:
in: data = [4,7,10,12,23,5,1,100,50,20,42,123] #disorder
in: search_numbers = [-10,5,130]
out:
No value matched -10.
The location of the value 5 is : 2
No value matched 130.

add testing:
in: data_added = [-100,4.1,7.2,10.3,12,23,5,1,100,50,20,42,123]
out: [-100, 1, 4.1, 5, 7.2, 10.3, 12, 20, 23, 42, 50, 100, 123]
'''

class Data:
    def __init__(self,data=[]):
        self.data = data
        self.data.sort()

    def add(self,value):
        self.data.append(value)
        self.data.sort()

    def search(self,value):
        first = 0
        last = len(self.data)-1
        while first < last:
            mid = first + (last-first)//2
            if self.data[mid] < value:
                first = mid +1
            else:
                last = mid
        if self.data[first] == value:
            print("The location of the value {} is : {}".format(value,first))
        else:
            print("No value matched {}.".format(value))

def test1():
    data = [4,7,10,12,23,5,1,100,50,20,42,123]
    data = Data(data)
    search_numbers = [-10,5,130]
    for num in search_numbers:
        data.search(num)

def test2():
    data_added = [-100,4.1,7.2,10.3,12,23,5,1,100,50,20,42,123]
    data = Data()
    for num in data_added:
        data.add(num)
    print(data.data)


if __name__ == '__main__':
    test1()
    test2()

