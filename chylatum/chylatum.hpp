#ifndef CHYLATUM_H
#define CHYLATUM_H

struct point {
    const float x, y;
};
struct line {
    const float a, b, c;
};

point cross(const line&, const line&);

struct curve {
    const float alpha;
    inline float getX(float y) const {
        return y / (alpha - (alpha-1)*y);
    }
    inline float getY(float x) const {
        return alpha * x / (1 + (alpha-1)*x);
    }
};

struct result {
    const float xn;
    const int n;
};

line rectOpLine(float xd, float R);
line qLine(float xf, float q);
line striOpLine(float xw, const line& rl, const line& ql);
float minR(float xd, const line& ql, const curve& vle);

result rectify(const line& ql, const line& rl, const curve& vle, int inf, bool noisy);
result strip(const float xn, const line& sl, const curve& vle, int inf, bool noisy);

#endif
