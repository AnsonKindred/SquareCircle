#ifndef Vector2F_HEADER
#define Vector2F_HEADER
#include "Base.h"

template<typename T> struct triple
{
    triple(const T &x, const T &y, const T &z):
        x(x),
        y(y),
        z(z)
    {
    }

    triple():
        x(0),
        y(0),
        z(0)
    {
    }

    T x;
    T y;
    T z;
};

class VectorUtil
{

public:

    template <class T>
    static void normalize3(triple<T>* v)
    {
        float total = mag(v);
		if(total != 0)
		{
			v->x /= total;
			v->y /= total;
            v->z /= total;
		}
    }

    template <class T>
    static void crossProd3(const triple<T>& v1, const triple<T>& v2, triple<T>* result)
    {
        result->x = v1->x*v2->z - v1->z*v2->x;
        result->y = v1->y*v2->x - v1->x*v2->y;
        result->z = v1->z*v2->y - v1->y*v2->z;
    }

    template <class T>
    static float angleBetween3(const triple<T>& v1, const triple<T>& v2)
    {
        float dot = VectorUtil::dot(v1, v2);
        float v1Mag = VectorUtil::mag(v1);
        float v2Mag = VectorUtil::mag(v2);
        return (float) acos( dot / (v1Mag * v2Mag) );
    }

    template <class T>
    static float dot3(const triple<T>& v1, const triple<T>& v2)
    {
        return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
    }

    template <class T>
    static float mag3(const triple<T>& v)
	{
		return sqrt(pow(v->x, 2)+pow(v->y, 2)+pow(v->z, 2));
	}

    template <class T>
	static void scaleTo3(triple<T>* v, float mag)
	{
		if(mag != 0)
		{
            float total = VectorUtil::mag(triple);
			if(total != 0)
			{
				total *= mag;
				v->x /= total;
				v->y /= total;
                v->z /= total;
			}
		}
	}

    template <class T>
	static void mult3(triple<T>* v, float vm)
	{
		v->x *= vm;
		v->y *= vm;
        v->z *= vm;
	}

    //--

};
#endif
