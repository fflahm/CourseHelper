#include "recommenddialog.h"
#include "ui_recommendresult.h"
#include <QDebug>
#include <Set>
#include <QString>

const int MIN = -0x7fffffff;

RecommendResult::RecommendResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecommendResult)
{
    ui->setupUi(this);
    way = point_low = point_up = time = -1;
    RecommendDialog input(this);
    input.exec();

    layouts[1] = ui->C1Layout;
    layouts[2] = ui->C2Layout;
    layouts[3] = ui->C3Layout;
    layouts[4] = ui->C4Layout;
    layouts[5] = ui->C5Layout;
    layouts[6] = ui->C6Layout;
    layouts[7] = ui->C7Layout;
    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++){
            table[i][j] = new QLabel(this);
            layouts[i]->addWidget(table[i][j]);
        }
    }
}

RecommendResult::~RecommendResult()
{
    delete ui;
}
bool RecommendResult::ok(){
    return way != -1;
}
void RecommendResult::get_courses(std::vector<Course*>::iterator first, std::vector<Course*>::iterator last){
    courses.assign(first, last);
}

int RecommendResult::state_to_value(int s){
    //根据名称重复，时间冲突，总优先度，总学分，总学时等信息计算每个状态的得分
    // value=total_priority-sqrt(total_time - time_expect) if valid else MIN
    int total_priority = 0;
    int total_time = 0;
    int total_point = 0;
    bool table[8][13] = {false};
    std::set<QString> names;

    for (int k = 0;(1 << k) <= s;k++){
        if ((s >> k) & 1){//选了course k
            if (names.find(courses[k]->name) != names.end())
                return MIN;//选重了
            names.insert(courses[k]->name);
            for (int i = 1;i <= 7;i++){
                for (int j = 1;j <= 12;j++){
                    if (courses[k]->time[i][j]){
                        if (table[i][j])
                            return MIN;//时间冲突
                        table[i][j] = true;
                    }
                }
            }
            total_priority += courses[k]->priority;
            total_time += courses[k]->total_time;
            total_point += courses[k]->point;

        }
    }
    if ((way == 0 || way == 2) && (total_point < point_low || total_point > point_up))
        return MIN;//不在总学分范围内
    int value = total_priority;
    if (way == 1 || way == 2)
        value -= (total_time - time) * (total_time - time);
    return value;
}


void RecommendResult::search(){
    for (int s = 0;s < (1 << courses.size());s++){
        value_to_state.insert(std::make_pair(state_to_value(s), s));
    }

    if (value_to_state.begin()->first != MIN){
        int s = value_to_state.begin()->second;
        int total_priority = 0;
        int total_time = 0;
        int total_point = 0;
        for (int k = 0;(1 << k) <= s;k++){
            if ((s >> k) & 1){//选了course k
                for (int i = 1;i <= 7;i++){
                    for (int j = 1;j <= 12;j++){
                        if (courses[k]->time[i][j]){
                            table[i][j]->setText(courses[k]->name);
                        }
                    }
                }
                total_priority += courses[k]->priority;
                total_time += courses[k]->total_time;
                total_point += courses[k]->point;

                QLabel* lb = new QLabel(this);
                lb->setText(courses[k]->name);
                ui->CourseLayout->addWidget(lb);
                courses_show.push_back(lb);
            }
        }
        ui->PriorityLabel->setNum(total_priority);
        ui->TimeLabel->setNum(total_time);
        ui->PointLabel->setNum(total_point);
    }
}
