#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "enums.h"
#include "card.h"
#include <QFile>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QClipboard>
#include <QMessageBox>



Ui::MainWindow* uiPtr;
QVector<Card> Cards;
QVector<Action> UpdateActions;
Card UpdateCard;
QVector<Action> NewActions;
Action NewAction;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    uiPtr = ui;
    ui->setupUi(this);
    ui->editCardJSON->setVisible(false);
    DeserializeEnums();
    SetupTables();
    PopulateComboBoxes();
    UpdateCardTable();
    MakeTablesUneditable();
    uiPtr->backgroundMat->lower();
    uiPtr->backgroundMat2->lower();

}

void MainWindow::SetupTables()
{



    ui->actionTable->setColumnCount(11);
    QStringList actionHeaders = {"Name", "Returned", "Required", "ReqLoc", "Duration", "FlavorText", "OutcomeText", "Attribute", "AttributeReq", "AttributeModified", "AttributeModifier"};
    uiPtr->actionTable->setHorizontalHeaderLabels(actionHeaders);
    ui->actionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->actionTable->verticalHeader()->setVisible(false);
    ui->actionTable->setSelectionBehavior(QAbstractItemView::SelectRows);




    uiPtr->cardTable->setRowCount(Cards.size());
    uiPtr->cardTable->setColumnCount(2);
    QStringList headers = {"ID", "Name"};
    uiPtr->cardTable->setHorizontalHeaderLabels(headers);
    ui->cardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->cardTable->verticalHeader()->setVisible(false);
    ui->cardTable->setSelectionBehavior(QAbstractItemView::SelectRows);




    uiPtr->actionRequiredTable->setColumnCount(3);
    QStringList actionReqHeaders = {"ID", "Type", "Property"};
    uiPtr->actionRequiredTable->setHorizontalHeaderLabels(actionReqHeaders);
    ui->actionRequiredTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->actionRequiredTable->setSelectionBehavior(QAbstractItemView::SelectRows);




    uiPtr->actionReturnedTable->setColumnCount(2);
    QStringList actionRetHeaders = {"ID", "Qty"};
    uiPtr->actionReturnedTable->setHorizontalHeaderLabels(actionRetHeaders);
    ui->actionReturnedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->actionReturnedTable->verticalHeader()->setVisible(false);
    ui->actionReturnedTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    uiPtr->orphanTable->setColumnCount(1);
    QStringList orphanHeaders = {"Undefined ID"};
    uiPtr->orphanTable->setHorizontalHeaderLabels(orphanHeaders);
    uiPtr->orphanTable->verticalHeader()->setVisible(false);
    uiPtr->orphanTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->orphanTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



}

