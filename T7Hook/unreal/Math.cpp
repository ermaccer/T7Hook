#include "Math.h"

FRotator FindLookAtRotation(FVector* start, FVector* target)
{
    FVector out;
    out.X = target->X - start->X;
    out.Y = target->Y - start->Y;
    out.Z = target->Z - start->Z;
    FMatrix mat;
    mat.MakeFromX(&out);
    FRotator result;
    mat.Rotator(&result);
    return result;
}
