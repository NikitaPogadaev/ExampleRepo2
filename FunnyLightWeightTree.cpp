#include <iostream>
#include <unordered_map>
#include <string>

// Внутреннее состояние (intrinsinc state)
class TreeType {
public:
    TreeType(const std::string& name) : name_(name) {}

    void Draw(int x, int y) const {
        std::cout << "Drawing tree of type " << name_ << " at position (" << x << ", " << y << ")" << std::endl;
    }

private:
    std::string name_;
};

class TreeFactory {
public:
    static TreeType* GetTreeType(const std::string& name) {
        auto it = treeTypes.find(name);
        if (it == treeTypes.end()) {
            // Если тип дерева не существует, создаем новый и сохраняем его
            TreeType* newType = new TreeType(name);
            treeTypes[name] = newType;
            return newType;
        }
        return it->second;
    }

private:
    static std::unordered_map<std::string, TreeType*> treeTypes;
};

std::unordered_map<std::string, TreeType*> TreeFactory::treeTypes;

// Внешнее состояние (extrinsic state)
class Tree {
public:
    Tree(int x, int y, const std::string& type) : x_(x), y_(y), type_(TreeFactory::GetTreeType(type)) {}

    void Draw() const {
        type_->Draw(x_, y_);
    }

private:
    int x_, y_;
    TreeType* type_;
};

int main() {
    Tree* tree1 = new Tree(1, 1, "pine");
    Tree* tree2 = new Tree(2, 2, "oak");
    Tree* tree3 = new Tree(3, 3, "pine");
    Tree* tree4 = new Tree(4, 4, "oak");

    tree1->Draw();
    tree2->Draw();
    tree3->Draw();
    tree4->Draw();

    delete tree1;
    delete tree2;
    delete tree3;
    delete tree4;

    return 0;
}
