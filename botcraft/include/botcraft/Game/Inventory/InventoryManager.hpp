#pragma once

#include <memory>
#include <map>
#include <mutex>
#include "protocolCraft/Types/Slot.hpp"
#include "protocolCraft/Handler.hpp"

#include "botcraft/Game/Enums.hpp"

namespace Botcraft
{
#if PROTOCOL_VERSION < 755
    enum class TransactionState
    {
        Waiting,
        Accepted,
        Refused
    };
#endif

    struct InventoryTransaction
    {
        std::shared_ptr<ProtocolCraft::ServerboundContainerClickPacket> msg;
#if PROTOCOL_VERSION < 755
        std::map<short, ProtocolCraft::Slot> changed_slots;
        ProtocolCraft::Slot carried_item;
#endif
    };

    class Window;

    class InventoryManager : public ProtocolCraft::Handler
    {
    public:
        InventoryManager();

        std::mutex& GetMutex();

        const std::shared_ptr<Window> GetWindow(const short window_id) const;
        const short GetFirstOpenedWindowId() const;
        const std::shared_ptr<Window> GetPlayerInventory() const;
        std::shared_ptr<Window> GetWindow(const short window_id);
        std::shared_ptr<Window> GetPlayerInventory();
        const short GetIndexHotbarSelected() const;
        const ProtocolCraft::Slot& GetHotbarSelected() const;
        const ProtocolCraft::Slot& GetOffHand() const;
        const ProtocolCraft::Slot& GetCursor() const;
        void EraseInventory(const short window_id);
#if PROTOCOL_VERSION < 755
        const TransactionState GetTransactionState(const short window_id, const int transaction_id);
        void AddPendingTransaction(const InventoryTransaction& transaction);
#endif
        /// @brief "think" about the changes made by this transaction, filling in the necessary values in the msg
        /// @param transaction The transaction to update with the modifications
        /// @return An InventoryTransaction with various info, depending on the version
        InventoryTransaction PrepareTransaction(const std::shared_ptr<ProtocolCraft::ServerboundContainerClickPacket>& transaction);
        
        /// @brief Apply a given transaction to a container
        /// @param transaction The transaction to apply
        void ApplyTransaction(const InventoryTransaction& transaction);
#if PROTOCOL_VERSION > 451
        const std::vector<ProtocolCraft::Trade>& GetAvailableTrades() const;
        ProtocolCraft::Trade& GetAvailableTrade(const int index);
#endif

    private:
        void SetHotbarSelected(const short index);
        void SetCursor(const ProtocolCraft::Slot& c);

        void AddInventory(const short window_id, const InventoryType window_type);
        void SetSlot(const short window_id, const short index, const ProtocolCraft::Slot& slot);
#if PROTOCOL_VERSION > 754
        void SynchronizeContainerPlayerInventory(const short window_id);
#endif
#if PROTOCOL_VERSION > 755
        void SetStateId(const short window_id, const int state_id);
#endif

    private:

        virtual void Handle(ProtocolCraft::Message& msg) override;
        virtual void Handle(ProtocolCraft::ClientboundContainerSetSlotPacket& msg) override;
        virtual void Handle(ProtocolCraft::ClientboundContainerSetContentPacket& msg) override;
        virtual void Handle(ProtocolCraft::ClientboundOpenScreenPacket& msg) override;
        virtual void Handle(ProtocolCraft::ClientboundSetCarriedItemPacket& msg) override;
#if PROTOCOL_VERSION < 755
        virtual void Handle(ProtocolCraft::ClientboundContainerAckPacket& msg) override;
#endif
#if PROTOCOL_VERSION > 451
        virtual void Handle(ProtocolCraft::ClientboundMerchantOffersPacket& msg) override;
#endif
        virtual void Handle(ProtocolCraft::ClientboundContainerClosePacket& msg) override;

        void ApplyTransactionInternal(const InventoryTransaction& transaction);

    private:
        std::mutex inventory_manager_mutex;
        std::map<short, std::shared_ptr<Window> > inventories;
        short index_hotbar_selected;
        ProtocolCraft::Slot cursor;

#if PROTOCOL_VERSION < 755
        // Storing all the transactions that have neither been accepted
        // nor refused by the server yet
        std::map<short, std::map<short, InventoryTransaction > > pending_transactions;
        // Storing the old transactions (accepted/refused) for all opened windows
        std::map<short, std::map<short, TransactionState> > transaction_states;
#endif
#if PROTOCOL_VERSION > 451
        int trading_container_id;
        std::vector<ProtocolCraft::Trade> available_trades;
#endif
    };
} // Botcraft