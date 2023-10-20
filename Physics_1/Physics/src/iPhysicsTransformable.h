
#include <Graphics/Debugger.h>

class iPhysicsTransformable
{
public:
    virtual ~iPhysicsTransformable() {};
    virtual glm::vec3 GetPosition() = 0;
    virtual glm::vec3 GetRotation() = 0;
    virtual void SetVisible(bool activeSelf) = 0;

    virtual void SetPosition(const glm::vec3& newPosition) = 0;
    virtual void SetDrawOrientation(const glm::vec3& newOrientation) = 0;
    virtual bool GetVisible() = 0;
    //virtual void SetDrawOrientation(const glm::quat& newOrientation) = 0;
};