void MainWindow::PopulateComboBoxes()
{
    //set blank defaults
    uiPtr->actionReqLocationCombo->addItem("");
    uiPtr->comboSystem->addItem("");
    uiPtr->comboHabitat->addItem("");
    uiPtr->comboType->addItem("");
    uiPtr->comboProperty->addItem("");
    uiPtr->actionRequiredType->addItem("");
    uiPtr->actionRequiredProperty->addItem("");
    uiPtr->actionComboAttributeReq->addItem("");
    uiPtr->actionComboAttribute->addItem("");

    for(const QString& a : Actions)
    {

        uiPtr->actionCombo->addItem(a);
    }
    for(const QString& s : Systems)
    {
        uiPtr->comboSystem->addItem(s);
        uiPtr->actionReqLocationCombo->addItem(s);
    }
    for(const QString& h : Habitats)
    {
        uiPtr->comboHabitat->addItem(h);
        uiPtr->actionReqLocationCombo->addItem(h);
    }
    for(const QString& t : Types)
    {
        uiPtr->comboType->addItem(t);
        uiPtr->actionRequiredType->addItem(t);
    }
    for(const QString& p : Properties)
    {
        uiPtr->comboProperty->addItem(p);
        uiPtr->actionRequiredProperty->addItem(p);
    }
    for(const QString& a : Attributes)
    {
        uiPtr->actionComboAttributeReq->addItem(a);
    }
    for(const QString& a : Attributes)
    {
        uiPtr->actionComboAttribute->addItem(a);
    }

}
void MainWindow::UpdateCardTable()
{
    Cards = Card::DeserializeCards();
    QVector<QString> SearchIDs;
    ui->cardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    uiPtr->cardTable->setRowCount(Cards.size());
    for (int i = 0; i < Cards.size(); ++i)
    {
        // Create a new item for the current piece of data
        QTableWidgetItem *newCardID = new QTableWidgetItem(Cards[i].ID);
        QTableWidgetItem *newName = new QTableWidgetItem(Cards[i].Name);
        // Set the item at the correct position in the table
        uiPtr->cardTable->setItem(i, 0, newCardID);
        uiPtr->cardTable->setItem(i, 1, newName);

        for(Action& a : Cards[i].Actions)
        {
            SearchIDs.append(a.ReturnedCardIDs);
            for(int i = 0; i < a.SecondaryCardSpecifiers.size(); i++)
            {
                SearchIDs.append(a.SecondaryCardSpecifiers[i][0]);
            }
        }
    }

    //find orphan IDs
    for(Card& c : Cards)
    {
        for(int i = 0; i < SearchIDs.size(); i++)
        {
            if(SearchIDs[i] == c.ID || SearchIDs[i] == "")
            {
                SearchIDs.removeAt(i);
            }
        }
    }

    for(QString& s : SearchIDs)
    {
        int rowCount = uiPtr->orphanTable->rowCount();
        uiPtr->orphanTable->insertRow(rowCount);
        QTableWidgetItem *newID = new QTableWidgetItem(s);
        uiPtr->orphanTable->setItem(rowCount, 0, newID);
    }

    uiPtr->cardTable->resizeColumnToContents(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QJsonObject MainWindow::SerializeCard(QVector<Action> Actions)
{
    QJsonObject cardObject;

    cardObject["ID"] = uiPtr->ID->text();
    cardObject["Name"] = uiPtr->name->text();
    cardObject["ImagePath"] = uiPtr->filename->text();
    cardObject["Price"] = uiPtr->price->text().toInt();
    cardObject["FlavorText"] = uiPtr->flavorText->toPlainText();
    cardObject["System"] = uiPtr->comboSystem->currentText();
    cardObject["Habitat"] = uiPtr->comboHabitat->currentText();
    cardObject["Type"] = uiPtr->comboType->currentText();
    cardObject["Property"] = uiPtr->comboProperty->currentText();
    cardObject["Lifespan"] = uiPtr->lifespan->text().toInt();


    QJsonArray actionsArray;

    for (int i = 0; i < Actions.size(); ++i) {
        QJsonObject actionKey;
        // Populate actionKey with the action details
        actionKey["ActionName"] = Actions[i].ActionName;
        actionKey["ID"] = Actions[i].ID;
        actionKey["ReqLocation"] = Actions[i].RequiredLocation;
        actionKey["Attribute"] = Actions[i].Attribute;
        actionKey["AttributeMinimum"] = Actions[i].AttributeMinimum;

        // Serialize SecondaryCardSpecifiers
        QJsonArray specifiersArray;
        for (const auto& specifierVec : Actions[i].SecondaryCardSpecifiers) {
            QJsonArray specifierArray;
            for (const auto& specifier : specifierVec) {
                specifierArray.append(specifier);
            }
            specifiersArray.append(specifierArray);
        }
        actionKey["SecondaryCardSpecifiers"] = specifiersArray;

        // Populate actionResult with the result details
        QJsonObject actionResult;
        actionResult["AttributeModified"] = Actions[i].AttributeModified;
        actionResult["AttributeModifier"] = Actions[i].AttributeModifier;
        actionResult["Duration"] = Actions[i].Duration;
        actionResult["FlavorText"] = Actions[i].FlavorText;
        actionResult["OutcomeText"] = Actions[i].OutcomeText;

        // Serialize ReturnedCardIDs
        QJsonArray returnedIDsArray;
        for (const auto& id : Actions[i].ReturnedCardIDs) {
            returnedIDsArray.append(id);
        }
        actionResult["ReturnedCardIDs"] = returnedIDsArray;

        // Serialize ReturnedQuantities
        QJsonArray returnedQuantitiesArray;
        for (const auto& qty : Actions[i].ReturnedQuantities) {
            returnedQuantitiesArray.append(qty);
        }
        actionResult["ReturnedQuantities"] = returnedQuantitiesArray;

        // Combine actionKey and actionResult into actionObject
        QJsonObject actionObject;
        actionObject["ActionKey"] = actionKey;
        actionObject["ActionResult"] = actionResult;

        // Append the current actionObject to the actionsArray
        actionsArray.append(actionObject);
    }

    // Assign actionsArray to the cardObject and return it
    cardObject["Actions"] = actionsArray;

    return cardObject;
}


void MainWindow::on_createCardJSON_clicked()
{
    QJsonObject cardJSON = SerializeCard(NewActions);

    if (cardJSON.isEmpty()) {
        qDebug() << "Serialized card JSON is empty.";
    }

    QJsonDocument cardDoc(cardJSON);
    QString jsonString = cardDoc.toJson(QJsonDocument::Compact);

    if (jsonString.isEmpty()) {
        qDebug() << "JSON string is empty.";
    } else {
        qDebug() << "JSON string:" << jsonString;
    }

    // Access the clipboard
    QClipboard* clipboard = QApplication::clipboard();
    if (!clipboard) {
        qDebug() << "Failed to access clipboard.";
        return;
    }

    // Set text to the clipboard
    clipboard->setText(jsonString);
    qDebug() << "JSON copied to clipboard.";

    QString filePath = "cards.json";
    QFile file(filePath);

    // Open the file for reading
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << filePath;
        return;
    }

    // Read the existing JSON document
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    // Assuming the root of the document is a JSON array
    QJsonArray array = doc.isArray() ? doc.array() : QJsonArray();

    // Append the new object to the array
    array.append(cardJSON);

    // Open the file again, this time for writing
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << filePath;
        return;
    }

    // Write the updated JSON document back to the file
    doc.setArray(array);
    file.write(doc.toJson());
    file.close();

    Cards.emplace_back(Card(cardJSON));
    UpdateCardTable();
    ResetCardFields();


}


