#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <qpushbutton.h>
#include <QPushButton>
#include <vector>
#include <plan.h>
#include <QLayout>
#include <QLabel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Plan;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_AddPlan_clicked();

    void on_AddCourse_clicked();


    void on_DeletePlan_clicked();

    void on_DeleteCourse_clicked();

    void on_Recommend_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<Plan*> plans;
    std::vector<Course*> courses;

    Plan* plan_now; //当前正在显示的plan，没有置NULL
    bool delete_flag; //当前是否处在删除状态
    void click_plan(Plan* plan);
    void click_course(Course* course);
    void press_course(Course* course);
    void release_course();
    void press_table(int x, int y);
    void release_table();
    void renew_table();

    struct CourseCmp{
        bool operator()(Course* a, Course* b);
    };

    QVBoxLayout* layouts[8];

    QPushButton* table_show[8][13];//管理显示出的课表
    std::vector<Course*> table_inform[8][13];//管理课表信息：每个时段有哪些课

    std::vector<QLabel*> conflict_names;
    std::vector<QLabel*> conflict_priorities;
};
#endif // MAINWINDOW_H
