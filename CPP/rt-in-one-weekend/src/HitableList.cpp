#include "HitableList.h"

namespace RT
{

bool HitableList::hit(const Ray &ray, const float t_min, const float t_max, HitRecord &record) const
{
    HitRecord temp_record;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size_; i++)
    {
        if (list_[i]->hit(ray, t_min, closest_so_far, temp_record))
        {
            hit_anything = true;
            closest_so_far = temp_record.t;
            record = temp_record;
        }
    }
    return hit_anything;
}

} // namespace RT
