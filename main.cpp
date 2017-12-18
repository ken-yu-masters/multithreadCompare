#include "omp.h"

#include <iostream>
#include <string>
#include <iomanip>

#include <QtGui>
#include "qcustomplot.h"

extern int calculatePiSerialized(long int step, double & pi, double & runtime);
extern int calculatePiReductionVerison(long int step, double & pi, double & runtime);
extern int calculatePiOmpDetailed(long int step, double & pi, double & runtime);


int main(int argc, char * argv[])
{
  #define NUMBER_OF_THREAD 16
  //force to use the thread number I set.
  omp_set_dynamic(0);

  //to store result
  QVector<double> runtimResult_serial(NUMBER_OF_THREAD + 1);
  QVector<double> runtimResult_deuction(NUMBER_OF_THREAD + 1);
  QVector<double> runtimResult_OMP_detailed(NUMBER_OF_THREAD + 1);

  //GUI
  QApplication * main_gui_app = new QApplication(argc, argv);
  QCustomPlot *customPlot = new QCustomPlot();
  //X axis
  QVector<double> x(NUMBER_OF_THREAD+1);
  for (int i = 1; i<=NUMBER_OF_THREAD;i++)
  {
    x[i] = i;
  }

  //how many steps to run Taylor series.
  long int step = 100*1000*1000;

  //1: --------------------------
  std::cout << "***serialized program" << std::endl;
  double pi, runtime;
  calculatePiSerialized(step,pi,runtime);
  for (int i = 1; i <= NUMBER_OF_THREAD; i++)
  {
    runtimResult_serial[i] = runtime;
  }
  std::cout << "      "<< runtime << " seconds" << std::endl;
//   std::cout << std::setprecision(16) << "pi: " << pi << std::endl;

  //2: --------------------------
  std::cout << "***OMP reduction version program" << std::endl;
  for (int numberOfThread = 1; numberOfThread <= NUMBER_OF_THREAD; numberOfThread++)
  {
    omp_set_num_threads(numberOfThread);
    double result = calculatePiReductionVerison(step, pi, runtime);
    runtimResult_deuction[numberOfThread] = runtime;
    std::cout << "      " << "numberOfThread : " << numberOfThread << " : "<< runtime << " seconds" << std::endl;
//     std::cout << std::setprecision(16) << "pi: " << pi << std::endl;
  }

  std::cout << "***OMP detailed version program" << std::endl;
  for (int numberOfThread = 1; numberOfThread <= NUMBER_OF_THREAD; numberOfThread++)
  {
    omp_set_num_threads(numberOfThread);
    double result = calculatePiOmpDetailed(step, pi, runtime);
    runtimResult_OMP_detailed[numberOfThread] = runtime;
    std::cout << "      " << "numberOfThread : " << numberOfThread << " : " << runtime << " seconds" << std::endl;
//     std::cout << std::setprecision(16) << "pi: " << pi << std::endl;
  }



 // create graph and assign data to it:
  customPlot->addGraph();
  customPlot->graph(0)->setData(x, runtimResult_serial);
  customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  customPlot->addGraph();
  customPlot->graph(1)->setData(x, runtimResult_deuction);
  customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
  customPlot->addGraph();
  customPlot->graph(2)->setData(x, runtimResult_OMP_detailed);
  customPlot->graph(2)->setPen(QPen(Qt::yellow)); // line color red for second graph
  // give the axes some labels:
  customPlot->xAxis->setLabel("number Of Thread");
  customPlot->yAxis->setLabel("runtime (seconds)");
  // set axes ranges, so we see all data:
  customPlot->xAxis->setRange(1, NUMBER_OF_THREAD);
  customPlot->yAxis->setRange(0, 1);
  customPlot->setMinimumWidth(500);
  customPlot->setMinimumHeight(500);
  customPlot->replot();
  customPlot->show();

  int ret = main_gui_app->exec();
  return ret;

  std::cout << "**********************" << std::endl;
}
