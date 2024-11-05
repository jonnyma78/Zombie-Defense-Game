//Project Identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043

#include <iostream>
#include <getopt.h>
#include <queue>
#include <cassert>
#include <string>
#include <algorithm>
#include "P2random.h"

using namespace std;

class Info{
    public:
        uint32_t stat;
        bool statActive = false;
        bool verbose = false;
        bool median = false;
        bool help = false;

        Info(int argc, char *argv[]){
            int choice;
            int option_index = 0;
            option long_options[] = {
                {"verbose", no_argument, nullptr, 'v'},
                {"statistics", required_argument, nullptr, 's'},
                {"median", no_argument, nullptr, 'm'},
                {"help", no_argument, nullptr, 'h'},
                { nullptr, 0,                 nullptr, '\0' }
            };

            while((choice = getopt_long(argc, argv, "vs:mh", long_options, &option_index)) != -1){
                switch(choice){
                    case 'v':
                        verbose = true;
                        break;
                    case 's':
                        stat = stoi(optarg);
                        statActive = true;
                        break;
                        
                    case 'm':
                        median = true;
                        break;

                    case 'h':
                        help = true;
                        break;
                    default:
                    cerr << "Error: invalid option\n";
                    exit(1); 
        }              
    }
}        

};

struct Zombie{
    string name;
    uint32_t distance;
    uint32_t speed;
    uint32_t health;
    uint32_t activeRounds;
};

struct Compare{
    bool operator()(Zombie* zombOne, Zombie* zombTwo){
        uint32_t etaOne = (zombOne->distance)/(zombOne->speed);
        uint32_t etaTwo = (zombTwo->distance)/(zombTwo->speed);
        if(etaOne < etaTwo){
            return 0;
        }
        if(etaOne == etaTwo){
            if(zombOne->health < zombTwo->health){
                return 0;
            }
            if(zombOne->health == zombTwo->health){
                if(zombOne->name < zombTwo->name){
                    return 0;
                }
            }
        }
        return 1;

    }
};

struct CompareActivityDescending{//RANKS ZOMBIES IN ORDER OF Descending ACTIVITY
    bool operator()(Zombie* zombOne, Zombie* zombTwo){
        if(zombOne->activeRounds > zombTwo ->activeRounds){
            return 0;
        }
        if(zombOne->activeRounds == zombTwo -> activeRounds){
            if(zombOne -> name < zombTwo->name){
                return 0;
            }
        }
        return 1;
    }
};

struct CompareActivityAscending{//RANKS ZOMBIES IN ORDER OF ASCENDING ACTIVITY
    bool operator()(Zombie* zombOne, Zombie* zombTwo){
        if(zombOne->activeRounds < zombTwo ->activeRounds){
            return 0;
        }
        if(zombOne->activeRounds == zombTwo -> activeRounds){
            if(zombOne -> name < zombTwo->name){
                return 0;
            }
        }
        return 1;
    }
};

class Player{

    public:

    uint32_t quiver_capacity;
    uint32_t ammo;

    Player(uint32_t quiver_capacity)
        :quiver_capacity(quiver_capacity), ammo(quiver_capacity){}

    void reload(){
        ammo = quiver_capacity;
    }

    void fire(Zombie* zombie, uint32_t clip){
        ammo = ammo - clip;
        (zombie->health) = (zombie->health)- clip;

    }
    

};

void printZomb(Zombie* zombie){
    cout<<zombie->name;
    cout<<" (distance: ";
    cout<<zombie->distance;
    cout<<", speed: ";
    cout<<zombie->speed;
    cout<<", health: ";
    cout<<zombie->health;
    cout<<")\n";

}

