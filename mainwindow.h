#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "card.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    public:
    QString SelectedItem;
    bool bUpdateMode;
    Ui::MainWindow* uiPtr;
    QVector<Card> Cards;
    QVector<Action> UpdateActions; //same as new actions but for updating a card
    Card UpdateCard;
    QVector<Action> NewActions;
    Action NewAction;

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addAction_clicked();

    void on_actionRequiredID_textChanged(const QString &arg1);

    void on_actionReturned_textChanged(const QString &arg1);

    void on_actionAddRequiredButton_clicked();

    void on_actionAddReturnedButton_clicked();

    void on_name_textChanged(const QString &arg1);

    void on_createCardJSON_clicked();

    void on_resetFieldsButton_clicked();

    void on_cardSearch_textChanged(const QString &arg1);

    void on_cardTable_itemSelectionChanged();

    void on_editCardJSON_clicked();

    void on_actionRemoveReturnedButton_clicked();

    void on_actionRemoveRequiredButton_clicked();

    void on_removeAction_clicked();

    void on_actionTable_itemSelectionChanged();

    void on_orphanTable_itemSelectionChanged();

    void on_deleteCardButton_clicked();

    void on_cardTable_clicked(const QModelIndex &index);

    void on_actionReturnSelf_stateChanged(int arg1);

    void on_comboSystem_currentIndexChanged(int index);

private:
    QString CleanString(const QString &input);
    void PopulateComboBoxes();
    void UpdateCardTable();
    QJsonArray StringToJsonArray(const QString &input);
    Ui::MainWindow *ui;
    QString JsonArrayToString(const QJsonArray &jsonArray);
    void SetupTables();
    void ResetActionFields();
    void ResetCardFields();
    QJsonObject SerializeCard(QVector<Action> Actions);
    void PopulateWithCardToEdit(Card editCard);
    void SetComboBoxToString(QComboBox *comboBox, const QString &value);
    void MakeTablesUneditable();
    QString IdToString(const QString &input);
    void SetUpdateMode(bool status);
    bool GetUpdateMode();
    bool CheckValidCardID(QLineEdit *qle);
    bool CardIDExists(QString ID);
    template<typename T>
    void RemoveDuplicates(QVector<T>& vec);

};
#endif // MAINWINDOW_H
