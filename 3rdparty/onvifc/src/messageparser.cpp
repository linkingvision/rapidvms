#include "messageparser.h"
#include <QHashIterator>
#include <QDebug>
#include <QXmlResultItems>

using namespace ONVIF;

MessageParser::MessageParser(const QString &data, QHash<QString, QString> &namespaces, QObject *parent) : QObject(parent) {
    mBuffer.setData(data.toUtf8());
    mBuffer.open(QIODevice::ReadOnly);
    mQuery.bindVariable("inputDocument", &mBuffer);
    QHashIterator<QString, QString> i(namespaces);
    mNamespaceQueryStr = "";
    while (i.hasNext()) {
        i.next();
        mNamespaceQueryStr.append("declare namespace " + i.key() + " = \"" + i.value() + "\";\n");
    }
}

MessageParser::~MessageParser() {
    mBuffer.close();
}

QString MessageParser::getValue(const QString &xpath) {

    QString str;
    mQuery.setQuery(mNamespaceQueryStr + "doc($inputDocument)" + xpath + "/string()");
    if(!mQuery.isValid()) {
        return "";
    }
    mQuery.evaluateTo(&str);
    return str.trimmed();
}

bool MessageParser::find(const QString &xpath)
{
    mQuery.setQuery(mNamespaceQueryStr + "doc($inputDocument)" + xpath);
    if(!mQuery.isValid()){
        return false;
    }
    QXmlResultItems items;
    mQuery.evaluateTo(&items);
    QXmlItem item = items.next();
    if(item.isNull())
        return false;
    else
        return true;
}

QXmlQuery *MessageParser::query()
{
    return &mQuery;
}

QString MessageParser::nameSpace()
{
    return mNamespaceQueryStr;
}


