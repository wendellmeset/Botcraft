#pragma once

#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/MapDecoration.hpp"

namespace ProtocolCraft
{
    class ClientboundMapItemDataPacket : public BaseMessage<ClientboundMapItemDataPacket>
    {
    public:
#if PROTOCOL_VERSION == 340 // 1.12.2
        static constexpr int packet_id = 0x24;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 || PROTOCOL_VERSION == 404 // 1.13.X
        static constexpr int packet_id = 0x26;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
        static constexpr int packet_id = 0x26;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
        static constexpr int packet_id = 0x27;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736  // 1.16 or 1.16.1
        static constexpr int packet_id = 0x26;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 || PROTOCOL_VERSION == 754 // 1.16.2, 1.16.3, 1.16.4, 1.16.5
        static constexpr int packet_id = 0x25;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 // 1.17.X
        static constexpr int packet_id = 0x27;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 // 1.18, 1.18.1 or 1.18.2
        static constexpr int packet_id = 0x27;
#elif PROTOCOL_VERSION == 759 // 1.19
        static constexpr int packet_id = 0x24;
#elif PROTOCOL_VERSION == 760 // 1.19.1 or 1.19.2
        static constexpr int packet_id = 0x26;
#elif PROTOCOL_VERSION == 761 // 1.19.3
        static constexpr int packet_id = 0x25;
#else
#error "Protocol version not implemented"
#endif
        static constexpr std::string_view packet_name = "Map Item Data";

        virtual ~ClientboundMapItemDataPacket() override
        {

        }

        void SetMapId(const int map_id_)
        {
            map_id = map_id_;
        }

        void SetScale(const char scale_)
        {
            scale = scale_;
        }

#if PROTOCOL_VERSION < 755
        void SetTrackingPosition(const bool tracking_position_)
        {
            tracking_position = tracking_position_;
        }
#endif

#if PROTOCOL_VERSION > 451
        void SetLocked(const bool locked_)
        {
            locked = locked_;
        }
#endif

#if PROTOCOL_VERSION > 754
        void SetDecorations(const std::optional<std::vector<MapDecoration>>& decorations_)
        {
            decorations = decorations_;
        }
#else
        void SetDecorations(const std::vector<MapDecoration>& decorations_)
        {
            decorations = decorations_;
        }
#endif

        void SetStartX(const unsigned char start_x_)
        {
            start_x = start_x_;
        }

        void SetStartZ(const unsigned char start_z_)
        {
            start_z = start_z_;
        }

        void SetWidth(const unsigned char width_)
        {
            width = width_;
        }

        void SetHeight(const unsigned char height_)
        {
            height = height_;
        }

        void SetMapColors(const std::vector<unsigned char>& map_colors_)
        {
            map_colors = map_colors_;
        }


        int GetMapId() const
        {
            return map_id;
        }

        char GetScale() const
        {
            return scale;
        }

#if PROTOCOL_VERSION < 755
        bool GetTrackingPosition() const
        {
            return tracking_position;
        }
#endif

#if PROTOCOL_VERSION > 451
        bool GetLocked() const
        {
            return locked;
        }
#endif

#if PROTOCOL_VERSION > 754
        const std::optional<std::vector<MapDecoration>>& GetDecorations() const
        {
            return decorations;
        }
#else
        const std::vector<MapDecoration>& GetDecorations() const
        {
            return decorations;
        }
#endif

        unsigned char GetStartX() const
        {
            return start_x;
        }

        unsigned char GetStartZ() const
        {
            return start_z;
        }

        unsigned char GetWidth() const
        {
            return width;
        }

        unsigned char GetHeight() const
        {
            return height;
        }

        const std::vector<unsigned char>& GetMapColors() const
        {
            return map_colors;
        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            map_id = ReadData<VarInt>(iter, length);
            scale = ReadData<char>(iter, length);
#if PROTOCOL_VERSION < 755
            tracking_position = ReadData<bool>(iter, length);
#endif
#if PROTOCOL_VERSION > 451
            locked = ReadData<bool>(iter, length);
#endif
#if PROTOCOL_VERSION > 754
            decorations = ReadOptional<std::vector<MapDecoration>>(iter, length,
                [](ReadIterator& i, size_t& l)
                {
                    return ReadVector<MapDecoration>(i, l);
                }
            );
#else
            decorations = ReadVector<MapDecoration>(iter, length);
#endif

            width = ReadData<unsigned char>(iter, length);
            if (width > 0)
            {
                height = ReadData<unsigned char>(iter, length);
                start_x = ReadData<unsigned char>(iter, length);
                start_z = ReadData<unsigned char>(iter, length);
                map_colors = ReadVector<unsigned char>(iter, length);
            }
        }

        virtual void WriteImpl(WriteContainer & container) const override
        {
            WriteData<VarInt>(map_id, container);
            WriteData<char>(scale, container);
#if PROTOCOL_VERSION < 755
            WriteData<bool>(tracking_position, container);
#endif
#if PROTOCOL_VERSION > 451
            WriteData<bool>(locked, container);
#endif
#if PROTOCOL_VERSION > 754
            WriteOptional<std::vector<MapDecoration>>(decorations, container,
                [](const std::vector<MapDecoration>& v, WriteContainer& c)
                {
                    WriteVector<MapDecoration>(v, c);
                }
            );
#else
            WriteVector<MapDecoration>(decorations, container);
#endif

            WriteData<unsigned char>(width, container);
            if (width > 0)
            {
                WriteData<unsigned char>(height, container);
                WriteData<unsigned char>(start_x, container);
                WriteData<unsigned char>(start_z, container);
                WriteVector<unsigned char>(map_colors, container);
            }
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["map_id"] = map_id;
            output["scale"] = scale;
#if PROTOCOL_VERSION < 755
            output["tracking_position"] = tracking_position;
#endif
#if PROTOCOL_VERSION > 451
            output["locked"] = locked;
#endif

#if PROTOCOL_VERSION > 754
            if (decorations.has_value())
            {
                output["decorations"] = decorations.value();
            }
#else
            output["decorations"] = decorations;
#endif
            output["width"] = width;
            if (width > 0)
            {
                output["height"] = height;
                output["start_x"] = start_x;
                output["start_z"] = start_z;
                output["map_colors"] = "Vector of " + std::to_string(map_colors.size()) + " unsigned chars";
            }

            return output;
        }

    private:
        int map_id;
        char scale;
#if PROTOCOL_VERSION < 755
        bool tracking_position;
#endif
#if PROTOCOL_VERSION > 451
        bool locked;
#endif
#if PROTOCOL_VERSION > 754
        std::optional<std::vector<MapDecoration>> decorations;
#else
        std::vector<MapDecoration> decorations;
#endif
        unsigned char start_x;
        unsigned char start_z;
        unsigned char width;
        unsigned char height;
        std::vector<unsigned char> map_colors;
    };
} //ProtocolCraft
