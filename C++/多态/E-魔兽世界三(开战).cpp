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
static constexpr int max_city_count = 20;
static constexpr int max_weapon_count = 10;
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
    oss << std::setw(3) << std::setfill('0') << (tick / 60) << ":" << std::setw(2) << std::setfill('0') << (tick % 60);
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
    friend class Warrior;
    friend class Wolf;

private:
    int damage;
    int durability;

public:
    Weapon(const weapon_type type_, const int warrior_damage_, const int durability_)
        : Entity(weapon_name[type_], type_), durability(durability_)
    {
        this->update(warrior_damage_);
    }
    virtual ~Weapon() = default;
    void update(int damage);
};

class Warrior : public Entity
{
    friend class Wolf;
    friend class City;
    friend class Headquarter;

protected:
    int id;
    int position;
    int hp;
    int damage;
    faction_type faction;
    std::vector<std::unique_ptr<Weapon>> weapon;

public:
    Warrior(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Entity(std::string(faction_name[faction_]) + " " + warrior_name[type_] + " " + std::to_string(id_), type_),
          id(id_), position(position_), hp(hp_), damage(damage_), faction(faction_) {}
    virtual ~Warrior() = 0;
    virtual int get_loyalty() const { return 0; }
    void move(int tick);
    virtual void move_update() {}
    virtual void loot_before_fight(int tick, std::unique_ptr<Warrior> &warrior) {}
    void get_available_weapon(int &index) const;
    void attack(std::unique_ptr<Weapon> &weapon, Warrior &warrior);
    void fight(std::unique_ptr<Warrior> &warrior);
    void loot_after_fight(std::unique_ptr<Warrior> &warrior);
    virtual void celebrate(int tick) const {}
    void report(int tick) const;
};

class Dragon : public Warrior
{
private:
    double morale;

public:
    Dragon(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_, const double morale_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_), morale(morale_)
    {
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), damage_, weapon_durability[id_ % 3]));
    }
    virtual ~Dragon() = default;
    virtual void celebrate(int tick) const override;
};

class Ninja : public Warrior
{
public:
    Ninja(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_)
    {
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), damage_, weapon_durability[id_ % 3]));
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>((id_ + 1) % 3), damage_, weapon_durability[(id_ + 1) % 3]));
    }
    virtual ~Ninja() = default;
};

class Iceman : public Warrior
{
public:
    Iceman(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_)
    {
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), damage_, weapon_durability[id_ % 3]));
    }
    virtual ~Iceman() = default;
    virtual void move_update() override;
};

class Lion : public Warrior
{
private:
    int loyalty;

public:
    Lion(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_, const int loyalty_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_), loyalty(loyalty_)
    {
        weapon.push_back(std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), damage_, weapon_durability[id_ % 3]));
    }
    virtual ~Lion() = default;
    virtual int get_loyalty() const override;
    virtual void move_update() override;
};

class Wolf : public Warrior
{
public:
    Wolf(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_) {}
    virtual ~Wolf() = default;
    virtual void loot_before_fight(int tick, std::unique_ptr<Warrior> &warrior) override;
};

class City : public Entity
{
    friend class Game;

protected:
    int id;
    std::array<std::unique_ptr<Warrior>, faction_count> warrior;

public:
    City(const int id_) : Entity("city " + std::to_string(id_)), id(id_) {}
    City(const int id_, const std::string name_, const faction_type type_) : Entity(name_, type_), id(id_) {}
    virtual ~City() = default;
    virtual void spawn(int tick) {}
    void runaway(int tick);
    void loot(int tick);
    void fight(int tick);
    virtual void report(int tick) const;
};

class Headquarter : public City
{
private:
    int hp;
    int current_type;
    bool stop;
    int total;

public:
    Headquarter(const faction_type type_, const int id_, const int hp_)
        : City(id_, faction_name[type_] + std::string(" headquarter"), type_), hp(hp_), current_type(0), stop(false), total(0) {}
    virtual ~Headquarter() = default;
    std::unique_ptr<Warrior> spawn_warrior(int tick, warrior_type warrior_type, int warrior_id, int warrior_position, int warrior_hp, int warrior_damage) const;
    virtual void spawn(int tick) override;
    virtual void report(int tick) const override;
};

