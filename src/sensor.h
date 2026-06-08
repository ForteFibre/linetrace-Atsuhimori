#pragma once

void sensorUpdate();

float readNormalized(int index);

float getLinePosition();

int readRaw(int index);

void calibrationStart();

void calibrationUpdate();

bool calibrationFinished();

// ライン状態

bool isLineLost();

bool isCrossLine();

int getLastSensor();

int getLostDirection();