#pragma once

// ========================================
// Drive Mode
// ========================================

#define DRIVE_MODE_SPEED_CONTROL 0
#define DRIVE_MODE_DIRECT_PWM 1

// ========================================
// Debug
// ========================================

#define DEBUG_LINE_TRACE 0
#define DEBUG_SPEED_CONTROL 0
#define DEBUG_SENSOR 1

// ========================================
// Debug Function
// ========================================

#define DEBUG_CROSS 1
#define DEBUG_LOST 1

// ========================================
// Tuning Mode
// ========================================
//
// 1 = PID調整用
//     交差点処理OFF
//     ロスト探索OFF
//
// 0 = 本番用
// ========================================

#define TUNING_MODE 0

// ========================================
// Special Function
// ========================================

#if TUNING_MODE

#define ENABLE_CROSS_STRAIGHT 0
#define ENABLE_LINE_LOST_SEARCH 0

#else

#define ENABLE_CROSS_STRAIGHT 1
#define ENABLE_LINE_LOST_SEARCH 1

#endif

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

#define BASE_PWM 0.75f

#define PWM_KP 0.0025f
#define PWM_KI 0.0000f
#define PWM_KD 0.00003f

/*
調整記録

BASE_PWM  PWM_KP   PWM_KI    PWM_KD

0.50      0.0015   0.0       0.000075
0.70      0.0080   0.0       0.000400
0.80      0.0200   0.0       0.000250
0.8       0.003　　0.0　　　　0.000001　確実
0.8       0.003   0.0        0.00005　　安定
0.75      0.0025  0.0        0.00003　　完璧

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

// 白線判定閾値
//
// lineValue = 1.0 - normalized
//
// lineValue > WHITE_DETECT_THRESHOLD
// なら白線とみなす
//

#define WHITE_DETECT_THRESHOLD 0.85f

// ========================================
// Lost Detection
// ========================================

// denominator がこれ以下ならロスト候補

#define LOST_THRESHOLD 0.30f

// 連続何周期でロスト確定か

#define LOST_COUNT_THRESHOLD 20

#define LOST_RECOVER_COUNT 15

// ========================================
// Cross Detection
// ========================================

// 白線を何個以上読んだら
// 交差点候補にするか

#define CROSS_WHITE_COUNT 5

// 何周期連続で交差点候補なら
// 交差点確定にするか

#define CROSS_COUNT_THRESHOLD 20

// ========================================
// Cross Straight
// ========================================

// 交差点検出後
// この時間だけ直進

#define CROSS_STRAIGHT_TIME_MS 300

// ========================================
// Lost Direction
// ========================================

// 0 = 直前5ms方式
// 1 = 履歴方式

#define LOST_DIRECTION_MODE 1

// 履歴長

#define LOST_DIRECTION_HISTORY 10

// ========================================
// Lost Search
// ========================================

// 方向記憶式ロスト探索
//
// dir = +1 → 右回転
// dir = -1 → 左回転
//

#define LOST_SEARCH_PWM_STAGE1 0.80f
#define LOST_SEARCH_PWM_STAGE2 0.80f

#define LOST_SEARCH_SPEED_STAGE1 40.0f
#define LOST_SEARCH_SPEED_STAGE2 50.0f

// ========================================
// LED
// ========================================

// ロスト検知LED保持時間

#define LOST_LED_TIME_MS 3000
