//index.js
const app = getApp();
var openBol = false;

Page({
  data: {
    avatarUrl: './user-unlogin.png',
    userInfo: {},
    logged: false,
    takeSession: false,
    requestResult: '',
    deviceName: ''
  },

  onLoad: function(){
    const wsAPI = 'ws://39.96.172.62:3001';
    try{
      wx.connectSocket({
        url: wsAPI,
        header: {
          'content-type': 'application/json'
        },
        protocols: ['protocol1'],
        method: "GET",
        success: function () {
          console.log('已与服务器建立连接');
          wx.showToast({
            title: '已连接到服务器',
            duration: 2000
          })
        },
        fail: function (err) {
          console.log(err);
        }
      });
      wx.onSocketOpen(function () {
        console.log('连接已打开');

        openBol = true;
      });

      wx.onSocketMessage(function (msg) {
        wx.showToast({
          title: '已开锁',
          duration: 2000
        })
      });
    } catch (err){
      console.log(err);
    }
    
  },

  onShow: function() {
    this.setData({deviceName: app.globalData.deviceName})

    wx.getSetting({
      success: res => {
        if (res.authSetting['scope.userInfo']) {
          wx.getUserInfo({
            success: res => {
              this.setData({
                avatarUrl: res.userInfo.avatarUrl,
                userInfo: res.userInfo
              })
            }
          })
        }
      }
    });
    const db = wx.cloud.database();
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
    

  },

  modifyPassword: function(e){
    wx.navigateTo({
      url: '../modifyPassword/modifyPassword',
    })
  },

  openLock: function(e){
    if (app.globalData.deviceName == '没有设备'){
      wx.showToast({
        title: '没有绑定设备',
        icon: 'none',
        duration:2000
      });
      return
    }

    if (openBol){
      wx.sendSocketMessage({
        data: '111111',
      });
    };
  }

})
