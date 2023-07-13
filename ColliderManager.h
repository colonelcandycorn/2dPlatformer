//
// Created by sarah on 7/11/23.
//

#ifndef COLLIDERMANAGER_H
#define COLLIDERMANAGER_H

#include <SDL2/SDL_rect.h>
#include <glm/glm.hpp>


class ColliderManager {
public:
    static bool rect_collision(SDL_Rect src, glm::vec2& src_vel, SDL_Rect dest, Uint64 delta_time, float& time_to_collision, glm::vec2& contact_normal);
    static bool ray_vs_rect(glm::vec2 ray_origin, glm::vec2 ray_dir, SDL_Rect rect, glm::vec2 &contact_point, glm::vec2 &contact_normal, float &t_hit_near);
    static void resolve_collision(glm::vec2 &vel, glm::vec2 contact_normal, float t_hit_near);

    ColliderManager()=default;

};


#endif //COLLIDERMANAGER_H