void MainWindow::CheckValidCardID(QLineEdit* qle)
{
    qle->setStyleSheet("");

    if(qle->text() != "")
    {
        qle->setStyleSheet("color:rgb(0,0,0); background-color: rgb(255, 202, 203);");
    }
    for(const Card& c : Cards)
    {
        if(qle->text() == c.ID)
        {
            qle->setStyleSheet("color:rgb(0,0,0); background-color: rgb(142, 255, 140);");
        }
    }
}


void MainWindow::on_addAction_clicked()
{
    NewAction.ID = uiPtr->ID->text();
    NewAction.ActionName = uiPtr->actionCombo->currentText();
    NewAction.FlavorText = uiPtr->actionFlavorText->toPlainText();
    NewAction.OutcomeText = uiPtr->actionOutcomeText->toPlainText();
    NewAction.Duration = uiPtr->actionDuration->text().toInt();
    NewAction.RequiredLocation = uiPtr->actionReqLocationCombo->currentText();
    NewAction.Attribute = uiPtr->actionComboAttributeReq->currentText();
    NewAction.AttributeMinimum = uiPtr->actionAttributeReq->text().toInt();
    NewAction.AttributeModified = uiPtr->actionComboAttribute->currentText();
    NewAction.AttributeModifier = uiPtr->actionAttribute->text().toDouble();
    //other fields assigned when adding to the sub tables
    //NewAction initialized

    int rowCount = uiPtr->actionTable->rowCount();
    uiPtr->actionTable->insertRow(rowCount);

    QTableWidgetItem *newName = new QTableWidgetItem(NewAction.ActionName);
    QTableWidgetItem *newFlavorText = new QTableWidgetItem(NewAction.FlavorText);
    QTableWidgetItem *newOutcomeText = new QTableWidgetItem(NewAction.OutcomeText);
    QTableWidgetItem *newDuration = new QTableWidgetItem(QString::number(NewAction.Duration));
    QTableWidgetItem *newAttributeReq = new QTableWidgetItem(NewAction.Attribute);
    QTableWidgetItem *newAttributeMin = new QTableWidgetItem(QString::number(NewAction.AttributeMinimum));
    QTableWidgetItem *newAttributeMod = new QTableWidgetItem(NewAction.AttributeModified);
    QTableWidgetItem *newAttributeModAmount = new QTableWidgetItem(QString::number(NewAction.AttributeModifier));
    QTableWidgetItem *newReturned = new QTableWidgetItem(NewAction.FlattenReturned());
    QTableWidgetItem *newRequired = new QTableWidgetItem(NewAction.FlattenRequired());
    QTableWidgetItem *newReqLocation = new QTableWidgetItem(NewAction.RequiredLocation);

    // Set the item at the correct position in the table
    uiPtr->actionTable->setItem(rowCount, 0, newName);
    uiPtr->actionTable->setItem(rowCount, 1, newReturned);
    uiPtr->actionTable->setItem(rowCount, 2, newRequired);
    uiPtr->actionTable->setItem(rowCount, 3, newReqLocation);
    uiPtr->actionTable->setItem(rowCount, 4, newDuration);
    uiPtr->actionTable->setItem(rowCount, 5, newFlavorText);
    uiPtr->actionTable->setItem(rowCount, 6, newOutcomeText);
    uiPtr->actionTable->setItem(rowCount, 7, newAttributeReq);
    uiPtr->actionTable->setItem(rowCount, 8, newAttributeMin);
    uiPtr->actionTable->setItem(rowCount, 9, newAttributeMod);
    uiPtr->actionTable->setItem(rowCount, 10, newAttributeModAmount);

    uiPtr->cardTable->resizeColumnToContents(0);
    MakeTablesUneditable();
    if(uiPtr->createCardJSON->isVisible() && uiPtr->createCardJSON->isEnabled())
    {
        NewActions.emplace_back(NewAction);
    }
    else
    {
        UpdateActions.emplace_back(NewAction);
    }

    NewAction = Action();

    ResetActionFields();

}
void MainWindow::ResetCardFields()
{
    uiPtr->ID->setText("");
    uiPtr->flavorText->setPlainText("");
    uiPtr->comboHabitat->setCurrentIndex(0);
    uiPtr->comboSystem->setCurrentIndex(0);
    uiPtr->comboType->setCurrentIndex(0);
    uiPtr->comboProperty->setCurrentIndex(0);
    uiPtr->price->setValue(0);
    uiPtr->name->setText("");
    uiPtr->lifespan->setValue(0);

    //if we are resetting the card we probably want to reset the actions table
    uiPtr->actionTable->setRowCount(0);
}
void MainWindow::ResetActionFields()
{
    //clean out other tables
    uiPtr->actionRequiredTable->setRowCount(0);
    uiPtr->actionReturnedTable->setRowCount(0);

    uiPtr->actionFlavorText->setPlainText("Flavor text...");
    uiPtr->actionOutcomeText->setPlainText("Outcome text...");
    uiPtr->actionComboAttribute->setCurrentIndex(0);
    uiPtr->actionAttribute->setValue(0.0);
    uiPtr->actionComboAttributeReq->setCurrentIndex(0);
    uiPtr->actionAttributeReq->setValue(0);
    uiPtr->actionDuration->setValue(5);
    uiPtr->actionReqLocationCombo->setCurrentIndex(0);

    uiPtr->actionReqLocCheckbox->setChecked(false);
    uiPtr->actionAttributeMinimumCheckbox->setChecked(false);
    uiPtr->actionAttributeModifierCheckbox->setChecked(false);
    uiPtr->actionRequiredCheckbox->setChecked(false);
}

