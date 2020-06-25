import numpy as np

class BP:
    w1 = np.ones((10,4),dtype = float)/10
    b1 = np.zeros((10,1),dtype = float)
    w2 = np.ones((3,10),dtype = float)/10
    b2 = np.zeros((3,1),dtype = float)
    learningRate = float(0.95)
    def __init__(self,trainX,trainY,testX,testY):
        self.trainX = trainX
        self.trainY = trainY
        self.testX = testX
        self.testY = testY
        self.trainSize = trainY.shape[0]

    def sigmoid(self,z):
        res = 1/(1+np.e**(-z))
        return res

    def propagate(self):
        z1 = np.dot(self.w1,self.trainX.T)+self.b1
        a1 = self.sigmoid(z1)
        z2 = np.dot(self.w2,a1) + self.b2
        z2 -= np.max(z2,axis = 0)
        a2 = np.e**z2
        a2 /= np.sum(a2,axis=0)

        score = -np.log(a2[self.trainY,range(self.trainSize)])
        cost = np.sum(score,axis = 0)
        cost/=self.trainSize

        a2[self.trainY,range(self.trainSize)] -= 1
        dw2 = 1/self.trainSize*np.dot(a2,a1.T)
        db2 = np.sum(1/self.trainSize*a2,axis = 1,keepdims= True)
        dw1 = 1/self.trainSize*np.dot(np.dot(np.dot(np.dot(self.w2.T, a2),a1.T),1-a1),self.trainX)
        db1 = np.sum(1/self.trainSize*np.dot(np.dot(np.dot(self.w2.T, a2),a1.T),1-a1),axis = 1,keepdims= True)
        parameter = {
            "cost":cost,
            "dw2":dw2,
            "db2":db2,
            "dw1":dw1,
            "db1":db1
        }
        return parameter

    def update(self,times):
        for i in range(times):
            parameter = self.propagate()
            cost = parameter["cost"]
            dw2 = parameter["dw2"]
            dw1 = parameter["dw1"]
            db2 = parameter["db2"]
            db1 = parameter["db1"]
            # if (i+1)%10 == 0:
            #     print("cost:{}".format(cost))
            self.w2 -= self.learningRate*dw2
            self.b2 -= self.learningRate*db2
            self.w1 -= self.learningRate * dw1
            self.b1 -= self.learningRate * db1

    def test(self):
        testSize = self.testX.shape[0]
        correct = 0
        wrong = 0
        z1 = np.dot(self.w1, self.testX.T) + self.b1
        a1 = self.sigmoid(z1)
        z2 = np.dot(self.w2, a1) + self.b2
        z2 -= np.max(z2, axis=0)
        a2 = np.e ** z2
        a2 /= np.sum(a2, axis=0)
        a2T = a2.T
        for i in range(testSize):
            max = a2T[i][0]
            pos = 0
            for j in range(1,a2T.shape[1]):
                if a2T[i][j] > max:
                    max = a2T[i][j]
                    pos = j
            if testY[i] != pos:
                correct += 1
            else:
                wrong += 1

        rate = correct/(correct+wrong)
        print("Correction:{}".format(rate))


if __name__ == "__main__":
    trainX = []
    trainY = []
    testX = []
    testY = []
    with open("D:\Data\Work\人工智能答案解析\人工智能-实验-材料-2019-钟秀琴\人工智能-实验-材料-2019-钟秀琴\实验数据\Iris-train.txt", 'r') as file:
        while True:
            lines = file.readline()
            if not lines: break
            temp = []
            for item in lines.split(" ")[:-2]:
                temp.append(float(item))
            trainX.append(temp)
            trainY.append(int(lines.split(" ")[-2:-1][0]))
    with open("D:\Data\Work\人工智能答案解析\人工智能-实验-材料-2019-钟秀琴\人工智能-实验-材料-2019-钟秀琴\实验数据\Iris-test.txt", 'r') as file:
        while True:
            lines = file.readline()
            if not lines: break
            temp = []
            for item in lines.split(" ")[:-2]:
                temp.append(float(item))
            testX.append(temp)
            testY.append(int(lines.split(" ")[-2:-1][0]))
    trainX = np.array(trainX)
    trainY = np.array(trainY)
    testX = np.array(testX)
    testY = np.array(testY)
    # trainY = trainY.reshape([75,1])

    bp = BP(trainX,trainY,testX,testY)
    bp.update(100)
    bp.test()