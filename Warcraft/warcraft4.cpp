#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <sstream>

static constexpr int faction_count = 2;
static constexpr int warrior_count = 5;
static constexpr int weapon_count = 3;
static constexpr int max_city_count = 20;
static constexpr int arrow_durability = 3;
static constexpr double dragon_morale_change = 0.2;
static constexpr double dragon_celebrate_threshold = 0.8;
static constexpr int iceman_damage_increase = 20;
static constexpr int iceman_hp_reduction = 9;
static constexpr int city_hp_increase = 10;
static constexpr int headquarter_hp_bonus = 8;
static constexpr std::array<int, warrior_count> spawn_order[faction_count] =
    {{2, 3, 4, 1, 0}, {3, 0, 1, 2, 4}};
static constexpr std::array<const char *, faction_count> faction_name = {"red", "blue"};
static constexpr std::array<const char *, warrior_count> warrior_name = {"dragon", "ninja", "iceman", "lion", "wolf"};
static constexpr std::array<const char *, weapon_count> weapon_name = {"sword", "bomb", "arrow"};

enum faction_type
{
    red,
    blue,
    none
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
    friend class City;

private:
    int damage;
    int durability;

public:
    Weapon(const weapon_type type_, const int warrior_damage_)
        : Entity(weapon_name[type_], type_)
    {
        this->init(warrior_damage_);
    }
    void init(const int warrior_damage);
    virtual ~Weapon() = default;
};

class Warrior : public Entity
{
    friend class Wolf;
    friend class City;
    friend class Headquarter;
    friend class Game;

protected:
    int id;
    int position;
    int hp;
    int damage;
    faction_type faction;
    std::array<std::unique_ptr<Weapon>, weapon_count> weapon;

public:
    Warrior(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Entity(std::string(faction_name[faction_]) + " " + warrior_name[type_] + " " + std::to_string(id_), type_),
          id(id_), position(position_), hp(hp_), damage(damage_), faction(faction_) {}
    virtual ~Warrior() = 0;
    virtual int get_loyalty() const { return 0; }
    void move(int tick);
    virtual void reduce_hp() {}
    void shoot(int tick, std::unique_ptr<Warrior> &warrior);
    void attack(int tick, std::unique_ptr<Warrior> &warrior);
    virtual void counter_attack(int tick, std::unique_ptr<Warrior> &warrior);
    virtual void update_morale(bool win) {}
    virtual void update_loyalty(bool win) {}
    virtual void celebrate(int tick) const {}
    virtual void loot(std::unique_ptr<Warrior> &warrior) {}
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
        if (id_ % 3 != sword || (damage_ * 2 / 10))
            weapon[id_ % 3] = std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), damage_);
    }
    virtual ~Dragon() = default;
    virtual void update_morale(bool win) override;
    virtual void celebrate(int tick) const override;
};

class Ninja : public Warrior
{
public:
    Ninja(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_)
    {
        if (id_ % 3 != sword || (damage_ * 2 / 10))
            weapon[id_ % 3] = std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), damage_);
        if ((id_ + 1) % 3 != sword || (damage_ * 2 / 10))
            weapon[(id_ + 1) % 3] = std::make_unique<Weapon>(static_cast<weapon_type>((id_ + 1) % 3), damage_);
    }
    virtual ~Ninja() = default;
    virtual void counter_attack(int tick, std::unique_ptr<Warrior> &warrior) override {}
};

class Iceman : public Warrior
{
private:
    int step;

public:
    Iceman(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_), step(0)
    {
        if (id_ % 3 != sword || (damage_ * 2 / 10))
            weapon[id_ % 3] = std::make_unique<Weapon>(static_cast<weapon_type>(id_ % 3), damage_);
    }
    virtual ~Iceman() = default;
    virtual void reduce_hp() override;
};

class Lion : public Warrior
{
private:
    int loyalty;

public:
    Lion(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_, const int loyalty_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_), loyalty(loyalty_) {}
    virtual ~Lion() = default;
    virtual int get_loyalty() const override;
    virtual void update_loyalty(bool win) override;
};

