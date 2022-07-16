#include "runtime/function/framework/component/particle/flock_particle.h"

constexpr const float MaxSpeed       = 1.0f;
constexpr const float MinUrgency     = 0.05f;
constexpr const float MaxUrgency     = 0.1f;
constexpr const float MaxChange      = (MaxSpeed * MaxUrgency);
constexpr const float DesiredSpeed   = (MaxSpeed / 2);
constexpr const float KeepAwayDist   = 6.0f;
constexpr const float SeparationDist = 3.0f;

constexpr const int   MaxBoids                 = 200;
constexpr const int   MaxFlocks                = 4;
constexpr const int   Max_Friends_Visible      = 10;
constexpr const float Default_Perception_Range = 8.0f;

constexpr const bool UseTruth       = false;
constexpr const bool ReactToEnemies = true;

constexpr const float Gravity = 9.806650f;

Piccolo::RandomNumberGenerator flock_generator;

namespace Piccolo
{
    std::list<std::shared_ptr<Boid>>  Boid::s_visible_friends = {};
    int                               Flock::s_count          = 0;
    std::list<std::shared_ptr<Flock>> Flock::s_flocks         = {};

    Boid::Boid(int id, std::shared_ptr<Flock> flock, Vector3 min_pos, Vector3 max_pos) :
        m_id(id), m_min(min_pos), m_max(max_pos)
    {
        m_perception_range = Default_Perception_Range;

        // generate random position
        m_pos.x = flock_generator.normalDistribution(0, 1) * (min_pos + max_pos).x / 2 / 3;
        m_pos.y = flock_generator.normalDistribution(0, 1) * (min_pos + max_pos).y / 2 / 3;
        m_pos.z = flock_generator.normalDistribution(0, 1) * (min_pos + max_pos).z / 2 / 3;

        // flip positions for greater randomness
        if (flock_generator.normalDistribution(0, 1) > 0.5f)
            m_pos.x *= -1;
        if (flock_generator.normalDistribution(0, 1) > 0.5f)
            m_pos.y *= -1;
        if (flock_generator.normalDistribution(0, 1) > 0.5f)
            m_pos.z *= -1;

        // generate random velocity in x y plane
        m_vel.x = flock_generator.normalDistribution(0, 1);
        m_vel.z = flock_generator.normalDistribution(0, 1);
        // flip velocities for greater randomness
        if (flock_generator.normalDistribution(0, 1) > 0.5f)
            m_vel.x *= -1;
        if (flock_generator.normalDistribution(0, 1) > 0.5f)
            m_vel.z *= -1;

        // compute speed based on velocity
        m_speed = m_vel.length();

        // zero out orientation
        m_ang.x = m_ang.y = m_ang.z = 0;

        // init other values
        m_num_flockmates_seen       = 0;
        m_nearest_flockmate         = nullptr;
        m_dist_to_nearest_flockmate = std::numeric_limits<float>::max();

        m_num_enemies_seen      = 0;
        m_nearest_enemy         = nullptr;
        m_dist_to_nearest_enemy = std::numeric_limits<float>::max();

        m_next = m_prev = nullptr;
    }

    Boid::Boid(int                    id,
               const Vector3&         pos,
               const Vector3&         vel,
               const Vector3&         ang,
               std::shared_ptr<Flock> flock,
               Vector3                min_pos,
               Vector3                max_pos) :
        m_id(id),
        m_pos(), m_vel(vel), m_ang(ang), m_min(min_pos), m_max(max_pos)
    {
        m_perception_range = Default_Perception_Range;

        // compute speed based on velocity
        m_speed = m_vel.length();

        // zero out orientation
        m_ang.x = m_ang.y = m_ang.z = 0;

        // init other values
        m_num_flockmates_seen       = 0;
        m_nearest_flockmate         = nullptr;
        m_dist_to_nearest_flockmate = std::numeric_limits<float>::max();

        m_num_enemies_seen      = 0;
        m_nearest_enemy         = nullptr;
        m_dist_to_nearest_enemy = std::numeric_limits<float>::max();

        m_next = m_prev = nullptr;
    }