class Game
{
    friend class Warrior;
    friend class Lion;
    friend class Headquarter;

private:
    static Game *game_instance;
    bool gameover;
    int headquarter_hp;
    int city_count;
    int loyalty_reduction;
    int time_limit;
    std::array<int, warrior_count> warrior_hp;
    std::array<int, warrior_count> warrior_damage;
    std::array<std::unique_ptr<City>, max_city_count + 2> city;
    Game() {}
    ~Game() = default;

public:
    void init(const int headquarter_hp_, const int city_count_, const int loyalty_reduction_, const int time_limit_,
              const std::array<int, warrior_count> &warrior_hp_, const std::array<int, warrior_count> &warrior_damage_);
    static Game *get_game_instance();
    static void delete_game_instance();
    void end();
    void run();
};

Entity::~Entity() = default;
Warrior::~Warrior() = default;

void Weapon::update(int damage)
{
    switch (this->type)
    {
    case sword:
        this->damage = damage * 2 / 10;
        break;
    case bomb:
        this->damage = damage * 4 / 10;
        break;
    case arrow:
        this->damage = damage * 3 / 10;
        break;
    }
}

void Warrior::move(int tick)
{
    Game *game = Game::get_game_instance();
    if (this->faction == red)
        ++this->position;
    else
        --this->position;
    this->move_update();
    if (!this->position || this->position == game->city_count + 1)
    {
        game->end();
        std::cout << time_convert(tick) << " " << this->name << " reached " << faction_name[!this->faction] << " headquarter with "
                  << this->hp << " elements and force " << this->damage << std::endl;
        std::cout << time_convert(tick) << " " << faction_name[!this->faction] << " headquarter was taken" << std::endl;
        return;
    }
    std::cout << time_convert(tick) << " " << this->name << " marched to city " << this->position << " with "
              << this->hp << " elements and force " << this->damage << std::endl;
}

void Warrior::get_available_weapon(int &index) const
{
    if (index == -1 || !this->weapon.size())
    {
        index = -1;
        return;
    }
    int original_index = index;
    while (1)
    {
        if ((this->weapon[index]->type == sword && this->weapon[index]->damage) || (this->weapon[index]->type != sword && this->weapon[index]->durability))
            break;
        index = (index + 1) % this->weapon.size();
        if (index == original_index)
        {
            index = -1;
            break;
        }
    }
}

void Warrior::attack(std::unique_ptr<Weapon> &weapon, Warrior &warrior)
{
    switch (weapon->type)
    {
    case sword:
        warrior.hp -= weapon->damage;
        break;
    case bomb:
        if (this->type != ninja)
            this->hp -= weapon->damage / 2;
    case arrow:
        warrior.hp -= weapon->damage;
        --weapon->durability;
        break;
    }
}

void Warrior::fight(std::unique_ptr<Warrior> &warrior)
{
    if (!this->weapon.size() && !warrior->weapon.size())
        return;
    std::sort(this->weapon.begin(), this->weapon.end(),
              [](const std::unique_ptr<Weapon> &weapon1, const std::unique_ptr<Weapon> &weapon2)
              {
                  if (weapon1->type == weapon2->type)
                      return weapon1->durability < weapon2->durability;
                  return weapon1->type < weapon2->type;
              });
    std::sort(warrior->weapon.begin(), warrior->weapon.end(),
              [](const std::unique_ptr<Weapon> &weapon1, const std::unique_ptr<Weapon> &weapon2)
              {
                  if (weapon1->type == weapon2->type)
                      return weapon1->durability < weapon2->durability;
                  return weapon1->type < weapon2->type;
              });
    for (int i = 0, j = 0;;)
    {
        this->get_available_weapon(i);
        warrior->get_available_weapon(j);
        if (i == -1 && j == -1)
            break;
        if (i >= 0)
        {
            this->attack(this->weapon[i], *warrior);
            if (warrior->hp <= 0)
                break;
            i = (i + 1) % this->weapon.size();
        }
        if (j >= 0)
        {
            warrior->attack(warrior->weapon[j], *this);
            if (this->hp <= 0)
                break;
            j = (j + 1) % warrior->weapon.size();
        }
    }
    this->weapon.erase(std::remove_if(this->weapon.begin(), this->weapon.end(), [](const std::unique_ptr<Weapon> &weapon)
                                      { return !weapon->durability; }),
                       this->weapon.end());
    warrior->weapon.erase(std::remove_if(warrior->weapon.begin(), warrior->weapon.end(), [](const std::unique_ptr<Weapon> &weapon)
                                         { return !weapon->durability; }),
                          warrior->weapon.end());
}

