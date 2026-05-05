#include <cmath>
#include <cstdio>
#include "chylatum.hpp"

point cross(const line & l1, const line & l2){
    return point{
        .x = (l2.c*l1.b - l1.c*l2.b) / (l1.a*l2.b - l2.a*l1.b),
        .y = (l2.c*l1.a - l1.c*l2.a) / (l1.b*l2.a - l2.b*l1.a)
    };
}

line rectOpLine(float xd, float R){
    return line{.a = R, .b = -(R+1), .c = xd};
}

line qLine(float xf, float q){
    return line{.a = q, .b = 1-q, .c = -xf};
}

line striOpLine(float xw, const line & rl, const line & ql){
    point ins = cross(rl, ql);
    return line{.a = ins.y, .b=-ins.x, .c=xw*(ins.x-ins.y)};
}

float minR(float xd, const line & ql, const curve & vle){
    const float& q = ql.a, xf = -ql.c, ap = vle.alpha;
    
    if (q == 0){
        const float x = vle.getX(xf), y = xf;
        return (xd - y) / (y - x);
    }

    const float a = (ap-1)*q, b = ap+(q+xf)*(1-ap), c = -xf;
    const float x = (std::sqrt(b*b-4*a*c) - b) / (2*a);
    const float y = vle.getY(x);
    return (xd - y) / (y - x);
}

result rectify(const line &ql, const line &rl, const curve &vle, int inf, bool noisy){
    if (rl.a < minR(rl.c, ql, vle))
        return result{.xn = 0, .n = 0};

    const float xe = cross(rl, ql).x;
    float xj = vle.getX(rl.c), yj; int i = 0;

    while (xj >= xe) {
        ++i;
        yj = cross(rl, line{.a = 1, .b = 0, .c = -xj}).y;
        xj = vle.getX(yj);
        if (noisy) 
            std::printf("\rRectifying: %-36d", i);
        if (i > inf)
            return result{.xn = 0, .n = 0};
    }
    if (noisy) 
        std::printf("\n");
    return result{.xn = xj, .n = i};
}

result strip(const float xn, const line &sl, const curve &vle, int inf, bool noisy){
    const float xw = cross(sl, line{.a = 1, .b = -1, .c = 0}).x;
    float xj = xn, yj; int i = 0;

    while (xj > xw) {
        ++i;
        yj = cross(sl, line{.a = 1, .b = 0, .c = -xj}).y;
        xj = vle.getX(yj);
        if (noisy)
            std::printf("\rStripping: %-37d", i);
    }
    if (noisy) 
        std::printf("\n");
    return result{.xn = xj, .n = i};
}
