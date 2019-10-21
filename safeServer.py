import socket
import threading
import struct
import pymysql
from enum import Enum

class Database:
    def __init__(self):
        self.connecter = pymysql.connect(host='127.0.0.1',
                                         user='root',
                                         password='123456',
                                         db='safeguard',
                                         charset='utf8',
                                         cursorclass=pymysql.cursors.DictCursor)
        #返回字典型游标
        self.cursor = self.connecter.cursor(cursor=pymysql.cursors.DictCursor)

# 构造一个全局的数据库对象
_db = Database()
# 消息类型
class Request(Enum):

    scanVirus = 1
    pushVirus = 2
    done= 3
# 消息状态
class ResultCode(Enum):

    success   = 1
    error = 0
    exist = 2

#维护一个字典，保存了所有在线的用户
_clientDict = {}

def sendToclientSocket(clientSocket,type, status, strData):
    # 编码
    data = strData.encode('gb2312')
    # 加密
    data = bytearray(data)
    for x in range(len(data)):
        data[x] ^= 5

    size = len(data)
    strFormat = 'iii%ds'%(size)
    rawData = struct.pack( strFormat ,type, status, size,data)
    try:
        clientSocket.send( rawData )
    except Exception as e:
        print(e)


def recvFromclientSocket(clientSocket):
    try:
        data = clientSocket.recv(8)

        type, size = struct.unpack('ii', data)
        print('type=%d , size%d' % (type, size))

        # 接收后续的内容
        bodyData = clientSocket.recv(size)
        # 解密
        bodyData = bytearray(bodyData)
        for x in range(len(bodyData)):
            bodyData[x] ^= 5
        print('bodyData: %s' % (bodyData.decode('gb2312')))
        # 解码之后分割参数
        return type , bodyData.decode('gb2312').split('\t')
    except Exception as e:
        print(e)

class Handler():
    def __init__(self):
        #实例数据属性
        self.args = None
        self.socket=None
    def scanVirus(self):
        print('MD5' , self.args)
        # 1. 查表,判断是否存在
        sql = "select * from virus where md5='%s';"%(self.args[0])
        _db.cursor.execute(sql)
        userInfo =_db.cursor.fetchone()
        if userInfo == None:
            sendToclientSocket(self.socket,
                               Request.scanVirus.value,
                               ResultCode.success.value,
                               "安全")
            return
        else:
            sendToclientSocket(self.socket,
                               Request.scanVirus.value,
                               ResultCode.success.value,
                               "不安全")
            return

    def pushVirus(self):
        print('上传病毒：' , self.args)

        # 1. 先看看在不在病毒库
        _db.cursor.execute("select * from virus where md5 ='%s';" % (self.args[1]))
        online = _db.cursor.fetchone()
        msg = self.args[0] + "\t" + self.args[1]
        print(msg)
        if online != None:
            sendToclientSocket(self.socket,
                               Request.pushVirus.value,
                               ResultCode.exist.value,
                               msg)
        else:
            # 2. 将病毒添加到数据库
            _db.cursor.execute(
                "insert into virus (name,md5) values ('%s','%s');" % (self.args[0], self.args[1]))
            _db.cursor.execute("commit;")
            msg = self.args[0] + "\t" + self.args[1]
            sendToclientSocket(self.socket,
                               Request.pushVirus.value,
                               ResultCode.success.value,
                               msg)
    def done(self):
            sendToclientSocket(self.socket,
                           Request.done.value,
                           ResultCode.success.value,
                           '结束')

class Server:
    def __init__(self, ip , port):
        # 1. 创建一个套接字
        self.socket = socket.socket()
        # 2. 绑定ip和端口
        self.socket.bind( (ip,port))
        # 3. 监听
        self.socket.listen( socket.SOMAXCONN )

    def recvclientSocketData(self,handler):
        type,args = None,None

        while  True:
            type , args = None,None
            try:
                # 接收数据
                type , args = recvFromclientSocket(handler.socket)
            except Exception as e:
                print(e)
                #print(handler.user_name +"断开了服务器连接")
                print("断开")
                for k,v in _clientDict:
                    print(k)
                #del _clientDict[ handler.user_name ]
                break
            try:
                # 将一个整型值转换成枚举类型
                # 如果这个整型值在枚举类型中没有定义
                # 就会抛出异常
                type = Request(type)
            except:
                sendToclientSocket(handler.socket,
                                   type,
                                   ResultCode.bad_pack.value ,
                                   "无效的请求")
            # 1. 获取函数
            func = getattr(handler , type.name)
            # 2. 设置属性, 将客户端传来的参数设置成类对象的一个成员变量
            handler.args = args
            func()

    def run(self):
        # 4. 循环接收客户端连接
        while True:
            clientSocket , addr = self.socket.accept()
            h = Handler()
            h.socket = clientSocket
            h.addr = addr
            print('接入新客户端: ' , addr)
            threading.Thread(
                target=self.recvclientSocketData,
                args=(h,)
            ).start()


if __name__ == '__main__':
    s = Server('127.0.0.1',10086)
    print("服务器开始运行")
    s.run()