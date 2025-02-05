#pragma once

#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/NetworkPosition.hpp"

namespace ProtocolCraft
{
    class ServerboundPlayerActionPacket : public BaseMessage<ServerboundPlayerActionPacket>
    {
    public:
#if PROTOCOL_VERSION == 340 // 1.12.2
        static constexpr int packet_id = 0x14;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 || PROTOCOL_VERSION == 404 // 1.13.X
        static constexpr int packet_id = 0x18;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
        static constexpr int packet_id = 0x1A;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
        static constexpr int packet_id = 0x1A;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736  // 1.16 or 1.16.1
        static constexpr int packet_id = 0x1B;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 || PROTOCOL_VERSION == 754 // 1.16.2, 1.16.3, 1.16.4, 1.16.5
        static constexpr int packet_id = 0x1B;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 // 1.17.X
        static constexpr int packet_id = 0x1A;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 // 1.18, 1.18.1 or 1.18.2
        static constexpr int packet_id = 0x1A;
#elif PROTOCOL_VERSION == 759 // 1.19
        static constexpr int packet_id = 0x1C;
#elif PROTOCOL_VERSION == 760 // 1.19.1 or 1.19.2
        static constexpr int packet_id = 0x1D;
#elif PROTOCOL_VERSION == 761 // 1.19.3
        static constexpr int packet_id = 0x1C;
#else
#error "Protocol version not implemented"
#endif
        static constexpr std::string_view packet_name = "Player Action";

        virtual ~ServerboundPlayerActionPacket() override
        {

        }


        void SetPos(const NetworkPosition& pos_)
        {
            pos = pos_;
        }

        void SetDirection(const int direction_)
        {
            direction = direction_;
        }

        void SetAction(const int action_)
        {
            action = action_;
        }

#if PROTOCOL_VERSION > 758
        void SetSequence(const int sequence_)
        {
            sequence = sequence_;
        }
#endif


        const NetworkPosition& GetPos() const
        {
            return pos;
        }

        int GetDirection() const
        {
            return direction;
        }

        int GetAction() const
        {
            return action;
        }

#if PROTOCOL_VERSION > 758
        int GetSequence() const
        {
            return sequence;
        }
#endif

    protected:
        virtual void ReadImpl(ReadIterator &iter, size_t &length) override
        {
            action = ReadData<VarInt>(iter, length);
            pos = ReadData<NetworkPosition>(iter, length);
            direction = ReadData<char>(iter, length);
#if PROTOCOL_VERSION > 758
            sequence = ReadData<VarInt>(iter, length);
#endif
        }

        virtual void WriteImpl(WriteContainer &container) const override
        {
            WriteData<VarInt>(action, container);
            WriteData<NetworkPosition>(pos, container);
            WriteData<char>(direction, container);
#if PROTOCOL_VERSION > 758
            WriteData<VarInt>(sequence, container);
#endif
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["pos"] = pos;
            output["direction"] = direction;
            output["action"] = action;
#if PROTOCOL_VERSION > 758
            output["sequence"] = sequence;
#endif

            return output;
        }

    private:
        NetworkPosition pos;
        char direction;
        int action;
#if PROTOCOL_VERSION > 758
        int sequence;
#endif
    };
} //ProtocolCraft
