#if PROTOCOL_VERSION > 754
#pragma once

#include "protocolCraft/Types/Particles/Particle.hpp"

namespace ProtocolCraft
{
    class DustColorTransitionParticle : public Particle
    {
    public:
        DustColorTransitionParticle();
        
        virtual ~DustColorTransitionParticle();
        
        virtual std::string GetName() const override;
        
        virtual ParticleType GetType() const override;

        float GetFromRed() const;
        float GetFromGreen() const;
        float GetFromBlue() const;
        float GetScale() const;
        float GetToRed() const;
        float GetToGreen() const;
        float GetToBlue() const;

        void SetFromRed(const float from_red_);
        void SetFromGreen(const float from_green_);
        void SetFromBlue(const float from_blue_);
        void SetScale(const float scale_);
        void SetToRed(const float to_red_);
        void SetToGreen(const float to_green_);
        void SetToBlue(const float to_blue_);
        
    protected:
        virtual void ReadImpl(ReadIterator &iter, size_t &length) override;
        
        virtual void WriteImpl(WriteContainer &container) const override;
        
        virtual Json::Value SerializeImpl() const override;

    private:
        float from_red;
        float from_green;
        float from_blue;
        float scale;
        float to_red;
        float to_green;
        float to_blue;
        
    };
}
#endif