//returns true if the game is over
bool advanceZombies(vector<Zombie*> &masterZombieList, const int round, const bool verbose){
    //Advance Zombies
    int killerIndex;
    bool playerDied = 0;
    int size = static_cast<int>(masterZombieList.size());
    for(int i = 0; i < size; i++){
        if(masterZombieList[i]->health > 0){
            masterZombieList[i]->activeRounds++;
            uint32_t new_distance;
            if(masterZombieList[i]->distance <= masterZombieList[i]->speed){
                new_distance = 0;
            }
            else{
                new_distance = masterZombieList[i]->distance - masterZombieList[i]->speed;
            }
            masterZombieList[i]->distance = new_distance;
            if(verbose){
                cout<<"Moved: ";
                printZomb(masterZombieList[i]);
            }
            if(masterZombieList[i]->distance == 0 && !playerDied){
                killerIndex = i;
                playerDied = 1;
            }
        }
    }   
    if(playerDied){
        cout<<"DEFEAT IN ROUND "<<round<<"! ";
        cout<<masterZombieList[killerIndex]->name;
        cout<<" ate your brains!\n";
    }
    return playerDied;
}

void spawnZombies(vector<Zombie*> &masterZombieList, priority_queue<Zombie*, vector<Zombie*>, Compare> &zombiePq, const bool verbose){
    string line;
    string name;
    uint32_t distance;
    uint32_t speed;
    uint32_t health;
    getline(cin,line);
    uint32_t randZombs = stoi(line.substr(15));
    getline(cin,line);
    uint32_t namedZombs = stoi(line.substr(14));

    for(uint32_t i = 0; i < randZombs; i++){
        name  = P2random::getNextZombieName();
        distance = P2random::getNextZombieDistance();
        speed = P2random::getNextZombieSpeed();
        health = P2random::getNextZombieHealth();

        Zombie *zombie = new Zombie;
        zombie->name = name;
        zombie->distance = distance;
        zombie->speed = speed;
        zombie->health = health;
        zombie->activeRounds = 1;
        masterZombieList.push_back(zombie);
        zombiePq.push(zombie);
        if(verbose){
            cout<<"Created: ";
            printZomb(zombie);
        }
    }
        //add the named zombies
    for(uint32_t i = 0; i < namedZombs; i++){
        getline(cin, line);
        uint32_t healthIdx = static_cast<uint32_t>(line.find_last_of(':'));
        health = stoi(line.substr(healthIdx + 1));
        line.erase(healthIdx - 7);
        uint32_t speedIdx = static_cast<uint32_t>(line.find_last_of(':'));
        speed = stoi(line.substr(speedIdx + 1));
        line.erase(speedIdx - 6);
        uint32_t distanceIdx = static_cast<uint32_t>(line.find_last_of(':'));
        distance = stoi(line.substr(distanceIdx + 1));
        line.erase(distanceIdx - 9);
        name = line;
        Zombie *zombie = new Zombie;
        zombie->name = name;
        zombie->distance = distance;
        zombie->speed = speed;
        zombie->health = health;
        zombie->activeRounds = 1;
        masterZombieList.push_back(zombie);
        zombiePq.push(zombie);
        if(verbose){
            cout<<"Created: ";
            printZomb(zombie);
        }
    }
}
    //print output if verbose

