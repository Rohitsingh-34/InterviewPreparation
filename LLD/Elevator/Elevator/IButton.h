#pragma once

class Floor; // forward declaration to avoid circular include

class IButton
{
public:
    virtual bool isPressed() = 0;
    virtual ~IButton() = default;
    virtual void press(Floor* floor) = 0;
    virtual void reset() = 0;
};
