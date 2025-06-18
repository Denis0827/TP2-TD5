#ifndef SAVING_H
#define SAVING_H
#include <vector>

struct Saving {
    int i, j;
    double saving;
    bool operator<(const Saving& other) const {
        return saving > other.saving; // Ordena de mayor a menor
    }
};

#endif // SAVING_H