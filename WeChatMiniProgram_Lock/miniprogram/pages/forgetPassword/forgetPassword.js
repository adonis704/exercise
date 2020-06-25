const app = getApp();
const db = wx.cloud.database();
var codeBol = true;
Page({
  data: {
    deviceID: '',
    phone: '',
    code1: '',
    PUK: '',
    codeCor: '9421',
    phoneCor:'fhsdajfhsdf',
    PUKCor: 'fajskldawhjq',
    code:'获取验证码',
  },

  inputDevice: function (e) {
    var that = this
    that.setData({ deviceID: e.detail.value });
    db.collection('device').where({
      deviceID: that.data.deviceID
    }).get({
      success: function (res) {
        that.setData({ 
          phoneCor: res.data[0].phone,
          PUKCor: res.data[0].puk
           });
        console.log(that.data.phoneCor)
      }
    });
  },
  inputPhone: function (e) {
    this.setData({ phone: e.detail.value });

  },
  inputCode: function (e) {
    this.setData({ code1: e.detail.value });
  },

  getcode: function(e){
    var that = this;
    if (codeBol){
      db.collection('code').add({
        data: {
          deviceID: that.data.deviceID,
          code: '9421'
        },
        success: function (res) {
          console.log('code submit')
          wx.showToast({
            title: '验证码已发送',
            duration: 2000
          });
          codeBol = false;
          that.countCode();
        }
      });
    } ;
  },

  countCode: function(){
    var that = this;
    var count = 60
    var timer = setInterval(function () {
      count--;
      that.setData({ code: count < 10 ? `请等待(0${count}s)` : `请等待(${count}s)`})
      if (count == 0) {
        count = 60;
        clearInterval(timer);
        that.setData({code: '获取验证码'});
        codeBol = true;
      }
    }, 1000);

  },

  inputPUK: function(e){
    this.setData({ PUK: e.detail.value });
  },

  submit: function(e){
    var that = this
    db.collection('device').where({
      deviceID: that.data.deviceID
    }).get({
      success: function (res) {
        console.log('device finded.');
        if (that.data.PUK != that.data.PUKCor){
          wx.showToast({
            title: 'PUK码错误',
            icon: 'none',
            duration: 2000
          });
          return
        }
        if (that.data.phone == that.data.phoneCor){
          if (that.data.code1 == that.data.codeCor) {
            console.log('password matched.');
            wx.cloud.callFunction({
              name: 'update',
              data: {
                _id: res.data[0]._id,
                password: '123'
              },
              success: function (res) {
                // console.log('yes');
                wx.showToast({
                  title: '密码已重置',
                  duration: 2000
                });

                wx.cloud.callFunction({
                  name: 'delete',
                  data: {
                    deviceID: that.data.deviceID
                  },
                  success: function (res) {
                    console.log('delete done.')
                    app.globalData.deviceName = '没有设备';
                    app.globalData.userDevice = '';
                  }
                });
              },
              fail: function (err) {
                console.log(err);
              }
            });
          } else {
            wx.showToast({
              title: '验证码错误',
              icon: 'none',
              duration: 2000
            })
          }
        } else {
          wx.showToast({
            title: '手机号不正确',
            icon: 'none',
            duration: 2000
          })
        }
      },
    });
  },

  onShow: function(){
    
  }
})
