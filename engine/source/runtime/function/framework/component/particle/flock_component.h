#pragma once
#include "runtime/function/framework/component/component.h"
#include "runtime/function/render/render_object.h"
#include "runtime/resource/res_type/components/flock.h"
#include "runtime/resource/res_type/components/mesh.h"

#include <list>
#include <vector>

namespace Piccolo
{
    class Flock;

    class Boid
    {
    public:
        Boid(int id, std::shared_ptr<Flock> flock, Vector3 min_pos, Vector3 max_pos, MeshComponentRes& mesh);
        ~Boid();

        void flockIt(int flock_id, std::shared_ptr<Boid> first_boid, float deltaTime);

        void addToVisibleList(std::shared_ptr<Boid> ptr);
        void clearVisibleList(void);

        std::shared_ptr<Boid> getNext(void);
        void                  linkOut(void);
        void                  setNext(std::shared_ptr<Boid> ptr);
        void                  setPrev(std::shared_ptr<Boid> ptr);

    private:
        // Cruising.
        // Generates a vector indicating how a flock boid would
        // like to move, if it were all up to him and he was under
        // no other influences of any kind.
        Vector3 cruising(void);

        // FleeEnemies.
        // Generates a vector for a flock boid to avoid the
        // nearest enemy (boid of a different flock) it sees.
        Vector3 fleeEnemies(void);

        // KeepDistance.
        // Generates a vector for a flock boid to maintain his
        // desired separation distance from the nearest flockmate he sees.
        Vector3 keepDistance(void);

        // MatchHeading.
        // Generates a vector for a flock boid to try
        // to match the heading of its nearest flockmate.
        Vector3 matchHeading(void);

        // SeeEnemies.
        // Determines which enemy flock boids a given flock boid can see.
        int seeEnemies(int flock_id);

        // SeeFriends.
        // Determines which flockmates a given flock boid can see.
        int seeFriends(std::shared_ptr<Boid> first_boid);

        // SteerToCenter.
        // Generates a vector to guide a flock boid towards
        // the "center of mass" of the flockmates he can see.
        Vector3 steerToCenter(void);

        // WorldBound.
        // Implements a world boundary so that flocks don't fly
        // infinitely far away from the camera, instead remaining
        // in a nice viewable area.  It does this by wrapping flock
        // boids around to the other side of the world, so (for example)
        // they move out the right and return on the left.
        void worldBound(void);

        //////////////////////////
        // miscellaneous functions
        //////////////////////////

        // AccumulateChanges.
        // Adds vector values in changes into the accumumlator vector.
        // Returns magnitude of accumulator vector after adding changes.
        float accumulateChanges(Vector3& accumulator, const Vector3& changes);

        // CanISee.
        // Determine whether a given invoking boid can see the boid in question.
        // Returns the distance to the boid.
        float canISee(std::shared_ptr<Boid> ptr);

        // ComputeRPY.
        // Computes the roll/pitch/yaw of the flock boid based on its
        // latest velocity vector changes.  Roll/pitch/yaw are stored in
        // the "ang" data member as follows:
        //   pitch is about the x axis
        //   yaw is about the y axis
        //   roll is about the z axis
        // All calculations assume a right-handed coordinate system:
        //   +x = through the left side of the object
        //   +y = up
        //   +z = through the nose of the model
        void computeRPY(void);

    private:
        static std::list<std::shared_ptr<Boid>> m_visible_friends;

        int m_id;

        Vector3 m_pos; // position of member (in meters)
        Vector3 m_vel; // velocity of member (meters/sec)
        Vector3 m_ang; // orientation of member

        Vector3 m_min;
        Vector3 m_max;

        float m_perception_range;
        float m_speed;
        int   m_num_flockmates_seen; // # of flockmates this member sees
        int   m_num_enemies_seen;    // # of enemies this member sees

        std::shared_ptr<Boid> m_nearest_flockmate; // pointer to nearest flockmate (if any)
        std::shared_ptr<Boid> m_nearest_enemy;     // pointer to nearest enemy (if any)

        float m_dist_to_nearest_flockmate; // distance to nearest flockmate
                                           // (if any), in meters

        float m_dist_to_nearest_enemy; // distance to nearest enemy
                                       // (if any), in meters

        Vector3 m_oldpos; // last position
        Vector3 m_oldvel; // last velocity

        std::shared_ptr<Boid> m_next; // pointer to next flockmate
        std::shared_ptr<Boid> m_prev; // pointer to previous flockmate

        MeshComponentRes& m_mesh;
    };

    class Flock
    {
    public:
        Flock();
        ~Flock();

        void tick(float delta_time);

        void addTo(std::shared_ptr<Boid> boid);
        void removeFrom(std::shared_ptr<Boid> boid);

        std::shared_ptr<Boid> getFirstMember();

    private:
        static int                               s_count;
        static std::list<std::shared_ptr<Flock>> s_flocks;

        int                              m_id;
        std::list<std::shared_ptr<Boid>> m_boids;
    };

    REFLECTION_TYPE(FlockComponent)
    CLASS(FlockComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(FlockComponent)

    public:
        FlockComponent() {};
        virtual ~FlockComponent() {};

        void postLoadResource(std::weak_ptr<GObject> parent_object) override;

        void tick(float delta_time) override;

    private:
        META(Enable)
        BasicFlockRes m_flock_res;

        META(Enable)
        MeshComponentRes m_mesh_res;

        META(Enable)
        MeshComponentRes m_mesh_instance;

        std::vector<GameObjectPartDesc>     m_raw_meshes;
        std::vector<std::shared_ptr<Boid>>  m_boids;
        std::vector<std::shared_ptr<Flock>> m_flocks;
    };
} // namespace Piccolo
