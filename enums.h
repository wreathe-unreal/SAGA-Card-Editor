#ifndef ENUMS_H
#define ENUMS_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QFile>
#include <QString>


extern QVector<QString> Actions, Properties, Types, Systems, Habitats, Attributes;

bool ValidateProperty(QString enumToFind);
bool ValidateType(QString enumToFind);
bool ValidateSystem(QString enumToFind);
bool ValidateHabitat(QString enumToFind);
bool ValidateAttribute(QString enumToFind);
void DeserializeEnums();

#endif // ENUMS_H
