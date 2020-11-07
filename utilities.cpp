#include "utilities.h"

using namespace std;

int random_int(int min, int max) {
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

float random_float(float min, float max) {
    random_device device;
    mt19937 generator(device());
    uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

bool random_bool() {
    random_device device;
    mt19937 generator(device());
    uniform_int_distribution<int> distribution(0, 1);
    return distribution(generator);
}