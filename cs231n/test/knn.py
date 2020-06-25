import numpy as np
import random
import matplotlib.pyplot as plt
def prepare_data_train():
    sigma_1 = np.array([[1, 0], [0, 1]])
    #4簇正态分布，
    n = 500
    mu = 1.6
    mu_1 = np.array([mu, mu])
    mu_2 = np.array([-mu, mu])
    mu_3 = np.array([mu, -mu])
    mu_4 = np.array([-mu, -mu])
    data_1 = np.dot(np.random.randn(n, 2),sigma_1)+mu_1
    label_1 = np.zeros(n)
    data_2 = np.dot(np.random.randn(n, 2),sigma_1)+mu_2
    label_2 = np.zeros(n)+1
    data_3 = np.dot(np.random.randn(n, 2),sigma_1)+mu_3
    label_3 = np.zeros(n)+2
    data_4 = np.dot(np.random.randn(n, 2),sigma_1)+mu_4
    label_4 = np.zeros(n)+3
    data_train = np.concatenate((data_1, data_2, data_3, data_4), axis=0)
    label = np.concatenate((label_1, label_2, label_3, label_4), axis=0)
    return data_train, label

def prepare_data_test():
    sigma_1 = np.array([[1,0],[0,1]])
    mu_1 = np.array([0, 0])
    data_1 = np.dot(np.random.randn(300, 2), sigma_1) + mu_1
    return data_1

def dis(vecA, vecB):
    return np.sqrt(np.sum(np.power(vecA - vecB, 2)))

def KNN(data_train, label_train,data_test, k):
    n_train = data_train.shape[0]
    n_test = data_test.shape[0]
    dis_array = np.zeros((n_test, n_train))
    label_test = np.zeros(n_test)

    for num_test in range(n_test):
        for num_train in range(n_train):
            dis_array[num_test,num_train] = dis(data_test[num_test],data_train[num_train])

    for i in range(n_test):
        temp_dic = {}
        for j in range(k):
            if(label_train[np.argsort(dis_array[i])[j]] not in temp_dic.keys()):
                temp_dic[label_train[np.argsort(dis_array[i])[j]]] = 1
            else:
                temp_dic[label_train[np.argsort(dis_array[i])[j]]]+=1

        pos = max(temp_dic, key=temp_dic.get)
        label_test[i] = pos.astype(int)

    return label_test

    # data_train是训练数据集，shape= （n_train，2）
    #data_test是测试数据集，shape= （n_test，2）
    #label_train 是每个测试用例的类别，shape = （n_train)
    # dis_array是存放每个测试用例和训练用例之间的距离， shape=（n_test， n_train）
    # label_test是最后要输出的结果，存放每个测试用例的类别， shape = （n_test)

if __name__ == "__main__":
    data_train, label_train = prepare_data_train()
    data_test = prepare_data_test()
    # parameters
    plot_data = False
    k = 1

    if plot_data:
        plt.subplot(121)
        plt.scatter(data_train[:, 0], data_train[:, 1], c= label_train)
        plt.subplot(122)
        plt.scatter(data_test[:, 0], data_test[:, 1],c='b')
        plt.show()

    label_test = KNN(data_train, label_train,data_test, k)

    plt.subplot(121)
    plt.scatter(data_train[:, 0], data_train[:, 1], c=label_train)
    plt.subplot(122)
    plt.scatter(data_test[:, 0], data_test[:, 1], c=label_test)
    plt.show()