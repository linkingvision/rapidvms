#include "client.h"
#include <QHttp>
#include <QEventLoop>
#include <QUrl>
#include <QDebug>
#include <QTimer>

using namespace ONVIF;

Client::Client(const QString &url) {
    mUrl = url;
}

QString Client::sendData(const QString &data) {
//    qDebug() << "send to url => " << mUrl << " | data => " << data;
    QHttp http;
    http.ignoreSslErrors();
    QUrl url(mUrl);
    if (url.port() >= 655535 || url.port() <= 0)
    {
        http.setHost(url.host());
    }else
    {
        http.setHost(url.host(), url.port());
    }
    http.post(url.path(), data.toUtf8());
    waitForFinish(http);
    if(http.error() != QHttp::NoError || mTimerIsTrue == true) {
        return "";
    }
    return QString(http.readAll());
}

void Client::waitForFinish(const QHttp &http) {
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    mTimerIsTrue = false;
    connect(&http, SIGNAL(done(bool)), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(2000);
    loop.exec();
    if (timer.isActive())
    {
        mTimerIsTrue  = false;
    }else
    {
        mTimerIsTrue  = true;
    }
}
