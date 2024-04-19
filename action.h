#ifndef ACTION_H
#define ACTION_H

#include <QJsonObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>




class Action {
public:

    //key
    QString ActionName;
    QString ID;
    QString RequiredLocation;
    QString Attribute;
    int AttributeMinimum;
    QVector<QVector<QString>> SecondaryCardSpecifiers;
    int MinRepetitions;
    int MaxRepetitions;


    //result
    QString Title;
    QString AttributeModified;
    double AttributeModifier;
    int Duration;
    QString FlavorText;
    QString OutcomeText;
    QVector<QString> ReturnedCardIDs;
    QVector<int> ReturnedQuantities;

    Action(){}

    Action(const QJsonObject &json);
    QString FlattenRequired();
    QString FlattenReturned();
};

#endif // ACTION_H
