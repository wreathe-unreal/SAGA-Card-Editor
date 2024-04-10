#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QList>
#include <QVariantMap>
#include "enums.h"
#include <QDir>
#include "card.h"

Card::Card()
{

}
Card::Card(const QJsonObject &cardJson)
{
    this->Lifespan = cardJson["Lifespan"].toInt();
    this->ID = cardJson["ID"].toString();
    this->Name = cardJson["Name"].toString();
    this->Filepath = cardJson["ImagePath"].toString();
    this->Price = cardJson["Price"].toInt();
    this->FlavorText = cardJson["FlavorText"].toString();
    this->System = cardJson["System"].toString();
    this->Habitat = cardJson["Habitat"].toString();
    this->Type = cardJson["Type"].toString();
    this->Property = cardJson["Property"].toString();

    QJsonArray actionsArray = cardJson["Actions"].toArray();
    for (const auto& actionValue : actionsArray)
    {
        Action action(actionValue.toObject());
        this->Actions.push_back(action);
    }
}

QVector<Card> Card::DeserializeCards()
{
    QVector<Card> cards;

    QFile file("cards.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open the file";
        return cards;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray jsonArray = doc.array();

    for (const auto& item : jsonArray)
    {
        QJsonObject obj = item.toObject();
        Card card(obj);
        cards.push_back(card);
    }
    return cards;
}


