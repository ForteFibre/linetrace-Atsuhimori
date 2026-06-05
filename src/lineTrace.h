#pragma once

void lineTraceUpdate();

void resetLineTrace();

void setLineTracePID(float p, float i, float d);

void setDirectPwmPID(float p, float i, float d);

void setBaseSpeed(float speed);

void setBasePwm(float pwm);