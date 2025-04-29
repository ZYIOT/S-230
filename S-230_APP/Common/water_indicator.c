#include "water_indicator.h"

#define WATER_INDICATOR_MAP_CASE(_name, _index, _scale, _offset, _min, _max) \
    case _index:                                                             \
        min = _min;                                                          \
        max = _max;                                                          \
        scale = _scale;                                                      \
        offset = _offset;                                                    \
        break;

//name,index, scale,offset,min,max
#define WATER_INDICATOR_MAP(F)                                          \
    F(Temperature, WATER_INDICATOR_INDEX_TEMPERATURE, 10, 0, 0, 200)    \
    F(DO, WATER_INDICATOR_INDEX_DO, 100, 0, 0, 100)                     \
    F(pH, WATER_INDICATOR_INDEX_PH, 100, 0, 0, 14)                      \
    F(Salinity, WATER_INDICATOR_INDEX_SALINITY, 10, 0, 0, 50)           \
    F(Chlorophyll, WATER_INDICATOR_INDEX_CHLOROPHYLL, 10, 0, 0, 2000)   \
    F(COD, WATER_INDICATOR_INDEX_COD, 10, 0, 0, 2000)                   \
    F(NH3, WATER_INDICATOR_INDEX_NH3, 10, 0, 0, 1000)                   \
    F(Level, WATER_INDICATOR_INDEX_LEVEL, 100, 0, 0, 50)

int water_indicator_to_uint16_t(uint8_t index, float f, uint16_t *u)
{
    float min = 0, max = 0, scale = 1, offset = 0;

    switch (index)
    {
        WATER_INDICATOR_MAP(WATER_INDICATOR_MAP_CASE)
    default:
        return -3; //J1939_PARAM_ERROR
    }
    if (f < min || f > max)
    {
        return -3;
    }
    *u = (uint16_t)((f + offset) * scale);
    return 0;
}

int water_indicator_from_uint16_t(uint8_t index, uint16_t u, float *f)
{
    float min = 0, max = 0, scale = 1, offset = 0;

    switch (index)
    {
        WATER_INDICATOR_MAP(WATER_INDICATOR_MAP_CASE)
    default:
        return -3;
    }
    *f = u / scale - offset;

    if (*f < min || *f > max)
    {
        return -3;
    }
    return 0;
}

#define CONCAT(a, b) a##b

#define WATER_INDICATOR_NAME_CASE(_name, _index, _scale, _offset, _min, _max) \
    case _index:                                                              \
        return CONCAT(_n_, _index);

#define WATER_INDICATOR_NAME_DEFINE(_name, _index, _scale, _offset, _min, _max) \
    static char *CONCAT(_n_, _index) = #_name;

WATER_INDICATOR_MAP(WATER_INDICATOR_NAME_DEFINE)
static char *_unknow = "Unkown";

char *water_indicator_name_by_id(uint8_t id)
{

    switch (id)
    {
        WATER_INDICATOR_MAP(WATER_INDICATOR_NAME_CASE)
    default:
        return _unknow;
    }
}
