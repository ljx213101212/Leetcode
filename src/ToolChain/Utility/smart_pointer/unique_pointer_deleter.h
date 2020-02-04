#include "../pch.h"
namespace my_util{


class SampleOfUniquePtrDeleter{
    template <typename T>
    using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T *)>>;

public:
    template <typename T>
    void customdeleter(T *data)
    {
        delete data;
    }
    void my_test()
    {
        deleted_unique_ptr<uint8_t> foo(new uint8_t(7), [this](uint8_t *data) { customdeleter(data); });
        int i = 1;
        cout << i;
    }
};
}
