#类型
#自动分配，无需申明

x_int = 5
x_float = 5.0
x_str1 = "xww"
x_str2 = 'xww'#''和""完全一样
x_str3 = '''xww'''#'''可以实现换行

x_tuple = (1,2,3)
x_list = [1,2,3]
x_dict = {"a":1,"b":1,"c":2}

# io
x_str =input("your name")#输入的是字符串
y_str = 'wml'
print (x_str,y_str)# ,是一个空格
print(x_str)#每一个print完自带换行
print(y_str)

#字符串'' "" ''' '''
x_str = "i am a freshman in yingcai honor college\n class %d"%(1)
print (x_str)
#字符串的清洗
x_str_clear = x_str.strip()
print(x_str_clear)
#字符串的分割
x_list = x_str_clear.split(' ')
print (x_list)
#字符串的访问
print("0",x_str[0])
print("2",x_str[2])
#字符串的拼接
y_str = "she and "
result_str = y_str + x_str
print("result_str",result_str)


#tuple  ()
#元组是不可变的数据结构，即不能对一个元组进行添加或删除元素，初始化是什么就是什么，有点像c中的const类型
#因为安全性，常用于函数返回值
x_tuple = (1,2,3)
print ("tuple",x_tuple)


#list  []
#列表是可变的数据结构，可以对其进行添加或删除，比较类似c++的vector
x_list = [1,2,3]
print(x_list)
#列表的添加
x_list.append(4)
print ("append 4",x_list)
#列表的删除1
x_list.pop(3)#pop删除对应索引的元素
print ("pop 3",x_list)
#列表的删除2
x_list.remove(2)#remove删除对应值的元素
print("remove 2",x_list)
#列表的合并
y_list = ['a','b','c']
x_list.extend(y_list)#extend是两个列表的拼接，拼接结果保存在x_list中
print ("extend y_list" ,x_list)
#列表的引用
z_list = x_list#python中的=是一种引用，对于可变数据类型，引用的是指针，对于不可变数据类型，引用的是值
#指针和引用不同，指针指向内存区域，引用引的是一个对象
print (
    "before\n",
    "x_list",x_list,'\n',
    "z_list",z_list,'\n'
)
z_list.append('d')
print (
    "after\n",
    "x_list",x_list,'\n',
    "z_list",z_list,'\n'
)


#dictionary  {}
#字典也是一个可变的数据结构，类似c++中的map
#字典是可以理解成一个特殊的数组，一般的数组是通过索引编号来访问某一个值，而字典可以创建key—value对，通过key来访问value
#注意key 只能是不可变数据结构，并且key值不能重复，重复将替代之前的key
x_dict = {"a":1,"b":1,"c":2}
print("x_dict['a']:",x_dict['a'])
#字典的动态创建及添加
x_dict= {}
x_dict['a'] = 2
print ("1",x_dict)
x_dict["b"] = 2
print ("2",x_dict)
x_dict["c"] = 3
print ("3",x_dict)
x_dict['a'] = 1
print ("4",x_dict)
x_dict[('a','b')]=4
print ("5",x_dict)

#字典的遍历
print("keys\n",x_dict.keys())
print("keys\n",x_dict.values())

#字典和列表都是无序的，是按照创建顺序排序
y_dict = {'z':26,'a':1,"y":25}
print(y_dict)

#字典的删除
x_dict.pop(('a','b'))#有返回值
print (x_dict)

#字典的更新
y_dict = {"d":4,"e":5}
print ("before",x_dict)
x_dict.update(y_dict)
print("update",x_dict)


#数组
#数组和矩阵不同，数组是对数组中每个元素做相同的操作，矩阵是一个整体，有自己的运算法则
import numpy as np
x_list = [[1,2,3],[4,5,6]]
x_array = np.array(x_list)
print ("list",x_list)
print ("array",x_array)
y_array = np.array([[7,8,9],[10,11,12]])

#数组加法
result_sum = x_array + y_array
print ("sum",result_sum)

#数组乘法
result_mul = x_array * y_array
print ("mul",result_mul)

#矩阵乘法
result_dot = np.dot(x_array,x_array.T)
print ('dot',result_dot)
result_mat =  np.matmul(x_array,x_array.T)#这两种方法完全一样
print ('mat',result_mat)

#numpy中的矩阵格式
x_mat = np.mat(x_array)
y_mat = np.mat(y_array)
print ("matrix",x_mat)
print ("mul",x_mat*x_mat.T)
print('reverse',(x_mat).I)#mat这种数据结构有逆的功能，其他和array类似
print('reverse2',np.mat([[2,0],[0,1]]).I)

#array的自动补全
x_array = np.array( [ [ 1,2 ], [ 3,4 ], [5,6] ] )
y_array = np.array( [ [2], [1], [3] ] )
result_sum = x_array + y_array
result_mul = x_array * y_array
print ('sum',result_sum)
print ('mul',result_mul)

#if ,for，while 缩进
x=2
if x>1:
    print("x>1")
else:
    print("x<=1")

for i in range(5): #in 后面必须是个可迭代对象
    print (i)

for i in [1,2,3]:
    print(i)

for i in "i am a freshman in yingcai honor college":
    print (i)

while(x>-1):
    print (x)
    x-=1

#函数
def hello(in_arg):
    print("hello",in_arg)
    print("this is hello func")

hello("xww")

#类
class people(object):
    def __init__(self,name,sex,hobby):
        self.name = name
        self.sex = sex
        self.hobby = hobby
    def introduce(self):
        print("hello i am",self.name)
        print("i am a",self.sex)
        print("i like",self.hobby)

xww = people("xww","man","woman")
xww.introduce()

#主函数
if __name__ == "__main__":
    pass#写调试语句
