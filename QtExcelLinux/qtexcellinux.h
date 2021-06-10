#ifndef QTEXCELLINUX_H
#define QTEXCELLINUX_H

#include <QObject>
#include <QDebug>
#include <QtXlsx>
#include <QNetworkRequest>
#include <QNetworkReply>
class QtExcelLinux : public QObject
{
    Q_OBJECT
public:
    explicit QtExcelLinux(QObject *parent = nullptr);
    int Row,Col;
    void CheckDeviceID(const QString &strmac);
    QString A_Row,B_Row,C_Row,MAC;


signals:

private slots:
    void finishedSlot(QNetworkReply* reply);
private:
    QNetworkAccessManager* qnam;
};

#endif // QTEXCELLINUX_H
