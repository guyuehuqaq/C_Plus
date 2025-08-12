

template<typename T>
class LinearList{
public:
    // 析构函数
    virtual ~LinearList();
    // 获取数据大小
    virtual int size() const = 0;
    // 输入下标pos，得到数据
    virtual T get(size_t pos) const = 0;
    // 查找数据的pos
    virtual int find(const T& val) const = 0;
    // 在pos插入数据
    virtual void insert(size_t pos, const T& val) = 0;
    // 删除pos位置的数据
    virtual void remove(size_t pos) = 0;

};