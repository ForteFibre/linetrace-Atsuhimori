#pragma once

// ========================================
// Drive Mode
// ========================================

#define DRIVE_MODE_SPEED_CONTROL 0
#define DRIVE_MODE_DIRECT_PWM 1

// ========================================
// Debug
// ========================================

#define DEBUG_LINE_TRACE 1
#define DEBUG_SPEED_CONTROL 1
#define DEBUG_SENSOR 1

// ========================================
// Line Trace (Speed Control Mode)
// ========================================

#define BASE_SPEED 10.0f

#define SPEED_KP 0.002f
#define SPEED_KI 0.0f
#define SPEED_KD 0.0f

// ========================================
// Line Trace (Direct PWM Mode)
// ========================================

#define BASE_PWM 0.40f

#define PWM_KP 0.0006f
#define PWM_KI 0.0f
#define PWM_KD 0.0f

// ========================================
// Speed Control
// ========================================

#define FF_GAIN 0.035f

#define KP_SPEED 0.020f
#define KI_SPEED 0.010f

#define ACCEL_LIMIT 0.30f

#define INTEGRAL_LIMIT 20.0f

// ========================================
// Sensor
// ========================================

#define LOST_THRESHOLD 0.15f