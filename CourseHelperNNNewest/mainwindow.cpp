#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "coursedialog.h"
#include "coursetimedialog.h"
#include "recommendresult.h"
#include <QPushButton>
#include <QInputDialog>
#include <QLabel>
#include <QDialog>
#include <QLayout>
#include <QCheckBox>
#include <QMessageBox>
#include <QDebug>
#include <algorithm>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    read_all();
    plan_now = NULL;
    delete_flag = false;
    layouts[1] = ui->C1Layout;
    layouts[2] = ui->C2Layout;
    layouts[3] = ui->C3Layout;
    layouts[4] = ui->C4Layout;
    layouts[5] = ui->C5Layout;
    layouts[6] = ui->C6Layout;
    layouts[7] = ui->C7Layout;
    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++){
            table_show[i][j] = new QPushButton(this);
            layouts[i]->addWidget(table_show[i][j]);
            connect(table_show[i][j], &QPushButton::pressed, this, [=](){this->press_table(i, j);});
            connect(table_show[i][j], &QPushButton::released, this, [=](){this->release_table();});
            table_show[i][j]->setText("");
            table_show[i][j]->setStyleSheet("background-color:white");
        }
    }
    ui->InformNameLabel->setVisible(false);
    ui->InformPriorityLabel->setVisible(false);
    ui->InformPointLabel->setVisible(false);
    ui->InformTimeLabel->setVisible(false);
    ui->Recommend->setVisible(false);
}

MainWindow::~MainWindow()
{
    save_all();
    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++)
            delete table_show[i][j];
    }
    delete ui;
}


void MainWindow::press_table(int x, int y){
    if (plan_now == NULL){
        return;
    }
    else{//只显示冲突信息
        if (table_inform[x][y].size() > 1){
            ui->InformNameLabel->setVisible(true);
            ui->InformPriorityLabel->setVisible(true);
            ui->InformPointLabel->setVisible(true);
            ui->InformTimeLabel->setVisible(true);
            for (int k = 0;k < (int)table_inform[x][y].size();k++){
                QLabel* name = new QLabel(this);
                QLabel* pri = new QLabel(this);
                QLabel* point = new QLabel(this);
                QLabel* time = new QLabel(this);
                name->setText(table_inform[x][y][k]->name);
                pri->setNum(table_inform[x][y][k]->priority);
                point->setNum(table_inform[x][y][k]->point);
                time->setNum(table_inform[x][y][k]->total_time);
                name->setAlignment(Qt::AlignCenter);
                pri->setAlignment(Qt::AlignCenter);
                point->setAlignment(Qt::AlignCenter);
                time->setAlignment(Qt::AlignCenter);
                name->setStyleSheet("border: 1px solid black");
                pri->setStyleSheet("border: 1px solid black");
                point->setStyleSheet("border: 1px solid black");
                time->setStyleSheet("border: 1px solid black");
                course_inform[0].push_back(name);
                course_inform[1].push_back(pri);
                course_inform[2].push_back(point);
                course_inform[3].push_back(time);
                ui->InformNameLayout->addWidget(name);
                ui->InformPriorityLayout->addWidget(pri);
                ui->InformPointLayout->addWidget(point);
                ui->InformTimeLayout->addWidget(time);
            }
        }
    }
}

void MainWindow::release_table(){
    if (plan_now == NULL){
        return;
    }
    else{
        ui->InformNameLabel->setVisible(false);
        ui->InformPriorityLabel->setVisible(false);
        ui->InformPointLabel->setVisible(false);
        ui->InformTimeLabel->setVisible(false);
        for (int k = 0;k < (int)course_inform[0].size();k++){
            for (int l = 0;l < 4;l++)
                delete course_inform[l][k];
        }
        for (int l = 0;l < 4;l++)
            course_inform[l].clear();
    }
}

