#include "coursetimedialog.h"
#include "ui_coursetimedialog.h"
#include "course.h"
#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>
CourseTimeDialog::CourseTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CourseTimeDialog)
{
    ui->setupUi(this);
    layouts[1] = ui->C1Layout;
    layouts[2] = ui->C2Layout;
    layouts[3] = ui->C3Layout;
    layouts[4] = ui->C4Layout;
    layouts[5] = ui->C5Layout;
    layouts[6] = ui->C6Layout;
    layouts[7] = ui->C7Layout;

    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++){
            courses[i][j] = new QPushButton(this);
            layouts[i]->addWidget(courses[i][j]);
            checked[i][j] = false;
            courses[i][j]->setStyleSheet("background-color:white");
            connect(courses[i][j], &QPushButton::clicked, this, [=](){this->click_course(i, j);});
        }
    }
    my_course = NULL;
    ok = NULL;
}

CourseTimeDialog::~CourseTimeDialog()
{
    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++)
            delete courses[i][j];
    }
    delete ui;
}
void CourseTimeDialog::get_course(Course* _mc){
    my_course = _mc;
}
void CourseTimeDialog::get_ok(bool* o){
    ok = o;
}
void CourseTimeDialog::click_course(int i, int j){
    if (checked[i][j]){
        courses[i][j]->setStyleSheet("background-color:white");
        checked[i][j] = false;
    }
    else{
        courses[i][j]->setStyleSheet("background-color:red");
        checked[i][j] = true;
    }
}
void CourseTimeDialog::on_CreateButton_clicked()
{
    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++){
            if (checked[i][j])
            {

                my_course->time[i][j] = true;
                my_course->total_time++;
            }
        }
    }
    if (my_course->total_time == 0){
        QMessageBox::warning(0, "", "课时数不能为零！");
        return;
    }
    *ok = true;
    this->close();
}

