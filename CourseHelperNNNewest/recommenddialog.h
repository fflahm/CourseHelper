#ifndef RECOMMENDDIALOG_H
#define RECOMMENDDIALOG_H

#include <QDialog>
#include "recommendresult.h"
namespace Ui {
class RecommendDialog;
}

class RecommendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecommendDialog(QWidget *parent = nullptr);
    ~RecommendDialog();

private slots:
    void on_WaySelect_activated(int index);

    void on_CreateButton_clicked();

private:
    Ui::RecommendDialog *ui;
    RecommendResult* my_result;
};

#endif // RECOMMENDDIALOG_H