void MainWindow::on_actionRequiredID_textChanged(const QString &arg1)
{
    CheckValidCardID(ui->actionRequiredID);

}


void MainWindow::on_actionReturned_textChanged(const QString &arg1)
{
    CheckValidCardID(ui->actionReturned);

}


void MainWindow::on_actionAddRequiredButton_clicked()
{
    QVector<QString> newRequired;
    newRequired.emplace_back(ui->actionRequiredID->text());
    newRequired.emplace_back(ui->actionRequiredType->currentText());
    newRequired.emplace_back(ui->actionRequiredProperty->currentText());

    NewAction.SecondaryCardSpecifiers.emplace_back(newRequired);

    QTableWidgetItem *newReqID = new QTableWidgetItem(newRequired[0]);
    QTableWidgetItem *newReqType = new QTableWidgetItem(newRequired[1]);
    QTableWidgetItem *newReqProperty = new QTableWidgetItem(newRequired[2]);

    int rowCount = uiPtr->actionRequiredTable->rowCount();
    uiPtr->actionRequiredTable->insertRow(rowCount); // Insert a new row
    uiPtr->actionRequiredTable->setItem(rowCount, 0, newReqID);
    uiPtr->actionRequiredTable->setItem(rowCount, 1, newReqType);
    uiPtr->actionRequiredTable->setItem(rowCount, 2, newReqProperty);
    MakeTablesUneditable();
    uiPtr->actionRequiredID->setText("");
    uiPtr->actionRequiredType->setCurrentIndex(0);
    uiPtr->actionRequiredProperty->setCurrentIndex(0);
}

void MainWindow::on_actionAddReturnedButton_clicked()
{
    QString newReturnedID = ui->actionReturned->text();
    int newReturnedQty = ui->actionReturnedQty->text().toInt();
    NewAction.ReturnedCardIDs.emplace_back(newReturnedID);
    NewAction.ReturnedQuantities.emplace_back(newReturnedQty);

    QTableWidgetItem *newRetID = new QTableWidgetItem(newReturnedID);
    QTableWidgetItem *newRetQty = new QTableWidgetItem(QString::number(newReturnedQty));

    int rowCount = uiPtr->actionReturnedTable->rowCount();
    uiPtr->actionReturnedTable->insertRow(rowCount); // Insert a new row
    uiPtr->actionReturnedTable->setItem(rowCount, 0, newRetID);
    uiPtr->actionReturnedTable->setItem(rowCount, 1, newRetQty);
    MakeTablesUneditable();
    uiPtr->actionReturned->setText("");
    uiPtr->actionReturnedQty->setValue(1);
}


