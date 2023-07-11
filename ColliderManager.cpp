//
// Created by sarah on 7/11/23.
//

#include "ColliderManager.h"

void ColliderManager::rect_collision(SDL_Rect src, glm::vec2 &src_vel, SDL_Rect dest, Uint64 delta_time)
{
    glm::vec2 src_pos = {src.x + (src.w / 2), src.y + (src.h / 2)};

    glm::vec2 src_vel_scaled = src_vel * (float)delta_time / 1000.0f;

    SDL_Rect dest_scaled = dest;

    dest_scaled.w += src.w;
    dest_scaled.h += src.h;
    dest_scaled.x -= src.w / 2;
    dest_scaled.y -= src.h / 2;


    glm::vec2 contact_point;
    glm::vec2 contact_normal;
    float t_hit_near = 0.0f;

    if (ray_vs_rect(src_pos, src_vel_scaled, dest_scaled, contact_point, contact_normal, t_hit_near))
    {
        if ((t_hit_near >= 0.0f && t_hit_near < 1.0f))
            resolve_collision(src_vel, contact_normal, t_hit_near);
    }

}

bool ColliderManager::ray_vs_rect(glm::vec2 ray_origin, glm::vec2 ray_dir, SDL_Rect rect, glm::vec2 &contact_point,
                                  glm::vec2 &contact_normal, float &t_hit_near)
{
    glm::vec2 rec_pos = {rect.x, rect.y};
    glm::vec2 rec_dim = {rect.w, rect.h};

    glm::vec2 t_near = (rec_pos - ray_origin) / ray_dir;
    glm::vec2 t_far = (rec_pos + rec_dim - ray_origin) / ray_dir;

    if (t_near.x > t_far.x)
    {
        std::swap(t_near.x, t_far.x);
    }

    if (t_near.y > t_far.y)
    {
        std::swap(t_near.y, t_far.y);
    }

    if (t_near.x > t_far.y || t_near.y > t_far.x)
    {
        return false;
    }

    t_hit_near = std::max(t_near.x, t_near.y);

    float t_hit_far = std::min(t_far.x, t_far.y);

    if (t_hit_far < 0)
    {
        return false;
    }

    contact_point = ray_origin + t_hit_near * ray_dir;

    if (t_near.x > t_near.y)
    {
        contact_normal = ray_dir.x < 0 ? glm::vec2{1, 0} : glm::vec2{-1, 0};
    }
    else if (t_near.x < t_near.y)
    {
        contact_normal = ray_dir.y < 0 ? glm::vec2{0, 1} : glm::vec2{0, -1};
    }

    return true;
}

void ColliderManager::resolve_collision(glm::vec2 &vel, glm::vec2 contact_normal, float t_hit_near)
{
    glm::vec2 abs_vel = {std::abs(vel.x), std::abs(vel.y)};
    vel += contact_normal * abs_vel * (1.0f - t_hit_near);
}