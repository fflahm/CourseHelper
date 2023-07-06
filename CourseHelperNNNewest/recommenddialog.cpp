#include "recommenddialog.h"
#include "ui_recommenddialog.h"
#include <QMessageBox>

RecommendDialog::RecommendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecommendDialog)
{
    ui->setupUi(this);
    ui->PointLabel->setVisible(false);
    ui->PointLow->setVisible(false);
    ui->PointUp->setVisible(false);
    ui->PointLow->setValue(18);
    ui->PointUp->setValue(25);
    ui->TimeLabel->setVisible(false);
    ui->TimeExpect->setVisible(false);
    ui->TimeExpect->setValue(25);
    ui->WaySelect->addItem("设置总学分的范围");
    ui->WaySelect->addItem("设置期望的总学时");
    ui->WaySelect->addItem("都设置");
    ui->WaySelect->addItem("都不设置");
    ui->WaySelect->setCurrentIndex(3);
    my_result = (RecommendResult*)parent;
}

RecommendDialog::~RecommendDialog()
{
    delete ui;
}

void RecommendDialog::on_WaySelect_activated(int index)
{
    if (index == 0){
        ui->PointLabel->setVisible(true);
        ui->PointLow->setVisible(true);
        ui->PointUp->setVisible(true);
        ui->TimeLabel->setVisible(false);
        ui->TimeExpect->setVisible(false);
    }
    else if (index == 1){
        ui->PointLabel->setVisible(false);
        ui->PointLow->setVisible(false);
        ui->PointUp->setVisible(false);
        ui->TimeLabel->setVisible(true);
        ui->TimeExpect->setVisible(true);
    }
    else if (index == 2){
        ui->PointLabel->setVisible(true);
        ui->PointLow->setVisible(true);
        ui->PointUp->setVisible(true);
        ui->TimeLabel->setVisible(true);
        ui->TimeExpect->setVisible(true);
    }
    else{
        ui->PointLabel->setVisible(false);
        ui->PointLow->setVisible(false);
        ui->PointUp->setVisible(false);
        ui->TimeLabel->setVisible(false);
        ui->TimeExpect->setVisible(false);
    }
}



void RecommendDialog::on_CreateButton_clicked()
{
    int index = ui->WaySelect->currentIndex();


    if (index == 0 || index == 2){
        if (ui->PointLow->value() > ui->PointUp->value()){
            QMessageBox::warning(this, "", "总学分的下限不应该超过上限！");
            return;
        }
        my_result->point_low = ui->PointLow->value();
        my_result->point_up = ui->PointUp->value();
    }
    if (index == 1 || index == 2){
        my_result->time = ui->TimeExpect->value();
    }
    my_result->way = index;
    this->close();
}

