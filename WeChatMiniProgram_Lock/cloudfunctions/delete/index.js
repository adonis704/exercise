// 云函数入口文件
const cloud = require('wx-server-sdk')
const db = cloud.database();
const _ = db.command;

cloud.init()

// 云函数入口函数
exports.main = async (event, context) => {


  return await db.collection('user').where({
    deviceID: event.deviceID
  }).remove()
}