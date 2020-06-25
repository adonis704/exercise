import numpy as np

class layers:
    def softmax_loss(x, y):
        shifted_logits = x - np.max(x, axis=1, keepdims=True)
        Z = np.sum(np.exp(shifted_logits), axis=1, keepdims=True)
        log_probs = shifted_logits - np.log(Z)
        probs = np.exp(log_probs)
        N = x.shape[0]
        loss = -np.sum(log_probs[np.arange(N), y]) / N
        dx = probs.copy()
        dx[np.arange(N), y] -= 1
        dx /= N
        return loss, dx

    def affine_forward(x, w, b):
        out = None
        out = np.dot(x, w) + b
        cache = (x, w, b)
        return out, cache

    def affine_backward(dout, cache):
        x, w, b = cache
        dx, dw, db = None, None, None

        dx = np.dot(dout, w.T)
        dx = dx.reshape(x.shape)
        dw = np.dot(x.reshape(x.shape[0], -1).T, dout)
        db = np.sum(dout, axis=0)

        return dx, dw, db

    def relu_forward(x):
        out = None
        out = np.maximum(0, x)
        cache = x
        return out, cache

    def relu_backward(dout, cache):
        dx, x = None, cache
        dx = dout * (x > 0).astype(np.int)
        return dx

class myBP:
    w1 = np.ones((4,10),dtype = float)/1000
    b1 = np.zeros((1,10),dtype = float)
    w2 = np.ones((10,3),dtype = float)/1000
    b2 = np.zeros((1,3),dtype = float)
    learningRate = 0.2
    def __init__(self,trainX,trainY,testX,testY):
        self.trainX = trainX
        self.trainY = trainY
        self.testX = testX
        self.testY = testY

    def propagate(self):
        z1, cachZ1 = layers.affine_forward(self.trainX,self.w1,self.b1)
        a1,cachA1 = layers.relu_forward(z1)
        z2,cachZ2 = layers.affine_forward(a1,self.w2,self.b2)
        loss,dz2 = layers.softmax_loss(z2,self.trainY)

        da1,dw2,db2 = layers.affine_backward(dz2,cachZ2)
        dz1 = layers.relu_backward(da1,cachA1)
        _,dw1,db1 = layers.affine_backward(dz1,cachZ1)

        self.w1 -= self.learningRate*dw1
        self.w2 -= self.learningRate * dw2
        self.b1 -= self.learningRate * db1
        self.b2 -= self.learningRate * db2
        return loss

    def train(self,times):
        for i in range(times):
            loss = self.propagate()
            if (i+1)%500 == 0:
                print("Trained {} times, cost:{}".format(i+1,loss))

    def test(self):
        print("Testing.")
        testSize = self.testX.shape[0]
        correct = 0
        wrong = 0
        z1, _ = layers.affine_forward(self.trainX,self.w1,self.b1)
        a1,_ = layers.relu_forward(z1)
        z2,_ = layers.affine_forward(a1,self.w2,self.b2)
        score = np.e**z2
        for i in range(testSize):
            max = score[i][0]
            pos = 0
            for j in range(1,score.shape[1]):
                if score[i][j] > max:
                    max = score[i][j]
                    pos = j
            if testY[i] == pos:
                correct += 1
            else:
                wrong += 1

        rate = correct/(correct+wrong)
        print("Correction:{}%".format(rate*100))

if __name__ == "__main__":
    trainX = []
    trainY = []
    testX = []
    testY = []
    with open(r"D:\Data\cProject\aiExp\Iris-train.txt", 'r') as file:
        while True:
            lines = file.readline()
            if not lines: break
            temp = []
            for item in lines.split(" ")[:-2]:
                temp.append(float(item))
            trainX.append(temp)
            trainY.append(int(lines.split(" ")[-2:-1][0]))
    with open(r"D:\Data\cProject\aiExp\Iris-test.txt", 'r') as file:
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

    bp = myBP(trainX,trainY,testX,testY)
    bp.train(10000)
    bp.test()