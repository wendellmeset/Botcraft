#pragma once

#if PROTOCOL_VERSION > 754
#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/Chat/Chat.hpp"

namespace ProtocolCraft
{
    class ClientboundPlayerCombatEnterPacket : public BaseMessage<ClientboundPlayerCombatEnterPacket>
    {
    public:
        virtual const int GetId() const override
        {
#if PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 // 1.17.X
            return 0x34;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 // 1.18, 1.18.1 or 1.18.2
            return 0x34;
#elif PROTOCOL_VERSION == 759 // 1.19
            return 0x32;
#elif PROTOCOL_VERSION == 760 // 1.19.1 or 1.19.2
            return 0x35;
#elif PROTOCOL_VERSION == 761 // 1.19.3
            return 0x33;
#else
#error "Protocol version not implemented"
#endif
        }

        virtual const std::string GetName() const override
        {
            return "Player Combat Enter";
        }

        virtual ~ClientboundPlayerCombatEnterPacket() override
        {

        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {

        }

        virtual void WriteImpl(WriteContainer& container) const override
        {

        }

        virtual const nlohmann::json SerializeImpl() const override
        {
            nlohmann::json output;


            return output;
        }

    private:
    };
} //ProtocolCraft
#endif