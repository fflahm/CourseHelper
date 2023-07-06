#include <QDebug>
#include "course.h"
#include <memory.h>
Course::Course(){
    memset(time, 0, sizeof(time));
    total_time = 0;
}
Course::Course(char* _name, bool* _time, int _total_time, int _point, int _priority){
    name = QString(_name);
    memcpy(time, _time, sizeof(time));
    total_time = _total_time;
    point = _point;
    priority = _priority;
    btn = NULL;
}
Course::~Course(){
    delete btn;
}
void Course::show(){
    qDebug() << name << ' ' << point << ' ' << priority << ' ' << total_time << Qt::endl;
    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++){
            if (time[i][j])
                qDebug() << i << ' ' << j << Qt::endl;
        }
    }
}
