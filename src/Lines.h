#pragma once
#include "Orientation.h"
#include "Mesh.h"
#include "Color.h"

inline Matrix TransformLine(Vector3 start, Vector3 end, float xScale = 0.01f, float yScale = 0.01f)
{
    float length = Length(end - start);         // scale
    Vector3 direction = Normalize(end - start); // rotation
    Vector3 midpoint = (start + end) * 0.5f;    // translation

    Matrix transform =
        Scale(xScale, yScale, length) *
        Orientate(direction) *
        Translate(midpoint.x, midpoint.y, midpoint.z);

    return transform;
}

inline void DrawLine(Vector3 start, Vector3 end, Matrix view, Matrix proj,
    const Mesh& cube, GLuint shader, Color color = {}, float xScale = 0.01f, float yScale = 0.01f)
{
    Matrix model = TransformLine(start, end, xScale, yScale);
    Matrix mvp = model * view * proj;
    GLint uColor = glGetUniformLocation(shader, "u_color");
    GLint uTransform = glGetUniformLocation(shader, "u_mvp");
    glUseProgram(shader);
    glUniform3f(uColor, color.r, color.g, color.b);
    glUniformMatrix4fv(uTransform, 1, GL_TRUE, &mvp.m0);
    glBindVertexArray(cube.vao);
    glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);
}