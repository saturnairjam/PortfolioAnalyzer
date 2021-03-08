#include <cmath> // for std::pow(), std::fabs()

#include "Xirr.hpp"

float paymentDurationInYears(Date paymentDate, Date startDate)
{
    auto paymentDateOffsetInMonths = (paymentDate.Year * 12) + (paymentDate.Month + 1);

    auto startDateOffsetInMonths = (startDate.Year * 12) + (startDate.Month + 1);

    return (paymentDateOffsetInMonths - startDateOffsetInMonths) / 12.0f;
}

float fXirr(const std::pair<Date, float>& payment, Date startDate, float x)
{
    auto paymentDate = payment.first;

    auto paymentValue = payment.second;

    auto paymentDuration = paymentDurationInYears(paymentDate, startDate);

    return paymentValue * std::pow(1 + x, -paymentDuration);
}

float dfXirr(const std::pair<Date, float>& payment, Date startDate, float x)
{
    auto paymentDate = payment.first;

    auto paymentValue = payment.second;

    auto paymentDuration = paymentDurationInYears(paymentDate, startDate);

    return (-paymentDuration) * paymentValue * std::pow(1 + x, -(paymentDuration + 1));
}

float totalFXirr(const std::vector<std::pair<Date, float>>& payments, float x)
{
    float result = 0.0f;

    auto startDate = payments[0].first;

    for (auto& payment : payments)
    {
        result += fXirr(payment, startDate, x);
    }

    return result;
}

float totalDfXirr(const std::vector<std::pair<Date, float>>& payments, float x)
{
    float result = 0.0f;

    auto startDate = payments[0].first;

    for (auto& payment : payments)
    {
        result += dfXirr(payment, startDate, x);
    }

    return result;
}

float Xirr(const std::vector<std::pair<Date, float>>& payments)
{
    float x0 = 0.1f; // initial guess

    float maxError = 1e-3f; // accurate up to 0.01 %

    int maxIterations = 100;

    for (int iteration = 0; iteration < maxIterations; iteration++)
    {
        float x1 = x0 - (totalFXirr(payments, x0) / totalDfXirr(payments, x0));

        float error = std::fabs(x1 - x0);

        x0 = x1;

        if (error <= maxError)
        {
            return x0;
        }
    }

    return nanf(""); // not-a-number
}