#include <QString>
#include <QPushButton>
#ifndef COURSE_H
#define COURSE_H


class Course
{
    friend class CourseDialog;
    friend class CourseTimeDialog;
    friend class MainWindow;
    friend class RecommendResult;

private:
    QString name;
    bool time[8][13]; //表示周几第几节是否有课
    int point;
    int total_time;
    int priority;
    QPushButton* btn;


public:

    Course();
    Course(char* _name, bool* _time, int _total_time, int _point, int _priority);
    ~Course();
    void show();
};

#endif // COURSE_H
