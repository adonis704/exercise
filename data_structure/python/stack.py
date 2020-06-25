'''
testing:
in:  exp1 = '10-(3-5)*2+15/3'
     exp2 = '10-(3-5)*2+15/4'
out: 19.0
     17.75
'''

def to_RPN(express):
    stack =[]
    list = []
    num = []
    sig1 = ['+','-']
    sig2 = ['*','/']

    for char in express:
        if char.isdigit():
            num.append(char)

        if not char.isdigit():
            if num:
                raw = ''
                num.reverse()
                while num:
                    raw += num.pop()
                list.append(raw)

            if not len(stack) and not char.isdigit():
                stack.append(char)
                continue

            if char in sig1 or char == '(':
                if stack[-1] not in sig2:
                    stack.append(char)
                else:
                    while stack:
                        list.append(stack.pop())
                    stack.append(char)

            if char in sig2:
                stack.append(char)

            if char == ')':
                temp = stack.pop()
                while temp!= '(':
                    list.append(temp)
                    temp = stack.pop()

    if num:
        raw = ''
        num.reverse()
        while num:
            raw += num.pop()
        list.append(raw)

    while stack:
        list.append(stack.pop())

    return list

def cal(express):
    stack = []
    express = to_RPN(express)

    for char in express:
        if char.isdigit():
            stack.append(char)

        if char == '+':
            temp1 = float(stack.pop())
            temp2 = float(stack.pop())
            temp3 = temp2 + temp1
            stack.append(temp3)

        if char == '-':
            temp1 = float(stack.pop())
            temp2 = float(stack.pop())
            temp3 = temp2 - temp1
            stack.append(temp3)

        if char == '*':
            temp1 = float(stack.pop())
            temp2 = float(stack.pop())
            temp3 = temp2 * temp1
            stack.append(temp3)

        if char == '/':
            temp1 = float(stack.pop())
            temp2 = float(stack.pop())
            temp3 = temp2 / temp1
            stack.append(temp3)

    return stack.pop()



def test_to_RPN():
    exp1 = '10-(3-5)*2+15/3'
    print(to_RPN(exp1))

def test_cal():
    exp1 = '10-(3-5)*2+15/3'
    exp2 = '10-(3-5)*2+15/4'
    print(cal(exp1),cal(exp2))

if __name__ == '__main__':
    test_cal()


