#pragma once

#include "../DLLDefs.h"
#include <functional>
#include <string>

namespace glib
{
    namespace Easing
    {
        typedef std::function<float(float)> EasingFunc;

        GLIB_API EasingFunc StringToEasing(const std::string& name);

        GLIB_API float _linear(float x);
#define GLIB_E_LINEAR glib::Easing::_linear

        GLIB_API float _quadraticIn(float x);
#define GLIB_E_QUAD_IN glib::Easing::_quadraticIn

        GLIB_API float _quadraticOut(float x);
#define GLIB_E_QUAD_OUT glib::Easing::_quadraticOut

        GLIB_API float _quadraticInOut(float x);
#define GLIB_E_QUAD_INOUT glib::Easing::_quadraticInOut

        GLIB_API float _cubicIn(float x);
#define GLIB_E_CUBIC_IN glib::Easing::_cubicIn

        GLIB_API float _cubicOut(float x);
#define GLIB_E_CUBIC_OUT glib::Easing::_cubicOut

        GLIB_API float _cubicInOut(float x);
#define GLIB_E_CUBIC_INOUT glib::Easing::_cubicInOut

        GLIB_API float _quartIn(float x);
#define GLIB_E_QUART_IN glib::Easing::_quartIn

        GLIB_API float _quartOut(float x);
#define GLIB_E_QUART_OUT glib::Easing::_quartOut

        GLIB_API float _quartInOut(float x);
#define GLIB_E_QUART_INOUT glib::Easing::_quartInOut

        GLIB_API float _quintIn(float x);
#define GLIB_E_QUINT_IN glib::Easing::_quintIn

        GLIB_API float _quintOut(float x);
#define GLIB_E_QUINT_OUT glib::Easing::_quintOut

        GLIB_API float _quintInOut(float x);
#define GLIB_E_QUINT_INOUT glib::Easing::_quintInOut

        GLIB_API float _sinIn(float x);
#define GLIB_E_SIN_IN glib::Easing::_sinIn

        GLIB_API float _sinOut(float x);
#define GLIB_E_SIN_OUT glib::Easing::_sinOut

        GLIB_API float _sinInOut(float x);
#define GLIB_E_SIN_INOUT glib::Easing::_sinInOut

        GLIB_API float _expoIn(float x);
#define GLIB_E_EXPO_IN glib::Easing::_expoIn

        GLIB_API float _expoOut(float x);
#define GLIB_E_EXPO_OUT glib::Easing::_expoOut

        GLIB_API float _expoInOut(float x);
#define GLIB_E_EXPO_INOUT glib::Easing::_expoInOut

        GLIB_API float _circIn(float x);
#define GLIB_E_CIRC_IN glib::Easing::_circIn

        GLIB_API float _circOut(float x);
#define GLIB_E_CIRC_OUT glib::Easing::_circOut

        GLIB_API float _circInOut(float x);
#define GLIB_E_CIRC_INOUT glib::Easing::_circInOut

        GLIB_API float _bounceIn(float x);
#define GLIB_E_BOUNCE_IN glib::Easing::_bounceIn

        GLIB_API float _bounceOut(float x);
#define GLIB_E_BOUNCE_OUT glib::Easing::_bounceOut

        GLIB_API float _bounceInOut(float x);
#define GLIB_E_BOUNCE_INOUT glib::Easing::_bounceInOut

        GLIB_API float _elasticIn(float x);
#define GLIB_E_ELASTIC_IN glib::Easing::_elasticIn

        GLIB_API float _elasticOut(float x);
#define GLIB_E_ELASTIC_OUT glib::Easing::_elasticOut

        GLIB_API float _elasticInOut(float x);
#define GLIB_E_ELASTIC_INOUT glib::Easing::_elasticInOut

        GLIB_API float _backIn(float x);
#define GLIB_E_BACK_IN glib::Easing::_backIn

        GLIB_API float _backOut(float x);
#define GLIB_E_BACK_OUT glib::Easing::_backOut

        GLIB_API float _backInOut(float x);
#define GLIB_E_BACK_INOUT glib::Easing::_backInOut

    }
}