void Warrior::loot_after_fight(std::unique_ptr<Warrior> &warrior)
{
    if (!warrior->weapon.size() || this->weapon.size() == max_weapon_count)
        return;
    std::sort(warrior->weapon.begin(), warrior->weapon.end(),
              [](const std::unique_ptr<Weapon> &weapon1, const std::unique_ptr<Weapon> &weapon2)
              {
                  if (weapon1->type == weapon2->type)
                      return weapon1->durability > weapon2->durability;
                  return weapon1->type < weapon2->type;
              });
    int original_count = this->weapon.size(), loot_count = warrior->weapon.size();
    if (original_count + loot_count > max_weapon_count)
        loot_count = max_weapon_count - original_count;
    for (int i = 0; i < loot_count; ++i)
    {
        warrior->weapon[i]->update(this->damage);
        this->weapon.push_back(std::move(warrior->weapon[i]));
    }
    warrior->weapon.erase(warrior->weapon.begin(), warrior->weapon.begin() + loot_count);
}

void Warrior::report(int tick) const
{
    std::array<int, weapon_count> count = {};
    for (const auto &iter : this->weapon)
        ++count[iter->type];
    std::cout << time_convert(tick) << " " << this->name << " has ";
    for (int i = 0; i < weapon_count; ++i)
        std::cout << count[i] << " " << weapon_name[i] << " ";
    std::cout << "and " << this->hp << " elements" << std::endl;
}

void Dragon::celebrate(int tick) const
{
    std::cout << time_convert(tick) << " " << this->name << " yelled in city " << this->position << std::endl;
}

void Iceman::move_update()
{
    this->hp -= this->hp / 10;
}

int Lion::get_loyalty() const
{
    return this->loyalty;
}

void Lion::move_update()
{
    Game *game = Game::get_game_instance();
    this->loyalty -= game->loyalty_reduction;
}

void Wolf::loot_before_fight(int tick, std::unique_ptr<Warrior> &warrior)
{
    if (!warrior->weapon.size() || this->weapon.size() == max_weapon_count)
        return;
    std::sort(warrior->weapon.begin(), warrior->weapon.end(),
              [](const std::unique_ptr<Weapon> &weapon1, const std::unique_ptr<Weapon> &weapon2)
              {
                  if (weapon1->type == weapon2->type)
                      return weapon1->durability > weapon2->durability;
                  return weapon1->type < weapon2->type;
              });
    int original_count = this->weapon.size(), loot_count = 0;
    while (loot_count < warrior->weapon.size() && warrior->weapon[loot_count]->type == warrior->weapon[0]->type)
        ++loot_count;
    if (original_count + loot_count > max_weapon_count)
        loot_count = max_weapon_count - original_count;
    std::cout << time_convert(tick) << " " << this->name << " took " << loot_count << " " << warrior->weapon[0]->name
              << " from " << warrior->name << " in city " << this->position << std::endl;
    for (int i = 0; i < loot_count; ++i)
    {
        warrior->weapon[i]->update(this->damage);
        this->weapon.push_back(std::move(warrior->weapon[i]));
    }
    warrior->weapon.erase(warrior->weapon.begin(), warrior->weapon.begin() + loot_count);
}

void City::runaway(int tick)
{
    for (faction_type type : {red, blue})
    {
        if (this->warrior[type] != nullptr && this->warrior[type]->type == lion && this->warrior[type]->get_loyalty() <= 0)
        {
            std::cout << time_convert(tick) << " " << this->warrior[type]->name << " ran away" << std::endl;
            this->warrior[type].reset();
        }
    }
}

