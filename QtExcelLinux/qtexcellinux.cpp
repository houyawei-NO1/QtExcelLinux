#include "qtexcellinux.h"
QXlsx::Document xlsx("90个设备的MAC.xlsx");
QtExcelLinux::QtExcelLinux(QObject *parent) : QObject(parent)
{
    qDebug()<<"xlsx start"<<endl;

//    xlsx.write("A1", "Hello Qt!");
//    xlsx.saveAs("Test.xlsx");
    for(Row=1;Row<100;Row++)
    {
    A_Row = "A" + QString::number(Row);
    B_Row = "B" + QString::number(Row);
    C_Row = "C" + QString::number(Row);
    MAC = xlsx.read(A_Row).toString().remove(":").remove("\"").remove(" ");
    if(!(MAC.isNull()))
    {
     qDebug()<<MAC;
     CheckDeviceID(MAC);
     QEventLoop loop;
     QTimer::singleShot(800,&loop,SLOT(quit()));
     loop.exec();
    }
    else
    {
    qDebug()<<"xlsx end";
    return;
    }
    }

}
void QtExcelLinux::CheckDeviceID(const QString &strmac)
{

    QString url("xxxxxxx");
    const QUrl aurl(url);
    QNetworkRequest qnr(aurl);
    qnr.setRawHeader("Content-Type", "application/json;charset=utf8");

    QString md5Str = QCryptographicHash::hash(strmac.toLatin1(),QCryptographicHash::Md5).toHex();

    QString strData;
    QString strCPU = md5Str.mid(0,12);
    QString strMAC = md5Str.mid(20,12);
    qDebug()<<"strCPU"<<strCPU;
    qDebug()<<"strMAc"<<strMAC;
    strData.append(strCPU);
    strData.append(strMAC);
    QString pwd = strCPU.mid(1, 3) + strMAC.mid(1,3);
    QString md5;
    QByteArray ba, bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(pwd);
    md.addData(ba);
    bb = md.result();
    md5.append(bb.toHex());
    strData.append(md5);

    QJsonObject obj;
    obj.insert("cpuId", strCPU);
    obj.insert("mac", strMAC);
    obj.insert("key", md5);
    QJsonDocument jsonDoc(obj);

    QString str(jsonDoc.toJson());

    qnam = new QNetworkAccessManager();
       QObject::connect(qnam, SIGNAL(finished(QNetworkReply*)),
           this, SLOT(finishedSlot(QNetworkReply*)));

    QNetworkReply *reply = qnam->post(qnr, jsonDoc.toJson());//*updata*CheckDeviceID:    cpuId,mac,key
       qDebug()<<jsonDoc;
    if (reply->isFinished())
        reply->deleteLater();
}

void QtExcelLinux::finishedSlot(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
         {
             QByteArray bytes = reply->readAll();
             QJsonParseError e;
             QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &e);
             if (e.error == QJsonParseError::NoError && !jsonDoc.isNull())
             {

                 if((!jsonDoc.isNull()) || (!jsonDoc.isEmpty()))
                 {

                     if(jsonDoc.isObject())

                     {
                         QJsonObject obj = jsonDoc.object();
                         if(obj.contains(QString("picStream")))
                         {
 //                            qDebug() << "in picStream...";

                             QString m_strDeviceID = obj.value("deviceId").toString();
                             QString picStream = obj.value("picStream").toString();
//                             QByteArray tmpBytes = picStream.toLatin1();
//                             QImage img;
//                             img.loadFromData(QByteArray::fromBase64(tmpBytes));
//                             QString path = m_savePath;
//                             path.append("/profiles/qr.jpg");
//                             img.save(path);
//                             _captured = "此设备号为：" + m_strDeviceID;

                             qDebug()<<A_Row<<" "<<B_Row<<" 此设备号为："<<m_strDeviceID;
                             xlsx.write(B_Row,MAC);
                             xlsx.write(C_Row,m_strDeviceID);
                             xlsx.save();




                         }

                  }
             }

         }
         else
         {
             qDebug() << "finishedSlot errors here";
             qDebug( "found error .... code: %d\n", (int)reply->error());
             qDebug(qPrintable(reply->errorString()));
         }
         reply->deleteLater();

}}
