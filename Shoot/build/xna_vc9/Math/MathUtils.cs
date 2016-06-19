/* 

Amine Rehioui
Created: October 31st 2011

*/

using System;
using Microsoft.Xna.Framework;

namespace shoot
{
    //! Math utilities
	public class MathUtils
	{
        //! compare a floating point with zero
        public static bool FIsZero(float fValue)
        {
            return Math.Abs(fValue) < float.Epsilon;
        }

        //! swaps two values		
        public static void Swap<T>(ref T value1, ref T value2)
		{
			T temp = value1;
			value1 = value2;
			value2 = temp;
		}

        //! linear interpolation		
        public static float Lerp(float a, float b, float k)
		{
			return (b - a)*k + a;
		}       
	}
}

