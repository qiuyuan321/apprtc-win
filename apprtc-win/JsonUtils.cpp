#include "JsonUtils.h"
#include <QJsonDocument>
#include <QDebug>

// QString >> QJson  
QJsonObject getJsonObjectFromString(const QString& jsonString)
{
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
    if (jsonDocument.isNull()) 
    {
        qDebug() << "Convert jsonobject error: " << jsonString;
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

// QJson >> QString  
QString getStringFromJsonObject(const QJsonObject& jsonObject)
{
    return QString(QJsonDocument(jsonObject).toJson(QJsonDocument::Compact));
}