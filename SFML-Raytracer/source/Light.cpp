#include "..\include\Light.h"

Light::Light(Hittable* staticObjects, Hittable* dynamicObjects, AA::Vec3 pos, sf::Color lightColour, double intensityMod, bool debugRender)
    : _statics(staticObjects), _dynamics(dynamicObjects), _position(pos), _debugRender(debugRender), _intensityMod(intensityMod)
{
    SetLightColour(lightColour);
}

Light::~Light()
{
}

void Light::CalculateLighting(const AA::Ray& inRay, Hittable::HitResult& res, const bool& isRecursive)
{
    Hittable::HitResult staticRes, dynamicRes;
    bool staticHit = false;
    bool dynamicHit = false;

    //Create the collision point and material calc as they will be used more than once, set up the other vars for later use
    AA::Vec3 collisionPoint = res.p;
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));

    //Adjust outray to match the new position its sampled to and shift it slightly along its normal
    outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

    //Otherwise move onto the visibility check
    //Calc the distance from hit to light
    double dist = collisionPoint.Distance(_position);

    //Check against a hit with both static and dynamics
    staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, dist, staticRes);
    dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, dist, dynamicRes);


    if (staticHit || dynamicHit)
    {
        res.col = _shadowColour;
    }
}

AA::Vec3 Light::CalculateLightingForMaterial(const AA::Ray& inRay, const Hittable::HitResult& res)
{
    Hittable::HitResult staticRes, dynamicRes;
    bool staticHit = false;
    bool dynamicHit = false;

    //Create the collision point and material calc as they will be used more than once, set up the other vars for later use
    AA::Vec3 collisionPoint = res.p;
    AA::Ray outRay = AA::Ray(collisionPoint, AA::Vec3::UnitVector(_position - collisionPoint));

    //Adjust outray to match the new position its sampled to and shift it slightly along its normal
    outRay._startPos = outRay.GetPointAlongRay(AA::kEpsilon);

    //Otherwise move onto the visibility check
    //Calc the distance from hit to light
    double dist = collisionPoint.Distance(_position);

    //Check against a hit with both static and dynamics
    staticHit = _statics == nullptr ? false : _statics->IntersectedRayOnly(outRay, 0.0, dist, staticRes);
    dynamicHit = _dynamics == nullptr ? false : _dynamics->IntersectedRayOnly(outRay, 0.0, dist, dynamicRes);

    return staticHit || dynamicHit ? AA::colToVec3(_shadowColour) : AA::colToVec3(res.col);
}

bool Light::IntersectedRay(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
    AA::Vec3 oc = ray._startPos - _position;
    double a = ray._dir.DotProduct(ray._dir);
    double b = oc.DotProduct(ray._dir);
    double c = oc.DotProduct(oc) - _sphereRadius * _sphereRadius;

    double discrim = b * b - a * c;
    if (discrim > 0)
    {
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            res.col = _lightColour;
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            res.col = _lightColour;
            return true;
        }
    }

    return false;
}

bool Light::IntersectedRayOnly(const AA::Ray& ray, double t_min, double t_max, HitResult& res)
{
    AA::Vec3 oc = ray._startPos - _position;
    double a = ray._dir.DotProduct(ray._dir);
    double b = oc.DotProduct(ray._dir);
    double c = oc.DotProduct(oc) - _sphereRadius * _sphereRadius;

    double discrim = b * b - a * c;
    if (discrim > 0)
    {
        double temp = (-b - sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            return true;
        }

        temp = (-b + sqrt(discrim)) / a;
        if (temp < t_max && temp > t_min)
        {
            res.t = temp;
            res.p = ray.GetPointAlongRay(res.t);
            res.normal = (res.p - _position) / _sphereRadius;
            return true;
        }
    }

    return false;
}
bool Light::BoundingBox(double t0, double t1, AABB& outBox) const
{
    return false;
}

void Light::Move(AA::Vec3 newPos)
{
    if (_isStatic)
    {
        return;
    }
    _position = newPos;
}

void Light::Scale(AA::Vec3 newScale)
{
}
