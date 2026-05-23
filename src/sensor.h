#pragma once

void sensorUpdate();

float readNormalized(int index);

float getLinePosition();

int readRaw(int index);

void calibrationStart();

void calibrationUpdate();

bool calibrationFinished();