class Wolf : public Warrior
{
public:
    Wolf(const warrior_type type_, const int id_, const int position_, const int hp_, const int damage_, const faction_type faction_)
        : Warrior(type_, id_, position_, hp_, damage_, faction_) {}
    virtual ~Wolf() = default;
    virtual void loot(std::unique_ptr<Warrior> &warrior) override;
};

class City : public Entity
{
    friend class Game;

protected:
    int id;
    int hp;
    faction_type last_winner, winner;
    faction_type flag;
    std::array<std::unique_ptr<Warrior>, faction_count> warrior;

public:
    City(const int id_) : Entity("city " + std::to_string(id_)), id(id_), hp(0), last_winner(none), winner(none), flag(none) {}
    City(const int id_, const int hp_, const std::string name_, const faction_type type_) : Entity(name_, type_), id(id_), hp(hp_) {}
    virtual ~City() = default;
    virtual void spawn(int tick) {}
    void runaway(int tick);
    void produce();
    void harvest(int tick);
    void use_bomb(int tick);
    void fight(int tick);
    virtual void report(int tick, faction_type type) const;
};

class Headquarter : public City
{
private:
    int current_type;
    int total;

public:
    Headquarter(const faction_type type_, const int id_, const int hp_)
        : City(id_, hp_, faction_name[type_] + std::string(" headquarter"), type_), current_type(0), total(0) {}
    virtual ~Headquarter() = default;
    std::unique_ptr<Warrior> spawn_warrior(int tick, warrior_type warrior_type, int warrior_id, int warrior_position, int warrior_hp, int warrior_damage) const;
    virtual void spawn(int tick) override;
    virtual void report(int tick, faction_type type) const override;
};

class Game
{
    friend class Weapon;
    friend class Warrior;
    friend class Lion;
    friend class City;
    friend class Headquarter;

private:
    static Game *game_instance;
    bool gameover;
    int headquarter_hp;
    int city_count;
    int arrow_damage;
    int loyalty_reduction;
    int time_limit;
    std::array<int, warrior_count> warrior_hp;
    std::array<int, warrior_count> warrior_damage;
    std::array<std::unique_ptr<City>, max_city_count + 2> city;
    Game() {}
    ~Game() = default;

public:
    void init(const int headquarter_hp_, const int city_count_, const int arrow_damage_, const int loyalty_reduction_, const int time_limit_,
              const std::array<int, warrior_count> &warrior_hp_, const std::array<int, warrior_count> &warrior_damage_);
    static Game *get_game_instance();
    static void delete_game_instance();
    void warrior_move(int tick);
    void update_after_fight();
    void end();
    void run();
};

Entity::~Entity() = default;
Warrior::~Warrior() = default;

void Weapon::init(const int warrior_damage)
{
    switch (this->type)
    {
    case sword:
        damage = warrior_damage * 2 / 10;
        durability = warrior_damage * 2 / 10;
        break;
    case arrow:
        damage = Game::get_game_instance()->arrow_damage;
        durability = arrow_durability;
        break;
    }
}

void Warrior::move(int tick)
{
    if (this->faction == red)
        ++this->position;
    else
        --this->position;
    this->reduce_hp();
    if (!this->position || this->position == Game::get_game_instance()->city_count + 1)
        std::cout << time_convert(tick) << " " << this->name << " reached " << faction_name[!this->faction] << " headquarter with "
                  << this->hp << " elements and force " << this->damage << std::endl;
    else
        std::cout << time_convert(tick) << " " << this->name << " marched to city " << this->position << " with "
                  << this->hp << " elements and force " << this->damage << std::endl;
}

void Warrior::shoot(int tick, std::unique_ptr<Warrior> &warrior)
{
    if (this->weapon[arrow] == nullptr)
        return;
    std::cout << time_convert(tick) << " " << this->name << " shot";
    warrior->hp -= Game::get_game_instance()->arrow_damage;
    if (warrior->hp <= 0)
        std::cout << " and killed " << warrior->name;
    --this->weapon[arrow]->durability;
    if (!this->weapon[arrow]->durability)
        this->weapon[arrow].reset();
    std::cout << std::endl;
}

