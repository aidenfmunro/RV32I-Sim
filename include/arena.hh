    template <typename NodeType, typename... Args>

    NodeType *construct(Args &&...args)
    {
        auto node_ptr = std::make_unique<NodeType>(std::forward<Args>(args)...);

        auto raw_data = node_ptr.get();

        data_.push_back(std::move(node_ptr));

        return raw_data;
    }
