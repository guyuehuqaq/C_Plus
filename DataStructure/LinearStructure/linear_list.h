
template<typename T>
class LinearList{
public:
    // 析构函数
    virtual ~LinearList() = default;
    // 获取数据大小
    virtual size_t size() const = 0;
    // 输入下标pos，得到数据
    virtual T& get(size_t pos) const = 0;
};