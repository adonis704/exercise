import numpy as np
import random
import matplotlib.pyplot as plt
def prepare_data():#生成3个数据集
    dataset=[]
    #2簇正态分布 dataset[0]
    mu_1 = np.array([5,5])
    mu_2 = np.array([-5,-5])
    sigma_1 = np.array([[1,0],[0,1]])
    data_1 = np.dot(np.random.randn(1000, 2),sigma_1)+mu_1
    data_2 = np.dot(np.random.randn(1000, 2),sigma_1)+mu_2
    dataset.append(np.concatenate((data_1, data_2),axis=0))

    #1簇正态分布，均值【0,0】, dataset[1]
    mu_1 = np.array([0, 0])
    data_1 = np.dot(np.random.randn(1000, 2), sigma_1) + mu_1
    dataset.append(data_1)

    #4簇正态分布，dataset[2]
    mu_1 = np.array([5, 5])
    mu_2 = np.array([-5, 5])
    mu_3 = np.array([5, -5])
    mu_4 = np.array([-5, -5])
    data_1 = np.dot(np.random.randn(300, 2),sigma_1)+mu_1
    data_2 = np.dot(np.random.randn(300, 2),sigma_1)+mu_2
    data_3 = np.dot(np.random.randn(300, 2),sigma_1)+mu_3
    data_4 = np.dot(np.random.randn(300, 2),sigma_1)+mu_4
    dataset.append(np.concatenate((data_1, data_2, data_3, data_4), axis=0))

    return dataset

def dis(vecA, vecB):#返回两个点之间的欧氏距离
    return np.sqrt(np.sum(np.power(vecA - vecB, 2)))

def init_centers(data, k):#初始化簇中心
    lens = data.shape[0]
    centers = random.sample(range(lens), k)
    return data[centers]


def Kmeans(data, k):
    lens = data.shape[0]
    centers = init_centers(data, k)
    dis_array = np.zeros([lens, k])
    clusters = np.zeros(lens)
    T = 0
    while T<10000:
        T+=1
        #更新dis_array
        for k_count in np.arange(k):
            for i in np.arange(lens):
                distance = dis(data[i],centers[k_count])
                dis_array[i,k_count] = distance
        #更新clusters
        for i in np.arange(lens):
            min_dis = dis_array[i,0]
            min_k_num = 0
            for k_count in np.arange(1,k):
                 if(dis_array[i,k_count]<min_dis):
                     min_dis = dis_array[i,k_count]
                     min_k_num = k_count
            clusters[i] = min_k_num

        dis_sum = np.zeros((k,2))
        clu_num = np.zeros(k)
        for i in np.arange(lens):
            dis_sum[int(clusters[i])] += data[i]
            clu_num[int(clusters[i])]+=1

        centers_new = dis_sum/np.tile(clu_num, (2,1)).T

        if ((centers_new == centers).all()):
            break

        else:
            centers = centers_new

        #你需要在这里实现kmeans的两大主体部分，其一是给每个数据归簇，其二是计算新的簇中心。之前的T是指迭代次数，就是说迭代10000次必然结束，防止出现永不收敛而一直运行的情况，之后的plt相关内容是画图，即每轮kmeans都画一次图
        #代码中已给出一些变量，如果不习惯，可自行修改，其中
        #centers是簇中心，随机初始化，是一个2-d array，shape是（k，2），k是簇的个数，2是指数据是2维的
        #dis_array是存放数据和簇中心距离的2-d array，shape是（lens，k），lens是数据个数，k是簇的个数
        #clusters是存放每个数据对应簇编号的2-d array，shape是（len，1）
        #注意dis_array，clusters，centers都是动态更新的，即每一轮迭代后结果都会保存在这些变量中，因此会覆盖之前的值

        plt.cla()
        plt.scatter(data[:, 0], data[:, 1], c=clusters)
        plt.plot(centers[:, 0], centers[:, 1], "+", c='r')
        plt.pause(0.5)
    return centers, clusters


if __name__ == "__main__":
    # prepare data
    dataset = prepare_data()
    #parameters
    plot_data = False
    k = 4
    data = dataset[2]

    if plot_data:
        plt.scatter(data[:, 0], data[:, 1])
        plt.show()
    # plot
    plt.ion()
    plt.show()

    #kmeans
    centers, clusters = Kmeans(data, k)

    #plot
    plt.scatter(data[:, 0], data[:, 1], c=clusters)
    plt.plot(centers[:,0],centers[:,1],"+",c='r')
    plt.pause(2)