#ifndef COURSEDIALOG_H
#define COURSEDIALOG_H

#include <QDialog>
#include "course.h"
namespace Ui {
class CourseDialog;
}

class CourseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CourseDialog(QWidget *parent = nullptr);
    ~CourseDialog();
    void get_course(Course* mc);
    void get_ok(bool* o);

private slots:
    void on_CreateButton_clicked();

private:
    Ui::CourseDialog *ui;
    Course* my_course;
    bool* ok;
};

#endif // COURSEDIALOG_H
