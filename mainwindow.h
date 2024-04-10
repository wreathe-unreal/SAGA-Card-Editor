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

private:
    QString CleanString(const QString &input);
    void PopulateComboBoxes();
    void UpdateCardTable();
    QJsonArray StringToJsonArray(const QString &input);
    Ui::MainWindow *ui;
    QString JsonArrayToString(const QJsonArray &jsonArray);
    void CheckValidCardID(QLineEdit *qle);
    void SetupTables();
    void ResetActionFields();
    void ResetCardFields();
    QJsonObject SerializeCard(QVector<Action> Actions);
    void PopulateWithCardToEdit(Card editCard);
    void SetComboBoxToString(QComboBox *comboBox, const QString &value);
    void MakeTablesUneditable();
    QString IdToString(const QString &input);
};
#endif // MAINWINDOW_H