void City::loot(int tick)
{
    if (this->warrior[red] == nullptr || this->warrior[blue] == nullptr)
        return;
    if (this->warrior[red]->type != wolf && this->warrior[blue]->type != wolf)
        return;
    if (this->warrior[red]->type == wolf && this->warrior[blue]->type == wolf)
        return;
    if (this->warrior[red]->type == wolf)
        this->warrior[red]->loot_before_fight(tick, this->warrior[blue]);
    else
        this->warrior[blue]->loot_before_fight(tick, this->warrior[red]);
}

void City::fight(int tick)
{
    if (this->warrior[red] == nullptr || this->warrior[blue] == nullptr)
        return;
    if (this->id % 2)
        this->warrior[red]->fight(this->warrior[blue]);
    else
        this->warrior[blue]->fight(this->warrior[red]);
    if (this->warrior[red]->hp <= 0 && this->warrior[blue]->hp <= 0)
    {
        std::cout << time_convert(tick) << " both " << this->warrior[red]->name << " and "
                  << this->warrior[blue]->name << " died in " << this->name << std::endl;
        this->warrior[red].reset();
        this->warrior[blue].reset();
    }
    else if (this->warrior[blue]->hp <= 0)
    {
        std::cout << time_convert(tick) << " " << this->warrior[red]->name << " killed " << this->warrior[blue]->name
                  << " in " << this->name << " remaining " << this->warrior[red]->hp << " elements" << std::endl;
        this->warrior[red]->loot_after_fight(this->warrior[blue]);
        this->warrior[red]->celebrate(tick);
        this->warrior[blue].reset();
    }
    else if (this->warrior[red]->hp <= 0)
    {
        std::cout << time_convert(tick) << " " << this->warrior[blue]->name << " killed " << this->warrior[red]->name
                  << " in " << this->name << " remaining " << this->warrior[blue]->hp << " elements" << std::endl;
        this->warrior[blue]->loot_after_fight(this->warrior[red]);
        this->warrior[blue]->celebrate(tick);
        this->warrior[red].reset();
    }
    else
    {
        std::cout << time_convert(tick) << " both " << this->warrior[red]->name << " and " << this->warrior[blue]->name
                  << " were alive in " << this->name << std::endl;
        this->warrior[red]->celebrate(tick);
        this->warrior[blue]->celebrate(tick);
    }
}

void City::report(int tick) const
{
    if (this->warrior[red] != nullptr)
        this->warrior[red]->report(tick);
    if (this->warrior[blue] != nullptr)
        this->warrior[blue]->report(tick);
}

std::unique_ptr<Warrior> Headquarter::spawn_warrior(int tick, warrior_type warrior_type, int warrior_id, int warrior_position, int warrior_hp, int warrior_damage) const
{
    std::unique_ptr<Warrior> new_warrior;
    switch (warrior_type)
    {
    case dragon:
        new_warrior = std::make_unique<Dragon>(warrior_type, warrior_id, warrior_position, warrior_hp, warrior_damage,
                                               static_cast<faction_type>(this->type), static_cast<double>(this->hp) / warrior_hp);
        break;
    case ninja:
        new_warrior = std::make_unique<Ninja>(warrior_type, warrior_id, warrior_position, warrior_hp, warrior_damage, static_cast<faction_type>(this->type));
        break;
    case iceman:
        new_warrior = std::make_unique<Iceman>(warrior_type, warrior_id, warrior_position, warrior_hp, warrior_damage, static_cast<faction_type>(this->type));
        break;
    case lion:
        new_warrior = std::make_unique<Lion>(warrior_type, warrior_id, warrior_position, warrior_hp, warrior_damage, static_cast<faction_type>(this->type), this->hp);
        break;
    case wolf:
        new_warrior = std::make_unique<Wolf>(warrior_type, warrior_id, warrior_position, warrior_hp, warrior_damage, static_cast<faction_type>(this->type));
        break;
    }
    std::cout << time_convert(tick) << " " << new_warrior->name << " born" << std::endl;
    if (warrior_type == lion)
        std::cout << "Its loyalty is " << this->hp << std::endl;
    return new_warrior;
}

