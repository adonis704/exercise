const app = getApp()
Page({
  data: {
    deviceID: '',
    oldPassword: '',
    newPassword1: '',
    newPassword2:'',
  },

  inputDevice: function (e) {
    this.setData({ deviceID: e.detail.value });
  },
  inputOldPassword: function (e) {
    this.setData({ oldPassword: e.detail.value });
  },
  inputNewPassword1: function (e) {
    this.setData({ newPassword1: e.detail.value });
  },
  inputNewPassword2: function (e) {
    this.setData({ newPassword2: e.detail.value });
  },

  modify: function(){
    var that = this
    if (that.data.deviceID == '' || that.data.oldPassword == '' || that.data.newPassword1 == '' || that.data.newPassword2 == '') {
      wx.showToast({
        title: '请完整输入信息',
        icon: 'none',
        duration: 2000
      });
      return
    }
    console.log('begin');
    if (that.data.newPassword1 != that.data.newPassword2){
      wx.showToast({
        title: '两次密码不一致',
        icon: 'none',
        duration: 2000
      });
      return
    }

    const db = wx.cloud.database();

    db.collection('device').where({
      deviceID: that.data.deviceID
    }).get({
      success: function(res){
        console.log('device finded.');
        if (that.data.oldPassword == res.data[0].password){
          console.log('password matched.');
          wx.cloud.callFunction({
            name: 'update',
            data: {
              _id: res.data[0]._id,
              password: that.data.newPassword1
            },
            success: function (res) {
              // console.log('yes');
              wx.showToast({
                title: '修改成功',
                duration: 2000    
              })
            },
            fail: function (err) {
              console.log(err);
            }
          });

          wx.cloud.callFunction({
            name: 'delete',
            data: {
              deviceID: that.data.deviceID
            },
            success: function(res){
              console.log('delete done.')
              app.globalData.deviceName = '没有设备';
              app.globalData.userDevice = '';
            }
          })
        } else {
          wx.showToast({
            title: '密码错误',
            icon: 'none',
            duration: 2000
          })
        }
      }
    });


  },

  onShow: function(){
    
  }
})