//returns true if the game is over
bool playRound(Player &player, priority_queue<Zombie*, vector<Zombie*>, Compare> &zombiePq,  priority_queue<Zombie*, vector<Zombie*>, CompareActivityDescending> &leftMed,  priority_queue<Zombie*, vector<Zombie*>, CompareActivityAscending> &rightMed,
vector<Zombie*> &killOrder, const uint32_t round, const bool verbose, const bool statActive, const bool medianMode){
    //bool gameOver = false;
    //Player Shoots
  
    uint32_t median = 0;
    uint32_t clip = 0;
    while(player.ammo > 0 && !zombiePq.empty()){
        
        if(zombiePq.top()->health < player.ammo){
            clip = zombiePq.top()->health;
        }
        else{
            clip = player.ammo;
        }
        player.fire(zombiePq.top(), clip);
        if(zombiePq.top()->health == 0){
            if(statActive){
                killOrder.push_back(zombiePq.top());
            }
            if(verbose){
                cout<<"Destroyed: ";
                printZomb(zombiePq.top());
            }
            if(medianMode){
                if(leftMed.size() == 0 && rightMed.size() == 0){
                    leftMed.push(zombiePq.top());
                    median = zombiePq.top()->activeRounds;
                }
                else if(zombiePq.top()->activeRounds <= median){
                    leftMed.push(zombiePq.top());
                    median = zombiePq.top()->activeRounds;
                }
                else if(zombiePq.top()->activeRounds > median){
                    rightMed.push(zombiePq.top());
                }
/*                 else{
                    if(leftMed.size() <= rightMed.size()){
                        leftMed.push(zombiePq.top());
                    }
                    else{
                        rightMed.push(zombiePq.top());
                    }
                } */
                if(leftMed.size() > rightMed.size() + 1){
                    rightMed.push(leftMed.top());
                    leftMed.pop();
                    median = leftMed.top()->activeRounds;
                }
                else if(rightMed.size() >= leftMed.size() + 1){
                    leftMed.push(rightMed.top());
                    median = rightMed.top()->activeRounds;
                    rightMed.pop();
                }
            }
            if(zombiePq.size() == 1 && cin.fail()){
                 if(medianMode && !leftMed.empty()){
                    cout<<"At the end of round "<<round<<", ";
                    cout<<"the median zombie lifetime is ";
                    if((leftMed.size() + rightMed.size())%2 == 1){
                        cout<<leftMed.top()->activeRounds<<"\n";
                    }
                    else{
                        cout<<(leftMed.top()->activeRounds + rightMed.top()->activeRounds)/2<<"\n";
                    }
                }
                cout<<"VICTORY IN ROUND "<<round<<"! ";
                cout<<zombiePq.top()->name<<" was the last zombie.\n";
                zombiePq.pop();
                return 1;
            }
            zombiePq.pop();

        }
    }
    if(medianMode && !leftMed.empty()){
        cout<<"At the end of round "<<round<<", ";
        cout<<"the median zombie lifetime is ";
        if((leftMed.size() + rightMed.size())%2 == 1){
            cout<<leftMed.top()->activeRounds<<"\n";
        }
        else{
            cout<<(leftMed.top()->activeRounds + rightMed.top()->activeRounds)/2<<"\n";
        }
    }
    return 0;
}

void cleanShit(vector<Zombie*> &masterZombieList){
    int size = static_cast<int>(masterZombieList.size());
    for(int i = 0; i < size; i++){
        delete masterZombieList[i];
    }
}