void MainWindow::on_name_textChanged(const QString &arg1)
{
    QString name = uiPtr->name->text();
    uiPtr->ID->setText(CleanString(name));
    uiPtr->filename->setText(CleanString(name) + ".png");



    if(ui->name->text() == "")
    {
         ui->name->setStyleSheet("");
    }
    else
    {
         ui->name->setStyleSheet("color:rgb(0,0,0); background-color: rgb(142, 255, 140);");
        for(const Card& c : Cards)
        {
            if(ui->name->text().toLower() == c.Name.toLower())
            {
                ui->name->setStyleSheet("color:rgb(0,0,0); background-color: rgb(255, 142, 140);");
                break;
            }
        }
    }
}

QString MainWindow::CleanString(const QString &input)
{
    QString result = input;

    // Convert the string to lowercase
    result = result.toLower();

    // Replace spaces with underscores
    result.replace(" ", "_");

    // Replace all non-word characters (anything except letters, numbers, and underscores) with an empty string
    result.replace(QRegularExpression("\\W"), "");

    return result;
}

void MainWindow::on_resetFieldsButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Confirmation", "Are you sure?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ResetActionFields();
        ResetCardFields();
    } else {
        // User clicked 'No' or closed the dialog
        qDebug() << "No was clicked";
    }
}


void MainWindow::on_cardSearch_textChanged(const QString &arg1)
{
    if(uiPtr->cardSearch->text() == "")
    {
        UpdateCardTable();
        return;
    }

    // Clear the table or reset it to its original state if needed
    uiPtr->cardTable->clearContents();
    uiPtr->cardTable->setRowCount(0);

    for (int i = 0; i < Cards.size(); ++i)
    {
        // Check if the card name contains the text entered in the search box (case-insensitive)
        if (Cards[i].Name.contains(arg1, Qt::CaseInsensitive))
        {
            // Add a new row for each matching card
            int currentRow = uiPtr->cardTable->rowCount();
            uiPtr->cardTable->insertRow(currentRow);

            QTableWidgetItem *newCardID = new QTableWidgetItem(Cards[i].ID);
            QTableWidgetItem *newName = new QTableWidgetItem(Cards[i].Name);

            uiPtr->cardTable->setItem(currentRow, 0, newCardID);
            uiPtr->cardTable->setItem(currentRow, 1, newName);

        }
    }
    MakeTablesUneditable();
    ui->cardTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}


void MainWindow::on_cardTable_itemSelectionChanged()
{
    QString selectedID;

    // Check if there is at least one selected item
    if (!ui->cardTable->selectedItems().isEmpty()){
        // Get the first selected item
        QTableWidgetItem *firstSelectedItem = ui->cardTable->selectedItems().first();

        if (firstSelectedItem != nullptr){
            // Get the row number of the first selected item
            int row = firstSelectedItem->row();

            // Retrieve the item from the first column of the selected row and get its text
            QTableWidgetItem *item = ui->cardTable->item(row, 0);
            if (item) {
                selectedID = item->text();
                uiPtr->createCardJSON->setEnabled(false);
                uiPtr->createCardJSON->setVisible(false);
                uiPtr->editCardJSON->setEnabled(true);
                uiPtr->editCardJSON->setVisible(true);

                for(const Card& c : Cards)
                {
                    if(c.ID == selectedID){

                        PopulateWithCardToEdit(c);
                    }
                }
            }
        }
    }
    else {
            ResetCardFields();
            uiPtr->createCardJSON->setEnabled(true);
            uiPtr->createCardJSON->setVisible(true);
            uiPtr->editCardJSON->setEnabled(false);
            uiPtr->editCardJSON->setVisible(false);

    }
}

