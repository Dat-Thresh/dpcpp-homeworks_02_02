#include <iostream>
#include <string>
#include <atomic>


class VeryHeavyDatabase {
public:
    std::string GetData(const std::string& key) const noexcept {
        return "Very Big Data String: " + key;
    }
};

class OneShotDB : public VeryHeavyDatabase {
public:
    explicit OneShotDB(VeryHeavyDatabase* real_object, int count) : db_obj_(real_object) { atom_count->store(count); };
    std::string GetData(const std::string& key) const noexcept {
        if (atom_count->load() == 0) {
            return ">>> error";
        }
        else {
            atom_count->fetch_sub(1);
            return ">>> value";            
          
        }
    }
private:
    VeryHeavyDatabase* db_obj_;
    std::atomic_int* atom_count = new std::atomic_int;
};

int main() {
    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);
    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;
    std::cout << limit_db.GetData("key") << std::endl;
	return 0;
}