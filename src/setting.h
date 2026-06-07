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

#define BASE_PWM 0.70f

#define PWM_KP 0.008f
#define PWM_KI 0.000f
#define PWM_KD 0.0004f

/*  
BASE_PWM,PWM_KP,PWM_KI,PWM_KD

0,5   0.0015,0.0 0.000075 上
0.7   0.008, 0.0, 0.0004 上
0.8   0.020, 0.0, 0.00025 上






























*/

// ========================================
// Line Trace Integral
// ========================================

#define LINE_INTEGRAL_LIMIT 1500.0f

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

#define LOST_THRESHOLD 0.50f

#define LOST_COUNT_THRESHOLD 3

// ========================================
// Lost Search
// ========================================

#define LOST_STAGE2_COUNT 30

#define LOST_SEARCH_PWM_STAGE1 0.50f
#define LOST_SEARCH_PWM_STAGE2 0.70f

#define LOST_SEARCH_SPEED_STAGE1 10.0f
#define LOST_SEARCH_SPEED_STAGE2 15.0f