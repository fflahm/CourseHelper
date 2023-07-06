#include <plan.h>
#include <course.h>
#include<QPushButton>
#include<QLabel>
#include<algorithm>
Plan::Plan(QString _n, QPushButton* _b){
    name = _n;
    btn = _b;
}
Plan::~Plan(){
    delete btn;
}

