/*
 * Ray Tracer @ https://github.com/yiyaowen/RayTracer
 *
 * yiyaowen 2021 (c) All Rights Reserved.
 *
 * Also see tutorial: Ray Tracing in One Weekend by Peter Shirley.
 * Web URL: https://raytracing.github.io/books/RayTracingInOneWeekend.html
*/

#ifndef SHAPE_H
#define SHAPE_H

#include "RayTracer/RayTracer.h"

class Material;

struct HitResult {
    Vector3d position = {};
    bool isOuter = true;
    Vector3d normal = {};
    double t = 0.0f;
    std::shared_ptr<Material> material = nullptr;
};

class Shape : public std::enable_shared_from_this<Shape> {
public:
    Shape() = default;
    Shape(const std::string& label, std::shared_ptr<Material> material = nullptr, uint32_t priority = 0)
        : m_label(label), m_priority(priority), m_material(material) {}

public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitResult& result) const = 0;

    bool hitAll(const Ray& r, double t_min, double t_max, HitResult& result) const {
        if (hit(r, t_min, t_max, result)) {
            return true;
        }
        else {
            for (const auto& child : m_children) {
                if (!child.expired() && child.lock()->hitAll(r, t_min, t_max, result)) {
                    return true;
                }
            }
        }
        return false;
    }

    std::vector<std::shared_ptr<Shape>> generatePriorityList(const std::optional<bool>& isAscending) {
        std::vector<std::shared_ptr<Shape>> list;
        list.push_back(shared_from_this());
        for (const auto& child : m_children) {
            if (!child.expired()) {
                auto sublist = child.lock()->generatePriorityList(isAscending);
                list.insert(list.end(), sublist.begin(), sublist.end());
            }
        }

        std::function<bool(const std::shared_ptr<Shape>&, const std::shared_ptr<Shape>&)> func = {};
        if (isAscending.has_value()) {
            if (isAscending.value()) {
                func = [&](const std::shared_ptr<Shape>& p1, const std::shared_ptr<Shape>& p2) {
                    return p1->priority() > p2->priority();
                };
            }
            else {
                func = [&](const std::shared_ptr<Shape>& p1, const std::shared_ptr<Shape>& p2) {
                    return p1->priority() < p2->priority();
                };
            }
            std::sort(list.begin(), list.end(), func);
        }

        return list;
    }

public:
    inline std::string label() const { return m_label; }
    inline void setLabel(const std::string& value) { m_label = label(); }

    inline uint32_t priority() const { return m_priority; }
    inline void setPriority(uint32_t value) { m_priority = value; }

    inline std::weak_ptr<Shape> parent() const { return m_parent; }

    inline void setParent(std::shared_ptr<Shape> parent) {
        if (!m_parent.expired()) {
            m_parent.lock()->removeChild(shared_from_this());
        }
        m_parent = parent;
    }

    inline std::vector<std::weak_ptr<Shape>> children() const { return m_children; }

    inline void addChild(std::shared_ptr<Shape> child) {
        m_children.push_back(child);
    }

    inline friend void bindShapes(std::shared_ptr<Shape> parent, std::shared_ptr<Shape> child) {
        parent->addChild(child);
        child->setParent(parent);
    }

    void removeChild(std::shared_ptr<Shape> ptr) {
        m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
                       [&](const std::weak_ptr<Shape>& p) {
            if (!p.expired()) {
                return p.lock() == ptr;
            }
            return true; // Always remove if expired.
        }), m_children.end());
    }

    void removeAllChildrenWithLabel(const std::string& label) {
        m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
                                        [&](const std::weak_ptr<Shape>& p) {
            if (!p.expired()) {
                return p.lock()->label() ==  label;
            }
            return true; // Always remove if expired.
        }), m_children.end());
    }

    inline std::shared_ptr<Material> material() const { return m_material; }
    inline void setMaterial(std::shared_ptr<Material> value) { m_material = value; }

protected:
    std::string m_label = {};

    uint32_t m_priority = 0;

    std::weak_ptr<Shape> m_parent = {}; // Weak ptr to avoid circular reference.

    std::vector<std::weak_ptr<Shape>> m_children = {};

    std::shared_ptr<Material> m_material = nullptr;
};

#endif // SHAPE_H