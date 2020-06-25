const app = getApp()
Page({
  data: {
    deviceID: '',
    password: '',
    deviceName: '',
    passwordCor: '',
    name: ''
  },

  inputDevice: function(e){
    var that = this
    that.setData({ deviceID: e.detail.value });
    const db = wx.cloud.database()
    db.collection('device').where({
      deviceID: that.data.deviceID
    }).get({
      success: function(res){
        if (res.data[0].deviceID){
          that.setData({passwordCor: res.data[0].password});
          console.log(that.data.passwordCor);
        }
      }
    })
  },
  inputPassword: function (e) {
    this.setData({ password: e.detail.value });
  },
  inputName: function (e) {
    this.setData({ deviceName: e.detail.value });
  },

  bind: function(e){
    const db = wx.cloud.database();
    if (this.data.deviceID == '' || this.data.password == '' || this.data.deviceName == ''){
      wx.showToast({
        title: '请完整输入信息',
        icon: 'none',
        duration: 2000
      });
      return
    }

    if (app.globalData.userDevice == ''){
      if (this.data.password == this.data.passwordCor){
        db.collection('user').add({
          data: {
            deviceID: this.data.deviceID,
            deviceName: this.data.deviceName,
          }
        });
        // console.log('绑定成功')
        wx.showToast({
          title: '绑定成功',
          duration: 2000
        });
        this.setData({name: ''});
        db.collection('user').where({
          _openid: app.globalData.openid
        }).get({
          success: function (res) {
            if (res.data[0].deviceID) {
              app.globalData.userDevice = res.data[0].deviceID;
              app.globalData.deviceName = res.data[0].deviceName;
              console.log('find.')
            };
          },
          fail: function (err) {
            console.log('no')
          }
        });
      } else {
        // console.log('密码错误')
        wx.showToast({
          title: '密码错误',
          icon: 'none',
          duration: 2000
        })
      }

    } else {
      // console.log('用户已绑定该设备')
      wx.showToast({
        title: '无法重复绑定',
        icon: 'none',
        duration: 2000
      })
    }
  },

  forgetpassword: function(e){
    wx.navigateTo({
      url: '../forgetPassword/forgetPassword',
    })
  }
})