    Vector3 Boid::cruising()
    {
        Vector3 change = m_vel;

        float diff    = (m_speed - DesiredSpeed) / MaxSpeed;
        float urgency = (float)std::fabs(diff);

        // constrain the urgency level
        if (urgency < MinUrgency)
            urgency = MinUrgency;
        if (urgency > MaxUrgency)
            urgency = MaxUrgency;

        // Now add some "jitter" so that each boid has a
        // bit of randomness just to keep things interesting.
        // This will also get us moving if we happen to start
        // things standing perfectly still (which is sorta boring).
        float jitter = flock_generator.normalDistribution(0, 1);
        if (jitter < 0.45f)
            change.x += MinUrgency * PICCOLO_SIGN(diff);
        else if (jitter < 0.90f)
            change.z += MinUrgency * PICCOLO_SIGN(diff);
        else
            change.y += MinUrgency * PICCOLO_SIGN(diff); // we don't like vertical motion all that much

        // compute velocity change necessary to get to our desired cruising speed
        float change_length = (urgency * (diff > 0 ? -1 : 1));
        change.normalise();
        change = change * change_length;

        return change;
    }

    Vector3 Boid::fleeEnemies(void)
    {
        Vector3 change;

        // test:  Are we too close to our nearest enemy?
        if (m_dist_to_nearest_enemy < KeepAwayDist)
        {
            // yep...compute vector away from enemy
            change = m_pos - m_nearest_enemy->m_pos;
        }

        // return change vector
        return (change);
    }

    Vector3 Boid::keepDistance(void)
    {
        float ratio = m_dist_to_nearest_flockmate / SeparationDist;

        // compute vector towards our nearest buddy
        Vector3 change = m_nearest_flockmate->m_pos - m_pos;

        // constrain computed ratio to our min/max Urgency levels
        if (ratio < MinUrgency)
            ratio = MinUrgency;
        if (ratio > MaxUrgency)
            ratio = MaxUrgency;

        // test:  are we too close to our nearest flockmate?
        if (m_dist_to_nearest_flockmate < SeparationDist)
        {
            // too close...move away from our neighbor
            change.normalise();
            change = change * -ratio;
        }
        else if (m_dist_to_nearest_flockmate > SeparationDist)
        {
            // too far away...move towards our neighbor
            change.normalise();
            change = change * ratio;
        }
        else
        {
            // in the UNLIKELY event we're exactly the right distance away, do nothing
            change = Vector3::ZERO;
        }

        // return change vector
        return (change);
    }

    Vector3 Boid::matchHeading(void)
    {
        // copy the heading of our nearest buddy
        Vector3 change = m_nearest_flockmate->m_vel;

        // normalize and then scale our vector change a bit...after
        // all we can't instantly snap to a new heading, can we?
        change.normalise();
        change = change * MinUrgency;

        // return change vector
        return (change);
    }

    int Boid::seeEnemies(int flock_id)
    {
        float                 dist;
        std::shared_ptr<Boid> enemy;

        // initialize enemy data
        m_num_enemies_seen      = 0;
        m_nearest_enemy         = nullptr;
        m_dist_to_nearest_enemy = std::numeric_limits<float>::max();

        // loop over each flock and determine the closest one we can see
        // for (int i = 0; i < Flock::s_count; i++)
        for (auto flock : Flock::s_flocks)
        {
            // test:  if this is OUR flock, skip it
            if (flock->getId() == flock_id)
                continue;

            // not our flock, so check it out
            enemy = flock->getFirstMember();

            while (enemy != NULL)
            {
                // if this enemy is visible...
                if ((dist = canISee(enemy)) < std::numeric_limits<float>::max())
                {
                    // I can see it..increment counter
                    m_num_enemies_seen++;

                    // Test:  Closest enemy?
                    if (dist < m_dist_to_nearest_enemy)
                    {
                        // yes...save it off
                        m_dist_to_nearest_enemy = dist;
                        m_nearest_enemy         = enemy;
                    }
                }
                // get next enemy in flock
                enemy = enemy->getNext();
            }
        }
        return (m_num_enemies_seen);
    }

