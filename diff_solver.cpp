/*   This program inputs a second order differential equation of the
 *   form y'' = f(y',y,t), the start and end times (t0 and tf), the initial
 *   values of  and y' at t=t0, and plots the function y(t).
*/

#include "diff_solver.h"
#include "ui_diff_solver.h"
#include <QFile>
#include <QtScript/QScriptEngine>

Diff_solver::Diff_solver(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Diff_solver)
{
    ui->setupUi(this);
    connect(ui->enter, SIGNAL(released()), this,  SLOT(solver()));
    // call the solver function when the button is pressed
}

Diff_solver::~Diff_solver()
{
    delete ui;
}

void Diff_solver::solver(){ // to solve the differential equation and write the data into a file- "data.txt"
    QFile file("data.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // do all this only if file opens without error
        QTextStream stream(&file);

        int n=1000;  // increase n to get smoother curve, computation time increases though
        double t0=ui->t1->toPlainText().toDouble(); // initial time
        double tf=ui->t2->toPlainText().toDouble(); // final time
        double h=(double)(tf-t0)/n;                 // step size
        QVector<double> t(n);
        t[0]=t0;
        QVector<double> y, dy, ddy;
        // y stores the values of y, dy stores the values of y', ddy stores the values of y''
        y.append(ui->y0->toPlainText().toDouble());   // initial value of y: y(t0)
        dy.append(ui->y10->toPlainText().toDouble()); // initial value of y': y'(tf)
        ddy=function(dy,y,t);
        stream<<y[0]<<"\n";
        for(int i=1; i<n; i++){    // Euler method to solve differential equations
            t[i]=t[i-1]+h;
            y.append(y[i-1]+h*dy[i-1]);      // y(t)=y(t-h)+h*y'(t-h)
            dy.append(dy[i-1]+h*ddy[i-1]);   // y'(t)=y'(t-h)+h*y'(t-h)
            ddy.append(function(dy,y,t)[i]); // y''(t)=f(y'(t),y(t),t)
            stream << y[i] <<"\n";
        }
        qDebug() << "Writing finished";
     }
    else{                   // show error in console if file cannot be opened
        qDebug() << "fail";
    }
    Diff_solver::plotter();    // plot the obtained data
}

QVector<double> Diff_solver::function(QVector<double> y1, QVector<double> y, QVector<double> t){
    // converts the function input (y'' = ?) taken as a string into a mathematical expression
    // which can be computed.
    QString expression_string(ui->func_input->toPlainText());
    QScriptEngine expression;
    QVector<double> ddy;
    for(int i=0; i < y1.size() ;i++){    // computes each term of the vector again and again
        // this isn't the most efficient way to do this, it would be easier to simply append
        // the last value.
        expression_string.replace("y'", "v");                //  this has to be done becuase "y"
        expression.globalObject().setProperty("v", y1[i]);   //  is also present in "y'"
        expression.globalObject().setProperty("y", y[i]);
        expression.globalObject().setProperty("t", t[i]);
        ddy.append(expression.evaluate(expression_string).toNumber());
    }
    return ddy;
}

void Diff_solver::plotter(){
    QVector<double> v;

    int n=1000;
    double t0=ui->t1->toPlainText().toDouble(), tf=ui->t2->toPlainText().toDouble();
    double h=(double)(tf-t0)/n;
        QVector<double> x;
        QFile textFile("data.txt");
        if(textFile.open(QIODevice::ReadOnly))   // send the data into a vector
        {
            QTextStream stream(&textFile);
            QString line = stream.readLine();
            while (!line.isNull()) {
                v.append(line.toDouble());
                line = stream.readLine();
             };

         for(int i =0; i<v.size(); i++) {     // recreate the time vector
             x.append(t0+i*h);
         }
         ui->Plot->clearGraphs();
         ui->Plot->addGraph();          // Plot

         ui->Plot->graph(0)->addData(x,v);
         ui->Plot->graph(0)->setPen(QPen(Qt::blue));
         ui->Plot->xAxis->setLabel("t");
         ui->Plot->yAxis->setLabel("y");
         ui->Plot->graph(0)->rescaleAxes();
         ui->Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
         ui->Plot->replot();


        }

}
