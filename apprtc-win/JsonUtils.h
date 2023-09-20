#pragma once
#include <QJsonObject>
#include <QJsonArray>
QJsonObject getJsonObjectFromString(const QString& jsonString);
QString getStringFromJsonObject(const QJsonObject& jsonObject);