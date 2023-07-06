#include "coursedialog.h"
#include "ui_coursedialog.h"
#include <QMessageBox>
CourseDialog::CourseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CourseDialog)
{
    ui->setupUi(this);
    my_course = NULL;
    ok = NULL;
}

CourseDialog::~CourseDialog()
{
    delete ui;
}
void CourseDialog::get_course(Course* mc){
    my_course = mc;
}
void CourseDialog::get_ok(bool* o){
    ok = o;
}
void CourseDialog::on_CreateButton_clicked()
{
    QString name = ui->NameInput->text();
    if (name.size() == 0){
        QMessageBox::warning(0, "", "课程名称不能为空!");
        return;
    }
    int point;
    if (ui->Point1->isChecked())
        point = 1;
    else if (ui->Point2->isChecked())
        point = 2;
    else if (ui->Point3->isChecked())
        point = 3;
    else if (ui->Point4->isChecked())
        point = 4;
    else if (ui->Point5->isChecked())
        point = 5;
    else{
        QMessageBox::warning(0, "", "必须选择学分！");
        return;
    }
    int priority = ui->PriorityInput->value();
    my_course->name = name;
    my_course->point = point;
    my_course->priority = priority;
    *ok = true;
    this->close();
}