void MainWindow::PopulateWithCardToEdit(Card editCard)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Edit Confirmation", "Are you sure you want to populate the app with the selected card's data?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        ResetActionFields();
        ResetCardFields();

        UpdateCard = editCard;
        UpdateActions = editCard.Actions;

        uiPtr->ID->setText(editCard.ID);
        uiPtr->name->setText(editCard.Name);
        uiPtr->filename->setText(editCard.Filepath);
        uiPtr->price->setValue(editCard.Price);
        uiPtr->flavorText->setPlainText(editCard.FlavorText);
        uiPtr->lifespan->setValue(editCard.Lifespan);
        SetComboBoxToString(uiPtr->comboSystem, editCard.System);
        SetComboBoxToString(uiPtr->comboHabitat, editCard.Habitat);
        SetComboBoxToString(uiPtr->comboProperty, editCard.Property);
        SetComboBoxToString(uiPtr->comboType, editCard.Type);

        for(Action& a : editCard.Actions)
        {
            int rowCount = uiPtr->actionTable->rowCount();
            uiPtr->actionTable->insertRow(rowCount);

            QTableWidgetItem *newName = new QTableWidgetItem(a.ActionName);
            QTableWidgetItem *newFlavorText = new QTableWidgetItem(a.FlavorText);
            QTableWidgetItem *newOutcomeText = new QTableWidgetItem(a.OutcomeText);
            QTableWidgetItem *newDuration = new QTableWidgetItem(QString::number(a.Duration));
            QTableWidgetItem *newAttributeReq = new QTableWidgetItem(a.Attribute);
            QTableWidgetItem *newAttributeMin = new QTableWidgetItem(QString::number(a.AttributeMinimum));
            QTableWidgetItem *newAttributeMod = new QTableWidgetItem(a.AttributeModified);
            QTableWidgetItem *newAttributeModAmount = new QTableWidgetItem(QString::number(a.AttributeModifier));
            QTableWidgetItem *newReturned = new QTableWidgetItem(a.FlattenReturned());
            QTableWidgetItem *newRequired = new QTableWidgetItem(a.FlattenRequired());
            QTableWidgetItem *newReqLocation = new QTableWidgetItem(a.RequiredLocation);

            // Set the item at the correct position in the table
            uiPtr->actionTable->setItem(rowCount, 0, newName);
            uiPtr->actionTable->setItem(rowCount, 1, newReturned);
            uiPtr->actionTable->setItem(rowCount, 2, newRequired);
            uiPtr->actionTable->setItem(rowCount, 3, newReqLocation);
            uiPtr->actionTable->setItem(rowCount, 4, newDuration);
            uiPtr->actionTable->setItem(rowCount, 5, newFlavorText);
            uiPtr->actionTable->setItem(rowCount, 6, newOutcomeText);
            uiPtr->actionTable->setItem(rowCount, 7, newAttributeReq);
            uiPtr->actionTable->setItem(rowCount, 8, newAttributeMin);
            uiPtr->actionTable->setItem(rowCount, 9, newAttributeMod);
            uiPtr->actionTable->setItem(rowCount, 10, newAttributeModAmount);
            MakeTablesUneditable();
        }
    }
    else
    {
            // User clicked 'No' or closed the dialog
            qDebug() << "No was clicked";
    }
}

void MainWindow::SetComboBoxToString(QComboBox *comboBox, const QString &value)
{
    int index = comboBox->findText(value, Qt::MatchExactly);
    if (index != -1) { // -1 means no match was found
            comboBox->setCurrentIndex(index);
    } else {
            // Handle the case where the value is not found. You might want to log an error, set a default value, etc.
            qDebug() << "Value not found in combo box: " << value;
    }
}

void MainWindow::on_editCardJSON_clicked()
{

    QJsonObject updatedCardJSON = SerializeCard(UpdateActions);

    if (updatedCardJSON.isEmpty()) {
            qDebug() << "Serialized card JSON is empty.";
            return;
    }

    QString id = updatedCardJSON["ID"].toString();
    if (id.isEmpty()) {
            qDebug() << "Card JSON does not have a valid ID.";
            return;
    }

    QString filePath = "cards.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file for reading:" << filePath;
            return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray array = doc.isArray() ? doc.array() : QJsonArray();
    bool found = false;

    for (int i = 0; i < array.size(); ++i) {
            QJsonObject obj = array[i].toObject();
            if (obj["ID"].toString() == id) {
            // Replace the existing object with the updated one
            array.replace(i, updatedCardJSON);
            found = true;
            break;
            }
    }

    if (!found) {
            qDebug() << "Card with ID" << id << "not found.";
            return;
    }

    if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to open file for writing:" << filePath;
            return;
    }

    doc.setArray(array);
    file.write(doc.toJson());
    file.close();

    // Find and update the card in the local Cards vector
    auto it = std::find_if(Cards.begin(), Cards.end(), [&id](const Card& card) {
        return card.ID == id;
    });

    if (it != Cards.end()) {
            *it = Card(updatedCardJSON);  // Assuming Card has a constructor from QJsonObject
    }

    UpdateCardTable();
    ResetCardFields();
}