void Warrior::attack(int tick, std::unique_ptr<Warrior> &warrior)
{
    std::cout << time_convert(tick) << " " << this->name << " attacked " << warrior->name << " in city "
              << this->position << " with " << this->hp << " elements and force " << this->damage << std::endl;
    int attack_damage = this->damage + (this->weapon[sword] != nullptr ? this->weapon[sword]->damage : 0);
    warrior->hp -= attack_damage;
    if (this->weapon[sword] != nullptr)
    {
        this->weapon[sword]->damage = this->weapon[sword]->damage * 8 / 10;
        this->weapon[sword]->durability = this->weapon[sword]->damage;
        if (!this->weapon[sword]->durability)
            this->weapon[sword].reset();
    }
}

void Warrior::counter_attack(int tick, std::unique_ptr<Warrior> &warrior)
{
    std::cout << time_convert(tick) << " " << this->name << " fought back against " << warrior->name << " in city " << this->position << std::endl;
    int counter_attack_damage = this->damage / 2 + (this->weapon[sword] != nullptr ? this->weapon[sword]->damage : 0);
    warrior->hp -= counter_attack_damage;
    if (this->weapon[sword] != nullptr)
    {
        this->weapon[sword]->damage = this->weapon[sword]->damage * 8 / 10;
        this->weapon[sword]->durability = this->weapon[sword]->damage;
        if (!this->weapon[sword]->durability)
            this->weapon[sword].reset();
    }
}

void Warrior::report(int tick) const
{
    std::cout << time_convert(tick) << " " << this->name << " has ";
    int count = std::count_if(this->weapon.begin(), this->weapon.end(), [](const std::unique_ptr<Weapon> &weapon)
                              { return weapon != nullptr; });
    if (!count)
    {
        std::cout << "no weapon" << std::endl;
        return;
    }
    if (this->weapon[arrow])
    {
        std::cout << "arrow(" << this->weapon[arrow]->durability << ")";
        if (--count)
            std::cout << ",";
    }
    if (this->weapon[bomb])
    {
        std::cout << "bomb";
        if (--count)
            std::cout << ",";
    }
    if (this->weapon[sword])
        std::cout << "sword(" << this->weapon[sword]->damage << ")";
    std::cout << std::endl;
}

void Dragon::update_morale(bool win)
{
    if (win)
        this->morale += dragon_morale_change;
    else
        this->morale -= dragon_morale_change;
}

void Dragon::celebrate(int tick) const
{
    if (this->morale > dragon_celebrate_threshold)
        std::cout << time_convert(tick) << " " << this->name << " yelled in city " << this->position << std::endl;
}

void Iceman::reduce_hp()
{
    ++this->step;
    if (this->step % 2)
        return;
    this->damage += iceman_damage_increase;
    this->hp = std::max(this->hp - iceman_hp_reduction, 1);
}

int Lion::get_loyalty() const
{
    return this->loyalty;
}

void Lion::update_loyalty(bool win)
{
    if (!win)
        this->loyalty -= Game::get_game_instance()->loyalty_reduction;
}

void Wolf::loot(std::unique_ptr<Warrior> &warrior)
{
    for (weapon_type type : {sword, bomb, arrow})
    {
        if (this->weapon[type] == nullptr && warrior->weapon[type] != nullptr)
        {
            this->weapon[type] = std::move(warrior->weapon[type]);
            warrior->weapon[type].reset();
        }
    }
}

void City::runaway(int tick)
{
    if (this->id <= Game::get_game_instance()->city_count)
    {
        if (this->warrior[red] != nullptr && this->warrior[red]->type == lion && this->warrior[red]->get_loyalty() <= 0)
        {
            std::cout << time_convert(tick) << " " << this->warrior[red]->name << " ran away" << std::endl;
            this->warrior[red].reset();
        }
    }
    if (this->id)
    {
        if (this->warrior[blue] != nullptr && this->warrior[blue]->type == lion && this->warrior[blue]->get_loyalty() <= 0)
        {
            std::cout << time_convert(tick) << " " << this->warrior[blue]->name << " ran away" << std::endl;
            this->warrior[blue].reset();
        }
    }
}

