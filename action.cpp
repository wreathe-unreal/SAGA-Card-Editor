#include "action.h"


Action::Action(const QJsonObject &actionJson)
{
    QJsonObject actionKey = actionJson["ActionKey"].toObject();
    this->ActionName = actionKey["ActionName"].toString();
    this->ID = actionKey["ID"].toString();
    this->RequiredLocation = actionKey["ReqLocation"].toString();
    this->Attribute = actionKey["Attribute"].toString();
    this->AttributeMinimum = actionKey["AttributeMinimum"].toInt();
    this->MinRepetitions = actionKey["MinRepetitions"].toInt();
    this->MaxRepetitions = actionKey["MaxRepetitions"].toInt();

    QJsonArray specifiersArray = actionKey["SecondaryCardSpecifiers"].toArray();
    for (const auto& specifiersValue : specifiersArray)
    {
        QJsonArray specifiersSubArray = specifiersValue.toArray();
        QVector<QString> specifiers;

        for (const auto& specifierValue : specifiersSubArray)
        {
            specifiers.push_back(specifierValue.toString());
        }
        this->SecondaryCardSpecifiers.push_back(specifiers);
    }

    QJsonObject actionResult = actionJson["ActionResult"].toObject();
    this->AttributeModified = actionResult["AttributeModified"].toString();
    this->AttributeModifier = actionResult["AttributeModifier"].toDouble();
    this->Duration = actionResult["Duration"].toInt();
    this->FlavorText = actionResult["FlavorText"].toString();
    this->OutcomeText = actionResult["OutcomeText"].toString();

    QJsonArray returnedCardIDsArray = actionResult["ReturnedCardIDs"].toArray();
    for (const auto& cardIdValue : returnedCardIDsArray)
    {
        this->ReturnedCardIDs.push_back(cardIdValue.toString());
    }

    QJsonArray returnedQuantitiesArray = actionResult["ReturnedQuantities"].toArray();
    for (const auto& quantityValue : returnedQuantitiesArray) {
        this->ReturnedQuantities.push_back(quantityValue.toInt());
    }
}

QString Action::FlattenRequired()
{
    QString required;

    for(auto v : SecondaryCardSpecifiers)
    {
        required += "[";
        required += v[0] + ",";
        required += v[1] + ",";
        required += v[2];
        required += "]";
    }
    return required;
}

QString Action::FlattenReturned()
{
    QString returned;
    for(int i = 0; i < ReturnedCardIDs.size(); i++)
    {
        returned += "[";
        returned += ReturnedCardIDs[i] + ",";
        returned += QString::number(ReturnedQuantities[i]);
        returned += "]";
    }
    return returned;
};