void MainWindow::on_actionRemoveReturnedButton_clicked()
{
    // Get the current selection model from the table
    QItemSelectionModel *selectionModel = ui->actionReturnedTable->selectionModel();

    // Check if there is any selected row
    if (selectionModel->hasSelection()) {
            // Get the indexes of the selected rows
            QModelIndexList selectedRows = selectionModel->selectedRows();

            // In case of multiple selection, this will remove all selected rows
            for (int i = selectedRows.count() - 1; i >= 0; i--) {
            ui->actionReturnedTable->removeRow(selectedRows.at(i).row());
            NewAction.ReturnedCardIDs.removeAt(i);
            NewAction.ReturnedQuantities.removeAt(i);
            }
    } else {
            qDebug() << "No row is selected to remove.";
    }
}


void MainWindow::on_actionRemoveRequiredButton_clicked()
{
    // Get the current selection model from the table
    QItemSelectionModel *selectionModel = ui->actionRequiredTable->selectionModel();

    // Check if there is any selected row
    if (selectionModel->hasSelection()) {
            // Get the indexes of the selected rows
            QModelIndexList selectedRows = selectionModel->selectedRows();

            // In case of multiple selection, this will remove all selected rows
            for (int i = selectedRows.count() - 1; i >= 0; i--)
            {
                ui->actionRequiredTable->removeRow(selectedRows.at(i).row());
                NewAction.SecondaryCardSpecifiers.removeAt(i);
            }
    } else {
            qDebug() << "No row is selected to remove.";
    }
}


void MainWindow::on_removeAction_clicked()
{
    // Get the current selection model from the table
    QItemSelectionModel *selectionModel = ui->actionTable->selectionModel();

    // Check if there is any selected row
    if (selectionModel->hasSelection())
    {
            // Get the index of the first selected row
            int rowIndex = selectionModel->selectedRows().first().row();

            // Remove the row from the table
            ui->actionTable->removeRow(rowIndex);

            // Check the visibility and enabled state of uiPtr->createCardJSON
            if (uiPtr->createCardJSON->isVisible() && uiPtr->createCardJSON->isEnabled())
            {
                // Remove the corresponding action from NewActions
                if (rowIndex < NewActions.size())
                {
                    NewActions.removeAt(rowIndex);
                }
            }
            else
            {
                // Remove the corresponding action from UpdateActions
                if (rowIndex < UpdateActions.size())
                {
                    UpdateActions.removeAt(rowIndex);
                }
            }
    }
    else
    {
            qDebug() << "No row is selected to remove.";
    }
}


