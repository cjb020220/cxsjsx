#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <vector>
#include <memory>
#include <string>
#include <sstream>

static constexpr int faction_count = 2;
static constexpr int warrior_count = 5;
static constexpr int weapon_count = 3;
static constexpr std::array<int, warrior_count> spawn_order[faction_count] =
    {{2, 3, 4, 1, 0}, {3, 0, 1, 2, 4}};
static constexpr std::array<const char *, faction_count> faction_name = {"red", "blue"};
static constexpr std::array<const char *, warrior_count> warrior_name = {"dragon", "ninja", "iceman", "lion", "wolf"};
static constexpr std::array<const char *, weapon_count> weapon_name = {"sword", "bomb", "arrow"};
static constexpr std::array<int, weapon_count> weapon_durability = {-1, 1, 2};

enum faction_type
{
    red,
    blue
};
enum warrior_type
{
    dragon,
    ninja,
    iceman,
    lion,
    wolf
};
enum weapon_type
{
    sword,
    bomb,
    arrow
};

inline std::string time_convert(int tick)
{
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << (tick / 60);
    return oss.str();
}

class Entity
{
protected:
    const std::string name;
    const int type;

public:
    Entity(const std::string name_, const int type_ = -1) : type(type_), name(name_) {}
    virtual ~Entity() = 0;
};

class Weapon : public Entity
{
    friend class Headquarter;

private:
    int damage;
    int durability;

public:
    Weapon(const weapon_type type_, const int damage_, const int durability_)
        : Entity(weapon_name[type_], type_), damage(damage_), durability(durability_) {}
    virtual ~Weapon() = default;
};

class Warrior : public Entity
{
    friend class Headquarter;

protected:
    int id;
    int hp;
    faction_type faction;
    std::vector<std::unique_ptr<Weapon>> weapon;

public:
    Warrior(const warrior_type type_, const int id_, const int hp_, const faction_type faction_)
        : Entity(std::string(faction_name[faction_]) + " " + warrior_name[type_] + " " + std::to_string(id_), type_),
          id(id_), hp(hp_), faction(faction_) {}
    virtual ~Warrior() = default;
};

class Dragon : public Warrior
{
private:
    double morale;

public:
    Dragon(const warrior_type type_, const int id_, const int hp_, const faction_type faction_, const double morale_)
        : Warrior(type_, id_, hp_, faction_), morale(morale_)
    {
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), -1, weapon_durability[id_ % 3]));
    }
    virtual ~Dragon() = default;
};

class Ninja : public Warrior
{
public:
    Ninja(const warrior_type type_, const int id_, const int hp_, const faction_type faction_) : Warrior(type_, id_, hp_, faction_)
    {
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), -1, weapon_durability[id_ % 3]));
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>((id_ + 1) % 3), -1, weapon_durability[(id_ + 1) % 3]));
    }
    virtual ~Ninja() = default;
};

class Iceman : public Warrior
{
public:
    Iceman(const warrior_type type_, const int id_, const int hp_, const faction_type faction_) : Warrior(type_, id_, hp_, faction_)
    {
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), -1, weapon_durability[id_ % 3]));
    }
    virtual ~Iceman() = default;
};

class Lion : public Warrior
{
private:
    int loyalty;

public:
    Lion(const warrior_type type_, const int id_, const int hp_, const faction_type faction_, const int loyalty_)
        : Warrior(type_, id_, hp_, faction_), loyalty(loyalty_) {}
    virtual ~Lion() = default;
};

class Wolf : public Warrior
{
public:
    Wolf(const warrior_type type_, const int id_, const int hp_, const faction_type faction_) : Warrior(type_, id_, hp_, faction_) {}
    virtual ~Wolf() = default;
};

class Headquarter : public Entity
{
private:
    int hp;
    int current_type;
    bool stop;
    std::array<int, warrior_count> count = {};
    int total;

public:
    Headquarter(const faction_type type_, const int hp_)
        : Entity(faction_name[type_] + std::string(" headquarter"), type_), hp(hp_), current_type(0), stop(false), total(0) {}
    virtual ~Headquarter() = default;
    std::unique_ptr<Warrior> spawn_warrior(int tick, warrior_type warrior_type, int warrior_id, int warrior_hp) const;
    bool spawn(int tick);
};

class Game
{
    friend class Headquarter;

private:
    static Game *game_instance;
    bool gameover;
    int headquarter_hp;
    std::array<int, warrior_count> warrior_hp;
    std::array<std::unique_ptr<Headquarter>, faction_count> headquarter;
    Game() {}
    ~Game() = default;

public:
    void init(const int headquarter_hp_, const std::array<int, warrior_count> &warrior_hp_);
    static Game *get_game_instance();
    static void delete_game_instance();
    void end();
    void run();
};

