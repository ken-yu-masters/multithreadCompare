#include "omp.h"

#include <iostream>
#include <string>
#include <iomanip>

//Taylor series
//pi = 4 * [ 1 - 1/3 + 1/5 - 1/7 + 1/9 … + ((-1)^n)/(2n + 1) ]
int calculatePiSerialized(long int step, double & pi,double & runtime)
{
  //start time
  double start = omp_get_wtime();
  //
  double sum = 0;
  for (int i = 0; i < step; i++)
  {
    int sign = 1;
    if (i %2 ==0)
    {
      sign = 1;
    }
    else
    {
      sign = -1;
    }
    sum += sign / (2.0*i + 1.0);
  }
  pi = 4.0*sum;
  //end time
  double end = omp_get_wtime();
  //used time
  runtime = end - start;

  return 0;
}

int calculatePiReductionVerison(long int step, double & pi, double & runtime)
{
  //start time
  double start = omp_get_wtime();
  //
  double sum = 0;
  #pragma  omp parallel 
  {
    int sign = 1;
    #pragma  omp for reduction (+ : sum)
    for (int i = 0; i < step; i++)
    {
      if (i % 2 == 0)
      {
        sign = 1;
      }
      else
      {
        sign = -1;
      }
      sum += sign / (2.0*i + 1.0);
    }
  }

  pi = 4.0*sum;
  //end time
  double end = omp_get_wtime();
  //used time
  runtime = end - start;

  return 0;
}

int calculatePiOmpDetailed(long int step, double & pi, double & runtime)
{
  //start time
  double start = omp_get_wtime();
  //
  double sum = 0;
  #pragma  omp parallel 
  {
    int currentThreadId = omp_get_thread_num();
    //cout << "currentThreadId: " << currentThreadId << endl;
    int numberOfThread = omp_get_num_threads();
    //cout << "numberOfThread: " << numberOfThread << endl;
    long int nubmerofStepInOneThread = step / numberOfThread;
    //cout << "nubmerofStepInOneThread: " << nubmerofStepInOneThread << endl;
    int lowerBound = currentThreadId * nubmerofStepInOneThread;
    int upperBound = (currentThreadId + 1) * nubmerofStepInOneThread;
    //last thread will cover all the steps left.
    if (currentThreadId == numberOfThread - 1)
    {
      upperBound = step;
    }
    //cout << "lowerBound: " << lowerBound << endl;
    //cout << "upperBound: " << upperBound << endl;

    //a thread local result
    double threadSum = 0;
    //
    int sign = 1;
    for (int i = lowerBound; i < upperBound; i++)
    {
      if (i % 2 == 0)
      {
        sign = 1;
      }
      else
      {
        sign = -1;
      }
      threadSum += sign / (2.0*i + 1.0);
    }

    #pragma  omp critical
    {
      sum += threadSum;
    }
  }
  pi = 4.0*sum;

  //end time
  double end = omp_get_wtime();
  //used time
  runtime = end - start;

  return 0;
}
