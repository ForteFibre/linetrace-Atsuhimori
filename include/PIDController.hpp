/// @file pid.hpp
/// @brief PID制御を行うクラス

#pragma once

#include <platform/NonCopyable.h>

#include <algorithm>

namespace fibril
{
inline namespace controller
{
/**
 * @brief PID制御を行うクラス
 *
 * @tparam T 制御対象のデータ型（デフォルトはfloat）
 * @note 一部の関数はメソッドチェーンで呼び出すことができます
 */
template <typename T = float>
class PIDController : mbed::NonCopyable<PIDController<T>>
{
  using Self = PIDController<T>;

public:
  /**
   * @brief デフォルトコンストラクタ
   *
   * 全てのパラメータを0で初期化します。
   */
  explicit PIDController() : PIDController(T(0), T(0), T(0)) {}

  /**
   * @brief パラメータ付きコンストラクタ
   *
   * @param kp 比例ゲイン
   * @param ki 積分ゲイン
   * @param kd 微分ゲイン
   * @param max 制御出力の最大値（デフォルトは1）
   */
  explicit PIDController(const T kp, const T ki, const T kd, T max = T(1))
  : _kp(kp),
    _ki(ki),
    _kd(kd),
    _ep(T(0)),
    _ei(T(0)),
    _u(T(0)),
    _max(max),
    _i_saturation(std::numeric_limits<T>::max()),
    _target(T(0))
  {
  }

  /**
   * @brief 制御器を更新
   *
   * @param current 制御量の値
   * @note
   * 速度の場合与える単位に注意、例えば10msごとのメートルでの速度なら単位はm/sではなくm/(10ms)となっている
   *    できるだけ単位をSI単位系に合わせると良い
   */
  void update(const T current) noexcept
  {
    auto error = _target - current;

    auto ed = error - _ep;
    _ei += error * _ki;
    _ei = std::clamp(_ei, -_i_saturation, _i_saturation);
    _ep = error;
    _u = _kp * error + _ei + _kd * ed;

    _u = std::clamp(_u, -_max, _max);
  }

  /**
   * @brief 制御出力を取得
   *
   * @return T 制御出力
   */
  T output() const noexcept { return _u; }

  /**
   * @brief 目標値を設定
   *
   * @param target 目標値
   */
  void target(const T target) noexcept { _target = target; }

  /**
   * @brief 微分・積分値をリセット
   */
  void reset() noexcept
  {
    _ep = T(0);
    _ei = T(0);
    _u = T(0);
  }

  /**
   * @brief 比例ゲインを設定
   *
   * @param kp 比例ゲイン
   * @return Self& 自身への参照
   */
  Self & kp(const T kp) noexcept
  {
    _kp = kp;
    return *this;
  }

  /**
   * @brief 積分ゲインを設定
   *
   * @param ki 積分ゲイン
   * @return Self& 自身への参照
   */
  Self & ki(const T ki) noexcept
  {
    _ki = ki;
    return *this;
  }

  /**
   * @brief 微分ゲインを設定
   *
   * @param kd 微分ゲイン
   * @return Self& 自身への参照
   */
  Self & kd(const T kd) noexcept
  {
    _kd = kd;
    return *this;
  }

  /**
   * @brief 制御出力の最大値(正)を設定
   *
   * @param max 制御出力の最大値
   * @return Self& 自身への参照
   */
  Self & max(const T max) noexcept
  {
    _max = max;
    return *this;
  }

  /**
   * @brief 積分項の飽和値を設定
   *
   * @param i_saturation 積分項の飽和値
   * @return Self& 自身への参照
   */
  Self & i_saturation(const T i_saturation) noexcept
  {
    _i_saturation = i_saturation;
    return *this;
  }

protected:
  T _kp;
  T _ki;
  T _kd;
  T _ep, _ei;
  T _u;
  T _max;
  T _i_saturation;
  T _target;
};
}  // namespace controller
}  // namespace fibril