Entity::~Entity() = default;

std::unique_ptr<Warrior> Headquarter::spawn_warrior(int tick, warrior_type warrior_type, int warrior_id, int warrior_hp) const
{
    std::unique_ptr<Warrior> new_warrior;
    std::cout << time_convert(tick) << " " << faction_name[this->type] << " " << warrior_name[warrior_type] << " " << warrior_id << " born with strength "
              << warrior_hp << "," << this->count[warrior_type] << " " << warrior_name[warrior_type] << " in " << this->name << std::endl;
    switch (warrior_type)
    {
    case dragon:
        new_warrior = std::make_unique<Dragon>(warrior_type, warrior_id, warrior_hp, static_cast<faction_type>(this->type),
                                               static_cast<double>(this->hp) / warrior_hp);
        std::cout << "It has a " << weapon_name[new_warrior->weapon[0]->type] << ",and it's morale is "
                  << static_cast<double>(this->hp) / warrior_hp << std::endl;
        break;
    case ninja:
        new_warrior = std::make_unique<Ninja>(warrior_type, warrior_id, warrior_hp, static_cast<faction_type>(this->type));
        std::cout << "It has a " << weapon_name[new_warrior->weapon[0]->type] << " and a "
                  << weapon_name[new_warrior->weapon[1]->type] << std::endl;
        break;
    case iceman:
        new_warrior = std::make_unique<Iceman>(warrior_type, warrior_id, warrior_hp, static_cast<faction_type>(this->type));
        std::cout << "It has a " << weapon_name[new_warrior->weapon[0]->type] << std::endl;
        break;
    case lion:
        new_warrior = std::make_unique<Lion>(warrior_type, warrior_id, warrior_hp, static_cast<faction_type>(this->type), this->hp);
        std::cout << "It's loyalty is " << this->hp << std::endl;
        break;
    case wolf:
        new_warrior = std::make_unique<Wolf>(warrior_type, warrior_id, warrior_hp, static_cast<faction_type>(this->type));
        break;
    }
    return new_warrior;
}

bool Headquarter::spawn(int tick)
{
    if (this->stop)
        return false;
    Game *game = Game::get_game_instance();
    auto iter = std::min_element(game->warrior_hp.begin(), game->warrior_hp.end());
    if (this->hp < *iter)
    {
        std::cout << time_convert(tick) << " " << this->name << " stops making warriors" << std::endl;
        this->stop = true;
        return false;
    }
    while (this->hp < game->warrior_hp[spawn_order[this->type][this->current_type]])
        this->current_type = (this->current_type + 1) % warrior_count;
    int spawn_type = spawn_order[this->type][this->current_type];
    ++this->total;
    ++this->count[spawn_type];
    this->hp -= game->warrior_hp[spawn_type];
    std::unique_ptr<Warrior> new_warrior = this->spawn_warrior(tick, static_cast<warrior_type>(spawn_type),
                                                               this->total, game->warrior_hp[spawn_type]);
    this->current_type = (this->current_type + 1) % warrior_count;
    return true;
}

void Game::init(const int headquarter_hp_, const std::array<int, warrior_count> &warrior_hp_)
{
    this->gameover = false;
    this->headquarter_hp = headquarter_hp_;
    this->warrior_hp = warrior_hp_;
    this->headquarter[0] = std::make_unique<Headquarter>(red, this->headquarter_hp);
    this->headquarter[1] = std::make_unique<Headquarter>(blue, this->headquarter_hp);
}

Game *Game::get_game_instance()
{
    if (game_instance == nullptr)
        game_instance = new Game();
    return game_instance;
}

void Game::delete_game_instance()
{
    if (game_instance != nullptr)
    {
        delete game_instance;
        game_instance = nullptr;
    }
}

void Game::end()
{
    this->gameover = true;
}

void Game::run()
{
    for (int tick = 0; !this->gameover; tick += 60)
    {
        bool end = true;
        for (int i = 0; i < faction_count; ++i)
            end = !this->headquarter[i]->spawn(tick) && end;
        if (end)
            this->end();
    }
}

Game *Game::game_instance = nullptr;
int cases;
int headquarter_hp;
std::array<int, warrior_count> warrior_hp;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(2);
    std::cin >> cases;
    for (int i = 1; i <= cases; ++i)
    {
        std::cin >> headquarter_hp;
        for (int i = 0; i < warrior_count; ++i)
            std::cin >> warrior_hp[i];
        std::cout << "Case:" << i << std::endl;
        Game *game = Game::get_game_instance();
        game->init(headquarter_hp, warrior_hp);
        game->run();
    }
    Game::delete_game_instance();
}