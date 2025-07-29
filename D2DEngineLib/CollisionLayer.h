#pragma once

enum class CollisionLayer : unsigned int
{
    None            = 0,

    PlayerHitBox    = 1 << 0,
    EnemyHitBox     = 1 << 1,
    PlayerAttack    = 1 << 2,
    EnemyAttack     = 1 << 3,
    Wall            = 1 << 4,
    Building        = 1 << 5,
    Sensor          = 1 << 6,
    PlayerMove      = 1 << 7,
    EnemyMove       = 1 << 8,

    All             = 0xFFFFFFFF
};

inline CollisionLayer operator|(CollisionLayer a, CollisionLayer b)
{
    return static_cast<CollisionLayer>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

inline CollisionLayer operator&(CollisionLayer a, CollisionLayer b)
{
    return static_cast<CollisionLayer>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

inline CollisionLayer operator~(CollisionLayer a)
{
    return static_cast<CollisionLayer>(~static_cast<unsigned int>(a));
}