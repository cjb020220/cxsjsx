#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <sstream>

static constexpr int faction_count = 2;
static constexpr int warrior_count = 5;
static constexpr std::array<int, warrior_count> spawn_order[faction_count] =
    {{2, 3, 4, 1, 0}, {3, 0, 1, 2, 4}};
static constexpr std::array<const char *, faction_count> faction_name = {"red", "blue"};
static constexpr std::array<const char *, warrior_count> warrior_name = {"dragon", "ninja", "iceman", "lion", "wolf"};

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
    std::cout << time_convert(tick) << " " << faction_name[this->type] << " " << warrior_name[spawn_type] << " " << this->total
              << " born with strength " << game->warrior_hp[spawn_type] << "," << this->count[spawn_type] << " "
              << warrior_name[spawn_type] << " in " << this->name << std::endl;
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