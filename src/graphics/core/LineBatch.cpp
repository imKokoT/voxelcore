#include "LineBatch.hpp"
#include "Mesh.hpp"

#include <GL/glew.h>


LineBatch::LineBatch(size_t capacity) : capacity(capacity) {

    buffer = std::make_unique<LineVertex[]>(capacity * 2);
    mesh = std::make_unique<Mesh<LineVertex>>(buffer.get(), 0);
    index = 0;
}

LineBatch::~LineBatch(){
}

void LineBatch::line(
    float x1, float y1,
    float z1, float x2,
    float y2, float z2,
    float r, float g, float b, float a
) {
    if (index + 2 >= capacity) {
        flush();
    }
    buffer[index].position = {x1,y1,z1};
    buffer[index].color = {r,g,b,a};
    index++;

    buffer[index].position = {x2,y2,z2};
    buffer[index].color = {r,g,b,a};
    index++;
}

void LineBatch::box(float x, float y, float z, float w, float h, float d,
        float r, float g, float b, float a) {
    w *= 0.5f;
    h *= 0.5f;
    d *= 0.5f;

    line(x-w, y-h, z-d, x+w, y-h, z-d, r,g,b,a);
    line(x-w, y+h, z-d, x+w, y+h, z-d, r,g,b,a);
    line(x-w, y-h, z+d, x+w, y-h, z+d, r,g,b,a);
    line(x-w, y+h, z+d, x+w, y+h, z+d, r,g,b,a);

    line(x-w, y-h, z-d, x-w, y+h, z-d, r,g,b,a);
    line(x+w, y-h, z-d, x+w, y+h, z-d, r,g,b,a);
    line(x-w, y-h, z+d, x-w, y+h, z+d, r,g,b,a);
    line(x+w, y-h, z+d, x+w, y+h, z+d, r,g,b,a);

    line(x-w, y-h, z-d, x-w, y-h, z+d, r,g,b,a);
    line(x+w, y-h, z-d, x+w, y-h, z+d, r,g,b,a);
    line(x-w, y+h, z-d, x-w, y+h, z+d, r,g,b,a);
    line(x+w, y+h, z-d, x+w, y+h, z+d, r,g,b,a);
}

void LineBatch::flush(){
    if (index == 0)
        return;
    mesh->reload(buffer.get(), index);
    mesh->draw(GL_LINES);
    index = 0;
}

void LineBatch::lineWidth(float width) {
    // Wide lines have been deprecated for glLineWidth and generates GL:DEPRECATED_BEHAVIOR:MEDIUM
    // for OpenGL 3.2+, so a little condition to disable wide lines. Besides, some GPU/drivers clamp
    // line width to 1.0
    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (width > 1.0 && major >= 3 && minor >= 2)
        width = 1.0;

    glLineWidth(width);
}
