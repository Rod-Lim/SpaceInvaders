#pragma once

class SubsystemCollection;

class Subsystem {
public:
    virtual ~Subsystem() = default;
    virtual bool ShouldCreateSubsystem(const void* outer) const { return true; }
    virtual void Initialize(SubsystemCollection& collection) {}
    virtual void Deinitialize() {}
};