void MainWindow::on_actionTable_itemSelectionChanged()
{
    // Get the current selection model from the table
    QItemSelectionModel *selectionModel = ui->actionTable->selectionModel();

    // Check if there is any selected row
    if (selectionModel->hasSelection())
    {
        int rowIndex = selectionModel->selectedRows().first().row();
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Edit Action Confirmation", "WARNING: This will remove this Action, reset the Actions fields and populate the fields for editing.  Continue?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            Action EditAction;
            // Check the visibility and enabled state of uiPtr->createCardJSON
            if (uiPtr->createCardJSON->isVisible() && uiPtr->createCardJSON->isEnabled())
            {
                // Remove the corresponding action from NewActions
                if (rowIndex < NewActions.size())
                {
                    EditAction = NewActions[rowIndex];
                    NewActions.removeAt(rowIndex);
                }
            }
            else
            {
                // Remove the corresponding action from UpdateActions
                if (rowIndex < UpdateActions.size())
                {
                    EditAction = UpdateActions[rowIndex];
                    UpdateActions.removeAt(rowIndex);
                }
            }
            ui->actionTable->removeRow(rowIndex);

            ResetActionFields();


            SetComboBoxToString(uiPtr->actionCombo, EditAction.ActionName);
            SetComboBoxToString(uiPtr->actionReqLocationCombo, EditAction.RequiredLocation);
            SetComboBoxToString(uiPtr->actionComboAttributeReq, EditAction.Attribute);
            SetComboBoxToString(uiPtr->actionComboAttribute, EditAction.AttributeModified);
            uiPtr->actionAttributeReq->setValue(EditAction.AttributeMinimum);
            uiPtr->actionAttribute->setValue(EditAction.AttributeModifier);
            uiPtr->actionDuration->setValue(EditAction.Duration);
            uiPtr->actionFlavorText->setPlainText(EditAction.FlavorText);
            uiPtr->actionOutcomeText->setPlainText(EditAction.OutcomeText);


            for(int i = 0; i < EditAction.ReturnedCardIDs.size(); i++)
            {
                QTableWidgetItem *newRetID = new QTableWidgetItem(EditAction.ReturnedCardIDs[i]);
                QTableWidgetItem *newRetQty = new QTableWidgetItem(QString::number(EditAction.ReturnedQuantities[i]));

                int rowCount = uiPtr->actionReturnedTable->rowCount();
                uiPtr->actionReturnedTable->insertRow(rowCount); // Insert a new row
                uiPtr->actionReturnedTable->setItem(rowCount, 0, newRetID);
                uiPtr->actionReturnedTable->setItem(rowCount, 1, newRetQty);


                QString newReturnedID = EditAction.ReturnedCardIDs[i];
                int newReturnedQty = EditAction.ReturnedQuantities[i];
                NewAction.ReturnedCardIDs.emplace_back(newReturnedID);
                NewAction.ReturnedQuantities.emplace_back(newReturnedQty);
            }
            for(QVector<QString> qv : EditAction.SecondaryCardSpecifiers)
            {
                QTableWidgetItem *newReqID = new QTableWidgetItem(qv[0]);
                QTableWidgetItem *newReqType = new QTableWidgetItem(qv[1]);
                QTableWidgetItem *newReqProperty = new QTableWidgetItem(qv[2]);

                int rowCount = uiPtr->actionRequiredTable->rowCount();
                uiPtr->actionRequiredTable->insertRow(rowCount); // Insert a new row
                uiPtr->actionRequiredTable->setItem(rowCount, 0, newReqID);
                uiPtr->actionRequiredTable->setItem(rowCount, 1, newReqType);
                uiPtr->actionRequiredTable->setItem(rowCount, 2, newReqProperty);


                QVector<QString> newRequired;
                newRequired.emplace_back(qv[0]);
                newRequired.emplace_back(qv[1]);
                newRequired.emplace_back(qv[2]);
                NewAction.SecondaryCardSpecifiers.emplace_back(newRequired);

            }
            MakeTablesUneditable();

        }
        else
        {
                // User clicked 'No' or closed the dialog
                qDebug() << "No was clicked";
        }
    }
    else
    {
        qDebug() << "No row is selected to remove.";
    }
}

void MainWindow::MakeTablesUneditable()
{
    QVector<QTableWidget*> Tables;
    Tables.emplace_back(uiPtr->actionRequiredTable);
    Tables.emplace_back(uiPtr->actionReturnedTable);
    Tables.emplace_back(uiPtr->actionTable);
    Tables.emplace_back(uiPtr->cardTable);
    Tables.emplace_back(uiPtr->orphanTable);

    for(QTableWidget* table : Tables)
    {
        int rowCount = table->rowCount();
        int columnCount = table->columnCount();

        for (int row = 0; row < rowCount; ++row) {
            for (int column = 0; column < columnCount; ++column) {
                    QTableWidgetItem *item = table->item(row, column);
                    // Check if the item exists before setting its flags
                    if (!item) {
                        item = new QTableWidgetItem();
                        table->setItem(row, column, item);
                    }
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }
    }
}


void MainWindow::on_orphanTable_itemSelectionChanged()
{
    // Get the current selection model from the table
    QItemSelectionModel *selectionModel = ui->orphanTable->selectionModel();

    // Check if there is any selected row
    if (selectionModel->hasSelection())
    {
        int rowIndex = selectionModel->selectedRows().first().row();
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(nullptr, "Create From ID Confirmation", "WARNING: This will clear tables and begin creating a new card from the selected ID.  Continue?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {

            ResetCardFields();
            ResetActionFields();
            ui->name->setText(IdToString(ui->orphanTable->item(rowIndex, 0)->text()));

            MakeTablesUneditable();

        }
        else
        {
            // User clicked 'No' or closed the dialog
            qDebug() << "No was clicked";
        }
    }
    else
    {
        qDebug() << "No row is selected to remove.";
    }
}

QString MainWindow::IdToString(const QString &input)
{
    QString result = input;

    // Replace all underscores with spaces
    result.replace('_', ' ');

    // Capitalize the first letter of each word
    bool capitalizeNext = true;
    for (int i = 0; i < result.length(); ++i) {
        if (result[i] == ' ') {
            capitalizeNext = true;
        } else if (capitalizeNext) {
            result[i] = result[i].toUpper();
            capitalizeNext = false;
        }
    }

    return result;
}

