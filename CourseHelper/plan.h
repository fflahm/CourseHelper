#include <QPushButton>
#include <QString>
#include <course.h>
#include <vector>

#ifndef PLAN_H
#define PLAN_H


class Plan
{
    friend class MainWindow;
private:
    QString name;
    QPushButton* btn;
    std::vector<Course*> courses;
    void renew_table();
public:
    Plan(QString _n, QPushButton* _b);
    ~Plan();
};

#endif // PLAN_H
