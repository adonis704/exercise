'''
Assuming:
Every customer take 4-6 mins to finish their business.
Every 5 mins comes a new customer.

Testing:
 No.1: waiting time 0 min(s)
 No.2: waiting time 1 min(s)
 No.2: waiting time 0 min(s)
 No.3: waiting time 6 min(s)
 No.3: waiting time 3 min(s)
 ...
'''

import time
import random
import sys

TIME_COME_PER = 5

class Myqueue:
    def __init__(self,data=[]):
        self.data = data

    def pop(self):
        self.data.reverse()
        self.data.pop()
        self.data.reverse()

    def push(self,value):
        self.data.append(value)

    def __getitem__(self, item):
        return self.data[item]


def bank_wait():
    time_process = 0
    time_come = 1
    myqueue = Myqueue([random.randint(4,6)])
    num = 1

    while True:
        if time_come % TIME_COME_PER == 0:
            time_process_new = random.randint(4,6)
            myqueue.push(time_process_new)
            num += 1

        if myqueue.data and time_process == myqueue[0]:
            myqueue.pop()
            time_process = 0

        sys.stdout.write("\r No.{}: waiting time {} min(s)".format(num,sum(myqueue[:-1])))

        time_process+=1
        time_come+=1
        if myqueue.data[0]>0:
            myqueue.data[0] -=1
        time.sleep(1)

if __name__ == '__main__':
    bank_wait()


