#ifndef RECOMMENDRESULT_H
#define RECOMMENDRESULT_H

#include <QDialog>
#include <vector>
#include <map>
#include <QLabel>
#include <QLayout>
#include "course.h"
namespace Ui {
class RecommendResult;
}

class RecommendResult : public QDialog
{
    Q_OBJECT
    friend class RecommendDialog;
public:
    explicit RecommendResult(QWidget *parent = nullptr);
    ~RecommendResult();
    bool ok();
    void get_courses(std::vector<Course*>::iterator first, std::vector<Course*>::iterator last);
    void search();
    int state_to_value(int s);//计算状态得分

private:
    Ui::RecommendResult *ui;
    int way;//0 point, 1 time, 2 both, 3 none
    int point_low;
    int point_up;
    int time;
    std::vector<Course*> courses;
    std::vector<QLabel*> courses_show;
    std::multimap<int, int, std::greater<int> > value_to_state;//按每种状态的得分从大到小排序

    QLabel* table[8][13];
    QVBoxLayout* layouts[8] ;
};

#endif // RECOMMENDRESULT_H
