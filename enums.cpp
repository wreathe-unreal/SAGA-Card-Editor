#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QString>
#include <QFile>
#include <QFileInfo>


QVector<QString> Actions, Properties, Types, Systems, Habitats, Attributes;


bool ValidateProperty(QString enumToFind)
{
    for(const auto &e : Properties)
    {
        if(enumToFind == e)
        {
            return true;
        }
    }
    return false;
}

bool ValidateType(QString enumToFind)
{
    for(const auto &e : Types)
    {
        if(enumToFind == e)
        {
            return true;
        }
    }
    return false;
}

bool ValidateSystem(QString enumToFind)
{
    for(const auto &e : Systems)
    {
        if(enumToFind == e)
        {
            return true;
        }
    }
    return false;
}

bool ValidateHabitat(QString enumToFind)
{
    for(const auto &e : Habitats)
    {
        if(enumToFind == e)
        {
            return true;
        }
    }
    return false;
}

bool ValidateAttribute(QString enumToFind)
{
    for(const auto &e : Attributes)
    {
        if(enumToFind == e)
        {
            return true;
        }
    }
    return false;
}

// Assuming jsonStr is a QString containing your JSON data
void DeserializeEnums()
{
    Actions = {"Explore", "Travel", "Work", "Research", "Dream", "Talk", "" "Craft", "Trade", "Battle"};

    QFile file("enums.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << file.fileName();
        QFileInfo fileInfo(file);
        qDebug() << fileInfo.absoluteFilePath();

        qDebug() << "Could not open file or file not text";
        return;
    }


    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        qWarning("Document is not a valid JSON object.");
        return;
    }

    QJsonObject jsonObj = doc.object();

    // Deserialize EProperty
    QJsonArray ePropertyArray = jsonObj["EProperty"].toArray();
    for (const auto& item : ePropertyArray) {
        Properties.append(item.toString());
    }

    // Deserialize EType
    QJsonArray eTypeArray = jsonObj["EType"].toArray();
    for (const auto& item : eTypeArray) {
        Types.append(item.toString());
    }

    // Deserialize ESystem
    QJsonArray eSystemArray = jsonObj["ESystem"].toArray();
    for (const auto& item : eSystemArray) {
        Systems.append(item.toString());
    }

    // Deserialize EHabitat
    QJsonArray eHabitatArray = jsonObj["EHabitat"].toArray();
    for (const auto& item : eHabitatArray) {
        Habitats.append(item.toString());
    }

    // Deserialize EAttribute
    QJsonArray eAttributeArray = jsonObj["EAttribute"].toArray();
    for (const auto& item : eAttributeArray) {
        Attributes.append(item.toString());
    }
}