    int Boid::seeFriends(std::shared_ptr<Boid> first_boid)
    {
        float                 dist;
        std::shared_ptr<Boid> flockmate = first_boid;

        // clear the existing visibility list of any holdover from last round
        clearVisibleList();

        // now figure out who we can see
        while (flockmate != nullptr)
        {
            // Test:  Within sight of this boid?
            if ((dist = canISee(flockmate)) < std::numeric_limits<float>::max())
            {
                // add it to the list
                addToVisibleList(flockmate);

                // Test:  If this guy is closer than the current
                // closest, make him the current closest
                if (dist < m_dist_to_nearest_flockmate)
                {
                    m_dist_to_nearest_flockmate = dist;
                    m_nearest_flockmate         = flockmate;
                }
            }
            // next flockmate
            flockmate = flockmate->getNext();
        }
        return (m_num_flockmates_seen);
    }

    Vector3 Boid::steerToCenter(void)
    {
        Vector3 center, change;

        // walk down the visibility list and sum up their position vectors
        for (auto& flockmate : s_visible_friends)
        {
            if (flockmate != nullptr)
            {
                center += flockmate->m_pos;
            }
        }

        // average the positions to get the perceived center of the flock
        center /= m_num_flockmates_seen;

        // now that we have the perceived center, computer a vector towards it
        change = center - m_pos;

        // Scale the change vector so we don't turn on a dime..
        change.normalise();
        change = change * MinUrgency;

        // return change vector
        return (change);
    }

    float Boid::accumulateChanges(Vector3& accumulator, const Vector3& changes)
    {
        // apply the change
        accumulator += changes;
        return (accumulator.length());
    }

    void Boid::addToVisibleList(std::shared_ptr<Boid> ptr)
    {
        // test:  do we see enough buddies already?
        if (s_visible_friends.size() < Max_Friends_Visible)
        {
            s_visible_friends.emplace_back(ptr);
        }
    }

    void Boid::clearVisibleList(void)
    {
        // walk down the list and clear each visible entry
        s_visible_friends.clear();

        // clear other visibility info
        m_num_flockmates_seen       = 0;
        m_nearest_flockmate         = nullptr;
        m_dist_to_nearest_flockmate = std::numeric_limits<float>::max();
    }

    float Boid::canISee(std::shared_ptr<Boid> ptr)
    {
        // Test:  if we're looking at ourselves, never mind
        if (this == ptr.get())
            return std::numeric_limits<float>::max();

        // figure out distance
        Vector3 temp = m_pos - ptr->m_pos;
        float   dist = temp.length();

        // if we're using truth data, don't bother to check sighting
        if (UseTruth)
            return (dist);

        // not using truth, so check sighting
        if (m_perception_range > dist)
            return (dist);

        // fell through; can't see it
        return std::numeric_limits<float>::max();
    }

    void Boid::computeRPY(void)
    {
        float roll, pitch, yaw;

        // Determine the direction of the lateral acceleration.
        Vector3 lateralDir = (m_vel.crossProduct(m_vel - m_oldvel)).crossProduct(m_vel);
        lateralDir.normalise();

        // Set the lateral acceleration's magnitude. The magnitude is the vector
        // projection of the appliedAcceleration vector onto the direction of the
        // lateral acceleration).
        float lateralMag = (m_vel - m_oldvel).dotProduct(lateralDir);

        // compute roll
        if (lateralMag == 0)
            roll = 0.0f;
        else
            roll = (float)-atan2(Gravity, lateralMag) + MATH_PI_2;

        // compute pitch
        pitch = (float)-atan(m_vel.y / sqrt((m_vel.z * m_vel.z) + (m_vel.x * m_vel.x)));

        // compute yaw
        yaw = (float)atan2(m_vel.x, m_vel.z);

        // store them
        m_ang.x = pitch;
        m_ang.y = yaw;
        m_ang.z = roll;
    }

    void Boid::worldBound(void)
    {
        float maxX = m_max.x;
        float maxY = m_max.y;
        float maxZ = m_max.z;

        float minX = m_min.x;
        float minY = m_min.y;
        float minZ = m_min.z;

        // test position of flock boid and
        // 'warp' if they've strayed out of bounds
        if (m_pos.x > maxX)
            m_pos.x = minX;
        else if (m_pos.x < minX)
            m_pos.x = maxX;
        else if (m_pos.y > maxY)
            m_pos.y = minY;
        else if (m_pos.y < minY)
            m_pos.y = maxY;
        else if (m_pos.z > maxZ)
            m_pos.z = minZ;
        else if (m_pos.z < minZ)
            m_pos.z = maxZ;
    }