void MainWindow::renew_table(){
    //清空信息
    for (int i = 1;i <= 7;i++){
        for (int j = 1;j <= 12;j++){
            table_inform[i][j].clear();
        }
    }

    if (plan_now == NULL){
        for (int i = 1;i <= 7;i++){
            for (int j = 1;j <= 12;j++){
                table_show[i][j]->setText("");
                table_show[i][j]->setVisible(true);
                table_show[i][j]->setStyleSheet("background-color:white");
            }
        }
    }
    else{
        std::vector<Course*>& courses_now = plan_now->courses;
        //先更新信息，再更新显示
        for (int k = 0;k < (int)courses_now.size();k++){
            for (int i = 1;i <= 7;i++){
                for (int j = 1;j <= 12;j++){
                    if (courses_now[k]->time[i][j]){
                        table_inform[i][j].push_back(courses_now[k]);
                    }
                }
            }
        }
        for (int i = 1;i <= 7;i++){
            for (int j = 1;j <= 12;j++){
                if (table_inform[i][j].empty()){
                    table_show[i][j]->setText("");
                    table_show[i][j]->setStyleSheet("background-color:white");
                    table_show[i][j]->setVisible(true);
                }
                else if (table_inform[i][j].size() == 1){
                    table_show[i][j]->setText(table_inform[i][j][0]->name);
                    table_show[i][j]->setStyleSheet("background-color:white");
                    table_show[i][j]->setVisible(true);
                }
                else{
                    //排序，优先度高的显示在上面
                    CourseCmp cc;
                    sort(table_inform[i][j].begin(), table_inform[i][j].end(), cc);
                    table_show[i][j]->setText(table_inform[i][j][0]->name);
                    table_show[i][j]->setStyleSheet("background-color:red");
                    table_show[i][j]->setVisible(true);
                }
            }
        }
    }
}

bool MainWindow::CourseCmp::operator()(Course* a, Course* b){
    return a->priority > b->priority;
}

void MainWindow::click_plan(Plan* plan){
    if (delete_flag){//删除plan
        QString tmp = "确定要删除选课计划 ";
        tmp += plan->name;
        tmp += " 吗？";
        int ret = QMessageBox::question(this, "", tmp, QMessageBox::Yes, QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return;
        plans.erase(std::find(plans.begin(), plans.end(), plan));
        delete plan;
        return;
    }
    if (plan_now == NULL){  //显示plan
        ui->AddPlan->setVisible(false);
        ui->AddCourse->setVisible(false);
        ui->DeletePlan->setVisible(false);
        ui->DeleteCourse->setVisible(false);
        ui->Recommend->setVisible(true);
        for (int k = 0;k < (int)plans.size();k++){
            if (plan != plans[k]){
                plans[k]->btn->setVisible(false);
                //qDebug() << i;
            }
        }
        for (int k = 0;k < (int)courses.size();k++){

            courses[k]->btn->setVisible(true);
            courses[k]->btn->setStyleSheet("background-color:white");

        }
        for (int k = 0;k < (int)plan->courses.size();k++){
            plan->courses[k]->btn->setStyleSheet("background-color:yellow");
        }

        plan_now = plan;

        renew_table();

    }
    else{//关闭plan
        ui->AddPlan->setVisible(true);
        ui->AddCourse->setVisible(true);
        ui->DeletePlan->setVisible(true);
        ui->DeleteCourse->setVisible(true);
        ui->Recommend->setVisible(false);
        for (int k = 0;k < (int)plans.size();k++){
            plans[k]->btn->setVisible(true);
        }
        for (int k = 0;k < (int)courses.size();k++){
            //courses[i]->btn->setVisible(false);

            courses[k]->btn->setStyleSheet("background-color:white");
        }
        plan_now = NULL;
        renew_table();
    }
}

void MainWindow::click_course(Course* course){
    if (delete_flag){
        QString tmp = "确定要删除课程 ";
        tmp += course->name;
        tmp += " 吗？";
        int ret = QMessageBox::question(this, "", tmp, QMessageBox::Yes, QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return;
        courses.erase(std::find(courses.begin(), courses.end(), course));
        for (int k = 0;k < (int)plans.size();k++){
            std::vector<Course*>::iterator iter = std::find(plans[k]->courses.begin(), plans[k]->courses.end(), course);
            if (iter != plans[k]->courses.end())
                plans[k]->courses.erase(iter);
        }
        delete course;
        return;
    }
    if (plan_now == NULL){
        return;
    }
    else{
        std::vector<Course*>::iterator iter = std::find(plan_now->courses.begin(), plan_now->courses.end(), course);
        if (iter == plan_now->courses.end()){//向plan_now添加course
            plan_now->courses.push_back(course);
            course->btn->setStyleSheet("background-color:yellow");
        }
        else{//删除plan_now中的course
            plan_now->courses.erase(iter);
            course->btn->setStyleSheet("background-color:white");
        }
        renew_table();
    }
}

void MainWindow::press_course(Course* course){
    if (delete_flag)
        return;
    if (plan_now != NULL){
        return;
    }
    else{
        for (int i = 1;i <= 7;i++){
            for (int j = 1;j <= 12;j++){
                if (course->time[i][j]){
                    table_show[i][j]->setStyleSheet("background-color:green");
                }
            }
        }
        ui->InformNameLabel->setVisible(true);
        ui->InformPriorityLabel->setVisible(true);
        ui->InformPointLabel->setVisible(true);
        ui->InformTimeLabel->setVisible(true);
        QLabel* name = new QLabel(this);
        QLabel* pri = new QLabel(this);
        QLabel* point = new QLabel(this);
        QLabel* time = new QLabel(this);
        name->setText(course->name);
        pri->setNum(course->priority);
        point->setNum(course->point);
        time->setNum(course->total_time);
        name->setAlignment(Qt::AlignCenter);
        pri->setAlignment(Qt::AlignCenter);
        point->setAlignment(Qt::AlignCenter);
        time->setAlignment(Qt::AlignCenter);
        name->setStyleSheet("border: 1px solid black");
        pri->setStyleSheet("border: 1px solid black");
        point->setStyleSheet("border: 1px solid black");
        time->setStyleSheet("border: 1px solid black");
        course_inform[0].push_back(name);
        course_inform[1].push_back(pri);
        course_inform[2].push_back(point);
        course_inform[3].push_back(time);
        ui->InformNameLayout->addWidget(name);
        ui->InformPriorityLayout->addWidget(pri);
        ui->InformPointLayout->addWidget(point);
        ui->InformTimeLayout->addWidget(time);

    }
}
void MainWindow::release_course(){
    if (delete_flag)
        return;
    if (plan_now != NULL){
        return;
    }
    else{
        for (int i = 1;i <= 7;i++){
            for (int j = 1;j <= 12;j++){
                table_show[i][j]->setStyleSheet("background-color:white");
            }
        }
        ui->InformNameLabel->setVisible(false);
        ui->InformPriorityLabel->setVisible(false);
        ui->InformPointLabel->setVisible(false);
        ui->InformTimeLabel->setVisible(false);
        for (int k = 0;k < (int)course_inform[0].size();k++){

            for (int l = 0;l < 4;l++)
                delete course_inform[l][k];
        }
        for (int l = 0;l < 4;l++)
            course_inform[l].clear();
    }
}

void MainWindow::on_AddPlan_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr(""), tr("请输入选课计划的名称："), QLineEdit::Normal, tr("我的选课计划"), &ok);
    if (!ok)
        return;
    QPushButton* btn = new QPushButton(ui->PlanWidget);
    btn->setText(name);
    btn->setStyleSheet("background-color:white");
    ui->PlanLayout->addWidget(btn);
    ui->PlanWidget->setLayout(ui->PlanLayout);


    Plan* plan = new Plan(name, btn);
    connect(btn, &QPushButton::clicked, this, [=](){this->click_plan(plan);});

    plans.push_back(plan);

}