void City::produce()
{
    this->hp += city_hp_increase;
}

void City::harvest(int tick)
{
    if (this->warrior[red] != nullptr && this->warrior[blue] == nullptr)
    {
        std::cout << time_convert(tick) << " " << this->warrior[red]->name << " earned " << this->hp << " elements for his headquarter" << std::endl;
        Game::get_game_instance()->city[0]->hp += this->hp;
        this->hp = 0;
    }
    if (this->warrior[red] == nullptr && this->warrior[blue] != nullptr)
    {
        std::cout << time_convert(tick) << " " << this->warrior[blue]->name << " earned " << this->hp << " elements for his headquarter" << std::endl;
        Game::get_game_instance()->city[Game::get_game_instance()->city_count + 1]->hp += this->hp;
        this->hp = 0;
    }
}

void City::use_bomb(int tick)
{
    if (this->warrior[red] == nullptr || this->warrior[red]->hp <= 0 || this->warrior[blue] == nullptr || this->warrior[blue]->hp <= 0)
        return;
    faction_type attacker, defender;
    if (this->flag == red || (this->flag == none && this->id % 2))
    {
        attacker = red;
        defender = blue;
    }
    else
    {
        attacker = blue;
        defender = red;
    }
    int attack_damage = this->warrior[attacker]->damage + (this->warrior[attacker]->weapon[sword] != nullptr ? this->warrior[attacker]->weapon[sword]->damage : 0);
    int counter_attack_damage = this->warrior[defender]->damage / 2 +
                                (this->warrior[defender]->weapon[sword] != nullptr ? this->warrior[defender]->weapon[sword]->damage : 0);
    bool die_together = false;
    if (attack_damage >= this->warrior[defender]->hp && this->warrior[defender]->weapon[bomb] != nullptr)
    {
        die_together = true;
        std::cout << time_convert(tick) << " " << this->warrior[defender]->name << " used a bomb and killed " << this->warrior[attacker]->name << std::endl;
        this->warrior[defender]->weapon[bomb].reset();
    }
    if (attack_damage < this->warrior[defender]->hp && this->warrior[defender]->type != ninja &&
        counter_attack_damage >= this->warrior[attacker]->hp && this->warrior[attacker]->weapon[bomb] != nullptr)
    {
        die_together = true;
        std::cout << time_convert(tick) << " " << this->warrior[attacker]->name << " used a bomb and killed " << this->warrior[defender]->name << std::endl;
        this->warrior[attacker]->weapon[bomb].reset();
    }
    if (die_together)
    {
        this->warrior[red].reset();
        this->warrior[blue].reset();
    }
}

void City::fight(int tick)
{
    this->winner = none;
    if (this->warrior[red] == nullptr || this->warrior[blue] == nullptr || (this->warrior[red]->hp <= 0 && this->warrior[blue]->hp <= 0))
        return;
    faction_type attacker, defender;
    if (this->flag == red || (this->flag == none && this->id % 2))
    {
        attacker = red;
        defender = blue;
    }
    else
    {
        attacker = blue;
        defender = red;
    }
    if (this->warrior[blue]->hp <= 0)
        this->winner = red;
    else if (this->warrior[red]->hp <= 0)
        this->winner = blue;
    else
    {
        std::array<int, faction_count> original_hp = {this->warrior[red]->hp, this->warrior[blue]->hp};
        this->warrior[attacker]->attack(tick, this->warrior[defender]);
        if (this->warrior[defender]->hp <= 0)
        {
            std::cout << time_convert(tick) << " " << this->warrior[defender]->name << " was killed in city " << this->id << std::endl;
            this->winner = attacker;
            if (this->warrior[defender]->type == lion)
                this->warrior[attacker]->hp += original_hp[defender];
        }
        else
        {
            this->warrior[defender]->counter_attack(tick, this->warrior[attacker]);
            if (this->warrior[attacker]->hp <= 0)
            {
                std::cout << time_convert(tick) << " " << this->warrior[attacker]->name << " was killed in city " << this->id << std::endl;
                this->winner = defender;
                if (this->warrior[attacker]->type == lion)
                    this->warrior[defender]->hp += original_hp[attacker];
            }
        }
    }
    for (faction_type type : {red, blue})
    {
        if (this->warrior[type]->hp > 0)
        {
            this->warrior[type]->update_morale(this->winner == type);
            this->warrior[type]->update_loyalty(this->winner == type);
            if (attacker == type)
                this->warrior[type]->celebrate(tick);
        }
        if (this->winner == type)
            this->warrior[type]->loot(this->warrior[!type]);
    }
    if (this->hp && this->winner != none)
        std::cout << time_convert(tick) << " " << this->warrior[this->winner]->name << " earned "
                  << this->hp << " elements for his headquarter" << std::endl;
    for (faction_type type : {red, blue})
    {
        if (this->last_winner == type && this->winner == type && this->flag != type)
        {
            std::cout << time_convert(tick) << " " << faction_name[type] << " flag raised in city " << this->id << std::endl;
            this->flag = type;
        }
    }
    this->last_winner = this->winner;
}

