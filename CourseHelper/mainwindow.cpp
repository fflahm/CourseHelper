#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "coursedialog.h"
#include "coursetimedialog.h"
#include "recommenddialog.h"
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
    ui->ConflictWidget->setVisible(false);
    ui->Recommend->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::press_table(int x, int y){
    if (plan_now == NULL){
        return;
    }
    else{//只显示冲突信息
        if (table_inform[x][y].size() > 1){
            for (int k = 0;k < (int)table_inform[x][y].size();k++){
                QLabel* name = new QLabel(this);
                QLabel* pri = new QLabel(this);
                name->setText(table_inform[x][y][k]->name);

                pri->setNum(table_inform[x][y][k]->priority);
                conflict_names.push_back(name);
                conflict_priorities.push_back(pri);
                ui->ConflictLeftLayout->addWidget(name);
                ui->ConflictRightLayout->addWidget(pri);
            }
            ui->ConflictWidget->setVisible(true);
        }
    }
}

void MainWindow::release_table(){
    if (plan_now == NULL){
        return;
    }
    else{
        for (int k = 0;k < (int)conflict_names.size();k++){
            //ui->ConflictLeftLayout->removeWidget(conflict_names[k]);
            //ui->ConflictRightLayout->removeWidget(conflict_priorities[k]);
            delete conflict_names[k];
            delete conflict_priorities[k];
        }
        conflict_names.clear();
        conflict_priorities.clear();
        ui->ConflictWidget->setVisible(false);
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
        int ret = QMessageBox::question(this, "删除", tmp, QMessageBox::Yes, QMessageBox::No);
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
            plan->courses[k]->btn->setStyleSheet("background-color:red");
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
        int ret = QMessageBox::question(this, "删除", tmp, QMessageBox::Yes, QMessageBox::No);
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
            course->btn->setStyleSheet("background-color:red");
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
                    table_show[i][j]->setText(course->name);
                }
            }
        }
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
                table_show[i][j]->setText("");
            }
        }
    }
}

void MainWindow::on_AddPlan_clicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("添加选课计划"), tr("请输入选课计划的名称："), QLineEdit::Normal, tr("new plan"), &ok);
    if (!ok)
        return;
    QPushButton* btn = new QPushButton(ui->PlanWidget);
    btn->setText(name);
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
    if (!result.ok())
        return;
    result.get_courses(plan_now->courses.begin(), plan_now->courses.end());
    result.search();
    result.exec();
}

