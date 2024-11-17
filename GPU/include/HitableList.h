#ifndef RT_HITABLELIST_H
#define RT_HITABLELIST_H

#include "Hitable.h"

namespace RT
{

// this iheritance is so color() function could take in either a single element or a list...
class HitableList: public Hitable 
{
public:
    HitableList() = default;
    __device__ HitableList(Hitable **list, const int size): list_(list), list_size_(size) {}
    __device__ virtual bool hit(const Ray &ray, const float t_min, const float t_max, HitRecord &record) const;

private:
    Hitable **list_;
    int list_size_;
};


} // namespace RT


#endif  // RT_HITABLELIST_H