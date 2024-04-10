#ifndef CARD_H
#define CARD_H
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include "action.h"

class MainWindow;
class Card
{
    public:
    int Lifespan;
    QString ID;
    QString Name;
    QString Filepath;
    int Price;
    QString FlavorText;
    QString System;
    QString Habitat;
    QString Type;
    QString Property;
    QVector<Action> Actions;
    Card();
    Card(const QJsonObject &json);
    static QVector<Card> DeserializeCards();

};

#endif // CARD_H
