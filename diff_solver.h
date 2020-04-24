#ifndef DIFF_SOLVER_H
#define DIFF_SOLVER_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Diff_solver; }
QT_END_NAMESPACE

class Diff_solver : public QMainWindow
{
    Q_OBJECT

public:
    Diff_solver(QWidget *parent = nullptr);
    ~Diff_solver();

private:
    Ui::Diff_solver *ui;

private slots:
    void solver();
    QVector<double> function(QVector<double>, QVector<double>, QVector<double>);
    void plotter();

};
#endif // DIFF_SOLVER_H
