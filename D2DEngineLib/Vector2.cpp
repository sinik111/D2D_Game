#include "pch.h"
#include "Vector2.h"

#include <sstream>

const Vector2 Vector2::Zero{ 0.0f, 0.0f };
const Vector2 Vector2::One{ 1.0f, 1.0f };
const Vector2 Vector2::Up{ 0.0f, 1.0f };
const Vector2 Vector2::Down{ 0.0f, -1.0f };
const Vector2 Vector2::Right{ 1.0f, 0.0f };
const Vector2 Vector2::Left{ -1.0f, 0.0f };

const Vector2 Vector2::EllipseRightUp{ 0.7071f, 0.3536f };
const Vector2 Vector2::EllipseLeftUp{ -0.7071f, 0.3536f };
const Vector2 Vector2::EllipseRightDown{ 0.7071f, -0.3536f };
const Vector2 Vector2::EllipseLeftDown{ -0.7071f, -0.3536f };

std::string Vector2::ToString() const
{
    std::ostringstream oss;

    oss << "{ " << x << ", " << y << " }";

    return oss.str();
}