void Headquarter::spawn(int tick)
{
    if (this->stop)
        return;
    Game *game = Game::get_game_instance();
    int spawn_type = spawn_order[this->type][this->current_type];
    int position = this->type ? game->city_count + 1 : 0;
    if (this->hp < game->warrior_hp[spawn_type])
    {
        this->stop = true;
        return;
    }
    ++this->total;
    this->hp -= game->warrior_hp[spawn_type];
    this->warrior[this->type] = this->spawn_warrior(tick, static_cast<warrior_type>(spawn_type), this->total, position,
                                                    game->warrior_hp[spawn_type], game->warrior_damage[spawn_type]);
    this->current_type = (this->current_type + 1) % warrior_count;
}

void Headquarter::report(int tick) const
{
    std::cout << time_convert(tick) << " " << this->hp << " elements in " << this->name << std::endl;
}

void Game::init(const int headquarter_hp_, const int city_count_, const int loyalty_reduction_, const int time_limit_,
                const std::array<int, warrior_count> &warrior_hp_, const std::array<int, warrior_count> &warrior_damage_)
{
    this->gameover = false;
    this->headquarter_hp = headquarter_hp_;
    this->city_count = city_count_;
    this->loyalty_reduction = loyalty_reduction_;
    this->time_limit = time_limit_;
    this->warrior_hp = warrior_hp_;
    this->warrior_damage = warrior_damage_;
    for (int i = 1; i <= this->city_count; ++i)
        this->city[i] = std::make_unique<City>(i);
    this->city[0] = std::make_unique<Headquarter>(red, 0, this->headquarter_hp);
    this->city[this->city_count + 1] = std::make_unique<Headquarter>(blue, this->city_count + 1, this->headquarter_hp);
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
    for (int tick = 0; tick <= this->time_limit && !this->gameover; tick += 5)
    {
        switch (tick % 60)
        {
        case 0:
            this->city[0]->spawn(tick);
            this->city[this->city_count + 1]->spawn(tick);
            break;
        case 5:
            for (int i = 0; i <= this->city_count + 1; ++i)
                this->city[i]->runaway(tick);
            break;
        case 10:
            if (this->city[1]->warrior[blue] != nullptr)
                this->city[1]->warrior[blue]->move(tick);
            for (int i = 1; i <= this->city_count; ++i)
            {
                if (this->city[i - 1]->warrior[red] != nullptr)
                    this->city[i - 1]->warrior[red]->move(tick);
                if (this->city[i + 1]->warrior[blue] != nullptr)
                    this->city[i + 1]->warrior[blue]->move(tick);
            }
            if (this->city[this->city_count]->warrior[red] != nullptr)
                this->city[this->city_count]->warrior[red]->move(tick);
            for (int i = this->city_count; i >= 0; --i)
            {
                if (this->city[i]->warrior[red] != nullptr)
                    this->city[i + 1]->warrior[red] = std::move(this->city[i]->warrior[red]);
            }
            for (int i = 1; i <= this->city_count + 1; ++i)
            {
                if (this->city[i]->warrior[blue] != nullptr)
                    this->city[i - 1]->warrior[blue] = std::move(this->city[i]->warrior[blue]);
            }
            break;
        case 35:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->loot(tick);
            break;
        case 40:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->fight(tick);
            break;
        case 50:
            this->city[0]->report(tick);
            this->city[this->city_count + 1]->report(tick);
            break;
        case 55:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->report(tick);
            break;
        }
    }
}

Game *Game::game_instance = nullptr;
int cases;
int headquarter_hp;
int city_count;
int loyalty_reduction;
int time_limit;
std::array<int, warrior_count> warrior_hp;
std::array<int, warrior_count> warrior_damage;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cout << std::fixed << std::setprecision(2);
    std::cin >> cases;
    for (int i = 1; i <= cases; ++i)
    {
        std::cin >> headquarter_hp >> city_count >> loyalty_reduction >> time_limit;
        for (int i = 0; i < warrior_count; ++i)
            std::cin >> warrior_hp[i];
        for (int i = 0; i < warrior_count; ++i)
            std::cin >> warrior_damage[i];
        std::cout << "Case " << i << ":" << std::endl;
        Game *game = Game::get_game_instance();
        game->init(headquarter_hp, city_count, loyalty_reduction, time_limit, warrior_hp, warrior_damage);
        game->run();
    }
    Game::delete_game_instance();
}