void City::report(int tick, faction_type type) const
{
    if (this->warrior[type] != nullptr)
        this->warrior[type]->report(tick);
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
    if (warrior_type == dragon)
        std::cout << "Its morale is " << static_cast<double>(this->hp) / warrior_hp << std::endl;
    if (warrior_type == lion)
        std::cout << "Its loyalty is " << this->hp << std::endl;
    return new_warrior;
}

void Headquarter::spawn(int tick)
{
    int spawn_type = spawn_order[this->type][this->current_type];
    int warrior_hp = Game::get_game_instance()->warrior_hp[spawn_type];
    if (this->hp < warrior_hp)
        return;
    this->hp -= warrior_hp;
    int position = this->type == red ? 0 : Game::get_game_instance()->city_count + 1;
    ++this->total;
    this->warrior[this->type] = this->spawn_warrior(tick, static_cast<warrior_type>(spawn_type), this->total, position,
                                                    warrior_hp, Game::get_game_instance()->warrior_damage[spawn_type]);
    this->current_type = (this->current_type + 1) % warrior_count;
}

void Headquarter::report(int tick, faction_type type) const
{
    std::cout << time_convert(tick) << " " << this->hp << " elements in " << this->name << std::endl;
}

void Game::init(const int headquarter_hp_, const int city_count_, const int arrow_damage_, const int loyalty_reduction_, const int time_limit_,
                const std::array<int, warrior_count> &warrior_hp_, const std::array<int, warrior_count> &warrior_damage_)
{
    this->gameover = false;
    this->headquarter_hp = headquarter_hp_;
    this->city_count = city_count_;
    this->arrow_damage = arrow_damage_;
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

void Game::warrior_move(int tick)
{
    if (this->city[1]->warrior[blue] != nullptr)
    {
        this->city[1]->warrior[blue]->move(tick);
        if (this->city[0]->warrior[blue] != nullptr)
        {
            this->city[0]->warrior[blue].reset();
            std::cout << time_convert(tick) << " red headquarter was taken" << std::endl;
            this->end();
        }
    }
    for (int i = 1; i <= this->city_count; ++i)
    {
        if (this->city[i - 1]->warrior[red] != nullptr)
            this->city[i - 1]->warrior[red]->move(tick);
        if (this->city[i + 1]->warrior[blue] != nullptr)
            this->city[i + 1]->warrior[blue]->move(tick);
    }
    if (this->city[this->city_count]->warrior[red] != nullptr)
    {
        this->city[this->city_count]->warrior[red]->move(tick);
        if (this->city[this->city_count + 1]->warrior[red] != nullptr)
        {
            this->city[this->city_count + 1]->warrior[red].reset();
            std::cout << time_convert(tick) << " blue headquarter was taken" << std::endl;
            this->end();
        }
    }
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
}

void Game::update_after_fight()
{
    for (int i = this->city_count; i; --i)
    {
        if (this->city[0]->hp < headquarter_hp_bonus)
            break;
        if (this->city[i]->winner == red)
        {
            this->city[i]->warrior[red]->hp += headquarter_hp_bonus;
            this->city[0]->hp -= headquarter_hp_bonus;
        }
    }
    for (int i = 1; i <= this->city_count; ++i)
    {
        if (this->city[this->city_count + 1]->hp < headquarter_hp_bonus)
            break;
        if (this->city[i]->winner == blue)
        {
            this->city[i]->warrior[blue]->hp += headquarter_hp_bonus;
            this->city[this->city_count + 1]->hp -= headquarter_hp_bonus;
        }
    }
    for (int i = 1; i <= this->city_count; ++i)
    {
        if (this->city[i]->hp && this->city[i]->winner == red)
        {
            this->city[0]->hp += this->city[i]->hp;
            this->city[i]->hp = 0;
        }
        if (this->city[i]->hp && this->city[i]->winner == blue)
        {
            this->city[this->city_count + 1]->hp += this->city[i]->hp;
            this->city[i]->hp = 0;
        }
        if (this->city[i]->warrior[red] != nullptr && this->city[i]->warrior[red]->hp <= 0)
            this->city[i]->warrior[red].reset();
        if (this->city[i]->warrior[blue] != nullptr && this->city[i]->warrior[blue]->hp <= 0)
            this->city[i]->warrior[blue].reset();
    }
}

void Game::end()
{
    this->gameover = true;
}

void Game::run()
{
    for (int tick = 0; tick <= this->time_limit && !this->gameover;)
    {
        switch (tick % 60)
        {
        case 0:
            this->city[0]->spawn(tick);
            this->city[this->city_count + 1]->spawn(tick);
            tick += 5;
            break;
        case 5:
            for (int i = 0; i <= this->city_count + 1; ++i)
                this->city[i]->runaway(tick);
            tick += 5;
            break;
        case 10:
            this->warrior_move(tick);
            tick += 10;
            break;
        case 20:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->produce();
            tick += 10;
            break;
        case 30:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->harvest(tick);
            tick += 5;
            break;
        case 35:
            for (int i = 1; i <= this->city_count; ++i)
            {
                if (this->city[i]->warrior[red] != nullptr && i < this->city_count && this->city[i + 1]->warrior[blue] != nullptr)
                    this->city[i]->warrior[red]->shoot(tick, this->city[i + 1]->warrior[blue]);
                if (this->city[i]->warrior[blue] != nullptr && i > 1 && this->city[i - 1]->warrior[red] != nullptr)
                    this->city[i]->warrior[blue]->shoot(tick, this->city[i - 1]->warrior[red]);
            }
            tick += 3;
            break;
        case 38:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->use_bomb(tick);
            tick += 2;
            break;
        case 40:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->fight(tick);
            this->update_after_fight();
            tick += 10;
            break;
        case 50:
            this->city[0]->report(tick, red);
            this->city[this->city_count + 1]->report(tick, blue);
            tick += 5;
            break;
        case 55:
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->report(tick, red);
            this->city[this->city_count + 1]->City::report(tick, red);
            this->city[0]->City::report(tick, blue);
            for (int i = 1; i <= this->city_count; ++i)
                this->city[i]->report(tick, blue);
            tick += 5;
            break;
        }
    }
}

Game *Game::game_instance = nullptr;
int cases;
int headquarter_hp;
int city_count;
int arrow_damage;
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
        std::cin >> headquarter_hp >> city_count >> arrow_damage >> loyalty_reduction >> time_limit;
        for (int i = 0; i < warrior_count; ++i)
            std::cin >> warrior_hp[i];
        for (int i = 0; i < warrior_count; ++i)
            std::cin >> warrior_damage[i];
        std::cout << "Case " << i << ":" << std::endl;
        Game *game = Game::get_game_instance();
        game->init(headquarter_hp, city_count, arrow_damage, loyalty_reduction, time_limit, warrior_hp, warrior_damage);
        game->run();
    }
    Game::delete_game_instance();
}