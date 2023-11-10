#pragma once

static float remap(float value, float inputMin, float inputMax, float outputMin, float outputMax) 
{
    if (value < inputMin) value = inputMin;
    if (value > inputMax) value = inputMax;

    float normalizedValue = (value - inputMin) / (inputMax - inputMin);

    float remapValue = outputMin + normalizedValue * (outputMax - outputMin);

    return remapValue;

}