void MainWindow::on_AddCourse_clicked()
{
    bool ok = false;
    CourseDialog input;
    input.setWindowTitle("");
    Course* course = new Course;
    input.get_course(course);
    input.get_ok(&ok);
    input.exec();
    //course->show();
    if (!ok){
        delete course;
        return;
    }
    ok = false;
    CourseTimeDialog time_input;
    time_input.setWindowTitle("");
    time_input.get_course(course);
    time_input.get_ok(&ok);
    time_input.exec();
    //course->show();
    if (!ok){
        delete course;
        return;
    }

    QPushButton* btn = new QPushButton(ui->CourseWidget);
    btn->setText(course->name);
    ui->CourseLayout->addWidget(btn);
    ui->CourseWidget->setLayout(ui->CourseLayout);


    connect(btn, &QPushButton::clicked, this, [=](){this->click_course(course);});
    connect(btn, &QPushButton::pressed, this, [=](){this->press_course(course);});
    connect(btn, &QPushButton::released, this, [=](){this->release_course();});

    btn->setStyleSheet("background-color:white");
    course->btn = btn;
    courses.push_back(course);
}

void MainWindow::on_DeletePlan_clicked()
{
    if (!delete_flag){
        delete_flag = true;
        ui->AddPlan->setVisible(false);
        ui->AddCourse->setVisible(false);
        ui->DeleteCourse->setVisible(false);
        ui->DeletePlan->setText("完成");
        for (int k = 0;k < (int)courses.size();k++){
            courses[k]->btn->setVisible(false);
        }
    }
    else{
        delete_flag = false;
        ui->AddPlan->setVisible(true);
        ui->AddCourse->setVisible(true);
        ui->DeleteCourse->setVisible(true);
        ui->DeletePlan->setText("删除选课计划");
        for (int k = 0;k < (int)courses.size();k++){
            courses[k]->btn->setVisible(true);
        }
    }
}