int main(int argc, char *argv[]){
    std::ios_base::sync_with_stdio(false);

    uint32_t quiver_capacity;
    uint32_t random_seed;
    uint32_t max_rand_distance;
    uint32_t max_rand_speed;
    uint32_t max_rand_health;

    bool gameOver = false;

    vector<Zombie*> masterZombieList;
    vector<Zombie*> killOrder;
    priority_queue<Zombie*, vector<Zombie*>, Compare> zombiePq; 
    priority_queue<Zombie*, vector<Zombie*>, CompareActivityDescending> leftMed;
    priority_queue<Zombie*, vector<Zombie*>, CompareActivityAscending> rightMed;


    Info info(argc, argv);    
    bool verbose = info.verbose;
    uint32_t stat = info.stat;
    bool statActive = info.statActive;
    bool median = info.median;
    bool help = info.help;
    if(help){
        return 0;
    }
    //priority_queue pqr;

    //read input
    string line;
    string substr;
    getline(cin, line);//ignore first line
    getline(cin, line);//read quiver capacity
    quiver_capacity = static_cast<uint32_t>(stoi(line.substr(17)));
    getline(cin, line);//read random seed
    random_seed = static_cast<uint32_t>(stoi(line.substr(13)));
    getline(cin, line);//read rand distance
    max_rand_distance = static_cast<uint32_t>(stoi(line.substr(19)));
    getline(cin, line);//read rand speed
    max_rand_speed = static_cast<uint32_t>(stoi(line.substr(16)));
    getline(cin, line); //read rand health
    max_rand_health = static_cast<uint32_t>(stoi(line.substr(17)));

    P2random::initialize(random_seed, max_rand_distance, max_rand_speed, max_rand_health);
    Player player(quiver_capacity);
    uint32_t actualRound = 0;
    getline(cin, line); //get rid of "---"
    uint32_t nextRound;
    getline(cin, line);
    nextRound = stoi(line.substr(6));
    //read rounds
    while(!gameOver){
        //increment the actual round and print round if verbose
        actualRound++;
        if(verbose){
            cout<<"Round: "<<actualRound<<"\n";
        }
        //Player replenishes Arrows
        player.reload();
        //Advance any zombies on the field
        if(!zombiePq.empty()){
            gameOver = advanceZombies(masterZombieList, actualRound, verbose);
            if(gameOver){
                break;
            }
        }
        //add the  zombies IF the round is the expected round
        if(actualRound == nextRound && !cin.fail()){
                      
  /*           if(cin.fail()){//exit if there is no more output
                cout<<"the last round"<<actualRound<<endl;
                continue;
                
            } */
            spawnZombies(masterZombieList, zombiePq, verbose);
            getline(cin, line); //get rid of "---"
            if(!cin.fail()){
  
                getline(cin, line);
                nextRound = stoi(line.substr(6));

            }

        }
        //Play Round: Player shoots zombies
        gameOver = playRound(player, zombiePq, leftMed, rightMed, killOrder, actualRound, verbose, statActive, median);


    }
     if(statActive){
        cout<<"Zombies still active: "<<zombiePq.size()<<"\n";
        cout<<"First zombies killed:\n";
        uint32_t killPrintNum = stat;
        if(killPrintNum > killOrder.size()){
            killPrintNum = static_cast<uint32_t>(killOrder.size());
        }
        for(uint32_t i = 0; i < killPrintNum; i++){
            cout<<killOrder[i]->name<<" "<<i+1<<"\n";
        }
        cout<<"Last zombies killed:\n";
        int size = static_cast<int>(killOrder.size());
        for(uint32_t i = 0; i < killPrintNum; i++){
            cout<<killOrder[size - i - 1]->name<<" "<<killPrintNum - i<<"\n";
        }
        priority_queue<Zombie*, vector<Zombie*>, CompareActivityDescending> activityRankingDescending(masterZombieList.begin(), masterZombieList.end()); 
        priority_queue<Zombie*, vector<Zombie*>, CompareActivityAscending> activityRankingAscending(masterZombieList.begin(), masterZombieList.end());
        uint32_t activityDescentSize = stat;
        uint32_t activityAscentSize = stat;
        if(activityDescentSize > activityRankingDescending.size()){
            activityDescentSize = static_cast<uint32_t>(activityRankingDescending.size());
        } 
        if(activityAscentSize > activityRankingAscending.size()){
            activityAscentSize = static_cast<uint32_t>(activityRankingAscending.size());
        }
        cout<<"Most active zombies:\n";
        for(uint32_t i = 0; i < activityDescentSize; i ++){
            cout<<activityRankingDescending.top()->name<<" ";
            cout<<activityRankingDescending.top()->activeRounds<<"\n";
            activityRankingDescending.pop();
        }
        cout<<"Least active zombies:\n";
        for(uint32_t i = 0; i < activityAscentSize; i++){
            cout<<activityRankingAscending.top()->name<<" ";
            cout<<activityRankingAscending.top()->activeRounds<<"\n";
            activityRankingAscending.pop();
        }

        
        

    }
    cleanShit(masterZombieList);
 


}