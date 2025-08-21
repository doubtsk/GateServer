#include "tcpmgr.h"
#include <QAbstractSocket>
TcpMgr::TcpMgr():_host(""),_port(0),_b_recv_pending(false),_message_id(0),_message_len(0)
{
    QObject::connect(&_socket, &QTcpSocket::connected, [&]()
    {
        qDebug() << "Connected to server!";
        // 连接建立后发送消息
        emit sig_con_success(true);
    });
    QObject::connect(&_socket, &QTcpSocket::readyRead, [&]()
    {
        // 当有数据可读时，读取所有数据
        // 读取所有数据并追加到缓冲区
        _buffer.append(_socket.readAll());
        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_0);
        forever
        {
            //先解析头部
            if(!_b_recv_pending)
            {
                // 检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度）
                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2))
                {
                    return; // 数据不够，等待更多数据
                }
                // 预读取消息ID和消息长度，但不从缓冲区中移除
                stream >> _message_id >> _message_len;
                //将buffer 中的前四个字节移除
                _buffer = _buffer.mid(sizeof(quint16) * 2);
                // 输出读取的数据
                qDebug() << "Message ID:" << _message_id << ", Length:" << _message_len;
            }
            //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
            if(_buffer.size() < _message_len)
            {
                _b_recv_pending = true;
                return;
            }
            _b_recv_pending = false;
            // 读取消息体
            QByteArray messageBody = _buffer.mid(0, _message_len);
            qDebug() << "receive body msg is " << messageBody ;
            _buffer = _buffer.mid(_message_len);
        }
    });
    //5.15 之后版本处理错误
    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        qDebug() << "Error:" << _socket.errorString();
    });
    // 处理连接断开
    QObject::connect(&_socket, &QTcpSocket::disconnected, [&]() {
        qDebug() << "Disconnected from server.";
    });
    //QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
}

void TcpMgr::initHandlers()
{
    _handlers.insert(ID_CHAT_LOGIN_RSP,[this](ReqId id,int len,QByteArray data){
        Q_UNUSED(len);
        qDebug()<<"handle id is"<<id<<"data is"<<data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        //检测是否成功
        if(jsonDoc.isNull())
        {
            qDebug()<<"failed to create QJsonDocument";
            return;
        }
        QJsonObject jsonobj = jsonDoc.object();
        if(!jsonobj.contains("error"))
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Login failed ,err is Json Prase Err"<<err;
            emit sig_login_failed(err);
            return;
        }
        int err = jsonDoc["error"].toInt();
        if(err!=ErrorCodes::SUCCESS)
        {
            qDebug()<<"Login failed,err is "<<err;
            emit sig_login_failed(err);
            return;
        }
        emit sig_switch_chatdlg();
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter = _handlers.find(id);
    if(find_iter==_handlers.end())
    {
        qDebug()<<"not found id["<<id<<"]to handler";
        return;
    }
    find_iter.value()(id,len,data);
}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug()<<"receive tcp connect signal";
    //尝试连接到服务器
    qDebug()<<"Connecting to Server....";
    _host = si.Host;
    _port = static_cast<uint16_t> (si.Port.toUInt());
    _socket.connectToHost(si.Host,_port);
}

void TcpMgr::slot_send_data(ReqId reqId, QByteArray data)
{
    uint16_t id = reqId;
    QString str = QString::fromUtf8(data);//要是出问题了就改改这里，改成其他格式看看
    QByteArray dataBytes = str.toUtf8();
    quint16 len = static_cast<quint16> (data.size());//计算长度
    //创建一个QByteArray用于存储要发送的数据
    QByteArray block;
    QDataStream out (&block,QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);//网络传输一般都是采用大端
    out << id << len;
    block.append(data);
    _socket.write(block);
}
