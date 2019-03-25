#include <stdio.h>

typedef struct PID PID;

struct PID{
    double p,i,d;
    double previousError;
    double cumulativeError;//so called derivative
};

PID* makePID(double p, double i, double d){
    PID* pid = (PID*) malloc(sizeof (PID));
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->previousError = 0;
    pid->cumulativeError = 0;
    return  pid;
}

double computePIDOutput(PID *pid, double currentValue, double targetValue, double dt){
    double error, derivative, result, pComponent, iComponent, dComponent;
    error = targetValue - currentValue;
    derivative = (error - pid->previousError)/dt;
    pid->cumulativeError += error * dt;

    pComponent = pid->p * error;
    iComponent = pid->i * pid->cumulativeError;
    dComponent = pid->d * derivative;

    result = pComponent + iComponent + dComponent;

    pid->previousError = error;
    return result;
}



int main()
{
    int i;
    double position=0, targetPosition=10;
    PID *pid;
    printf("Hello World!\n");
    pid = makePID(0.06,0.06,0.06);

    for(i=0;i<100;i++){
        position += computePIDOutput(pid,position,targetPosition,0.1) + (rand()%100)/(double)100;
        printf("%f\n",position);
    }

    return 0;
}
