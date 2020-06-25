// 云函数入口文件
const cloud = require('wx-server-sdk')

cloud.init()

// 云函数入口函数
exports.main = async (event, context) => {

  const db = cloud.database();
  return await db.collection("device").doc(event._id).update({
    data: {
      password: event.password
    },
    success: function(res){
      console.log(res.data);
      
    },
    fail: function(err){
      console.log(err)
    }

  })
}