    std::shared_ptr<Boid> Boid::getNext(void) { return m_next; }

    void Boid::linkOut(void)
    {
        // test location of boid
        if ((m_next == NULL) && (m_prev == NULL))
        {
            setNext(NULL);
            setPrev(NULL);
        }
        else if (m_next == NULL)
        {
            m_prev->setNext(NULL);
        }
        else if (m_prev == NULL)
        {
            m_next->setPrev(NULL);
        }
        else
        {
            m_prev->setNext(m_next);
            m_next->setPrev(m_prev);
        }
    }

    void Boid::setNext(std::shared_ptr<Boid> ptr) { m_next = ptr; }

    void Boid::setPrev(std::shared_ptr<Boid> ptr) { m_prev = ptr; }

    void Boid::flockIt(int flock_id, std::shared_ptr<Boid> first_boid, float deltaTime)
    {
        Vector3 acc;

        // Step 1:  Update our position.
        // Update our position based on the velocity
        // vector we computed last time around.
        m_oldpos = m_pos;
        m_pos += 0.001 * Vector3::UNIT_Z;

        // Step 2:  SeeFriends.
        // Determine if we can see any of our flockmates.
        seeFriends(first_boid);

        // Step 3:  Flocking behavior.
        // Do we see any of our flockmates?  If yes, it's time to implement
        // the first Three Rules (they don't matter if we can't see anybody)
        if (m_num_flockmates_seen)
        {
            // Step 4:  Implement Rule #1 (Separation).
            // Try to maintain our desired separation distance from our nearest flockmate.
            accumulateChanges(acc, keepDistance());

            // Step 5:  Implement Rule #2 (Alignment).
            // Try to move the same way our nearest flockmate does.
            accumulateChanges(acc, matchHeading());

            // Step 6:  Implement Rule #3 (Cohesion).
            // Try to move towards the center of the flock.
            accumulateChanges(acc, steerToCenter());
        }

        // Step 6:  The Fourth Rule (enemies)
        // If we're supposed to react to enemy flocks, determine
        // if there are any then avoid them if we can.
        if (ReactToEnemies)
        {
            seeEnemies(flock_id);
            accumulateChanges(acc, fleeEnemies());
        }

        // Step 7:  Cruising.
        // Flockmates or not, enemies or not, figure out
        // how fast we'd move if it were all up to us.
        accumulateChanges(acc, cruising());

        // Step 8:  Constrain acceleration
        // If our acceleration change is more than we allow, constrain it
        if (acc.length() > MaxChange)
        {
            // definitely too much...constrain to maximum change
            acc.normalise();
            acc = acc * MaxChange;
        }

        // Step 9:  Implementation.
        // Here's where we apply our newly computed acceleration vector
        // to create a new velocity vector to use next update cycle.
        m_oldvel = m_vel; // save off our previous velocity

        // now add in the acceleration
        m_vel += acc;

        // Step 10:  constraint Y velocity changes.
        // Attempt to restrict flight straight up/down by damping out Y axis velocity.
        // This isn't strictly necessary, but does lead to more realistic looking flight.
        m_vel.y *= MaxUrgency;

        // Step 11:  Constrain our speed.
        // If we're moving faster than we're allowed to move, constrain our velocity.
        if ((m_speed = m_vel.length()) > MaxSpeed)
        {
            m_vel.normalise();
            m_vel   = m_vel * MaxSpeed;
            m_speed = MaxSpeed;
        }

        // Step 12:  Compute roll/pitch/yaw.
        // Compute our orientation after all this speed adjustment nonsense.
        computeRPY();

        // Step 13:  World boundaries.
        // If we've wandered outside the world bounds put us back in.  You might
        // not need to do this for your world, but that depends on your implementation.
        worldBound();
    }

    Flock::Flock()
    {
        m_id = s_count;
        s_count++;
    }

    Flock::~Flock()
    {
        m_id = 0;
        s_count--;
    }

    void Flock::tick(float delta_time)
    {
        for (auto boid : m_boids)
        {
            boid->flockIt(m_id, m_boids.front(), delta_time);
        }
    }

    void Flock::addTo(std::shared_ptr<Boid> boid) { m_boids.push_front(boid); }

    void Flock::removeFrom(std::shared_ptr<Boid> boid) { m_boids.remove(boid); }
} // namespace Piccolo
