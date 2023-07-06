#ifndef COURSETIMEDIALOG_H
#define COURSETIMEDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QLayout>
#include "course.h"
namespace Ui {
class CourseTimeDialog;
}

class CourseTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CourseTimeDialog(QWidget *parent = nullptr);
    ~CourseTimeDialog();
    void get_course(Course* _mc);
    void get_ok(bool* o);

private slots:
    void on_CreateButton_clicked();

private:
    Ui::CourseTimeDialog *ui;
    QCheckBox* courses[8][13];
    QVBoxLayout* layouts[8] ;
    Course* my_course;
    bool* ok;
};

#endif // COURSETIMEDIALOG_H
