#pragma once
#include <chrono>
#include <cmath>
#include <cstdint>

#define CHECK_NULL_VOID(ptr)                                                                                           \
    do {                                                                                                               \
        if (!(ptr)) {                                                                                                  \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0)

#define CHECK_NULL_RETURN(ptr, ret)                                                                                    \
    do {                                                                                                               \
        if (!(ptr)) {                                                                                                  \
            return ret;                                                                                                \
        }                                                                                                              \
    } while (0)

#define CHECK_ERROR_CODE_RETURN(code)                                                                                  \
    do {                                                                                                               \
        if ((code) > 0) {                                                                                              \
            return code;                                                                                               \
        }                                                                                                              \
    } while (0)

#define CHECK_EQUAL_VOID(var, value)                                                                                   \
    do {                                                                                                               \
        if ((var) == (value)) {                                                                                        \
            return;                                                                                                    \
        }                                                                                                              \
    } while (0)

namespace rnoh {
namespace sharedEle {

inline double vpToPx(double vp) {
    //TODO get densityPixels in CAPI
    return vp * 3.25;
}

template<typename T, std::size_t N>
constexpr std::size_t ArraySize(T (&)[N]) noexcept
{
    return N;
}

template<typename T, int32_t N>
T ConvertIntToEnum(int32_t index, const T (&values)[N], T defaultValue)
{
    if (index >= 0 && index < N) {
        return values[index];
    }
    return defaultValue;
}

template<typename T>
constexpr T Infinity()
{
    return static_cast<const T>(1000000.0);
}

inline bool NearEqual(const double left, const double right, const double epsilon)
{
    return (std::abs(left - right) <= epsilon);
}

template<typename T>
constexpr bool NearEqual(const T& left, const T& right);

template<>
inline bool NearEqual<float>(const float& left, const float& right)
{
    constexpr double epsilon = 0.001f;
    return NearEqual(left, right, epsilon);
}

template<>
inline bool NearEqual<double>(const double& left, const double& right)
{
    constexpr double epsilon = 0.00001f;
    return NearEqual(left, right, epsilon);
}

template<typename T>
constexpr bool NearEqual(const T& left, const T& right)
{
    return left == right;
}

inline bool NearZero(const double value, const double epsilon)
{
    return NearEqual(value, 0.0, epsilon);
}

inline bool NearEqual(const double left, const double right)
{
    constexpr double epsilon = 0.001f;
    return NearEqual(left, right, epsilon);
}

inline bool NearZero(const double left)
{
    constexpr double epsilon = 0.001f;
    return NearZero(left, epsilon);
}

inline bool LessOrEqual(double left, double right)
{
    constexpr double epsilon = 0.001f;
    return (left - right) < epsilon;
}

inline bool LessOrEqualCustomPrecision(double left, double right, double epsilon = 0.000001f)
{
    return (left - right) < epsilon;
}

inline bool LessNotEqual(double left, double right)
{
    constexpr double epsilon = -0.001f;
    return (left - right) < epsilon;
}

inline bool LessNotEqualCustomPrecision(double left, double right, double epsilon = -0.000001f)
{
    return (left - right) < epsilon;
}

inline bool GreatOrEqual(double left, double right)
{
    constexpr double epsilon = -0.001f;
    return (left - right) > epsilon;
}

inline bool GreatOrEqualCustomPrecision(double left, double right, double epsilon = -0.000001f)
{
    return (left - right) > epsilon;
}

inline bool GreatNotEqual(double left, double right)
{
    constexpr double epsilon = 0.001f;
    return (left - right) > epsilon;
}

inline bool GreatNotEqualCustomPrecision(double left, double right, double epsilon = 0.000001f)
{
    return (left - right) > epsilon;
}

inline double Round(double rawNum)
{
    constexpr double epsilon = 0.001f;
    return std::round(rawNum + epsilon);
}

inline bool Negative(double value)
{
    return LessNotEqual(value, 0);
}

inline bool NonNegative(double value)
{
    return GreatOrEqual(value, 0);
}

inline bool Positive(double value)
{
    return GreatNotEqual(value, 0);
}

inline bool NonPositive(double value)
{
    return LessOrEqual(value, 0);
}

inline bool InRegion(double lowerBound, double upperBound, double destNum)
{
    return LessOrEqual(lowerBound, destNum) && LessOrEqual(destNum, upperBound);
}

inline bool GreaterOrEqualToInfinity(float num)
{
    return GreatOrEqual(num, Infinity<float>() / 2.0f);
}

inline uint64_t GetMilliseconds()
{
    auto now = std::chrono::system_clock::now();
    auto millisecs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return millisecs.count();
}

inline uint64_t GetNanoseconds()
{
    auto now = std::chrono::system_clock::now();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
    return nanoseconds.count();
}

inline float CalculateFriction(float gamma)
{
    constexpr float SCROLL_RATIO = 0.72f;
    if (GreatOrEqual(gamma, 1.0)) {
        gamma = 1.0;
    }
    return SCROLL_RATIO * static_cast<float>(std::pow(1.0 - gamma, 2));
}

} // namespace svg
} // namespace rnoh