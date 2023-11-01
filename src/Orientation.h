#pragma once
#include <Math.h>

inline Vector3 Forward(Matrix matrix)
{
    return { matrix.m8, matrix.m9, matrix.m10 };
}

inline Vector3 Right(Matrix matrix)
{
    return { matrix.m0, matrix.m1, matrix.m2 };
}

inline Vector3 Up(Matrix matrix)
{
    return { matrix.m4, matrix.m5, matrix.m6 };
}

inline Matrix Orientate(Vector3 forward, Vector3 up = { 0.0f, 1.0f, 0.0f })
{
    Vector3 right = Cross(forward, up);
    Vector3 above = Cross(forward, right);

    Matrix matrix = MatrixIdentity();
    matrix.m8 = forward.x;
    matrix.m9 = forward.y;
    matrix.m10 = forward.z;

    matrix.m0 = right.x;
    matrix.m1 = right.y;
    matrix.m2 = right.z;

    matrix.m4 = above.x;
    matrix.m5 = above.y;
    matrix.m6 = above.z;
    return matrix;
}