void MainWindow::on_DeleteCourse_clicked()
{
    if (!delete_flag){
        delete_flag = true;
        ui->AddPlan->setVisible(false);
        ui->AddCourse->setVisible(false);
        ui->DeletePlan->setVisible(false);
        ui->DeleteCourse->setText("完成");

        for (int k = 0;k < (int)plans.size();k++){

            plans[k]->btn->setVisible(false);
        }
    }
    else{
        delete_flag = false;
        ui->AddPlan->setVisible(true);
        ui->AddCourse->setVisible(true);
        ui->DeletePlan->setVisible(true);
        ui->DeleteCourse->setText("删除课程");
        for (int k = 0;k < (int)plans.size();k++){
            plans[k]->btn->setVisible(true);
        }
    }
}


void MainWindow::on_Recommend_clicked()
{

    RecommendResult result(this);
    result.setWindowTitle("");
    result.resize(1000,600);
    if (!result.ok())
        return;
    result.get_courses(plan_now->courses.begin(), plan_now->courses.end());
    result.search();
    result.exec();
}

struct CourseForSave{
    QChar name[20];
    bool time[8][13];
    int total_time;
    int point;
    int priority;
};
struct PlanForSave{
    QChar name[20];
    int courses[20];
};
void MainWindow::save_all(){
    //qDebug() << "begin save";
    std::ofstream output("storage.dat", std::ios::out | std::ios::binary);
    int course_num = courses.size();
    output.write((char*)&course_num, sizeof(int));//有几个course
    for (int k = 0;k < course_num;k++){

        CourseForSave tmp;
        memset(tmp.name, '\0',sizeof(tmp.name));
        for (int l = 0;l < courses[k]->name.size();l++){
            tmp.name[l] = courses[k]->name[l];
        }
        //qDebug() << tmp.name;
        memcpy(tmp.time, courses[k]->time, sizeof(tmp.time));
        tmp.total_time = courses[k]->total_time;
        tmp.point = courses[k]->point;
        tmp.priority = courses[k]->priority;
        output.write((char*)&tmp, sizeof(tmp));
    }

    int plan_num = plans.size();
    output.write((char*)&plan_num, sizeof(int));
    std::map<Course*, int> course_to_index;
    for (int k = 0;k < course_num;k++){
        course_to_index[courses[k]] = k;
    }
    for (int k = 0;k < plan_num;k++){
        PlanForSave tmp;
        memset(tmp.name, '\0',sizeof(tmp.name));
        for (int l = 0;l < plans[k]->name.size();l++){
            tmp.name[l] = plans[k]->name[l];
        }
        memset(tmp.courses, -1, sizeof(tmp.courses));
        for (int l = 0;l < (int)plans[k]->courses.size();l++){
            tmp.courses[l] = course_to_index[plans[k]->courses[l]];
        }
        output.write((char*)&tmp, sizeof(tmp));
    }
    output.close();

}
void MainWindow::read_all(){

    std::ifstream input("storage.dat", std::ios::in | std::ios::binary);
    int course_num;
    input.read((char*)&course_num, sizeof(int));
    for (int k = 0;k < course_num;k++){
        CourseForSave tmp;
        input.read((char*)&tmp, sizeof(tmp));
        Course* course = new Course;
        course->name = QString(tmp.name);
        for (int i = 1;i <= 7;i++){
            for (int j = 1;j <= 12;j++)
                course->time[i][j] = tmp.time[i][j];
        }
        course->total_time = tmp.total_time;
        course->point = tmp.point;
        course->priority = tmp.priority;

        QPushButton* btn = new QPushButton(ui->CourseWidget);
        btn->setText(course->name);
        ui->CourseLayout->addWidget(btn);
        ui->CourseWidget->setLayout(ui->CourseLayout);
        connect(btn, &QPushButton::clicked, this, [=](){this->click_course(course);});
        connect(btn, &QPushButton::pressed, this, [=](){this->press_course(course);});
        connect(btn, &QPushButton::released, this, [=](){this->release_course();});
        btn->setStyleSheet("background-color:white");
        course->btn = btn;
        courses.push_back(course);
    }

    int plan_num;
    input.read((char*)&plan_num, sizeof(int));
    for (int k = 0;k < plan_num;k++){
        PlanForSave tmp;
        input.read((char*)&tmp, sizeof(tmp));

        QString name(tmp.name);
        QPushButton* btn = new QPushButton(ui->PlanWidget);
        btn->setText(name);
        btn->setStyleSheet("background-color:white");
        ui->PlanLayout->addWidget(btn);
        ui->PlanWidget->setLayout(ui->PlanLayout);


        Plan* plan = new Plan(name, btn);
        connect(btn, &QPushButton::clicked, this, [=](){this->click_plan(plan);});
        for (int l = 0;l < 20;l++){
            if (tmp.courses[l] == -1)
                break;
            plan->courses.push_back(courses[tmp.courses[l]]);
        }

        plans.push_back(plan);
    }

    input.close();
}




