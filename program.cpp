#include <iostream>
#include <string>
#include <array>
#include <random>
#include <numeric>
#include "random.h"

void ignoreLine(){
    std::cin.ignore(100, '\n');
}

class Creature
{
protected:
    std::string m_name{};
    char m_symbol{};
    int m_health{};
    int m_damege{};
    int m_gold{};

public:
    Creature(const std::string &name, char symbol, int health, int demage, int gold) : m_name{name}, m_damege{demage}, m_health{health}, m_symbol{symbol}, m_gold{gold} {}

    void addGold(int num)
    {
        m_gold += num;
    }

    void reduceHealth(int num)
    {
        m_health -= num;
    }

    const std::string &getName() const { return m_name; }
    const int getHealth() const { return m_health; }
    const int getGold() const { return m_gold; }
    const int getSymbol() const { return m_symbol; }
    const int getDemage() const { return m_damege; }

    bool isDead()
    {
        if (m_health <= 0)
        {
            return true;
        }
        return false;
    }
};

class Player : public Creature
{
private:
    int m_level{1};

public:
    Player(const std::string &name) : Creature{name, '@', 10, 1, 0} {}

    void levelUp()
    {
        m_level++;
        m_damege++;
    }

    const bool hasWon() const
    {
        if (m_level >= 20)
        {
            return true;
        }
        return false;
    }

    const std::string &getName() const { return m_name; }
    const int getHealth() const { return m_health; }
    const int getGold() const { return m_gold; }
    const int getSymbol() const { return m_symbol; }
    const int getDemage() const { return m_damege; }
    const int getLevel() const { return m_level; }
};

class Monster : public Creature
{
public:
    enum Type
    {
        dragon,
        orc,
        slime,
        max_types,

    };

private:
    static const Creature &getDefaultCreature(Type type)
    {
        static const std::array<Creature, static_cast<std::size_t>(Type::max_types)> monsterData{
            {{"dragon", 'D', 20, 4, 100},
             {"orc", 'o', 4, 2, 25},
             {"slime", 's', 1, 1, 10}}};

        return monsterData.at(static_cast<std::size_t>(type));
    }

public:
    static Type getRandomMonster()
    {
        std::random_device rd{};
        std::seed_seq seed{rd(), rd(), rd(), rd(), rd()};
        std::uniform_int_distribution<> uid{0, static_cast<int>(Type::max_types) - 1};
        std::mt19937 mt{seed};

        return static_cast<Type>(uid(mt));
    }

    Monster(Type type) : Creature{getDefaultCreature(type)}
    {
    }
};

class play
{
public:
    play(Player& player)
    {
        while (true)
        {
            // if player has level 20
            if (player.hasWon())
            {
                std::cout << "YOU WON!!\n";
                return;
                // if player has died
            }
            else if (player.isDead())
            {
                std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " Gold\n";
                return;
            }

            Monster enemy{Monster::getRandomMonster()};

            std::cout << "You encoundered an " << enemy.getName() << " (" << static_cast<char>(enemy.getSymbol()) << ")\n";

            if (isFighting())
            {
                while (true)
                {
                    attackEnemy(player, enemy);
                    std::cout << "You attacked the enemy with the damege of " << player.getDemage() << '\n';
                    // if monster dies
                    if (enemy.isDead())
                    {
                        std::cout << "You killed the enemy\n";
                        player.addGold(enemy.getGold());
                        player.levelUp();
                        std::cout << "You at level of " << player.getLevel() << " And have " << player.getGold() << " Gold.\n";
                        break;
                    }
                    attackPlayer(player, enemy);
                    std::cout << "The enemy attacked you with the damege of " << enemy.getDemage() << '\n';
                    if (player.isDead())
                    {
                        std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " Gold\n";
                        return;
                    }

                    if (!isFighting())
                    {
                        break;
                    }
                }
            }
            else
            {
                
                auto random{Random::get(1, 2)};
                // fleed
                if (random == 1)
                {
                    std::cout << "Succsesfuly fled\n";
                
                    // didnt flee
                }else{
                    bool looping = true;
                    while (looping)
                    {
                        std::cout << "Faild to flee\n";
                        // attack player
                        attackPlayer(player, enemy);
                        std::cout << "The " << enemy.getName() << " Hited you with damage of " << enemy.getDemage() << '\n';
                        if (player.isDead())
                        {
                            std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " Gold\n";
                            return;
                        }
                        else if (enemy.isDead())
                        {
                            std::cout << "You killed the enemy\n";
                        }

                        if (isFighting())
                        {

                            while (true)
                            {
                                attackEnemy(player, enemy);
                                std::cout << "You attacked the enemy with the damege of " << player.getDemage() << '\n';
                                // if monster dies
                                if (enemy.isDead())
                                {
                                    std::cout << "You killed the enemy\n";
                                    player.addGold(enemy.getGold());
                                    player.levelUp();
                                    std::cout << "You at level of " << player.getLevel() << " And have " << player.getGold() << " Gold.\n";
                                    looping = false;
                                    break;
                                }
                                attackPlayer(player, enemy);
                                std::cout << "The enemy attacked you with the damege of " << enemy.getDemage() << '\n';
                                if (player.isDead())
                                {
                                    std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " Gold\n";
                                    return;
                                }

                                if (!isFighting())
                                {
                                    looping = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void attackPlayer(Player& player, Monster& enemy)
    {
        player.reduceHealth(enemy.getDemage());
    }

    void attackEnemy(Player& player, Monster& enemy)
    {
        enemy.reduceHealth(player.getDemage());
    }

    bool isFighting()
    {
        while(true){
        bool fight = false;
        std::cout << "(f)ight or (R)un: ";
        char input;
        std::cin >> input;
        ignoreLine();
        if (input == 'f' || input == 'F')
        {
            fight = true;
            return fight;
        }else if(input == 'r' || input == 'R'){
            return fight;
        }
      }
    }
};

int main()
{
    std::cout << "Enter your name: ";
    std::string name;
    std::cin >> name;
    ignoreLine();
    Player player{name};
    std::cout << "Welcome, " << name << ".\n";
    play p1(player);

    return 0;
}
