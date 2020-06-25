import numpy as np
import matplotlib.pyplot as plt



def prepare_data_train():
    sigma_1 = np.array([[1, 0], [0, 1]])
    #4簇正态分布，
    n = 500
    mu = 3
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

def sigmoid(x):
    return 1/(1 + np.exp(-x))
def sigmoid_derivative(x):
    return sigmoid(x)*(1-sigmoid(x))

class Net(object):
    def __init__(self):
        self.theta_1 = np.zeros([2,4])
        self.b_1 = np.zeros([1,4])
        self.theta_2 = np.zeros([4,4])
        self.b_2 = np.zeros([1,4])

    def train(self, x, y):
        '''
        :param x: x是我们输入的数据,每次一个数据，1*2
        :param y: x是我们输入的数据的标签,每次一个数据，1*4

        '''
        #forward
        hidden = np.dot(x,self.theta_1) + self.b_1
        hidden_a = sigmoid(hidden)
        out = np.dot(hidden_a,self.theta_2) + self.b_2
        out_a = sigmoid(out)
        predict = np.argmax(out_a,axis=1)[0]
        loss = 0.5 * ((y - out_a) ** 2).sum()

        #backward



        return loss, [predict]



if __name__ == "__main__":
    #parameter
    EPOCH = 60
    LR = 0.01
    plot_data = False
    embedding = {0:np.array([1,0,0,0]), 1:np.array([0,1,0,0]), 2:np.array([0,0,1,0]), 3:np.array([0,0,0,1])}
    #data
    data_train, label_train = prepare_data_train()
    n = data_train.shape[0]
    #model
    net = Net()

    #显示数据
    if plot_data:
        plt.subplot(121)
        plt.scatter(data_train[:, 0], data_train[:, 1], c= label_train)
        plt.subplot(122)
        plt.scatter(data_train[:, 0], data_train[:, 1],c='b')
        plt.show()
    #
    plt.ion()
    plt.show()

    for epoch in range(EPOCH):
        predict = []
        for i in range(n):
            loss,predict_one = net.train(np.array([data_train[i]]), embedding[int(label_train[i])])
            predict.append(predict_one)
        predict = np.array(predict)
        if epoch % 3 == 0:
            good = 0
            for k in range(n):
                if predict[k] == label_train[k]:
                    good+=1
            acc = good/n
            plt.cla()
            plt.subplot(121)
            plt.scatter(data_train[:, 0], data_train[:, 1], c=label_train)
            plt.subplot(122)
            plt.scatter(data_train[:, 0], data_train[:, 1], c=predict)
            plt.text(1, -8, "loss = %.4f\nacc = %.4f" % (loss, acc), fontdict={"size": 15, "color": "red"})
            plt.pause(0.5)


